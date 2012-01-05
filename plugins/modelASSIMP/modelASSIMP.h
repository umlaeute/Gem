/* -----------------------------------------------------------------

GEM - Graphics Environment for Multimedia

Load an asset (like .obj oder .dxf)

Copyright (c) 2001-2012 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef _INCLUDE__GEMPLUGIN__MODELASSIMP_MODELASSIMP_H_
#define _INCLUDE__GEMPLUGIN__MODELASSIMP_MODELASSIMP_H_

#include "plugins/modelloader.h"

#include "assimp.h"
#include "aiPostProcess.h"
#include "aiScene.h"

#include "Gem/GemGL.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  modelASSIMP
    
  loads an Alias WaveFront .obj file as an asset
    
  KEYWORDS
  asset model

-----------------------------------------------------------------*/

namespace gem { namespace plugins {
    class GEM_EXTERN modelASSIMP : public gem::plugins::modelloader {
 public:
  /////////
  // ctor/dtor
  modelASSIMP(void);
  virtual ~modelASSIMP(void);

  virtual bool isThreadable(void) { return true; }

  //////////
  // open/close an asset
  virtual bool open(const std::string&, const gem::Properties&);
  virtual void close(void);

  //////////
  // render the asset
  virtual bool render(void);
  virtual bool compile(void);

  //////////
  // property handling
  virtual bool enumProperties(gem::Properties&, gem::Properties&);
  virtual void setProperties(gem::Properties&);
  virtual void getProperties(gem::Properties&);

protected:
  virtual void destroy(void);
  bool    m_rebuild;

  const aiScene *m_scene;
  GLint	   m_dispList;

  float  m_scale;
  struct aiVector3D m_offset;

  struct aiVector3D m_min, m_max, m_center;
};
};}; // namespace gem::plugins

#endif	// for header file
