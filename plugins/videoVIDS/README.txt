videoVIDS
===


this is a plugin for the "videoInput" library by Theodore Watson.
The library gives us access to DirectShow video-grabbers
(so it's only useful on W32).
You can get it from
   https://github.com/ofTheo

Compiling
====

this is currently a bit tricky, as for unknown reasons `libtool`
fails to properly link some relevant libraries.

My (temporary) build-setup therefore looks like the following.

In `videoVIDS/Makefile.am` add:

    gem_videoVIDS_la_CXXFLAGS += -I$(srcdir)/videoInput/videoInputSrcAndDemos/libs/videoInput
    gem_videoVIDS_la_LIBADD   += -L$(srcdir)/videoInput/compiledLib/CodeBlocks12/ -lvideoInputLib -lole32 -loleaut32 -lstrmiids -luuid

then run `make`, which will give some linker warnings, and won't produce a `.dll`.
Then i run the following script to build the final `gem_videoVIDS.dll`:

~~~bash
VI_LIBS=-L./videoInput/compiledLib/CodeBlocks12         \
        -lvideoInputLib                                 \
        -lole32                                         \
        -loleaut32                                      \
        -lstrmiids                                      \
        -luuid
i686-w64-mingw32-g++                                    \
        -shared                                         \
        -nostdlib                                       \
        /usr/lib/gcc/i686-w64-mingw32/4.9-win32/../../../../i686-w64-mingw32/lib/../lib/dllcrt2.o \
        /usr/lib/gcc/i686-w64-mingw32/4.9-win32/crtbegin.o \
        .libs/gem_videoVIDS_la-videoVIDS.o              \
        -lstdc++                                        \
        -L/home/zmoelnig/lib/W32/pd/bin                 \
        -L../..                                         \
        ${VI_LIBS}                                      \
        -lgdi32                                         \
        -lws2_32                                        \
        -lmsvcrt                                        \
        -L/usr/lib/gcc/i686-w64-mingw32/4.9-win32       \
        -L/usr/lib/gcc/i686-w64-mingw32/4.9-win32/../../../../i686-w64-mingw32/lib/../lib \
        -L/usr/lib/gcc/i686-w64-mingw32/4.9-win32/../../../../i686-w64-mingw32/lib \
        -lstdc++                                        \
        -lmingw32                                       \
        -lgcc_s                                         \
        -lgcc                                           \
        -lmoldname                                      \
        -lmingwex                                       \
        -lmsvcrt                                        \
        -ladvapi32                                      \
        -lshell32                                       \
        -luser32                                        \
        -lkernel32                                      \
        -lmingw32                                       \
        -lgcc_s                                         \
        -lgcc                                           \
        -lmoldname                                      \
        -lmingwex                                       \
        -lmsvcrt                                        \
        /usr/lib/gcc/i686-w64-mingw32/4.9-win32/crtend.o \
        -mms-bitfields                                  \
        -O3                                             \
        -mmmx                                           \
        -Wl,-l:pd.dll                                   \
        -Wl,-l:Gem.dll                                  \
        -o "${OUTPUT}"                                  \
        -Wl,--enable-auto-image-base                    \
        -Xlinker --out-implib                           \
        -Xlinker .libs/gem_videoVIDS.dll.a
~~~
