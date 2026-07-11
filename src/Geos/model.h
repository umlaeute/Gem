/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_GEOS_MODEL_H_
#define _INCLUDE__GEM_GEOS_MODEL_H_

#include "Base/GemBase.h"
#include "Gem/Properties.h"
#include "Gem/model.h"
#include "Gem/GemGL.h"
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

  "open" - the 3D model to load

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
  virtual void    groupsMess(t_symbol*, int, t_atom*);

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
  virtual void  stopRendering();

  gem::plugins::modelloader*m_loader;
  gem::modelGL*m_loaded;

  gem::Properties m_readprops, m_writeprops;

  gem::RTE::Outlet m_infoOut;
  std::vector<std::string> m_backends;

  GLenum m_drawType;
  std::map<std::string, GLenum>m_drawTypes;
  bool m_blend;
  GLfloat m_linewidth;
  enum gem::modelGL::texturetype m_texType;
  enum gem::modelGL::rescale m_rescale;

  std::vector<unsigned int> m_group;
  bool m_useMaterial;
};

#endif  // for header file
