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
  model(t_symbol *filename);

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

  //////////
  // When a rescale is received
  virtual void  rescaleMess(bool state);
  //////////
  // When a reverse is received
  virtual void  reverseMess(bool state);
  //////////
  // Which texture type (linear, spheric)
  virtual void  textureMess(int state);
  //////////
  // Set smoothing factor
  virtual void  smoothMess(t_float fsmooth);
  //////////
  // Set material mode
  virtual void  materialMess(int material);

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

  void copyArray(const std::vector<std::vector<float> >&tab,
                 gem::VertexBuffer&vb);
  void copyAllArrays();
  void getVBOarray();
  void createVBO(void);

  gem::plugins::modelloader*m_loader;
  bool m_loaded, m_size_change_flag;

  gem::Properties m_readprops, m_writeprops;

  gem::VertexBuffer m_position, m_texture, m_color, m_normal;

  gem::RTE::Outlet m_infoOut;
  std::vector<std::string> m_backends;

  GLenum m_drawType;
  std::map<std::string, GLenum>m_drawTypes;
};

#endif  // for header file
