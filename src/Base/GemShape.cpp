////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute
//
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "GemShape.h"
#include <ctype.h>
/////////////////////////////////////////////////////////
//
// a generic GemShape
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GemShape :: GemShape(t_floatarg size)
  : m_linewidth(1.0f), m_size((float)size), m_inlet(NULL)
{
  if (m_size == 0.f)m_size = 1.f;

  // the size inlet
  m_inlet = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("ft1"));
}
GemShape :: GemShape()
  : m_linewidth(1.0f), m_size(1.0f), m_inlet(NULL)
{
  // no size inlet
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GemShape :: ~GemShape()
{
  if(m_inlet)inlet_free(m_inlet);
}

/////////////////////////////////////////////////////////
// SetVertex
// set up the texture-coordinates
/////////////////////////////////////////////////////////
void GemShape :: SetVertex(GemState* state,float x, float y, float z, float tx, float ty,int curCoord)
{
    if (state->numTexCoords) 
      glTexCoord2f(state->texCoordX(curCoord), state->texCoordY(curCoord));
    else glTexCoord2f(tx, ty);
    glVertex3f( x,  y, z);
}

/////////////////////////////////////////////////////////
// linewidthMess
//
/////////////////////////////////////////////////////////
void GemShape :: linewidthMess(float linewidth)
{
    m_linewidth = (linewidth < 0.0f) ? 0.0f : linewidth;
    setModified();
}

/////////////////////////////////////////////////////////
// sizeMess
//
/////////////////////////////////////////////////////////
void GemShape :: sizeMess(float size)
{
    m_size = size;
    setModified();
}

/////////////////////////////////////////////////////////
// typeMess
//
/////////////////////////////////////////////////////////
void GemShape :: typeMess(t_symbol *type)
{
  char c=toupper(*type->s_name);
  switch (c){
  case 'L': // line
    m_drawType = GL_LINE_LOOP;
    break;
  case 'F': // fill
    m_drawType = GL_POLYGON;
    break;
  case 'Q': // fill
    m_drawType = GL_QUADS;
    break;
  case 'P': // point
    m_drawType = GL_POINTS;
    break;
  case 'T': // point
    m_drawType = GL_TRIANGLES;
    break;
  case 's': // point
    m_drawType = GL_TRIANGLE_STRIP;
    break;  
    
  default:
    error ("GEM: square draw style");
    return;
  }
  setModified();
}

/////////////////////////////////////////////////////////
// static member functions
//
/////////////////////////////////////////////////////////
void GemShape :: obj_setupCallback(t_class *classPtr)
{
    class_addmethod(classPtr, (t_method)&GemShape::linewidthMessCallback,
    	    gensym("width"), A_FLOAT, A_NULL);
    class_addmethod(classPtr, (t_method)&GemShape::typeMessCallback,
    	    gensym("draw"), A_SYMBOL, A_NULL);
    class_addmethod(classPtr, (t_method)&GemShape::sizeMessCallback,
    	    gensym("ft1"), A_FLOAT, A_NULL);
}
void GemShape :: linewidthMessCallback(void *data, t_floatarg linewidth)
{
    GetMyClass(data)->linewidthMess((float)linewidth);
}
void GemShape :: typeMessCallback(void *data, t_symbol *type)
{
    GetMyClass(data)->typeMess(type);
}
void GemShape :: sizeMessCallback(void *data, t_floatarg size)
{
    GetMyClass(data)->sizeMess((float)size);
}

