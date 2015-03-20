/*-----------------------------------------------------------------
  LOG
  GEM - Graphics Environment for Multimedia

  A gemvertexbuffer

  Copyright (c) 1997-2000 Mark Danks. mark@danks.org
  Copyright (c) Günther Geiger. geiger@epy.co.at
  Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

  -----------------------------------------------------------------*/

#include "Base/GemShape.h"
#include "Gem/Image.h"
#include "Gem/State.h"
#include "Utils/GLUtil.h"
#include "Gem/VertexBuffer.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  gemvertexbuffer

  Creates a gemvertexbuffer

  KEYWORDS
  geo

  DESCRIPTION

  -----------------------------------------------------------------*/
class GEM_EXTERN gemvertexbuffer : public GemShape
{
  CPPEXTERN_HEADER(gemvertexbuffer, GemShape);

public:
  //////////
  // Constructor
  gemvertexbuffer(t_floatarg size);

  /////////////
  // Variables
  //float range_x;
  //float range_y;

protected:

  //////////
  // Destructor
  virtual ~gemvertexbuffer(void);

  //////////
  // Do the rendering
  virtual void renderShape(GemState *state);
  //virtual void 	runKernel();
  void tabMess(unsigned int argc, t_atom *argv, gem::VertexBuffer&array,
               unsigned int offset);
  void resizeMess(float size);

private :
  // GL functionality
  void createVBO(void);
  void copyArray(const std::string&tab_name, gem::VertexBuffer&array,
                 unsigned int stride, unsigned int offset, bool resize);

  void tableMess (gem::VertexBuffer&vb, std::string name, unsigned int argc,
                  t_atom *argv);
  void positionMess(t_symbol*,int,t_atom*);
  void textureMess(t_symbol*,int,t_atom*);
  void colorMess(t_symbol*,int,t_atom*);
  void normalMess(t_symbol*,int,t_atom*);

  void posxMess(t_symbol*,int,t_atom*);
  void posyMess(t_symbol*,int,t_atom*);
  void poszMess(t_symbol*,int,t_atom*);
  void colrMess(t_symbol*,int,t_atom*);
  void colgMess(t_symbol*,int,t_atom*);
  void colbMess(t_symbol*,int,t_atom*);
  void colaMess(t_symbol*,int,t_atom*);
  void texuMess(t_symbol*,int,t_atom*);
  void texvMess(t_symbol*,int,t_atom*);
  void normxMess(t_symbol*,int,t_atom*);
  void normyMess(t_symbol*,int,t_atom*);
  void normzMess(t_symbol*,int,t_atom*);
  void resizeMess(unsigned int size);
  void posVBO_enableMess(bool flag);
  void colVBO_enableMess(bool flag);
  void texVBO_enableMess(bool flag);
  void normVBO_enableMess(bool flag);
  void enableMess(t_symbol*,int,t_atom*);
  void disableMess(t_symbol*,int,t_atom*);
  void partialDrawMess(unsigned int start, unsigned int end);
  void setProgramID(float ID);
  void attribute(t_symbol*s, int argc, t_atom *argv);
  void attribVBO_enableMess(bool flag);
  void resetAttributes(void);
  void printAttributes(void);

  // Rendering window vars
  unsigned int vbo_size;
  unsigned int m_range[2];
  bool size_change_flag;
  unsigned int glsl_program;
  gem::VertexBuffer m_position, m_texture, m_color, m_normal;
  std::vector <gem::VertexBuffer> m_attribute;

  gem::utils::gl::GLuintMap m_idmapper;
};
