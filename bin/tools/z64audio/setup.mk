MKFILE_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
MKFILE_DIR  := $(dir $(MKFILE_PATH))

ifeq (,$(wildcard settings.mk))
  $(shell echo "PATH_EXTLIB = $(MKFILE_DIR)ExtLib/" > settings.mk)
endif

include settings.mk

ifeq (,$(wildcard $(PATH_EXTLIB)))
  $(shell git clone --recurse-submodules https://github.com/rankaisija64/ExtLib.git $(PATH_EXTLIB))
endif