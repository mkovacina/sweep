SHELL=/bin/sh

SRC_DIR = src

all: sweep test

sweep: trace

test: trace

TRACE_DIR = $(SRC_DIR)/trace
TRACE_MAKEFILE = $(TRACE_DIR)/makefile

trace: 
	$(MAKE) -f $(TRACE_MAKEFILE)


