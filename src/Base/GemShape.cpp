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
//    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "GemShape.h"
#include "Gem/State.h"
#include <algorithm>

/////////////////////////////////////////////////////////
//
// a generic GemShape
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////

namespace {
  static char mytolower(char in){
    if(in<='Z' && in>='A')
      return in-('Z'-'z');
    return in;
  }

  static void initialize_drawtypes(std::map<std::string, GLenum>&drawtypes) {
    drawtypes["default"]=GL_DEFAULT_GEM;

    drawtypes["point"]=GL_POINTS;
    drawtypes["points"]=GL_POINTS;

    /* how about GL_LINE ?? */
    drawtypes["line"]=GL_LINE_LOOP;
    drawtypes["lineloop"]=GL_LINE_LOOP;
    drawtypes["lines"]=GL_LINES;
    drawtypes["linestrip"]=GL_LINE_STRIP;

    drawtypes["tri"]=GL_TRIANGLES;
    drawtypes["triangle"]=GL_TRIANGLES;
    drawtypes["tristrip"]=GL_TRIANGLE_STRIP;
    drawtypes["trifan"]=GL_TRIANGLE_FAN;

    drawtypes["quad"]=GL_QUADS;
    drawtypes["quads"]=GL_QUADS;
    drawtypes["quadstrip"]=GL_QUAD_STRIP;

    drawtypes["strip"]=GL_TRIANGLE_STRIP;
    drawtypes["fill"]=GL_POLYGON;
  }

}

GemShape :: GemShape(t_floatarg size)
  : m_linewidth(1.0f), m_size((float)size), m_drawType(GL_DEFAULT_GEM), m_blend(0),
    m_inlet(NULL),
    m_texType(0), m_texNum(0),
    m_texCoords(NULL),
    m_lighting(false)
{
  if (m_size == 0.f)m_size = 1.f;

  // the size inlet
  m_inlet = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("ft1"));

  initialize_drawtypes(m_drawTypes);
}
GemShape :: GemShape()
  : m_linewidth(1.0f), m_size(1.0f), m_drawType(GL_DEFAULT_GEM), m_blend(0),
    m_inlet(NULL),
    m_texType(0), m_texNum(0),
    m_texCoords(NULL),
    m_lighting(false)
{
  // no size inlet
  initialize_drawtypes(m_drawTypes);
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
	int i;

  TexCoord*texcoords=NULL;
  int numCoords = 0;
  int numUnits = 0;

  state->get(GemState::_GL_TEX_NUMCOORDS, numCoords);
  state->get(GemState::_GL_TEX_UNITS, numUnits);


  if (numCoords) {
    tx=state->texCoordX(curCoord);
    ty=state->texCoordY(curCoord);
  }

  if (numUnits) {
    for( i=0; i<numUnits; i++) {
      glMultiTexCoord2fARB(GL_TEXTURE0+i, tx, ty);
    }
  } else { // no multitexturing!
    glTexCoord2f(tx, ty);
	}
  glVertex3f( x, y, z );
}

void GemShape :: SetVertex(GemState* state,float x, float y, float z,
                           float s, float t, float r, float q,
                           int curCoord)
{
	int i;
  int numCoords = 0;
  int numUnits = 0;

  state->get(GemState::_GL_TEX_NUMCOORDS, numCoords);
  state->get(GemState::_GL_TEX_UNITS, numUnits);



  if (numCoords) {
    s*=state->texCoordX(curCoord);
    t*=state->texCoordY(curCoord);
  }

  if (numUnits) {
    for( i=0; i<numUnits; i++)
      glMultiTexCoord4fARB(GL_TEXTURE0+i, s, t, r, q);
  } else { // no multitexturing!
    glTexCoord4f(s, t, r, q);
	}

  glVertex3f( x, y, z );
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
  if(0==m_drawTypes.size()) {
    error("unable to change drawstyle");
  }

  std::string name=type->s_name;
  std::transform(name.begin(), name.end(), name.begin(), mytolower);

  std::map<std::string, GLenum>::iterator it=m_drawTypes.find(name);
  if(m_drawTypes.end() == it) {
    error ("unknown draw style '%s'... possible values are:", name.c_str());
    it=m_drawTypes.begin();
    while(m_drawTypes.end() != it) {
      error("\t %s", it->first.c_str());
      it++;
    }
    return;
  }
  m_drawType=it->second;
  setModified();
}

/////////////////////////////////////////////////////////
// blendMess
//
/////////////////////////////////////////////////////////
void GemShape :: blendMess(float blend)
{
  m_blend = (blend>0);
  setModified();
}

void GemShape :: render(GemState *state)
{
  if (m_drawType == GL_LINE_LOOP || m_drawType == GL_LINE_STRIP || m_drawType == GL_LINES)
    glLineWidth(m_linewidth);

  if (m_blend) {
    glEnable(GL_POLYGON_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);
    glHint(GL_POLYGON_SMOOTH_HINT,GL_DONT_CARE);
  }

  m_texType=0;
  m_texNum =0;
  m_texCoords=NULL;
  m_lighting=false;

  state->get(GemState::_GL_TEX_COORDS, m_texCoords);
  state->get(GemState::_GL_TEX_TYPE, m_texType);
  state->get(GemState::_GL_TEX_NUMCOORDS, m_texNum);
  state->get(GemState::_GL_LIGHTING, m_lighting);

  renderShape(state);

  // LATER try to restore the original state
  if (m_blend) {
    glDisable(GL_POLYGON_SMOOTH);
    glDisable(GL_BLEND);
  }

  if (m_drawType == GL_LINE_LOOP || m_drawType == GL_LINE_STRIP || m_drawType == GL_LINES)
    glLineWidth(1.0);
}

/////////////////////////////////////////////////////////
// static member functions
//
/////////////////////////////////////////////////////////
void GemShape :: obj_setupCallback(t_class *classPtr)
{
  CPPEXTERN_MSG1(classPtr, "width", linewidthMess, float);
  CPPEXTERN_MSG1(classPtr, "draw", typeMess, t_symbol*);
  CPPEXTERN_MSG1(classPtr, "blend", blendMess, float);
  CPPEXTERN_MSG1(classPtr, "ft1", sizeMess, float);
}
