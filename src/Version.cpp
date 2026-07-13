////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// Implementation file
//
// SPDX-FileCopyrightText: © 2008, IOhannes m zmölnig and the GEM contributors
// SPDX-License-Identifier: GPL-2.0-or-later
//
////////////////////////////////////////////////////////

#include "Gem/GemConfig.h"

#define STRINGIFY2(x) #x
#define STRINGIFY(x) STRINGIFY2(x)

#include "Gem/Version.h"

#ifdef HAVE_VERSION_H
# include "version_current.h"
#endif

#ifndef GEM_VERSION_BUGFIX
# define GEM_VERSION_BUGFIX git
#endif

#ifndef GEM_VERSION_CODENAME
# define GEM_VERSION_CODENAME
#endif


const char* gem::Version :: versionString()
{
  return ( "" STRINGIFY(GEM_VERSION_MAJOR) "." STRINGIFY(
             GEM_VERSION_MINOR) "." STRINGIFY(GEM_VERSION_BUGFIX) \
           " " STRINGIFY(GEM_VERSION_CODENAME) );
}

bool gem::Version :: versionCheck(int major, int minor)
{
  return ((GEM_VERSION_MAJOR==major) && (GEM_VERSION_MINOR==minor));
}
