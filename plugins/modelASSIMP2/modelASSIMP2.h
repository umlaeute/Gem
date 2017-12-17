/* -----------------------------------------------------------------

GEM - Graphics Environment for Multimedia

Load an asset (like .obj oder .dxf)

Copyright (c) 2001-2012 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef _INCLUDE__GEMPLUGIN__MODELASSIMP2_MODELASSIMP2_H_
#define _INCLUDE__GEMPLUGIN__MODELASSIMP2_MODELASSIMP2_H_

#include "plugins/modelloader.h"

#include <assimp.h>
#include <aiPostProcess.h>
#include <aiScene.h>

#include "Gem/GemGL.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  modelASSIMP2

  loads an Alias WaveFront .obj file as an asset

  KEYWORDS
  asset model

-----------------------------------------------------------------*/

namespace gem
{
namespace plugins
{
class GEM_EXPORT modelASSIMP2 : public gem::plugins::modelloader
{
public:
  /////////
  // ctor/dtor
  modelASSIMP2(void);
  virtual ~modelASSIMP2(void);

  virtual bool isThreadable(void)
  {
    return true;
  }

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
  std::vector<std::vector<float> > getVector(std::string vectorName);
  std::vector<VBOarray> getVBOarray();
  void unsetRefresh();
  bool needRefresh();
  void fillVBOarray();

protected:
  virtual void destroy(void);
  bool    m_rebuild;

  const aiScene *m_scene;

  float  m_scale;
  struct aiVector3D m_offset;

  struct aiVector3D m_min, m_max, m_center;

  bool m_useMaterial;

  std::vector<std::vector<float> > m_vertices, m_normals, m_texcoords,
      m_colors;
  std::vector<VBOarray> m_VBOarray;
  bool m_refresh;

  bool m_have_texcoords;
  std::string m_textype;
};
};
}; // namespace gem::plugins

#endif  // for header file
