import json
import base64
import boto3

# When the lambda function is invoked
def lambda_handler(event, context):
    
    # Encode the message that has to be sent to TNN
    plain_mess = json.dumps(event)
    enc_message = base64.b64encode(plain_mess.encode('ascii'))
    
    # Get the DevEUI from the device ID
    node_id = event['id']
    dev_eui = str(node_id) + str(node_id) + str(node_id) + str(node_id) + str(node_id) + str(node_id) + str(node_id) + str(node_id)
    
    # Create an MQTT client
    client = boto3.client('iot-data', region_name='us-east-1')
    
    # Publish a message with lorawan topic that will be automatically forwarded to TTN
    response = client.publish (
        topic='lorawan/downlink',
        qos=1,
        payload=json.dumps({"thingName": dev_eui, "bytes": enc_message.decode('ascii'), "fPort": 10, "confirmed": True, "highPriority": True})
    )