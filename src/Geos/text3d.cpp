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
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "text3d.h"

#include "FTFace.h"
#include "GLTTFont.h"

CPPEXTERN_NEW_WITH_GIMME(text3d)

  /////////////////////////////////////////////////////////
//
// text3d
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
text3d :: text3d(int argc, t_atom *argv)
  : TextBase(argc, argv), m_font(NULL), m_face(NULL)
{
  m_fontSize = 20;
  fontNameMess(DEFAULT_FONT);
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
text3d :: ~text3d()
{
  delete m_font;
  delete m_face;
}

/////////////////////////////////////////////////////////
// setPrecision
//
/////////////////////////////////////////////////////////
void text3d :: setPrecision(float prec)
{
  m_precision = prec;
  m_valid = makeFontFromFace();
  setModified();
}

/////////////////////////////////////////////////////////
// setFontSize
//
/////////////////////////////////////////////////////////
void text3d :: setFontSize(int size)
{
  m_fontSize = size;
  m_valid = makeFontFromFace();
  setModified();
}

/////////////////////////////////////////////////////////
// fontNameMess
//
/////////////////////////////////////////////////////////
void text3d :: fontNameMess(const char *filename)
{
  m_valid = 0;
  delete m_font;
  m_font = NULL;
  delete m_face;
  m_face = new FTFace;

  char buf[MAXPDSTRING];
  canvas_makefilename(getCanvas(), (char *)filename, buf, MAXPDSTRING);

  if( ! m_face->open(buf) ) {
    error("GEM: text3d: unable to open font: %s", buf);
    return;
  }
  m_valid = makeFontFromFace();
  setModified();
}

/////////////////////////////////////////////////////////
// makeFontFromFace
//
/////////////////////////////////////////////////////////
int text3d :: makeFontFromFace()
{
  if (!m_face)
    {
      error("GEM: text3d: True type font doesn't exist");
      return(0);
    }

  delete m_font;
  m_font = new GLTTFont(m_face);
  m_font->setPrecision((double)m_precision);
  if( ! m_font->create(m_fontSize) )
    {
      error("GEM: text3d: unable to create polygonal font");
      return(0);
    }
  return(1);
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void text3d :: render(GemState *)
{
  if (m_valid && m_theString)
    {
      glPushMatrix();

      // compute the offset due to the justification
      float width = 0.f;
      if (m_widthJus == LEFT)
	width = 0.f;
      else if (m_widthJus == RIGHT)
	width = (float)(m_font->getWidth(m_theString));
      else if (m_widthJus == CENTER)
	width = (float)(m_font->getWidth(m_theString) / 2.f);

      float height = 0.f;
      if (m_heightJus == BOTTOM)
	height = 0.f;
      else if (m_heightJus == TOP)
	height = (float)(m_font->getHeight());
      else if (m_heightJus == MIDDLE)
	height = (float)(m_font->getHeight() / 2.f);

      glScalef(.05f, .05f, .05f);
      glTranslatef(-width, -height, 0.f);
      m_font->output(m_theString);

      glPopMatrix();
    }
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void text3d :: obj_setupCallback(t_class *)
{ }
