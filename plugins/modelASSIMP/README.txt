read a model using ASSIMP (http://assimp.sf.net)
------------------------------------------------




TODO
====
  animation:
 	-> http://nolimitsdesigns.com/game-design/open-asset-import-library-animation-loader/
  extract texture files


compilation hints
=================

w32-mingw
---------

on w32/MinGW, configure will most likely not find your installation of the assimp-sdk.
this is what i use approximately to enable assimp support when building Gem with MinGW
(the code is bash-script, change appropriately if you use another shell; the leading '$'
is to indicate newlines - don't copy them)

$ ASSIMP_PATH=/home/zmoelnig/src/assimp--2.0.863-sdk
$ ASSIMP_CFLAGS=-I${ASSIMP_PATH}/include
$ ASSIMP_LIBS=-L${ASSIMP_PATH}/bin/assimp_release-dll_win32 -Xlinker -l:Assimp32.dll
$ ./configure PKG_ASSIMP_CFLAGS="${ASSIMP_CFLAGS}" PKG_ASSIMP_LIBS="${ASSIMP_LIBS}"


