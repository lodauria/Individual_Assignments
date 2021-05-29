# Mosquitto setup

## Mosquitto RSMB

The RSMB is used to communicate with the RIOT sensor nodes. For more details about the installation of the Mosquitto RSMB refer to the official [Mosquitto repository](https://github.com/eclipse/mosquitto.rsmb). Below is reported a short guide on ho how to setup and start the broker on your local PC or on FIT IoT-Lab.

### Start broker on local PC (1st assignment)

To configure the Mosquitto RSMB copy the `config.conf` file in your Mosquitto RSMB folder:

    cp config.conf <INSTALLATION_FOLDER>/mosquitto.rsmb/rsmb/src/ 

To start the Mosquitto RSMB, from the folder`<INSTALLATION_FOLDER>/mosquitto.rsmb/rsmb/src/` run the command:

    ./broker_mqtts config.conf

Keep the terminal open while using the broker.

### Start broker on local PC bridged with A8-M3 node (2nd assignment)

To use the local sensor node in addition to the M3 nodes in the second assignment, modify the `config.conf` file by changing the outgoing bridge settings with the A8-M3 node IPv6 global address.

```
# MQTT-S outgoing bridge
connection local_bridge_to_mosquitto
	address [<A8-M3 node IPv6 global address>]:1883
	topic sensing out
	topic actuation in
```

Then copy the `config.conf` file in your Mosquitto RSMB folder:

    cp config.conf <INSTALLATION_FOLDER>/mosquitto.rsmb/rsmb/src/ 

To start the Mosquitto RSMB, from the folder`<INSTALLATION_FOLDER>/mosquitto.rsmb/rsmb/src/` run the command:

    ./broker_mqtts config.conf

Keep the terminal open while using the broker.

### Start broker on A8-M3 node (2nd assignment)

Connect via SSH to FIT IoT-Lab and copy the `config.conf` file needed by the RSMB in the `~/A8` folder:

    scp config.conf <user>@saclay.iot-lab.info:~/A8

Then, from the A8-M3 node terminal, go to the A8 folder and start the broker with the following command:

    broker_mqtts config.conf

Keep the terminal open while using the broker.

## Mosquitto broker

The Mosquitto broker will bridge the local RSMB to IoT Core using the necessary security certificates. For more details about the installation refer to the [Mosquitto repository](https://github.com/eclipse/mosquitto).

### Start broker on local PC (1st assignment) 

To configure the Mosquitto broker copy the file `bridge.conf` in the software folder:

    cp bridge.conf /etc/mosquitto/conf.d

Update the `bridge.conf` information with the paths of your AWS certificates and your AWS IoTCore ATS endpoint URL. Then restart the broker with this command:

    sudo service mosquitto restart

### Start broker on A8-M3 node (2nd assignment)

Connect via SSH to FIT IoT-Lab and copy all the file needed by the Mosquitto broker in the `~/A8` folder:

    scp <file_name> <user>@saclay.iot-lab.info:~/A8

In particular the three AWS certificates files and the `bridge.conf` file has to be present on the Saclay terminal.

Then, from the A8-M3 node terminal, go to the A8 folder and start the broker with the following command:

    mosquitto -c bridge.conf

Keep the terminal open while using the broker.