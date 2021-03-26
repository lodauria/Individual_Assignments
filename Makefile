APPLICATION = Assignment_1
BOARD ?= nucleo-f401re
RIOTBASE ?= $(CURDIR)/../RIOT

# Default to using ethos for providing the uplink when not on native
UPLINK ?= ethos

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
# IPV6_PREFIX ?= fe80::/64
# Based on the PC network configuration sometimes the prefix has to be changed
IPV6_PREFIX ?= fe80::6000:0:0:0/66


# The Broker address, port and the default MQTT topic to subscribe.
#tapbr0 address
SERVER_ADDR = fe80::1
SERVER_PORT = 1885
MQTT_TOPIC = status

CFLAGS += -DSERVER_ADDR='"$(SERVER_ADDR)"'
CFLAGS += -DSERVER_PORT=$(SERVER_PORT)
CFLAGS += -DMQTT_TOPIC='"$(MQTT_TOPIC)"'

ETHOS_BAUDRATE ?= 115200
include $(CURDIR)/Makefile.ethos.conf

include $(RIOTBASE)/Makefile.include

.PHONY: host-tools

host-tools:
	$(Q)env -u CC -u CFLAGS $(MAKE) -C $(RIOTTOOLS)