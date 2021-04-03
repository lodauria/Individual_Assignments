from __future__ import print_function
  
import json
import boto3
from datetime import datetime
  
print('Loading function')

def test_time():
    now = datetime.now()
    if now > now.replace(hour=6) and now < now.replace(hour=17) and now.weekday()<5:
        return True
    return False

def store_data(light, projector, relay, motor, dynamodb=None):
    if not dynamodb:
        dynamodb = boto3.resource('dynamodb', endpoint_url="http://dynamodb.us-east-1.amazonaws.com:80", region_name='us-east-1')
    
    table = dynamodb.Table('sensing_report')
    response = table.put_item(
       Item={
            'num': int(datetime.utcnow().timestamp()),  # Unix epoch format
            'light_level': light,
            'projector_status': projector,
            'relay': relay,
            'motor': motor
        }
    )
    return response

  
def lambda_handler(event, context):
  
    # Decode the JSON message 
    relay_var = event["relay"]
    motor_var = event["motor"]
    l_level = event["light_level"]
    p_status = event["projector_status"]
    
    # Add item to the DB
    response = store_data (l_level, p_status, relay_var, motor_var)
    print(response)
    
    # IoT logic
    new_motor_var = 0
    if test_time():
      if p_status == 1 and l_level > 70:
        if relay_var == 1: 
          relay_var = 0
        else:
          if motor_var != 1:
            new_motor_var = 1
      if p_status == 0 and l_level < 50:
        if motor_var != 2: 
          new_motor_var = 2
        else:
          if relay_var == 0:
            relay_var = 1
  
    # Create an MQTT client
    client = boto3.client('iot-data', region_name='us-east-1')
  
    # Publish a message to actuation topic
    response = client.publish (
        topic='actuation',
        qos=1,
        payload=json.dumps({"relay":relay_var, "motor":new_motor_var})
    )
  
    print(response)