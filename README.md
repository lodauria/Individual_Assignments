# IoT course: First individual assignment

## What is the problem and why do you need IoT?

For this assignment has been developed a prototype of an IoT system to be used in university's rooms. By using a photoresistor and an Hall sensor we are able to detect in which light condition the projector is usually used or not and at what time. This can be done with a periodical sampling of the sensors state.

From this data a model can be derived and used to automatically control the room lights with a relay and the windows curtains with a DC motor so to automatically have the ideal light condition in each situation.

The logic used to control the actuators can be summarized with these two rules:
- If the projector is open **AND** is lecture time **AND** there is too much light **=>** (if lights are on **=>** switch lights off) **AND** (if there is still too much light **AND** curtains are open **=>** close the curtains)
- If the projector is closed **AND** is lecture time **AND** there is low light **=>** (if curtains are closed **=>** open curtains) **AND** (if there is still too low light **AND** lights are off **=>** switch on the lights)

## What data are collected and by which sensors?

The light condition is measured based on the electric resistance of a photoresistor (__spec__) placed in the room. Using the circuit below is possible to measure the photoresistor electrical resistance from an analog pin of the STM32 board.

__image__

To be more tollerant to noise each measurement is computed as the average of 10 voltage readings made with a 0.5s delay. The measurement of illuminance in lux is computed using the following formula:

__formula__

where ... 
This measurement is repeated every 10s.

The Hall sensor (__spec__) is a simple digital sensor that can detect if a magnet placed on the end of the projector screen is near the sensor placed on the wall. In this way when the screen is rolled up (and so the projector is switched off) the Hall sensor detects the magnetic field of the magnet, while it doesn't when the screen is unrolled. The digital value given by this sensor is sampled after the illuminance measurement (every 10s).

## What are the connected components, the protocols to connect them and the overall IoT architecture?

The network diagram is the following.

# Hands-on walkthrough

## Photoresistor setup

## Hall sensor setup
