# Third individual assignment

For the third assignment the system developed for the [first](first_assignment.md) and the [second](second_assignment.md) assignments have been integrated with additional long-range low-power sensor nodes. In addition for this assignment have been developed a simple edge computing phase.

The actual version of the repository represent the work done up to the third assignment delivery.

## Network architecture

The network architecture for the third assignment is composed of a [LoRa compatible node](https://www.iot-lab.info/docs/boards/st-b-l072z-lrwan1/) of the [FIT IoT-Lab](https://www.iot-lab.info/) connected to [The Things Network](https://www.thethingsnetwork.org/) infrastructure. Since for each device connected to the network many details has to be set, we will consider only a single node, but this configuration can be simply extended to multiple devices. Using the integration from TTN to AWS Cloud we can forward the LoRaWAN messages received from the device to AWS Cloud as MQTT messages. Once the MQTT messages reach AWS Cloud, a preliminary edge computation can be done to filter the most relevant data. Then the data considered useful are sent to IoT Core to be processed in the same way described for the previous assignments.

<img src=src/network3.png width="800">

## Edge computing

For this simple prototype the edge computation is trivial, it is done only to have an idea of how this kind of operations can be implemented. In particular the only computation done on the edge is a threshold filter to ignore the outlier in the light value received by the sensor node.

The rule can be summarized as follows:

**If** *is day time* **and** *the light value is under 20 lux*, **then** *ignore the sensor reading*

## Other considerations



## Hands-on walkthrough

For the third assignment the interaction with the environment is simulated by a [ST B-L072Z-LRWAN1 board](https://www.iot-lab.info/docs/boards/st-b-l072z-lrwan1/) of the FIT IoT-Lab. Make sure to have installed [RIOTS-OS](https://github.com/RIOT-OS/RIOT) and to have active [AWS](https://www.awseducate.com/signin/SiteLogin?ec=302&startURL=%2Fstudent%2Fs%2F), [FIT IOT-Lab](https://www.iot-lab.info/testbed/login?next=%2Fdashboard) and [The Things Network](https://account.thethingsnetwork.org/) accounts.

### Setup a TTN device

From The Things Network community edition console, create an application. From the application console add a new device with manual configuration. Select the OTAA activation mode and the version 1.0.2 of LoRaWAN. Configure the AppEUI and the DevEUI at your choice, they will be used later. Then select SF9 as frequency plan and the REV B as regional parameter version. Finally generate the AppKey and finalize the device creation.

From the application console add an API key with all the individual rights granted and save it for the [AWS setup](aws/).

### Submit an IoT LAB experiment

Create a new experiment in Saclay with one (or more) [LoRaWAN compatible node](https://www.iot-lab.info/docs/boards/st-b-l072z-lrwan1/).

### Setup a LoRaWAN sensor node:

All the file needed for the RIOT application are contained in the [dedicated folder](devices/stm32_l072cz).

#### Makefile adjustments

Make sure to modify `Makefile` with the correct path of the RIOT folder and update the security configuration of the device with the values generated on the TTN website.

```
DEVEUI ?= <device EUI>
APPEUI ?= <application EUI>
APPKEY ?= <application key>
```

#### Compile and upload software

Compile the code for the sensor node with the command

    make NODE_ID=<XX>

where you have to make sure to use a different node ID number for each of the sensor nodes. Use numbers from 90 to 99.

Flash the application on the node of the experiment. If you want to access the shell of the node use the following command from the FIT IoT-Lab terminal

    nc st-lrwan1-XX 20000

If you want to use additional LoRa sensor nodes repeat this configuration for each of the nodes in the experiment.

### Integration with the previous assignments

The same system configuration seen for the [second assignment](second_assignment.md) can be used if we want to integrate the LoRa devices with the components developed in in the previous assignments.


### AWS setup

Refer to the [dedicated folder](aws/) to set up AWS. Note that for this assignment some additional components has to be added with respect to the previous assignments.