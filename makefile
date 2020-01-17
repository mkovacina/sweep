SHELL=/bin/sh

SRC_DIR = src

#all: sweep test

all-follow: build-all-follow build-tests run-sweep run-tests

build-all-follow:
	$(MAKE) --directory=src V=all_follow

build-tests:
	$(MAKE) --directory=src -f makefile-tests

run-sweep:
	./sweep exp/all_follow.exp 1234

run-tests:
	./sweep-tests