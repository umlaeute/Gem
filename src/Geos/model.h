/*-----------------------------------------------------------------
  LOG
  GEM - Graphics Environment for Multimedia

  read in a model file

  Copyright (c) 1997-1999 Mark Danks. mark@danks.org
  Copyright (c) Günther Geiger. geiger@epy.co.at
  Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

  -----------------------------------------------------------------*/

#ifndef _INCLUDE__GEM_GEOS_MODEL_H_
#define _INCLUDE__GEM_GEOS_MODEL_H_

#include "Base/GemBase.h"
#include "Gem/Properties.h"
#include "Gem/VertexBuffer.h"
#include "RTE/Outlet.h"
#include "plugins/modelloader.h"

#include <map>

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  model

  read in a model file

  DESCRIPTION

  Inlet for a list - "model"

  "open" - the RGB model to set the object to

  -----------------------------------------------------------------*/
namespace gem
{
namespace plugins
{
class modelloader;
};
};

class GEM_EXTERN model : public GemBase
{
  CPPEXTERN_HEADER(model, GemBase);

public:

  //////////
  // Constructor
  model(t_symbol* filename);

protected:

  //////////
  // Destructor
  virtual ~model(void);

  //////////
  // When an open is received
  virtual void  openMess(const std::string&filename);

  virtual void enumPropertyMess(void);
  virtual void clearPropertiesMess(void);
  virtual void getPropertyMess(t_symbol*s, int, t_atom*);
  virtual void setPropertyMess(t_symbol*s, int, t_atom*);
  virtual void setPropertiesMess(t_symbol*, int, t_atom*);
  virtual void applyProperties(void);

  virtual void  rescaleMess(bool state);
  virtual void  reverseMess(bool state);
  virtual void  textureMess(int state);
  virtual void  smoothMess(t_float fsmooth);
  virtual void  materialMess(int material);

  virtual void blendMess(bool blend);
  virtual void linewidthMess(t_float linewidth);

  //////////
  // Set groups to render
  virtual void    groupMess(int group);

  //////////
  // draw type
  virtual void    drawMess(std::string);
  virtual void    drawMess(int);

  //////////
  // Set backend to use
  virtual void  backendMess(t_symbol*s, int argc, t_atom*argv);

  //////////
  virtual void  render(GemState *state);
  virtual void  startRendering();

  gem::plugins::modelloader*m_loader;
  bool m_loaded, m_size_change_flag;

  gem::Properties m_readprops, m_writeprops;

  gem::RTE::Outlet m_infoOut;
  std::vector<std::string> m_backends;

  GLenum m_drawType;
  std::map<std::string, GLenum>m_drawTypes;
  bool m_blend;
  GLfloat m_linewidth;
  GLfloat m_texscale[2];

  std::vector<unsigned int> m_group;
  bool m_useMaterial;

  /* ============================== */
  struct modelmesh {
    gem::plugins::modelloader::mesh* mesh;
    gem::VBO vertices, normals, colors, texcoords;
    modelmesh(gem::plugins::modelloader::mesh*m);
    void update(void);
    void render(GLenum) const;
  };
  std::vector<struct modelmesh>m_mesh;
};

#endif  // for header file
