/*
 *  GemVersion.cpp
 *  Gem
 *
 *  Created by zmoelnig on 7/30/08.
 *  Copyright 2008 IEM @ KUG. All rights reserved.
 *
 */
#include "Base/GemConfig.h"
 
#define STRINGIFY2(x) #x
#define STRINGIFY(x) STRINGIFY2(x)

#include "GemVersion.h"

#ifdef HAVE_VERSION_H
# include "version.h"
#endif

#ifndef GEM_VERSION_BUGFIX
# define GEM_VERSION_BUGFIX CVS
#endif

#ifndef GEM_VERSION_CODENAME 
# define GEM_VERSION_CODENAME 
#endif


const char* GemVersion :: versionString() {
return ( "" STRINGIFY(GEM_VERSION_MAJOR) "." STRINGIFY(GEM_VERSION_MINOR) "." STRINGIFY(GEM_VERSION_BUGFIX) \
               " " STRINGIFY(GEM_VERSION_CODENAME) );
}

bool GemVersion :: versionCheck(int major, int minor) {
 return ((GEM_VERSION_MAJOR==major) && (GEM_VERSION_MINOR==minor));
}

