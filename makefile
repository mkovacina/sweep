SHELL=/bin/sh

SRC_DIR = src

#all: sweep test

life: build-life run-life
#life: build-life build-tests run-life run-tests

all-follow: build-all-follow build-tests run-sweep run-tests

build-all-follow:
	$(MAKE) --directory=src V=all_follow

build-life:
	$(MAKE) --directory=src V=life

build-tests:
	$(MAKE) --directory=src -f makefile-tests

run-sweep:
	./sweep exp/all_follow.exp 1234

run-life: 
	./sweep exp/life.exp 1234

run-tests: build-tests
	./src/sweep-tests

clean:
	rm -f sweep
	rm -f sweep-tests
	$(MAKE) --directory=src allclean
