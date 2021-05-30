# AWS setup

## For all the assignments

### Lambda functions

Create a python lambda function called `myReaction` using a role with full access to IoTCore and DynamoDB. In the code section paste the content of `myReaction.py`.

Create a python lambda function called `dashboard_handle` using a role with full access to IoTCore and DynamoDB. In the code section paste the content of `dashboard_handle.py`. Create an HTTP API gateway trigger with CORS enabled. The API gateway endpoint has to be substituted inside the web interface script `s3_bucket/myScript.js` at line 6 in the `url` attribute.

## IoT Core

In IoT Core create a thing called `mydevice`, generate and download the necessary certificates (they will be used in the Mosquitto broker setup).
Create a rule that calls the lambda function `myReaction` when a message with topic `sensing` is received.

## DynamoDB

Create a DynamoDB table called `sensing_report` with primary key name `num` and type `NUMBER`.

## S3 bucket

Create a S3 bucket with public files allowed. Upload the file contained in the folder `s3_bucket` and make them public. Open in your browser the URL associated to `dashboard.html` file to access the web interface.

## For the third assignment

Some additional configurations has to be done if we want to configure the system with the architecture described for the [third assignment](../third_assignment.md). On [The Things Network website](https://www.thethingsindustries.com/docs/integrations/aws-iot/default/deployment-guide/) there is a CloudFormation template to setup the AWS integration with TTN, refer to the community tab. When the stack status is `CREATE_COMPLETE`, go to AWS IoT Core settings and in the event-based messages section turn on the option *"Thing: created, updated, deleted"*.

Create a python lambda function called `lora-decoder` and one called `lora-encoder` using a role with full access to IoTCore and DynamoDB. In the code section of each function paste the content of `lora-decoder.py` and `lora-encoder.py` respectively.

From IoT Core create a rule that calls the function `lora-decoder` when a message with topic `lorawan/+/uplink` is received.

Then create a rule that calls the function `lora-encoder` when a message with topic `actuation` and attribute `id>89` is received.