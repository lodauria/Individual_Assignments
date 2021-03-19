APPLICATION = Assignment_1
BOARD ?= nucleo-f401re
RIOTBASE ?= $(CURDIR)/../RIOT
DEVELHELP ?= 1

USEMODULE += analog_util
USEMODULE += xtimer

FEATURES_REQUIRED = periph_gpio
FEATURES_REQUIRED = periph_adc
FEATURES_OPTIONAL += periph_rtc

include $(RIOTBASE)/Makefile.include