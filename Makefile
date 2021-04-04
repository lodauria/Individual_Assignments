APPLICATION = Assignment_1
BOARD ?= nucleo-f401re
RIOTBASE ?= $(CURDIR)/../RIOT

# Default to using ethos for providing the uplink when not on native
UPLINK ?= ethos

# Modules and packages
USEMODULE += analog_util
USEMODULE += xtimer
USEMODULE += gnrc_netdev_default
USEMODULE += auto_init_gnrc_netif
USEMODULE += gnrc_ipv6_default
USEMODULE += emcute
USEMODULE += gnrc_netif_single
USEMODULE += stdio_ethos
USEMODULE += gnrc_uhcpc

FEATURES_REQUIRED = periph_gpio
FEATURES_REQUIRED = periph_adc

USEPKG += jsmn

# Allow for env-var-based override of the nodes name (EMCUTE_ID)
ifneq (,$(EMCUTE_ID))
  CFLAGS += -DEMCUTE_ID=\"$(EMCUTE_ID)\"
endif

# include UHCP client
USE_DHCPV6 ?= 0

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

# Ethos/native TAP interface and UHCP prefix can be configured from make command
TAP ?= tap0
# Based on the PC network configuration sometimes the prefix has to be changed
# IPV6_PREFIX ?= fe80::/64
IPV6_PREFIX ?= fe80::6000:0:0:0/66

# The Broker address, port and the MQTT topics that will be used
SERVER_ADDR = fe80::1
SERVER_PORT = 1885
MQTT_TOPIC_S = sensing
MQTT_TOPIC_A = actuation

CFLAGS += -DSERVER_ADDR='"$(SERVER_ADDR)"'
CFLAGS += -DSERVER_PORT=$(SERVER_PORT)
CFLAGS += -DMQTT_TOPIC_S='"$(MQTT_TOPIC_S)"'
CFLAGS += -DMQTT_TOPIC_A='"$(MQTT_TOPIC_A)"'

ETHOS_BAUDRATE ?= 115200
include $(CURDIR)/Makefile.ethos.conf

include $(RIOTBASE)/Makefile.include

.PHONY: host-tools

host-tools:
	$(Q)env -u CC -u CFLAGS $(MAKE) -C $(RIOTTOOLS)