SHELL = /bin/sh

all:
	$(if $(V), $(MAKE) -f makefile_user V=$(V) ; $(MAKE) -f makefile_system all,$(error Usage: make V=<experiment name>))

tests:

new: main

allclean: clean
	-rm -f *.d
	-rm -f exp_touch/*
	-rm -f gen_probe gen_priority
	-rm -f user_actions.c user_actions.h user_actions.tbl

clean:
	-rm -f \#*
	-rm -f a.out *.o core *.out.new *~
	-rm -f probe.dat priority.dat error.log
