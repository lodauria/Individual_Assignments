import json
import boto3
from boto3.dynamodb.conditions import Key
from datetime import datetime

def get_last_data(table):
    
    response = table.scan(
        FilterExpression=Key('num').gte(int(datetime.utcnow().timestamp()-3600))
        #FilterExpression=Key('num').gte(1617264845)
        )

    return response['Items']

def lambda_handler(event, context):
    
    # Decode the JSON message
    messType = event['type']
    
    # Last hour values
    dynamodb = boto3.resource('dynamodb', endpoint_url="http://dynamodb.us-east-1.amazonaws.com:80", region_name='us-east-1')
    table = dynamodb.Table('sensing_report')
    last_data = get_last_data(table)
    
    # Last values
    last = last_data[0]
    
    # Aggregated values
    proj_min = 2
    proj_max = -1
    light_min = 2
    light_max = -1
    proj_avg = 0
    light_avg = 0
    proj_hist = []
    light_hist = []
    for item in last_data:
        p_status = item['projector_status']
        l_status = item['light_level']
        if p_status<proj_min: proj_min = p_status
        if p_status>proj_max: proj_max = p_status
        if l_status<light_min: light_min = l_status
        if l_status>light_max: light_max = l_status
        proj_avg += p_status
        light_avg += l_status
        proj_hist.append(str(p_status)+',')
        light_hist.append(str(l_status)+',')
        
    proj_avg = proj_avg/len(last)
    light_avg = light_avg/len(last)
    
    # Assemble message
    message ="{\"proj_last\": " + str(last['projector_status']) + ", \n" \
            + "\"light_last\": " + str(last['light_level']) + ", \n" \
            + "\"proj_avg\": " + str(proj_avg) + ", \n" \
            + "\"light_avg\": " + str(light_avg) + ", \n" \
            + "\"proj_max\": " + str(proj_max) + ", \n" \
            + "\"light_max\": " + str(light_max) + ", \n" \
            + "\"proj_min\": " + str(proj_min) + ", \n" \
            + "\"light_min\": " + str(light_min) + ", \n" \
            + "\"proj_hist\": " + ' '.join(proj_hist) + ", \n" \
            + "\"light_hist\": " + ' '.join(light_hist) + ", \n"
            
    # TODO: get required data
    relay_var = last['relay']
    motor_var = last['motor']
    
    if messType == 1:
        if relay_var==0:
            relay_var = 1
        else:
            relay_var = 0
            
    if messType == 2:
        if motor_var==1:
            motor_var = 2
        else:
            motor_var = 1
            
    if messType != 0:
        
        message = message + "\"act\": 1}"
        
        # Create an MQTT client
        client = boto3.client('iot-data', region_name='us-east-1')
      
        # Publish a message to actuation topic
        response = client.publish (
            topic='actuation',
            qos=1,
            payload=json.dumps({"relay":relay_var, "motor":new_motor_var})
        )
    
    else:
        message = message + "\"act\": 0}"
    
    return {
        'statusCode': 200,
        'headers': {
            'Access-Control-Allow-Origin' : '*',
        },
        'body': json.dumps(message)
    }