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
#include "Base/GemException.h"

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
  m_state.set("gl.tex.coords", static_cast<TexCoord*>(NULL));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
separator :: ~separator()
{
  TexCoord*tc=NULL;
  m_state.get("gl.tex.coords", tc);
  if(tc)
    delete [] tc;
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
#define SEPARATOR_STATEASSIGN(src, dst, T, key) do { T value; if ((src)->get((key), value)){ (dst)->set(key, value); } } while(0)

void separator :: render(GemState *state)
{
  if(!state)return;
  using namespace gem;
  GLStack*stacks=NULL;
  state->get("gl.stacks", stacks);
  // push the current matrix stacks
  if(stacks) {
#define PUSHGLSTACK(type)     if(m_active[type])m_pushed[type]=stacks->push(type)
    PUSHGLSTACK(GLStack::TEXTURE);
    PUSHGLSTACK(GLStack::COLOR); 
    PUSHGLSTACK(GLStack::PROJECTION);
    PUSHGLSTACK(GLStack::MODELVIEW);
  }

#ifdef __GNUC__
# warning use GemState copy rather than manually copying each state!
#endif

  SEPARATOR_STATEASSIGN(state, &m_state, bool, "gl.lighting");
  SEPARATOR_STATEASSIGN(state, &m_state, bool, "gl.smooth");
  SEPARATOR_STATEASSIGN(state, &m_state, int, "gl.tex.type");
  SEPARATOR_STATEASSIGN(state, &m_state, int, "gl.tex.numcoords");
  SEPARATOR_STATEASSIGN(state, &m_state, pixBlock*, "pix");

  TexCoord *myCoords=NULL, *coords=NULL;
  int mynum=0, num=0;

  m_state.get("gl.tex.coords", myCoords);
  state->get ("gl.tex.coords", coords);

  m_state.get("gl.tex.numcoords", mynum);
  state->get ("gl.tex.numcoords", num);


  if(mynum != num) {
    if(myCoords)delete [] myCoords; myCoords=NULL;
    if(coords) {
      myCoords = new TexCoord[num];
    }
  }

  if (myCoords && coords) {
    state->get("gl.tex.numcoords", num);
    for (int i = 0; i < num; i++)  {
      myCoords[i].s = coords[i].s;
      myCoords[i].t = coords[i].t;
    }
  } 

  m_state.set("gl.tex.coords", myCoords);
  m_state.set("gl.tex.numcoords", num);

  glMatrixMode(GL_MODELVIEW);
}

/////////////////////////////////////////////////////////
// postrender
//
/////////////////////////////////////////////////////////
void separator :: postrender(GemState *state)
{
  if(!state)return;
  using namespace gem;
  GLStack*stacks=NULL;
  state->get("gl.stacks", stacks);
  // pop the current matrix stacks
  if(stacks) {
#define POPGLSTACK(type)     if(m_pushed[type]){stacks->pop(type);}m_pushed[type]=false
    POPGLSTACK(GLStack::TEXTURE);
    POPGLSTACK(GLStack::COLOR); 
    POPGLSTACK(GLStack::PROJECTION);
    POPGLSTACK(GLStack::MODELVIEW); 
  }
  SEPARATOR_STATEASSIGN(&m_state, state, bool, "gl.lighting");
  SEPARATOR_STATEASSIGN(&m_state, state, bool, "gl.smooth");
  SEPARATOR_STATEASSIGN(&m_state, state, int, "gl.tex.type");
  SEPARATOR_STATEASSIGN(&m_state, state, pixBlock*, "pix");
	
//this is a partial fix for the separator memory leak
//
//if the texcoords are of equal number, which they almost always are
//then just copy them in a loop without delete and new being done
//
//when texcoords are not equal the memory leak happens as usual
  TexCoord *myCoords=NULL, *stateCoords=NULL;
  int mynum=0, num=0;

  m_state.get("gl.tex.coords", myCoords);
  state->get ("gl.tex.coords", stateCoords);

  m_state.get("gl.tex.numcoords", mynum);
  state->get ("gl.tex.numcoords", num);


  if(mynum != num) {
    //if(stateCoords)delete [] stateCoords;
    stateCoords=NULL;
    if(myCoords) {
      stateCoords = new TexCoord[mynum];
      num=mynum;
      post("state->numTexCoords %d != m_state.numTexCoords %d",mynum,num);
    }
  }

  if (myCoords && stateCoords) {
    for (int i = 0; i < mynum; i++)  {
      stateCoords[i].s = myCoords[i].s;
      stateCoords[i].t = myCoords[i].t;
    }
  } 

  state->set("gl.tex.coords", stateCoords);
  state->set("gl.tex.numcoords", num);
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void separator :: obj_setupCallback(t_class *)
{ }
