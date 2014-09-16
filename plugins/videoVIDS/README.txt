videoVIDS
===


this is a plugin for the "videoInput" library by Theodore Watson.
The library gives us access to DirectShow video-grabbers
(so it's only useful on W32).
You can get it from
   https://github.com/ofTheo
For convenience, it has been included in the videoInput/ library
(it's only two files)

Compiling
====

this is currently a bit tricky, as for unknown reasons `libtool`
fails to properly link some relevant libraries.

My (temporary) build-setup therefore looks like the following.

In `videoVIDS/Makefile.am` add:

    gem_videoVIDS_la_CXXFLAGS += -I$(srcdir)/videoInput
    gem_videoVIDS_la_LIBADD   += -lole32 -loleaut32 -lstrmiids -luuid

then run `make`, which will give some linker warnings, and won't produce a `.dll`.
Then i run the following script to build the final `gem_videoVIDS.dll`:

~~~bash
#!/bin/sh

OUTPUT=.libs/gem_videoVIDS.dll
VI_LIBS="-lole32                                        \
        -loleaut32                                      \
        -lstrmiids                                      \
        -luuid"

debug() {
  echo "$@"
  $@
}

rm -f "${OUTPUT}"

debug i686-w64-mingw32-g++				\
	-shared						\
	-nostdlib					\
	/usr/lib/gcc/i686-w64-mingw32/4.9-win32/../../../../i686-w64-mingw32/lib/dllcrt2.o \
	/usr/lib/gcc/i686-w64-mingw32/4.9-win32/crtbegin.o \
	.libs/gem_videoVIDS_la-videoVIDS.o		\
	.libs/gem_videoVIDS_la-videoInput.o		\
	-lstdc++					\
	-L/home/zmoelnig/lib/W32/pd/bin			\
	-L../..						\
 	${VI_LIBS}					\
	-lgdi32						\
	-lws2_32					\
	-L/usr/lib/gcc/i686-w64-mingw32/4.9-win32	\
	-L/usr/lib/gcc/i686-w64-mingw32/4.9-win32/../../../../i686-w64-mingw32/lib \
	-lstdc++					\
	-lmingw32					\
	-lgcc						\
	-lmoldname					\
	-lmingwex					\
	-lmsvcrt					\
	-ladvapi32					\
	-lshell32					\
	-luser32					\
	-lkernel32					\
	-lgcc_s						\
	/usr/lib/gcc/i686-w64-mingw32/4.9-win32/crtend.o \
	-mms-bitfields					\
	-O3						\
	-mmmx						\
	-Wl,-l:pd.dll					\
	-Wl,-l:Gem.dll					\
	-o "${OUTPUT}"					\
	-Wl,--enable-auto-image-base			\
	-Xlinker --out-implib				\
	-Xlinker .libs/gem_videoVIDS.dll.a

if [ -e "${OUTPUT}" ]; then
  echo 
  echo "created ${OUTPUT}" 1>&2
  exit 0
fi
exit 1

~~~
