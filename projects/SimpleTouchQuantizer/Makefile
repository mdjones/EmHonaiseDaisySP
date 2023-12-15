# Project Name
TARGET = SimpleTouchQuantizer

# Sources
CPP_SOURCES = SimpleTouchQuantizer.cpp

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
