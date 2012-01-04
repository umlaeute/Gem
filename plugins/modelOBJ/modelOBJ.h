/* -----------------------------------------------------------------

GEM - Graphics Environment for Multimedia

Load an asset (like .obj oder .dxf)

Copyright (c) 2001-2012 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef _INCLUDE__GEMPLUGIN__MODELOBJ_MODELOBJ_H_
#define _INCLUDE__GEMPLUGIN__MODELOBJ_MODELOBJ_H_
#include "plugins/modelloader.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  modelOBJ
    
  loads an Alias WaveFront .obj file as an asset
    
  KEYWORDS
  asset model

-----------------------------------------------------------------*/

namespace gem { namespace plugins {
    class GEM_EXTERN modelOBJ : public gem::plugins::modelloader {
 public:
  /////////
  // ctor/dtor
  modelOBJ(void);
  virtual ~modelOBJ(void);

  virtual bool isThreadable(void) { return true; }

  //////////
  // open/close an asset
  virtual bool open(const std::string&, const gem::Properties&);
  virtual void close(void);

  //////////
  // render the asset
  virtual void render(void);

  //////////
  // property handling
  virtual bool enumProperties(gem::Properties&, gem::Properties&);
  virtual void setProperties(gem::Properties&);
  virtual void getProperties(gem::Properties&);
};
};}; // namespace gem::plugins

#endif	// for header file
