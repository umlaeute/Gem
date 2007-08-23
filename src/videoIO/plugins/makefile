####################################
# videoIO for PD - makefile
# --------------------------
# this is the makefile for the plugins, the videoIO core will be build
# with the GEM buildsystem.
###################################

# ATTENTION: ld must not be used for linking ! use g++, otherwise we get errors

# the include paths may be different on your system
INCLUDE=-I/usr/local/include/gstreamer-0.10 -I/usr/include/gstreamer-0.10 -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include/ -I/usr/include/libxml2/

all: FileReadGst FileWriteGst DeviceReadGst

FileReadGst: FileReadGst.h FileReadGst.cpp
	g++ -DDEBUG -fPIC ${INCLUDE} -c FileReadGst.cpp
	g++ -shared -o FileReadGst.so FileReadGst.o -lgstreamer-0.10 -lgstapp-0.10

FileWriteGst: FileWriteGst.h FileWriteGst.cpp
	g++ -fPIC ${INCLUDE} -c FileWriteGst.cpp
	g++ -shared -o FileWriteGst.so FileWriteGst.o -lgstreamer-0.10 -lgstapp-0.10
	
DeviceReadGst: DeviceReadGst.h DeviceReadGst.cpp
	g++ -fPIC ${INCLUDE} -c DeviceReadGst.cpp
	g++ -shared -o DeviceReadGst.so DeviceReadGst.o -lgstreamer-0.10 -lgstapp-0.10
	
clean:
	rm *.o *.so
