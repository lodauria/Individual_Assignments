APPLICATION = Assignment_1
BOARD ?= nucleo-f401re
RIOTBASE ?= $(CURDIR)/../RIOT
DEVELHELP ?= 1

FEATURES_REQUIRED = periph_gpio

include $(RIOTBASE)/Makefile.include