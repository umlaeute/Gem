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
#include "Base/GemMan.h"


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

  /* GL_COLOR is only accepted if GL_ARB_imaging is present */
  if(GLEW_ARB_imaging && state->stackDepth[1]<GemMan::maxStackDepth[1]){
    glMatrixMode(GL_COLOR);
    glPushMatrix();
  }
  state->stackDepth[1]++;

  if(state->stackDepth[2]<GemMan::maxStackDepth[2]){
    glMatrixMode(GL_TEXTURE);
    glPushMatrix();
  }
  state->stackDepth[2]++;

  if(state->stackDepth[0]<GemMan::maxStackDepth[0]){
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
  }
  state->stackDepth[0]++;

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
  state->stackDepth[1]--;
  if(GLEW_ARB_imaging && state->stackDepth[1]<GemMan::maxStackDepth[1]){
    glMatrixMode(GL_COLOR);
    glPopMatrix();
  }
  state->stackDepth[2]--;
  if(state->stackDepth[2]<GemMan::maxStackDepth[2]){
    glMatrixMode(GL_TEXTURE);
    glPopMatrix();
  }
  state->stackDepth[0]--;
  if(state->stackDepth[0]<GemMan::maxStackDepth[0]){
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
  }

    state->lighting 	= m_state.lighting;
    state->smooth   	= m_state.smooth;
    state->texture  	= m_state.texture;
    state->image    	= m_state.image;
	
//this is a partial fix for the separator memory leak
//
//if the texcoords are of equal number, which they almost always are
//then just copy them in a loop without delete and new being done
//
//when texcoords are not equal the memory leak happens as usual
   
   if (state->numTexCoords != m_state.numTexCoords){
		if (state->texCoords) state->texCoords = NULL;
		state->texCoords = new TexCoord[m_state.numTexCoords];
		post("state->numTexCoords %d != m_state.numTexCoords %d",state->numTexCoords,m_state.numTexCoords);
		state->numTexCoords = m_state.numTexCoords;
   }
   

    if (m_state.texCoords) 
    {
    //	state->texCoords = new TexCoord[m_state.numTexCoords];
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
