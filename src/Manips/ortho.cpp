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

// I hate Microsoft...I shouldn't have to do this!
#ifdef _WINDOWS
#include <windows.h>
#endif

#ifdef MACOSX
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif // MACOSX

#include "ortho.h"
#include "Base/GemMan.h"

CPPEXTERN_NEW(ortho)

/////////////////////////////////////////////////////////
//
// ortho
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
ortho :: ortho()
       : m_state(1)
{ }

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
ortho :: ~ortho()
{ }

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void ortho :: render(GemState *)
{
    if (m_state)
	{
		glPushAttrib(GL_VIEWPORT_BIT);
		glViewport(0, 0, GemMan::m_width, GemMan::m_height);
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glOrtho(-4.f, 4.f, -4.f, 4.f, .1f, 100.f);
		glMatrixMode(GL_MODELVIEW);
	}
}

/////////////////////////////////////////////////////////
// postrender
//
/////////////////////////////////////////////////////////
void ortho :: postrender(GemState *)
{
    if (m_state)
	{
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopAttrib();
	}
}

/////////////////////////////////////////////////////////
// orthoMess
//
/////////////////////////////////////////////////////////
void ortho :: orthoMess(int state)
{
    m_state = state;
    setModified();
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void ortho :: obj_setupCallback(t_class *classPtr)
{
    class_addfloat(classPtr, (t_method)&ortho::orthoMessCallback);
}
void ortho :: orthoMessCallback(void *data, t_floatarg state)
{
    GetMyClass(data)->orthoMess((int)state);
}
