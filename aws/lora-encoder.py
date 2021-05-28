import json
import base64
import boto3

def lambda_handler(event, context):
    
    plain_mess = json.dumps(event)
    enc_message = base64.b64encode(plain_mess.encode('ascii'))
    
    dev_eui = 3223322332233223
    
    # Create an MQTT client
    client = boto3.client('iot-data', region_name='us-east-1')
    
    # Publish a message to actuation topic
    response = client.publish (
        topic='lorawan/downlink',
        qos=1,
        payload=json.dumps({"thingName": str(dev_eui), "bytes": enc_message.decode('ascii'), "fPort": 10, "confirmed": True, "highPriority": True})
    )