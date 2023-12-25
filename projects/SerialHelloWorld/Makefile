# Project Name
TARGET = SerialHelloWorld

# Sources
CPP_SOURCES = SerialHelloWorld.cpp

# Library Locations
#LIBDAISY_DIR := ../../libDaisy/
#DAISYSP_DIR := ../../DaisySP/

ifndef LIBDAISY_DIR
$(error LIBDAISY_DIR is not set)
endif

ifndef DAISYSP_DIR
$(error DAISYSP_DIR is not set)
endif


# Core location, and generic Makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile

LDFLAGS += -u _printf_float
