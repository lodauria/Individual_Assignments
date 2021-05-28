import json
import base64
import boto3

def lambda_handler(event, context):
    
    message = event['uplink_message']
    encoded_mess = message['frm_payload']
    dev_payload = base64.b64decode(encoded_mess)
    
    # Create an MQTT client
    client = boto3.client('iot-data', region_name='us-east-1')
    
    # Publish a message to actuation topic
    response = client.publish (
        topic='sensing',
        qos=1,
        payload=dev_payload
    )