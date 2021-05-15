# AWS setup

## Lambda functions

Create a python lambda function called `myReaction` using a role with full access to IoTCore and DynamoDB. In the code section paste the content of `myReaction.py`.

Create a python lambda function called `dashboard_handle` using a role with full access to IoTCore and DynamoDB. In the code section paste the content of `dashboard_handle.py`. Create an HTTP API gateway trigger with CORS enabled. The API gateway endpoint has to be substituted inside the web interface script `s3_bucket/myScript.js` at line 6 in the `url` attribute.

## IoT Core

In IoT Core create a thing called `mydevice`, generate and download the necessary certificates (they will be used in the Mosquitto broker setup).
Create a rule connected to `mydevice` that calls the lambda function `myReaction` when a message with topic `sensing` is received.

## DynamoDB

Create a DynamoDB table called `sensing_report` with primary key name `num` and type `NUMBER`.

## S3 bucket

Create a S3 bucket with public files allowed. Upload the file contained in the folder `s3_bucket` and make them public. Open in your browser the URL associated to `dashboard.html` file to access the web interface.