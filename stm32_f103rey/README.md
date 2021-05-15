# Second assignment: wireless sensor network

## Network architecture

In the configuration developed for the second assignment the network is composed of multiple M3 nodes of the [FIT-IoT Lab](https://www.iot-lab.info/). One of the nodes is used as border router to provide connection to Internet to the hole network. The additional A8-M3 node is used to run the Mosquitto RSMB broker connected with the nodes and the bridged Mosquitto broker connected to IoT Core with the proper certificates. The overall architecture is shown in the picture below.

<img src=../src/network2.png width="800">

## Hands-on walkthrough

To setup the system start by cloning this repository

    git clone https://github.com/lodauria/Individual_Assignments

Then make sure to have downloaded [RIOTS-OS](https://github.com/RIOT-OS/RIOT) and to have an active AWS account.

### Submit an IoT LAB experiment

Create a new experiment in Saclay with some M3 nodes (suggested number is 5) and an A8-M3 node. Make sure that their position allows a wireless connection.

### Setup A8-M3 node

To run the Mosquitto brokers on the A8-M3 node follow the istructions of the [dedicated folder](../mosquitto). You will need the IPv6 global address of the A8-M3 node to setup the sensor nodes.

### Setup a M3 border router node:
#### Makefile adjustments

Make sure to modify `Makefile` with the correct path of the RIOT folder. If needed you can also modify the `DEFUALT_CHANNEL` used by the 802.15.4 mesh network.

#### Compile and upload software

In the project directory compile the code for the ARM Cortex M3 board with the command

    make

Upload the compiled program to one of the M3 nodes of the experiment. Now start the border router from Saclay terminal with the following command:

    sudo ethos_uhcpd.py m3-10 tap4 2001:660:3207:04c9::1/64

Here the M3 node is assume to be the number 10, change this number based on the nodes assigned to your experiment. The number of the tap interface and the IP prefix can also be changed based on the availability of the resources on the Saclay terminal. After this command leave the terminal open.

### Setup the remaining M3 sensor nodes:
#### Makefile adjustments

Make sure to modify `Makefile` with the correct path of the RIOT folder. If needed you can also modify the `DEFUALT_CHANNEL` used by the 802.15.4 mesh network accordingly to the one choosed when compiling the bored router program.

#### Compile and upload software

In the project directory compile the code for each of the ARM Cortex M3 boards used as sensors with the command

    make NODE_ID=<XX> SERVER_ADDRESS=<A8_M3 node address>

Make sure to use a different node ID numbers for each of the sensor nodes. Use numbers from 1 to 100. The server address is the global IPv6 address of the A8-M3 node used to run the Mosquitto RSMB broker.

Upload each compiled program to the M3 nodes of the experiment that have to be used as sensor nodes. If you want to access the shell of a sensor node use the following command:

    nc m3-11 20000

Here the M3 node is assume to be the number 11, change this number based on the node you are interested in.