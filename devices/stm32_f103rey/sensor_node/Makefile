APPLICATION = wireless_sensor_node
BOARD ?= iotlab-m3
RIOTBASE ?= $(CURDIR)/../../../../RIOT

# Modules and packages
USEMODULE += xtimer
USEMODULE += gnrc_netdev_default
USEMODULE += auto_init_gnrc_netif
USEMODULE += gnrc_ipv6_default
USEMODULE += emcute
USEMODULE += gnrc_netif_single
USEMODULE += gnrc_uhcpc

USEPKG += jsmn

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

DEFAULT_CHANNEL ?= 13

# The broker address, port, MQTT topics and device ID that will be used
SERVER_ADDR ?= 2001:660:3207:400::64
SERVER_PORT ?= 1885
MQTT_TOPIC_S ?= sensing
MQTT_TOPIC_A ?= actuation
NODE_ID ?= 1

CFLAGS += -DSERVER_ADDR='"$(SERVER_ADDR)"'
CFLAGS += -DSERVER_PORT=$(SERVER_PORT)
CFLAGS += -DMQTT_TOPIC_S='"$(MQTT_TOPIC_S)"'
CFLAGS += -DMQTT_TOPIC_A='"$(MQTT_TOPIC_A)"'
CFLAGS += -DNODE_ID=$(NODE_ID)

include $(RIOTBASE)/Makefile.include

# Set a custom channel
include $(RIOTMAKE)/default-radio-settings.inc.mk