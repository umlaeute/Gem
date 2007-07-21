
all: FRDummy FileReadQT4L

FRDummy: FRDummy.h FRDummy.cpp
	g++ -fPIC -c FRDummy.cpp
	ld -shared -o FRDummy.so FRDummy.o

FileReadQT4L: FileReadQT4L.h FileReadQT4L.cpp
	g++ -fPIC -c FileReadQT4L.cpp
	ld -shared -o FileReadQT4L.so FileReadQT4L.o -lquicktime
	
clean:
	rm *.o *.so
