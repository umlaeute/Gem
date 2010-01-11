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


CPPEXTERN_NEW_WITH_GIMME(separator)

/////////////////////////////////////////////////////////
//
// separator
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
separator :: separator(int argc, t_atom*argv)
{
    m_pushed[GemMan::STACKMODELVIEW ]=false;
    m_pushed[GemMan::STACKCOLOR     ]=false;
    m_pushed[GemMan::STACKTEXTURE   ]=false;
    m_pushed[GemMan::STACKPROJECTION]=false;

    if(argc) {
      m_active[GemMan::STACKMODELVIEW ]=false;
      m_active[GemMan::STACKCOLOR     ]=false;
      m_active[GemMan::STACKTEXTURE   ]=false;
      m_active[GemMan::STACKPROJECTION]=false;

      while(argc) {
	t_symbol*s=atom_getsymbol(argv);
	if(gensym("model")==s) m_active[GemMan::STACKMODELVIEW ]=true;
	else if(gensym("modelview" )==s) m_active[GemMan::STACKMODELVIEW ]=true;
	else if(gensym("color"     )==s) m_active[GemMan::STACKCOLOR     ]=true;
	else if(gensym("texture"   )==s) m_active[GemMan::STACKTEXTURE   ]=true;
	else if(gensym("projection")==s) m_active[GemMan::STACKPROJECTION]=true;
	else if(gensym("m")==s) m_active[GemMan::STACKMODELVIEW ]=true;
	else if(gensym("c")==s) m_active[GemMan::STACKCOLOR     ]=true;
	else if(gensym("t")==s) m_active[GemMan::STACKTEXTURE   ]=true;
	else if(gensym("p")==s) m_active[GemMan::STACKPROJECTION]=true;
	else {
	  throw(GemException("invalid separator mode"));
	}

	argc--;
	argv++;
      }

    } else {
      m_active[GemMan::STACKMODELVIEW ]=true;
      m_active[GemMan::STACKCOLOR     ]=true;
      m_active[GemMan::STACKTEXTURE   ]=true;
      m_active[GemMan::STACKPROJECTION]=true;
    }
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

  if(m_active[GemMan::STACKMODELVIEW] && 
     state->stackDepth[GemMan::STACKMODELVIEW]<GemMan::maxStackDepth[GemMan::STACKMODELVIEW])
    {
      glMatrixMode(GL_MODELVIEW);
      glPushMatrix();
      m_pushed[GemMan::STACKMODELVIEW]=true;
    }
  state->stackDepth[GemMan::STACKMODELVIEW]++;

  if(m_active[GemMan::STACKCOLOR] && 
     state->stackDepth[GemMan::STACKCOLOR]<GemMan::maxStackDepth[GemMan::STACKCOLOR])
    {
      glMatrixMode(GL_COLOR);
      glPushMatrix();
      m_pushed[GemMan::STACKCOLOR]=true;
    }
  state->stackDepth[GemMan::STACKCOLOR]++;

#ifdef __GNUC__
# warning push/pop texture matrix has to be done per texunit
  // each texunit has it's own matrix to be pushed/popped
  // changing the texunit (e.g. in [pix_texture]) makes the 
  // local depthcounter a useless, and we get a lot of 
  // stack under/overflows
#endif  
  if(m_active[GemMan::STACKTEXTURE] && 
     state->stackDepth[GemMan::STACKTEXTURE]<GemMan::maxStackDepth[GemMan::STACKTEXTURE])
    {
      glMatrixMode(GL_TEXTURE);
      glPushMatrix();
      m_pushed[GemMan::STACKTEXTURE]=true;
    }
  state->stackDepth[GemMan::STACKTEXTURE]++;


  if(m_active[GemMan::STACKPROJECTION] && 
     state->stackDepth[GemMan::STACKPROJECTION]<GemMan::maxStackDepth[GemMan::STACKPROJECTION])
    {
      glMatrixMode(GL_PROJECTION);
      glPushMatrix();
      m_pushed[GemMan::STACKPROJECTION]=true;
    }
  state->stackDepth[GemMan::STACKPROJECTION]++;


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

    glMatrixMode(GL_MODELVIEW);
}

/////////////////////////////////////////////////////////
// postrender
//
/////////////////////////////////////////////////////////
void separator :: postrender(GemState *state)
{
  // pop the current matrix stacks
  state->stackDepth[GemMan::STACKPROJECTION]--;
  if(m_pushed[GemMan::STACKPROJECTION]){
    glMatrixMode(GL_PROJECTION); 
    glPopMatrix();
    m_pushed[GemMan::STACKPROJECTION]=false;
  }
  state->stackDepth[GemMan::STACKTEXTURE]--;
  if(m_pushed[GemMan::STACKTEXTURE]){
    glMatrixMode(GL_TEXTURE); 
    glPopMatrix();
    m_pushed[GemMan::STACKTEXTURE]=false;
  }
  state->stackDepth[GemMan::STACKCOLOR]--;
  if(m_pushed[GemMan::STACKCOLOR]){
    glMatrixMode(GL_COLOR); 
    glPopMatrix();
    m_pushed[GemMan::STACKCOLOR]=false;
  }
  state->stackDepth[GemMan::STACKMODELVIEW]--;
  if(m_pushed[GemMan::STACKMODELVIEW]){
    glMatrixMode(GL_MODELVIEW); 
    glPopMatrix();
    m_pushed[GemMan::STACKMODELVIEW]=false;
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
