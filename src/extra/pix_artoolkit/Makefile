default: all

-include Make.config

.SUFFIXES = $(EXTENSION)
INCLUDES+=-I../../
LDFLAGS+=-shared -Wl,--export-dynamic

SOURCES = pix_artoolkit.cpp

all: $(SOURCES:.cpp=.$(EXTENSION))

%.$(EXTENSION): %.o
	$(CXX) $(LDFLAGS) $(LIBS) -o $*.$(EXTENSION) $*.o $(LIBS)

.cpp.o:
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(INCLUDES) $(DEFS) -o $*.o -c $*.cpp

clean:
	rm -f pix_*.o
	rm -f pix_*.$(EXTENSION)

distro: clean all
	rm *.o
