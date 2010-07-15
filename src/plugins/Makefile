default: o_objects 
	@echo "in order to build external plugins, run 'make plugins'"

include ../Makefile.subdir

all:
	make -C ..

RECORDPLUGINS=recordQT recordQT4L recordV4L recordV4L2
VIDEOPLUGINS=videoDarwin videoDC1394 videoDS videoDV4L videoHALCON videoSGI videoV4L videoV4L2 videoVFW
FILMPLUGINS=filmAVI filmAVIPLAY filmDarwin filmDS filmGMERLIN filmMPEG1 filmMPEG3 filmQT filmQT4L


PLUGINS=$(FILMPLUGINS) $(VIDEOPLUGINS) $(RECORDPLUGINS)

.PHONY: all default $(PLUGINS) plugins

plugins: $(PLUGINS)
	-echo $(PLUGINS)

$(PLUGINS):
	if test -e $@/Makefile; then make -k -C $@; fi

%_install: 
	if test -e $(@:_install=)/Makefile; then make -k -C $(@:_install=) install; fi

install:: $(PLUGINS:%=%_install)

%_clean:
	if test -e $(@:_clean=)/Makefile; then make -k -C $(@:_clean=) clean; fi

clean:: $(PLUGINS:%=%_clean)

