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

#include "separator.h"

CPPEXTERN_NEW(separator)

/////////////////////////////////////////////////////////
//
// separator
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
separator :: separator()
{
    m_state.texCoords = NULL;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
separator :: ~separator()
{
    delete [] m_state.texCoords;
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void separator :: render(GemState *state)
{
    // push the current matrix stacks
    glMatrixMode(GL_COLOR);
    glPushMatrix();
    glMatrixMode(GL_TEXTURE);
    glPushMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    
    m_state.lighting 	 = state->lighting;
    m_state.smooth   	 = state->smooth;
    m_state.texture  	 = state->texture;
    m_state.image   	 = state->image;
    m_state.numTexCoords = state->numTexCoords;
    
    if(m_state.texCoords)delete [] m_state.texCoords;
    if (state->texCoords)
    {
    	m_state.texCoords = new TexCoord[m_state.numTexCoords];
    	for (int i = 0; i < m_state.numTexCoords; i++)
    	{
    	    m_state.texCoords[i].s = state->texCoords[i].s;
    	    m_state.texCoords[i].t = state->texCoords[i].t;
    	}
    }
    else m_state.texCoords = NULL;
}

/////////////////////////////////////////////////////////
// postrender
//
/////////////////////////////////////////////////////////
void separator :: postrender(GemState *state)
{
    // pop the current matrix stacks
    glMatrixMode(GL_COLOR);
    glPopMatrix();
    glMatrixMode(GL_TEXTURE);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    state->lighting 	= m_state.lighting;
    state->smooth   	= m_state.smooth;
    state->texture  	= m_state.texture;
    state->image    	= m_state.image;
    state->numTexCoords = m_state.numTexCoords;
#ifdef CRASH
    post("likely to crash:"); // do we have a memory hole without ? i think so...
    if (state->texCoords)delete [] (state->texCoords);
#endif
    if (m_state.texCoords)
    {
    	state->texCoords = new TexCoord[m_state.numTexCoords];
    	for (int i = 0; i < m_state.numTexCoords; i++)
    	{
    	    state->texCoords[i].s = m_state.texCoords[i].s;
    	    state->texCoords[i].t = m_state.texCoords[i].t;
    	}
    }
    else state->texCoords = NULL;
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void separator :: obj_setupCallback(t_class *)
{ }
