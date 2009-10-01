default: all

- include Make.config

# build flags

INCLUDES =  -I$(PD_DIR)/src -I.  -I$(GEM_DIR)/src -I$(PD_DIR)/src
CPPFLAGS  = -fPIC -DPD -O2 -funroll-loops -fomit-frame-pointer  -ffast-math \
    -Wall -W -Wno-unused -Wno-parentheses -Wno-switch \
    -g


UNAME := $(shell uname -s)
ifeq ($(UNAME),Linux)
 INCLUDES += `pkg-config --cflags opencv`
 LDFLAGS =  --exportr-dynamic -shared
 LIBS = `pkg-config --libs opencv`
 EXTENSION = pd_linux
endif
ifeq ($(UNAME),Darwin)
 INCLUDES += -I/System/Library/Frameworks/OpenCV.framework/Headers/
 LDFLAGS =  -bundle -undefined dynamic_lookup
 LIBS =  -lm -framework OpenCV
 EXTENSION = pd_darwin
endif

.SUFFIXES = $(EXTENSION)

SOURCES = pix_artoolkit.cpp

all: $(SOURCES:.cpp=.$(EXTENSION))

%.$(EXTENSION): %.o
	$(LD) $(LDFLAGS) $(LIBS) -o $*.$(EXTENSION) $*.o $(LIBS)

.cpp.o:
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(INCLUDES) $(DEFS) -o $*.o -c $*.cpp

clean:
	rm -f pix_*.o
	rm -f pix_*.$(EXTENSION)

distro: clean all
	rm *.o
