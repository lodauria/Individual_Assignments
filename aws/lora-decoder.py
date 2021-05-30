import json
import base64
import boto3
from datetime import datetime

def lambda_handler(event, context):
    
    message = event['uplink_message']
    encoded_mess = message['frm_payload']
    dev_payload = base64.b64decode(encoded_mess)
    
    # Edge computation
    now = datetime.now()
    mess = json.loads(dev_payload)
    light = mess['light_level']
    
    # Time zone = GTM, so 2 hours before Rome time zone in summer
    if now > now.replace(hour=8) and now < now.replace(hour=19) and light < 20:
        return None
        
    # Create an MQTT client
    client = boto3.client('iot-data', region_name='us-east-1')
    
    # Publish a message to actuation topic
    response = client.publish (
        topic='sensing',
        qos=1,
        payload=dev_payload
    )