APPLICATION = lora_sensor_node
BOARD ?= b-l072z-lrwan1
RIOTBASE ?= $(CURDIR)/../../../RIOT

# Default radio driver is Semtech SX1276 (used by the B-L072Z-LRWAN1 board)
DRIVER ?= sx1276

# Default region is Europe and default band is 868MHz
LORA_REGION ?= EU868

# Include the Semtech-loramac packages
USEPKG += semtech-loramac
USEMODULE += semtech_loramac_rx

# Modules and packages
USEMODULE += xtimer
USEMODULE += $(DRIVER)
USEMODULE += fmt

USEPKG += jsmn

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

# Sensor node ID
NODE_ID = 90

# The things network device configuration
DEVEUI ?= 9090909090909090
APPEUI ?= 5775577557755775
APPKEY ?= D12FF32610C3A92051B209FC3A023559

CFLAGS += -DNODE_ID=$(NODE_ID)

# Default IotLab Config to run the test
ifneq (,$(filter iotlab%,$(MAKECMDGOALS)))
  IOTLAB_NODES ?= 1
  IOTLAB_TYPE  ?= st-lrwan1:sx1276
  IOTLAB_SITE  ?= saclay
  include $(RIOTBASE)/dist/testbed-support/Makefile.iotlab
endif

ifneq (,$(filter test,$(MAKECMDGOALS)))
  DEFAULT_MODULE += test_utils_interactive_sync
endif

include $(RIOTBASE)/Makefile.include

ifndef CONFIG_KCONFIG_USEMODULE_LORAWAN
  # OTAA compile time configuration keys
  CFLAGS += -DCONFIG_LORAMAC_APP_KEY_DEFAULT=\"$(APPKEY)\"
  CFLAGS += -DCONFIG_LORAMAC_APP_EUI_DEFAULT=\"$(APPEUI)\"
  CFLAGS += -DCONFIG_LORAMAC_DEV_EUI_DEFAULT=\"$(DEVEUI)\"
endif