default: 

TARGETS=pix_opencv pix_artoolkit pdgst pix_drum pix_mano
.PHONY: $(TARGETS) default all

GEM_DIR=$(shell pwd)/../..


default: $(TARGETS)


$(TARGETS):
	if test -d $@; then make -k -C $@ GEM_DIR=$(GEM_DIR); fi


%_install: 
	if test -e $(@:_install=)/Makefile; then make -k -C $(@:_install=) install; fi

install:: $(TARGETS:%=%_install)

%_clean:
	if test -e $(@:_clean=)/Makefile; then make -k -C $(@:_clean=) clean; fi

clean:: $(TARGETS:%=%_clean)
