/* -----------------------------------------------------------------

GEM - Graphics Environment for Multimedia

Load an asset (like .obj or .dxf)

Copyright (c) 2001-2012 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef _INCLUDE__GEMPLUGIN__MODELASSIMP3_MODELASSIMP3_H_
#define _INCLUDE__GEMPLUGIN__MODELASSIMP3_MODELASSIMP3_H_

#include "plugins/modelloader.h"

#include <assimp/cimport.h>        // Plain-C interface
#include <assimp/scene.h>          // Output data structure
#include <assimp/postprocess.h>    // Post processing flags

#include "Gem/GemGL.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  modelASSIMP3

  loads an Alias WaveFront .obj file as an asset

  KEYWORDS
  asset model

-----------------------------------------------------------------*/

namespace gem
{
namespace plugins
{
class GEM_EXPORT modelASSIMP3 : public gem::plugins::modelloader
{
public:
  struct meshdata {
    gem::plugins::modelloader::mesh mesh;
    std::vector<float> vertices, normals, texcoords, colors;
  meshdata(void) : mesh({0}) { };
  };


  /////////
  // ctor/dtor
  modelASSIMP3(void);
  virtual ~modelASSIMP3(void);

  virtual bool isThreadable(void)
  {
    return true;
  }

  //////////
  // open/close an asset
  virtual bool open(const std::string&, const gem::Properties&);
  virtual void close(void);


  /**
   * get the named mesh
   * if meshNum exceeds the available meshes, NULL is returned
   */
  virtual struct mesh*getMesh(size_t meshNum);
  virtual size_t getNumMeshes(void);

  /**
   * update the mesh data (for all meshes)
   * the data pointers in previously obtained t_mesh'es stay valid
   * (but the data they point to might change)
   * returns TRUE if there was a change, FALSE otherwise
   */
  virtual bool updateMeshes(void);

  ///////////////////////////////////////////

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
  aiVector3D m_offset;

  aiVector3D m_min, m_max, m_center;

  bool m_useMaterial;

  std::vector<std::vector<float> > m_vertices, m_normals, m_texcoords, m_colors;
  std::vector<VBOarray> m_VBOarray;
  bool m_refresh;

  bool m_have_texcoords;
  std::string m_textype;

  aiVector2D m_texscale;
  float m_smooth;

  int m_group;

  std::vector<struct meshdata>m_meshes;

};
};
}; // namespace gem::plugins

#endif  // for header file
