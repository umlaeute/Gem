default: all

TARGETS=pix_opencv
.PHONY: $(TARGETS)

GEM_DIR=$(shell pwd)/../..


all: $(TARGETS)


pix_opencv:
	if test -d $@; then make -k -C $@ GEM_DIR=$(GEM_DIR); fi
