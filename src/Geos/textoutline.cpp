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

#include "textoutline.h"

#include "FTFace.h"
#ifndef FTGL
#include "GLTTOutlineFont.h"
#else
#include "FTGLOutlineFont.h"
#endif

#ifdef MACOSX
#include <AGL/agl.h>
extern bool HaveValidContext (void);
#endif

CPPEXTERN_NEW_WITH_GIMME(textoutline)

/////////////////////////////////////////////////////////
//
// textoutline
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
textoutline :: textoutline(int argc, t_atom *argv)
            : TextBase(argc, argv), m_font(NULL), m_face(NULL)
{
    m_fontSize = 20;
#ifdef MACOSX
  if (!HaveValidContext ()) {
    post("GEM: geo: textoutline - need window to load font");
    return;
  }
#endif
    fontNameMess(DEFAULT_FONT);
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
textoutline :: ~textoutline()
{
    delete m_font;
	delete m_face;
}

/////////////////////////////////////////////////////////
// setPrecision
//
/////////////////////////////////////////////////////////
void textoutline :: setPrecision(float prec)
{
	m_precision = prec;
	m_valid = makeFontFromFace();
	setModified();
}

/////////////////////////////////////////////////////////
// setFontSize
//
/////////////////////////////////////////////////////////
void textoutline :: setFontSize(int size)
{
	m_fontSize = size;
	m_valid = makeFontFromFace();
	setModified();
}

/////////////////////////////////////////////////////////
// fontNameMess
//
/////////////////////////////////////////////////////////
void textoutline :: fontNameMess(const char *filename)
{
	m_valid = 0;
	delete m_font;
	m_font = NULL;
	delete m_face;
	m_face = new FTFace;
    char buf[MAXPDSTRING];
    canvas_makefilename(getCanvas(), (char *)filename, buf, MAXPDSTRING);

	if( ! m_face->open(buf) )
	{
		error("GEM: textoutline: unable to open font: %s", buf);
		return;
	}
	m_valid = makeFontFromFace();
    setModified();
}

/////////////////////////////////////////////////////////
// makeFontFromFace
//
/////////////////////////////////////////////////////////
int textoutline :: makeFontFromFace()
{
	if (!m_face)
	{
		error("GEM: text2d: True type font doesn't exist");
		return(0);
	}

	delete m_font;
#ifndef FTGL
	m_font = new GLTTOutlineFont(m_face);
#else
	m_font = new FTGLOutlineFont(m_face);
#endif
	m_font->setPrecision((double)m_precision);
	if( ! m_font->create(m_fontSize) )
	{
		error("GEM: textoutline: unable to create vectored font");
		return(0);
	}
	return(1);
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void textoutline :: render(GemState *)
{
	if (m_valid)
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
void textoutline :: obj_setupCallback(t_class *)
{ }
