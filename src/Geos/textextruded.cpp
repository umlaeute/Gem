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
//    Copyright (c) 2001-2003 IOhannes m zmoelnig. forum::für::umläute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "textextruded.h"

#ifdef FTGL
#include "FTGLExtrdFont.h"
#endif

#ifdef MACOSX
#include <AGL/agl.h>
extern bool HaveValidContext (void);
#endif

CPPEXTERN_NEW_WITH_GIMME(textextruded)

/////////////////////////////////////////////////////////
//
// textextruded
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
textextruded :: textextruded(int argc, t_atom *argv)
  : TextBase(argc, argv)
#ifdef GLTT
    , m_font(NULL)
#endif
{
#ifdef MACOSX
  if (!HaveValidContext ()) {post("GEM: geo: textextruded - need window to load font");return;}
#endif
#ifdef FTGL
  m_font = new FTGLExtrdFont;
#endif
  fontNameMess(DEFAULT_FONT);
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
textextruded :: ~textextruded()
{
#ifdef FTGL
  if(m_font)delete m_font;
  if(m_face)delete m_face;
#endif
}

#ifdef GLTT
/////////////////////////////////////////////////////////
// makeFontFromFace
//
/////////////////////////////////////////////////////////
int textextruded :: makeFontFromFace()
{
  error("GEM: textextruded: FTGL support is needed for this object");
  return 0;
}
#endif

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void textextruded :: render(GemState *)
{
  if (m_valid && m_theString) {
    // compute the offset due to the justification
    float x1=0, y1=0, z1=0, x2=0, y2=0, z2=0;

#if defined FTGL
    m_font->BBox( m_theString, x1, y1, z1, x2, y2, z2); // FTGL
#endif
    glPushMatrix();
    justifyFont(x1, y1, z1, x2, y2, z2);
#ifdef FTGL
    m_font->render(m_theString);
#endif
    
    glPopMatrix();
  }
}

/////////////////////////////////////////////////////////
// setPrecision
//
/////////////////////////////////////////////////////////
void textextruded :: setDepth(float prec)
{
  m_fontDepth = prec;
#ifdef FTGL
  m_font->Depth(m_fontDepth);
  setFontSize(m_fontSize);
#endif
  setModified();
}
/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void textextruded :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr, (t_method)&textextruded::depthMessCallback,
		  gensym("depth"), A_FLOAT, A_NULL);  
}
void textextruded :: depthMessCallback(void *data, t_floatarg depth)
{
  GetMyClass(data)->setDepth((float)depth);
}

