import json
import base64
import boto3
from datetime import datetime

# When the lambda function is invoked
def lambda_handler(event, context):
    
    # Decode the JSON message (gives error and stops if wrong format)
    message = event['uplink_message']
    encoded_mess = message['frm_payload']
    dev_payload = base64.b64decode(encoded_mess)
    
    # Read the data for the edge computation
    now = datetime.now()
    mess = json.loads(dev_payload)
    light = mess['light_level']
    
    # Filter the outlier
    # Time zone = GTM, so 2 hours before Rome time zone in summer
    if now > now.replace(hour=8) and now < now.replace(hour=19) and light < 20:
        return None
        
    # Create an MQTT client
    client = boto3.client('iot-data', region_name='us-east-1')
    
    # Publish the message with sensing topic as the other sensors do
    response = client.publish (
        topic='sensing',
        qos=1,
        payload=dev_payload
    )