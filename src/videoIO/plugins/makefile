
# ATTENTION: ld must not be used for linking ! use g++, otherwise we get errors

INCLUDE=-I/usr/include/gstreamer-0.10 -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include/ -I/usr/include/libxml2/
LDPATH=-L/usr/local/lib

all: FRDummy FileReadGst FileWriteGst

FRDummy: FRDummy.h FRDummy.cpp
	g++ -fPIC -c FRDummy.cpp
	g++ -shared -o FRDummy.so FRDummy.o

FileReadQT4L: FileReadQT4L.h FileReadQT4L.cpp
	g++ -fPIC -c FileReadQT4L.cpp
	g++ -shared -o FileReadQT4L.so FileReadQT4L.o -lquicktime

FileReadGst: FileReadGst.h FileReadGst.cpp
	g++ -DDEBUG -fPIC ${INCLUDE} -c FileReadGst.cpp
#	gcc -fPIC ${INCLUDE} -c VIOGstVideoSink.c
	g++ -shared -o FileReadGst.so FileReadGst.o -lgstreamer-0.10 -lgstapp-0.10
# 	ld -shared -o FileReadGst.so FileReadGst.o VIOGstVideoSink.o -lgstvideo-0.10 -lgstbase-0.10 -lgstreamer-0.10  -lglib-2.0
# 	ld -static -o FileReadGst.so FileReadGst.o 

FileWriteGst: FileWriteGst.h FileWriteGst.cpp
	g++ -fPIC ${INCLUDE} -c FileWriteGst.cpp
	g++ -shared -o FileWriteGst.so FileWriteGst.o -lgstreamer-0.10 -lgstapp-0.10
	
clean:
	rm *.o *.so
