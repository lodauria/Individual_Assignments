# IoT course: Individual assignments

## System description

The IoT system chosen for the individual assignments is based on the idea of making university rooms smarter. By monitoring the light condition and time at which the rooms projectors are used is possible to automatically adjust the curtains and the lights so to reduce energy waste and provide students the best possible rooms conditions.

## Individual assignments

The different aspects of this system have been analyzed during the course by following the three individual assignments guidelines.

### First assignment

For the first individual assignment the goal was to develop a simplified version of the hole system based on a single sensor node implemented on the [STM32 Nucleo F401RE](https://www.st.com/en/evaluation-tools/nucleo-f401re.html) board with some real physical sensors and actuators connected.

The details of the first assignment can be found [here](first_assignment.md).

### Second assignment

In the second assignment the goal was to extend the structure by integrating multiple sensor nodes in the system and evaluating their performance. The nodes used are the [M3 boards](https://www.iot-lab.info/docs/boards/iot-lab-m3/) available in the [FIT IoT-Lab](https://www.iot-lab.info/) and they are connected using the 6LoWPAN protocols.

The details of the second assignment can be found [here](second_assignment.md).

### Third assignment

The goal of the third assignment was to complete the system with the integration of long-range nodes and the development of edge analytics. In this case the nodes used are the [ST B-L072Z-LRWAN1 boards](https://www.iot-lab.info/docs/boards/st-b-l072z-lrwan1/) available in the [FIT IoT-Lab](https://www.iot-lab.info/) and they are connected using [The Things Network](https://www.thethingsnetwork.org/) infrastructure.

The details of the third assignment can be found [here](third_assignment.md).

## System prototype

The final working prototype of the system can be seen in the [video demo](https://youtu.be/HOkv7nlomJc).

The web interface associated with the creator personal implementation is available at this [link](https://dauriaassignment1.s3.amazonaws.com/dashboard.html). Note that if you access the page when no sensor nodes are connected you can only use it in demo mode.

If you want to implement this system on your own start by cloning this repository with the following command:

		git clone https://github.com/lodauria/Individual_Assignments

Then follow the dedicated tutorials reachable from each of the assignments documents:

- [Assignment 1](first_assignment.md)
- [Assignment 2](second_assignment.md)
- [Assignment 3](third_assignment.md)

The main findings of the three individual assignments are discussed in [this blog post](https://lodauria.medium.com/iot-system-to-make-universities-smarter-27f18c4c7cf).