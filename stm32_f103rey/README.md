# Second assignment: wireless sensor network

## Network architecture

In the configuration developed for the second assignment the network is composed of 10 M3 nodes of the [FIT-IoT Lab](https://www.iot-lab.info/). One of the nodes is used as border router to provide connection to Internet to the hole network. The overall architecture is shown in the picture below.

<img src=../src/network2.png width="800">

## Hands-on walkthrough

To setup the system start by cloning this repository

    git clone https://github.com/lodauria/Individual_Assignments

Then make sure to have downloaded [RIOTS-OS](https://github.com/RIOT-OS/RIOT) and to have an active AWS account.

### Makefile adjustments

Make sure to modify `Makefile` with the correct path of the RIOT folder.

### Compile and upload software

In the project directory compile the code for the ARM Cortex M3 board with the following command

    make flash term

Don't forget to type also the super user password that will be required after executing this command.

### Submit an IoT LAB experiment

Setup the 10 node required.