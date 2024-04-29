# Project Name
TARGET = InitQuant

# For debugging float in hw.Print
LDFLAGS += -u _printf_float

# Sources
CPP_SOURCES = InitQuant.cpp

# Library Locations
LIBDAISY_DIR ?= ../../lib/libDaisy/
DAISYSP_DIR ?= ../../lib/DaisySP/

# Core location, and generic Makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile
