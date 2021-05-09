import json
import boto3
from boto3.dynamodb.conditions import Key, Attr
from datetime import datetime

# Get data from a specific device given the DB table
def get_last_data(table, d, d_id):
    
    if d:

        # Get old data if in demo mode
        response = table.scan(
        FilterExpression=Key('num').lt(1617264905) & Attr('id').eq(0)
        )
    else:

        # Get data of the last hour
        response = table.scan(
        FilterExpression=Key('num').gte(int(datetime.utcnow().timestamp()-3600)) & Attr('id').eq(d_id)
        )

    return response['Items']

# Get data from all devices given the DB table
def get_all_last_data(table, d):
    
    if d:

        # Get old data if in demo mode
        response = table.scan(
        FilterExpression=Key('num').lt(1617264905)
        )
    else:

        # Get data of the last hour
        response = table.scan(
        FilterExpression=Key('num').gte(int(datetime.utcnow().timestamp()-3600))
        )

    return response['Items']

# When the lambda function is invoked
def lambda_handler(event, context):
    
    # Decode the JSON message
    head = event.get('headers')
    messType = head.get('mess-type')
    debug = head.get('mess-debug')
    dev_id = head.get('dev-id')

    # Check for errors in the message
    if messType is None or debug is None or dev_id is None:
        return {
            'statusCode': 200,
            'headers': {
                'Access-Control-Allow-Origin' : '*',
            },
            'body': json.dumps("Missing custom headers")
        }
        
    # Initialize DynamoDB resource and get data
    dynamodb = boto3.resource('dynamodb', endpoint_url="http://dynamodb.us-east-1.amazonaws.com:80", region_name='us-east-1')
    table = dynamodb.Table('sensing_report')
    last_data = get_last_data(table, debug == "1", int(dev_id))
    all_last_data = get_all_last_data(table, debug == "1")

    # Check if no items have been found
    if len(last_data)==0:
        return {
        'statusCode': 200,
        'headers': {
            'Access-Control-Allow-Origin' : '*',
        },
        'body': json.dumps("No sensor values in the last hour")
    }
    
    # Get most recent item
    last_data = sorted(last_data, key=lambda k: k['num'], reverse=True)
    all_last_data = sorted(all_last_data, key=lambda k: k['num'], reverse=True)
    last = last_data[0]
    
    # Compute the aggregated values of the single device
    proj_min = 2
    proj_max = -1
    light_min = 101
    light_max = -1
    proj_avg = 0
    light_avg = 0
    proj_hist = ["\""]
    light_hist = ["\""]

    for item in last_data:
        p_status = item['projector_status']
        l_status = item['light_level']
        if p_status<proj_min: proj_min = p_status
        if p_status>proj_max: proj_max = p_status
        if l_status<light_min: light_min = l_status
        if l_status>light_max: light_max = l_status
        proj_avg += p_status
        light_avg += l_status
        proj_hist.append(str(p_status)+';')
        light_hist.append(str(l_status)+';')
        
    proj_avg = proj_avg/len(last_data)
    light_avg = light_avg/len(last_data)
    proj_hist[-1] = proj_hist[-1][:-1]+"\""
    light_hist[-1] = light_hist[-1][:-1]+"\""
    
    # Compute the aggregated values
    all_proj_min = 2
    all_proj_max = -1
    all_light_min = 101
    all_light_max = -1
    all_proj_avg = 0
    all_light_avg = 0
    all_proj_hist = ["\""]
    all_light_hist = ["\""]

    for item in all_last_data:
        p_status = item['projector_status']
        l_status = item['light_level']
        if p_status<all_proj_min: all_proj_min = p_status
        if p_status>all_proj_max: all_proj_max = p_status
        if l_status<all_light_min: all_light_min = l_status
        if l_status>all_light_max: all_light_max = l_status
        all_proj_avg += p_status
        all_light_avg += l_status
        all_proj_hist.append('(Dev '+str(dev_id)+', '+str(p_status)+');')
        all_light_hist.append('(Dev '+str(dev_id)+', '+str(l_status)+');')
        
    all_proj_avg = all_proj_avg/len(all_last_data)
    all_light_avg = all_light_avg/len(all_last_data)
    all_proj_hist[-1] = all_proj_hist[-1][:-1]+"\""
    all_light_hist[-1] = all_light_hist[-1][:-1]+"\""
    
    # Assemble the JSON message for the web page
    message ="{\"proj_last\": " + str(last['projector_status']) + ", \n" \
            + "\"light_last\": " + str(last['light_level']) + ", \n" \
            + "\"proj_avg\": " + "{:.2f}".format(proj_avg) + ", \n" \
            + "\"light_avg\": " + "{:.2f}".format(light_avg) + ", \n" \
            + "\"proj_max\": " + str(proj_max) + ", \n" \
            + "\"light_max\": " + str(light_max) + ", \n" \
            + "\"proj_min\": " + str(proj_min) + ", \n" \
            + "\"light_min\": " + str(light_min) + ", \n" \
            + "\"proj_hist\": " + ' '.join(proj_hist) + ", \n" \
            + "\"light_hist\": " + ' '.join(light_hist) + ", \n" \
            + "\"all_proj_avg\": " + "{:.2f}".format(all_proj_avg) + ", \n" \
            + "\"all_light_avg\": " + "{:.2f}".format(all_light_avg) + ", \n" \
            + "\"all_proj_max\": " + str(all_proj_max) + ", \n" \
            + "\"all_light_max\": " + str(all_light_max) + ", \n" \
            + "\"all_proj_min\": " + str(all_proj_min) + ", \n" \
            + "\"all_light_min\": " + str(all_light_min) + ", \n" \
            + "\"all_proj_hist\": " + ' '.join(all_proj_hist) + ", \n" \
            + "\"all_light_hist\": " + ' '.join(all_light_hist) + ", \n"
            
    # If request includes an actuation command send an MQTT message
    if messType != "0":

        # Get most recent actuators status
        relay_var = last['relay']
        motor_var = last['motor']
        
        # Change status for the required actuator
        if messType == "1":
            if relay_var==0:
                relay_var = 1
            else:
                relay_var = 0
                
        if messType == "2":
            if motor_var==1:
                motor_var = 2
            else:
                motor_var = 1
        
        # Create an MQTT client
        client = boto3.client('iot-data', region_name='us-east-1')
      
        # Publish a message to actuation topic
        response = client.publish (
            topic='actuation',
            qos=1,
            payload=json.dumps({"relay":str(relay_var), "motor":str(motor_var), "id":str(dev_id)})
        )

        # Communicate to the web page that actuation has been performed
        message = message + "\"act\": 1}"
    
    else:

        # Used to communicate that no actuation has been done
        message = message + "\"act\": 0}"
    
    # Return all the informations to the web page
    return {
        'statusCode': 200,
        'headers': {
            'Access-Control-Allow-Origin' : '*',
        },
        'body': message
    }