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

 protected:
  class VertexBuffer {
  public:
  	VertexBuffer();
    VertexBuffer(unsigned int size, unsigned int stride);
    ~VertexBuffer(void);
    void resize(unsigned int);
    bool create(void);
    bool render(void);
    void destroy(void);

    unsigned int size;
    unsigned int stride;
	
    GLuint vbo;
    float*array;
    bool dirty;
	
	GLuint attrib_index;
	const GLchar* attrib_name;
	std::string attrib_name_str;
	std::string attrib_array;
	
    bool enabled;
  };


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
  void tabMess(unsigned int argc, t_atom *argv, VertexBuffer&array, unsigned int offset);
  void resizeMess(float size);

 private :
  // GL functionality
  void createVBO(void);
  void copyArray(const std::string&tab_name, VertexBuffer&array, unsigned int stride, unsigned int offset);

  void tableMess (VertexBuffer&vb, std::string name, unsigned int argc, t_atom *argv);
  void attributeMess(t_symbol*,int,t_atom*);
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
  void attribVBO_enableMess(bool flag);
  void posVBO_enableMess(bool flag);
  void colVBO_enableMess(bool flag);
  void texVBO_enableMess(bool flag);
  void normVBO_enableMess(bool flag);
  void enableMess(t_symbol*,int,t_atom*);
  void disableMess(t_symbol*,int,t_atom*);
  
  void attribute(t_symbol*s, int argc, t_atom *argv);
  void setProgramID(float ID);
  void resetAttributes(void);
  void printAttributes(void);

  // Rendering window vars
  unsigned int vbo_size;
  bool size_change_flag;
  // glsl_program vars
  unsigned int glsl_program;

  VertexBuffer m_position, m_texture, m_color, m_normal;
  std::vector <VertexBuffer> m_attribute;
  };

