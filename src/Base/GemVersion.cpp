/*
 *  GemVersion.cpp
 *  Gem
 *
 *  Created by zmoelnig on 7/30/08.
 *  Copyright 2008 IEM @ KUG. All rights reserved.
 *
 */
 
 
#define STRINGIFY2(x) #x
#define STRINGIFY(x) STRINGIFY2(x)

#include "GemVersion.h"

char* GemVersion :: versionString() {
#if 0
return gensym( "" STRINGIFY(GEM_VERSION_MAJOR) "." STRINGIFY(GEM_VERSION_MINOR) "." STRINGIFY(GEM_VERSION_BUGFIX) \
               " " STRINGFY(GEM_VERSION_CODENAME) )->s_name;
#else
 return 0;
#endif
}

bool GemVersion :: versionCheck(int major, int minor) {
 return ((GEM_VERSION_MAJOR==major) && (GEM_VERSION_MINOR==minor));
}

