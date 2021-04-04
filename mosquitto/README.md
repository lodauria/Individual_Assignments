# Setup

Add `config.conf` to your Mosquitto RSMB

    cp config.conf <INSTALLATION_FOLDER>/mosquitto.rsmb/rsmb/src/ 

Start the Mosquitto RSMB from the `<INSTALLATION_FOLDER>/mosquitto.rsmb/rsmb/src/` folder with this command

    ./broker_mqtts config.conf

Add `bridge.conf` to your local Mosquitto broker

    cp bridge.conf /etc/mosquitto/conf.d

Update the `bridge.conf` informations with the path of your AWS certificates and your AWS IoTCore ATS endpoint url. Then restart the borker with this command

    sudo service mosquitto restart