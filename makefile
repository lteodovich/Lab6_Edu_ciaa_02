MODULES := hal
BOARD ?= EDU-CIAA-NXP
VERBOSE=y
# MUJU ?= ../../
MUJU ?= ./muju

include $(MUJU)/module/base/makefile
