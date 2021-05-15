# First assignment: local STM32-FE401RE board

## Network architecture

In the configuration developed for the first assignment the network uses Mosquitto RSMB to communicate via ETHOS with the board and a Mosquitto broker with authentication to communicate with IoT Core. In the AWS Cloud IoT Core calls a lambda function when new data are received. This function is used to implement the collective intelligence and to store the new data in a DynamoDB table. A web page hosted on an S3 bucket realizes a simple user interface. This page periodically calls an API gateway to get the data from the DynamoDB and, if requested, sends actuation command to the IoT Core MQTT broker. The overall network is represented in this scheme.

<img src=../src/network1.png width="800">

## Hands-on walkthrough

### Makefile adjustments

Make sure to modify `Makefile` with the correct path of the RIOT folder and the IPv6 prefix with the best option for your PC network configuration.

### Compile and upload software

In the project directory compile and upload the program on your STM32-FE401RE board with the following command

    make flash term

Don't forget to type also the super user password that will be required after executing this command.

### Connect the electronic components

Connect the 2 sensors, the relay and the motor as shown below.

<img src=../src/wiring.jpg width="800">