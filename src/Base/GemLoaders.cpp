////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.at
//
// Implementation file
//
//    Copyright (c) 1997-2000 Mark Danks.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    Copyright (c) 2002 James Tittle & Chris Clepper
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
// a wrapper for calling Pd's sys_register_loader()
//
/////////////////////////////////////////////////////////

#include "GemLoaders.h"

#if (defined PD_MAJOR_VERSION) && ((PD_MAJOR_VERSION > 0) || (PD_MINOR_VERSION >= 40))
# define GEM_LOADERS
#endif


#ifdef GEM_LOADERS
extern "C" {
  void sys_register_loader(gem_loader_t loader);
}
#endif

void gem_register_loader(gem_loader_t loader) {
#ifdef GEM_LOADERS
  sys_register_loader(loader);
#else
  // no loader for older Pd's....
#endif
}
