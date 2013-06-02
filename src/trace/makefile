# makefile for trace

include ../makefile_common

SOURCES = trace.c

include $(SOURCES:.c=.d)

all: trace

trace: $(objdir)/trace/trace.o $(includedir)/trace/trace.h

$(objdir)/trace.o: trace.c trace.h


$(includedir)/trace.h: trace.h
