import json
import boto3
from datetime import datetime

# Check if it's lecture time
def test_time():

    now = datetime.now()

    # Time zone = GTM, so 2 hours before Rome time zone in summer
    if now > now.replace(hour=6) and now < now.replace(hour=17) and now.weekday()<5:
        return True

    return False

# Save data as a new item in the DynamoDB
def store_data(light, projector, relay, motor, dynamodb=None):

    # Initialize the DB and the table
    if not dynamodb:
        dynamodb = boto3.resource('dynamodb', endpoint_url="http://dynamodb.us-east-1.amazonaws.com:80", region_name='us-east-1')
    table = dynamodb.Table('sensing_report')

    # Store new item
    response = table.put_item(
       Item={
            'num': int(datetime.utcnow().timestamp()),  # Unix epoch format
            'light_level': light,
            'projector_status': projector,
            'relay': relay,
            'motor': motor
        }
    )

# When the lambda function is invoked  
def lambda_handler(event, context):
  
    # Decode the JSON message (gives error and stop if wrong format)
    relay_var = event["relay"]
    motor_var = event["motor"]
    l_level = event["light_level"]
    p_status = event["projector_status"]
    
    # Add item to the DB
    store_data (l_level, p_status, relay_var, motor_var)
    
    # IoT logic
    new_motor_var = 0
    actuation = False

    # If is lecture time
    if test_time():

      # Projector open with too much light
      if p_status == 1 and l_level > 70:
        actuation = True

        if relay_var == 1: 
          relay_var = 0
        else:
          if motor_var != 1:
            new_motor_var = 1

      # Projector closed with too low light
      if p_status == 0 and l_level < 40:
        actuation = True

        if motor_var != 2: 
          new_motor_var = 2
        else:
          if relay_var == 0:
            relay_var = 1
    
    # If an actuation has to be performed
    if actuation:

      # Create an MQTT client
      client = boto3.client('iot-data', region_name='us-east-1')
    
      # Publish a message to actuation topic
      response = client.publish (
          topic='actuation',
          qos=1,
          payload=json.dumps({"relay":relay_var, "motor":new_motor_var})
      )