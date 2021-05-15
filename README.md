# IoT course: Individual assignments

## Prototype description

Introducing a prototype of an IoT system to be used in university's rooms. By using a photocell and an Hall sensor is possible to detect in which light condition and at what hours the projector is usually used or not. This can be done with a periodical sampling of the sensors state.

From this data a model can be derived and used to automatically control the room lights with a relay and the windows curtains with a DC motor so to automatically have the ideal light condition in each situation.

In this prototype to control the actuators is used a simple logic that can be summarized with these two rules:
- If the projector is open **AND** there is too much light **AND** is lecture time **=>** (if lights are on **=>** switch lights off) **ELSE** (if lights are off **AND** curtains are open **=>** close the curtains)
- If the projector is closed **AND** there is low light **AND** is lecture time **=>** (if curtains are closed **=>** open curtains) **ELSE** (if curtains are open **AND** lights are off **=>** switch on the lights).

## Sensors description

### Light sensor

The light condition is measured based on the electric resistance of a photocell (GL5528) placed in the room. Using the circuit below is possible to measure the photocell electrical resistance from an analog pin of an STM32 board.

<img src="./src/photo_res.jpg" width="300"> <img src="./src/photo_circ.png" width="500">

Each raw measurement uses a 12 bit ADC, so the signal will be between 0 and 4095, this values are then mapped in a 10 to 100 lux scale. The analog measurement is repeated every 10s.

### Hall sensor

The Hall sensor (A3144) is already mounted on a simple sensor module which provides a digital signal LOW when a magnetic field is detected, HIGH otherwise. With a 10K potentiometer on the board is possible to regulate the sensibility based on the type of magnet used so to trigger the sensor at the desired distance (around 2cm in this case).

<img src="./src/hall_sensor.jpg" width="600">

This sensor is used to detect if a magnet placed on the end of the projector screen is near the sensor placed on the wall. In this way when the screen is rolled up (and so the projector is switched off) the Hall sensor detects the magnetic field of the magnet, while it doesn't when the screen is unrolled. The digital value given by this sensor is sampled after the light measurement (every 10s).

## System configuration

### Devices setup

#### First assignment:

For the first assignment the interactions with the environment are mediated only by a single STM32-F401RE board connected to internet through a PC. For more informations about the network architecture and the devices setup visit the [dedicated folder](./stm32_f401re/).

#### Second assignment

For the second assignment the interaction with the environment are carried out by multiple M3 nodes of the [FIT-IoT Lab](https://www.iot-lab.info/). The overall wireless sensor network is connected to the Internet through an A8-M3 node. For more informations about the network architecture and the devices setup visit the [dedicated folder](./stm32_f103rey/).

### Mosquitto and AWS setup

Read the dedicated indications to set up the other components:
- [Mosquitto setup](./mosquitto/)
- [AWS setup](./aws/)

## Performance evaluation

The performance evaluation of the wireless sensor network required in the second assignment can be found [here](./stm32_f103rey/Evaluation.md).

## Prototype demo

The web interface associated with the creator personal implementation is available at this [link](https://dauriaassignment1.s3.amazonaws.com/dashboard.html). Note that if you access the page when the creator hardware is not connected to AWS Cloud you can only use it in demo mode.