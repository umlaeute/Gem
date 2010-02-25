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
#include "Base/GLStack.h"


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
  using namespace gem;
  m_pushed[GLStack::MODELVIEW ]=false;
  m_pushed[GLStack::COLOR     ]=false;
  m_pushed[GLStack::TEXTURE   ]=false;
  m_pushed[GLStack::PROJECTION]=false;
  
  if(argc) {
    m_active[GLStack::MODELVIEW ]=false;
    m_active[GLStack::COLOR     ]=false;
    m_active[GLStack::TEXTURE   ]=false;
    m_active[GLStack::PROJECTION]=false;
    
    while(argc) {
      t_symbol*s=atom_getsymbol(argv);
      if(gensym("model")==s) m_active[GLStack::MODELVIEW ]=true;
      else if(gensym("modelview" )==s) m_active[GLStack::MODELVIEW ]=true;
      else if(gensym("color"     )==s) m_active[GLStack::COLOR     ]=true;
      else if(gensym("texture"   )==s) m_active[GLStack::TEXTURE   ]=true;
      else if(gensym("projection")==s) m_active[GLStack::PROJECTION]=true;
      else if(gensym("m")==s) m_active[GLStack::MODELVIEW ]=true;
      else if(gensym("c")==s) m_active[GLStack::COLOR     ]=true;
      else if(gensym("t")==s) m_active[GLStack::TEXTURE   ]=true;
      else if(gensym("p")==s) m_active[GLStack::PROJECTION]=true;
      else {
        throw(GemException("invalid separator mode"));
      }
      
      argc--;
      argv++;
    }
    
  } else {
    m_active[GLStack::MODELVIEW ]=true;
    m_active[GLStack::COLOR     ]=true;
    m_active[GLStack::TEXTURE   ]=true;
    m_active[GLStack::PROJECTION]=true;
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
  using namespace gem;
  GLStack*stacks=NULL;
  if(state) {
    state->get("gl.stacks", stacks);
  }
  // push the current matrix stacks
  if(stacks) {
#define PUSHGLSTACK(type)     if(m_active[type])m_pushed[type]=stacks->push(type)
    PUSHGLSTACK(GLStack::TEXTURE);
    PUSHGLSTACK(GLStack::COLOR); 
    PUSHGLSTACK(GLStack::PROJECTION);
    PUSHGLSTACK(GLStack::MODELVIEW);
  }

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
  using namespace gem;
  GLStack*stacks=NULL;
  if(state) {
    state->get("gl.stacks", stacks);
  }
  // pop the current matrix stacks
  if(stacks) {
#define POPGLSTACK(type)     if(m_pushed[type]){stacks->pop(type);}m_pushed[type]=false
    POPGLSTACK(GLStack::TEXTURE);
    POPGLSTACK(GLStack::COLOR); 
    POPGLSTACK(GLStack::PROJECTION);
    POPGLSTACK(GLStack::MODELVIEW); 
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
