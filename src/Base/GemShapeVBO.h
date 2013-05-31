/*-----------------------------------------------------------------
  LOG
  GEM - Graphics Environment for Multimedia

  Base class for shapes

  Copyright (c) 1997-1999 Mark Danks. mark@danks.org
  Copyright (c) Günther Geiger. geiger@epy.co.at
  Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

  -----------------------------------------------------------------*/

#ifndef _INCLUDE__GEM_BASE_GEMSHAPEVBO_H_
#define _INCLUDE__GEM_BASE_GEMSHAPEVBO_H_

#include "Base/GemShape.h"
/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  GemShapeVBO

  Base class for shapes

  DESCRIPTION

  like GemShape but uses modern openGL features, like VBOs

  -----------------------------------------------------------------*/

#include <vector>
class GEM_EXTERN GemShapeVBO : public GemShape
{
public:

  //////////
  // Constructor
  // [in] size - A size of 0. means to just use the default (ie, 1.)
  GemShapeVBO(t_floatarg size);
  GemShapeVBO();

  /* arraytypes when passing the different data
   * when using glVertexAttribPointer(), these IDs can be used in the shader
   */
  typedef enum {
    VERTEX   = 0,
    COLOR    = 1,
    NORMAL   = 2,
    TEXTURE  = 3,

    _LAST
  } arraytype_t;

protected:

  //////////
  // Destructor
  virtual ~GemShapeVBO();

  /* allow user to disable VBO-rendering (for testing purposes) */
  int m_vbo;
  void vboMess(int mode);

  /* add/delete a given array */
  void setArray  (arraytype_t type, std::vector<float>&values, unsigned int stride=3);
  void unsetArray(arraytype_t type);
  /* provide a default drawtype */
  void setDefaultDrawtype(GLenum);

  ////////
  // override this member-function for immediate rendering
  virtual void renderShape(GemState *state) {;}


  //////////
  // creation callback
  static void 	real_obj_setupCallback(t_class *classPtr)
  { GemShape::real_obj_setupCallback(classPtr); GemShapeVBO::obj_setupCallback(classPtr); }

private:
  // selects immediate/VBO/... rendering
  virtual void render(GemState *state);


  static inline GemShapeVBO *GetMyClass(void *data) {return((GemShapeVBO *)((Obj_header *)data)->data);}

  //////////
  // static member functions
  static void 	obj_setupCallback(t_class *classPtr);

  class PIMPL;
  PIMPL*m_pimpl;
};

#endif	// for header file
