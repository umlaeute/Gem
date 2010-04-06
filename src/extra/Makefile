default: 

TARGETS=pix_opencv pix_artoolkit
.PHONY: $(TARGETS) default all

GEM_DIR=$(shell pwd)/../..


default: $(TARGETS)


$(TARGETS):
	if test -d $@; then make -k -C $@ GEM_DIR=$(GEM_DIR); fi

