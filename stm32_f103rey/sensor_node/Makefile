APPLICATION = ind_assignment
BOARD ?= iotlab-m3
RIOTBASE ?= $(CURDIR)/../../../RIOT

# Modules and packages
USEMODULE += gnrc_netdev_default
USEMODULE += auto_init_gnrc_netif
USEMODULE += gnrc_lorawan
USEMODULE += gnrc_pktdump
USEMODULE += gnrc_neterr
USEMODULE += analog_util
USEMODULE += xtimer
USEMODULE += gnrc_ipv6_default
USEMODULE += emcute
USEMODULE += gnrc_netif_single
USEMODULE += stdio_ethos
USEMODULE += gnrc_uhcpc
USEMODULE += gnrc_icmpv6_error
USEMODULE += gnrc_ipv6_router_default
USEMODULE += gnrc_udp
USEMODULE += gnrc_rpl
USEMODULE += auto_init_gnrc_rpl
USEMODULE += gnrc_icmpv6_echo

USEPKG += jsmn

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

# The Broker address, port and the MQTT topics that will be used
SERVER_ADDR = 2001:660:5307:30ff::5
SERVER_PORT = 1885
MQTT_TOPIC_S = sensing
MQTT_TOPIC_A = actuation
NODE_ID = 1

CFLAGS += -DSERVER_ADDR='"$(SERVER_ADDR)"'
CFLAGS += -DSERVER_PORT=$(SERVER_PORT)
CFLAGS += -DMQTT_TOPIC_S='"$(MQTT_TOPIC_S)"'
CFLAGS += -DMQTT_TOPIC_A='"$(MQTT_TOPIC_A)"'
CFLAGS += -DNODE_ID='"$(NODE_ID)"'

DEVELHELP ?= 1

# Instead of simulating an Ethernet connection, we can also simulate
# an IEEE 802.15.4 radio using ZEP
USE_ZEP ?= 0

# set the ZEP port for native
ZEP_PORT_BASE ?= 17754
ifeq (1,$(USE_ZEP))
  TERMFLAGS += -z [::1]:$(ZEP_PORT_BASE)
  USEMODULE += socket_zep
endif

include $(RIOTBASE)/Makefile.include

# Set a custom channel if needed
include $(RIOTMAKE)/default-radio-settings.inc.mk
