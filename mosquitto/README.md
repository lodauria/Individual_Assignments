# Mosquitto setup

Add or substitute `config.conf` in your Mosquitto RSMB folder:

    cp config.conf <INSTALLATION_FOLDER>/mosquitto.rsmb/rsmb/src/ 

If not done yet, compile the code (refer to [Mosquitto repository](https://github.com/eclipse/mosquitto.rsmb)). Then start the Mosquitto RSMB from the `<INSTALLATION_FOLDER>/mosquitto.rsmb/rsmb/src/` folder with this command:

    ./broker_mqtts config.conf

Add `bridge.conf` to your local Mosquitto broker:

    cp bridge.conf /etc/mosquitto/conf.d

Update the `bridge.conf` informations with the path of your AWS certificates and your AWS IoTCore ATS endpoint url. Then restart the borker with this command:

    sudo service mosquitto restart


Connect via SSH to FIT-IoT Lab and copy all the file needed by the Mosquitto brokers in the `~/A8` folder:

    scp <file_name> <user>@saclay.iot-lab.info:~/A8