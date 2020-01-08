SHELL=/bin/sh

SRC_DIR = src

#all: sweep test

all-follow: build-all-follow
	./sweep exp/all_follow.exp 1234

build-all-follow:
	$(MAKE) --directory=src V=all_follow



