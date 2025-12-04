# Makefile for user application

# Specify this directory relative to the current application.
TOCK_USERLAND_BASE_DIR = ../../..

# Which files to compile.
CXX_SRCS := $(wildcard *.cc) 
CPPFLAGS += -I$(TOCK_USERLAND_BASE_DIR)/examples/ents/lorawan
# CPPFLAGS += -DRADIOLIB_DEBUG_PROTOCOL=1 

STACK_SIZE=16384
APP_HEAP_SIZE=4096

# Use the libtock-c Make system
EXTERN_LIBS += $(TOCK_USERLAND_BASE_DIR)/RadioLib

include $(TOCK_USERLAND_BASE_DIR)/AppMakefile.mk
