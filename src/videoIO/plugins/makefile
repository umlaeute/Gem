
INCLUDE=-I/usr/include/gstreamer-0.10 -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include/ -I/usr/include/libxml2/

all: FRDummy FileReadQT4L FileReadGst

FRDummy: FRDummy.h FRDummy.cpp
	g++ -fPIC -c FRDummy.cpp
	ld -shared -o FRDummy.so FRDummy.o

FileReadQT4L: FileReadQT4L.h FileReadQT4L.cpp
	g++ -fPIC -c FileReadQT4L.cpp
	ld -shared -o FileReadQT4L.so FileReadQT4L.o -lquicktime

FileReadGst: FileReadGst.h FileReadGst.cpp
	g++ -fPIC ${INCLUDE} -c FileReadGst.cpp
	ld -shared -o FileReadGst.so FileReadGst.o -lgstreamer-0.10

	
clean:
	rm *.o *.so
