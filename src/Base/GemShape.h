/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_BASE_GEMSHAPE_H_
#define _INCLUDE__GEM_BASE_GEMSHAPE_H_

#include "Base/GemBase.h"
#include "Gem/GemGL.h"
#include <map>
/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  GemShape

  Base class for shapes

  DESCRIPTION

  Inlet for a float - "ft1"

  "ft1" - the size of the shape
  "draw" - the drawing style
  "width" - the line width when drawing with lines

  -----------------------------------------------------------------*/
class TexCoord;
class GEM_EXTERN GemShape : public GemBase
{
public:

  //////////
  // Constructor
  // [in] size - A size of 0. means to just use the default (ie, 1.)
  explicit GemShape(t_floatarg size);
  GemShape();

protected:

  //////////
  // Destructor
  virtual ~GemShape();

  //-----------------------------------
  // GROUP:     Access functions
  //-----------------------------------

  //////////
  // The width of the lines in line draw mode
  void          linewidthMess(float linewidth);

  //////////
  // The size of the object
  void          sizeMess(float size);

  //////////
  // How the object should be drawn
  virtual void  typeMess(t_symbol* type);

  //-----------------------------------
  // GROUP:     Utility functions
  //-----------------------------------

  void SetVertex(GemState* state,float x, float y, float z,
                 float tx, float ty,
                 int curCoord);

  void SetVertex(GemState* state,float x, float y, float z,
                 float s, float t, float r, float q,
                 int curCoord);

  //-----------------------------------
  // GROUP:     Member variables
  //-----------------------------------

  //////////
  // The line width for GL_LINE mode
  GLfloat               m_linewidth;

  //////////
  // The size of the object
  GLfloat               m_size;

  //////////
  // The drawing style (GL_LINE, GL_POLYGON, etc)
  GLenum                m_drawType;

  //////////
  // do we want blending?
  GLboolean             m_blend;
  void  blendMess(float blend);


  ////////
  // override this memberfunction to automatically enable softblended rendering,...
  virtual void renderShape(GemState *state) {;}

  // OR
  // override this memberfunction if you don't want softblending
  virtual void render(GemState *state);


  //////////
  // The size inlet
  t_inlet         *m_inlet;

  //-----------------------------------
  // GROUP:     Setup functions
  //-----------------------------------

  //////////
  // creation callback
  static void   real_obj_setupCallback(t_class *classPtr)
  {
    GemBase::real_obj_setupCallback(classPtr);
    GemShape::obj_setupCallback(classPtr);
  }

private:

  static inline GemShape *GetMyClass(void *data)
  {
    return((GemShape *)((Obj_header *)data)->data);
  }

  //////////
  // static member functions
  static void   obj_setupCallback(t_class *classPtr);

protected:
  int m_texType, m_texNum;
  TexCoord*m_texCoords;
  bool m_lighting;

  std::map<std::string, GLenum>m_drawTypes;
};

#endif  // for header file
