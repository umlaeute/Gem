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

#include "alpha.h"

CPPEXTERN_NEW(alpha)

/////////////////////////////////////////////////////////
//
// alpha
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
alpha :: alpha()
       : m_alphaState(1), m_alphaTest(1)
{ }

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
alpha :: ~alpha()
{ }

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void alpha :: render(GemState *)
{
    if (m_alphaState)
    {
	    glEnable(GL_BLEND);
	    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		if (m_alphaTest)
		{
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GREATER, 0.f);
		}
    }
}

/////////////////////////////////////////////////////////
// postrender
//
/////////////////////////////////////////////////////////
void alpha :: postrender(GemState *)
{
    if (m_alphaState)
	{
		glDisable(GL_BLEND);
		if (m_alphaTest)
			glDisable(GL_ALPHA_TEST);
	}
}

/////////////////////////////////////////////////////////
// alphaMess
//
/////////////////////////////////////////////////////////
void alpha :: alphaMess(int alphaState)
{
    m_alphaState = alphaState;
    setModified();
}

/////////////////////////////////////////////////////////
// testMess
//
/////////////////////////////////////////////////////////
void alpha :: testMess(int alphaTest)
{
    m_alphaTest = alphaTest;
    setModified();
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void alpha :: obj_setupCallback(t_class *classPtr)
{
    class_addfloat(classPtr, (t_method)&alpha::alphaMessCallback);
    class_addmethod(classPtr, (t_method)&alpha::testMessCallback,
    	gensym("test"), A_FLOAT, A_NULL);
}
void alpha :: alphaMessCallback(void *data, t_floatarg alphaState)
{
    GetMyClass(data)->alphaMess((int)alphaState);
}
void alpha :: testMessCallback(void *data, t_floatarg alphaTest)
{
    GetMyClass(data)->testMess((int)alphaTest);
}
