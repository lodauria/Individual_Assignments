# Setup

Add `config.conf` to your Mosquitto rsmb

    cp config.conf <INSTALLATION_FOLDER>/mosquitto.rsmb/rsmb/src/ 

Add `bridge.conf` to your local Mosquitto broker

    cp bridge.conf /etc/mosquitto/conf.d

Update the `bridge.conf` informations with the path of your AWS certificates and your AWS IoTCore ATS endpoint url