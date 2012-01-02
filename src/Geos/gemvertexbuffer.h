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
  void tabMess(int argc, t_atom *argv, VertexBuffer&array, int offset);
  void resizeMess(float size);

 private :
  // GL functionality
  void createVBO(void);
  void copyArray(const std::string&tab_name, VertexBuffer&array, unsigned int stride, unsigned int offset);
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

  // Rendering window vars
  int vbo_size;
  bool size_change_flag;
  VertexBuffer m_position, m_texture, m_color, m_normal;
};
