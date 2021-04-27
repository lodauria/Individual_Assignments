APPLICATION = ind_assignment
BOARD ?= iotlab-m3
RIOTBASE ?= $(CURDIR)/../../../RIOT

# Default to using ethos for providing the uplink when not on native
UPLINK ?= ethos

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

# use SX1276 by default
DRIVER ?= sx1276

USEMODULE += $(DRIVER)

include $(RIOTBASE)/Makefile.include

########################### COMPILE TIME CONFIGURATION ########################
# NOTE: The following options can be configured on runtime as well using
# `ifconfig`

# The configurations can be accessed via menuconfig using `make menuconfig`

# OTAA compile time configuration keys

# Check if being configured via Kconfig
ifndef CONFIG_KCONFIG_USEMODULE_LORAWAN

  CFLAGS += -DCONFIG_LORAMAC_APP_KEY_DEFAULT=\"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\"
  CFLAGS += -DCONFIG_LORAMAC_APP_EUI_DEFAULT=\"BBBBBBBBBBBBBBBB\"
  CFLAGS += -DCONFIG_LORAMAC_DEV_EUI_DEFAULT=\"CCCCCCCCCCCCCCCC\"

  # For TTN, It's necessary to set the RX2 DR to 3 in EU_868 region
  # CFLAGS += -DCONFIG_LORAMAC_DEFAULT_RX2_DR_3

  # Comment/uncomment as necessary
  CFLAGS += -DCONFIG_LORAMAC_DEFAULT_JOIN_PROCEDURE_OTAA
  # CFLAGS += -DCONFIG_LORAMAC_DEFAULT_JOIN_PROCEDURE_ABP

  # Uncomment and replace with proper keys for joining with ABP
  # NOTE: This values will be overriten in case of OTAA.
  # CFLAGS += -DCONFIG_LORAMAC_DEV_ADDR_DEFAULT=\"00000000\"
  # CFLAGS += -DCONFIG_LORAMAC_NWK_SKEY_DEFAULT=\"00000000000000000000000000000000\"
  # CFLAGS += -DCONFIG_LORAMAC_APP_SKEY_DEFAULT=\"00000000000000000000000000000000\"

  ## For FIT-IoT Lab usage. Use the highest DR since gateway is nearby.
  # If uncommented, the default value (DR0) is used.
  # Note this value is also used for the OTAA.
  # CFLAGS += -DCONFIG_LORAMAC_DEFAULT_DR_5

  # # Set default messages to unconfirmable
  CFLAGS += -DCONFIG_LORAMAC_DEFAULT_TX_MODE_UNCNF

  # Set region
  CFLAGS += -DCONFIG_LORAMAC_REGION_EU_868
endif

# We can reduce the size of the packet buffer for LoRaWAN, since there's no IP
# support. This will reduce RAM consumption.
# Set GNRC_PKTBUF_SIZE via CFLAGS if not being set via Kconfig.
ifndef CONFIG_GNRC_PKTBUF_SIZE
  CFLAGS += -DCONFIG_GNRC_PKTBUF_SIZE=512
endif

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