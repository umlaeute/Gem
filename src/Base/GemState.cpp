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
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute
//
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "GemState.h"
#include "Base/GemGL.h"

/* for GemMan::StackIDs */
#include "GemMan.h"

class GemStateData {
  friend class GemState;
 public:
  GemStateData(void);
  ~GemStateData(void);

 protected:
#if 0
  // dictionary for setting values
  std::map <char*, t_atom*> data;

  virtual t_atom*get(char*name) {
    return data[name];
  }
  virtual void set(char*name, t_atom*value) {
    // LATER: we should expand envvariables
    if(value) {
      t_atom*a=(t_atom*)getbytes(sizeof(t_atom));
      memcpy(a, value, sizeof(t_atom));
      data[name]=a;
    } else {
      data.erase(name);
    }
  }
  void set(char*name, int i) {
    t_atom a;
    SETFLOAT(&a, i);
    set(name, &a);
  }
  void set(char*name, t_float f) {
    t_atom a;
    SETFLOAT(&a, f);
    set(name, &a);
  }
  void set(char*name, char*s) {
    t_atom a;
    SETSYMBOL(&a, s);
    set(name, &a);
  }
  void print(void) {
    std::map <char*, t_atom*>::iterator it;
    for(it = data.begin(); 
        it != data.end();
        it++)
      {
        if(it->first && it->first->s_name && it->second) {
          startpost("key ['%s']: ", it->first->s_name);
          postatom(1, it->second);
          endpost();
        }
      }
  }
#endif
};

/////////////////////////////////////////////////////////
//
// GemState
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GemState :: GemState()
		  : dirty(0), inDisplayList(0), lighting(0), smooth(0), texture(0),
                    image(0), texCoords(0), numTexCoords(0), multiTexUnits(0),
                    tickTime(50.f), drawType(0),
                    VertexArray(0), VertexArraySize(0), VertexArrayStride(0),
                    ColorArray(0), HaveColorArray(0),
                    NormalArray(0), HaveNormalArray(0),
                    TexCoordArray(0), HaveTexCoordArray(0)
{
  stackDepth[GemMan::STACKMODELVIEW]=
    stackDepth[GemMan::STACKCOLOR]=
    stackDepth[GemMan::STACKTEXTURE]=
    stackDepth[GemMan::STACKPROJECTION]=
    1; // 1 is the current matrix
}

void GemState :: reset() {
  image = 0;
  VertexArray = 0;
  VertexArraySize = 0;
  ColorArray = 0;
  NormalArray = 0;
  TexCoordArray = 0;
  HaveColorArray = 0;
  HaveNormalArray = 0;
  HaveTexCoordArray = 0;
  drawType = 0;

  stackDepth[GemMan::STACKMODELVIEW]=
    stackDepth[GemMan::STACKCOLOR]=
    stackDepth[GemMan::STACKTEXTURE]=
    stackDepth[GemMan::STACKPROJECTION]=
    1; // 1 is the current matrix

  if(GemMan::windowExists()) {
    glGetIntegerv(GL_MODELVIEW_STACK_DEPTH, &stackDepth[GemMan::STACKMODELVIEW]);
    glGetIntegerv(GL_PROJECTION_STACK_DEPTH, &stackDepth[GemMan::STACKPROJECTION]);
    glGetIntegerv(GL_TEXTURE_STACK_DEPTH, &stackDepth[GemMan::STACKTEXTURE]);
    glGetIntegerv(GL_COLOR_MATRIX_STACK_DEPTH, &stackDepth[GemMan::STACKCOLOR]);
  }
}

GemState :: ~GemState() {
  reset();
}

bool GemState::get(const char*key, long&value) {
  return false;
}
bool GemState::get(const char*key, double&value){
  return false;
}
bool GemState::get(const char*key, char*&value) {
  return false;
}

/* raw accessor: returns a pointer to shallow copy of an anonymous type
 * all get's get the same copy
 * the copy is controlled by GemState (e.g. freed, when GemState is destroyed)
 * you have to know the type of value yourself
 * use at your own risk
 */
bool GemState::get(const char*key, void*&value, size_t&length) {
  return false;
}
/* raw accessor: returns an anonymous pointer
 * the pointer is controlled by the setter
 * you have to know the type of value yourself
 * use at your own risk
 */
bool GemState::get(const char*key, void*&value) {
  return false;
}


/* set a named property */
bool GemState::set(const char*key, const long value) {
  return false;
}
bool GemState::set(const char*key, const double value) {
  return false;
}
bool GemState::set(const char*key, const char*value) {
  return false;
}
bool GemState::set(const char*key, const void*value ) {
  return false;
}
bool GemState::set(const char*key, const void*value, const size_t length) {
  return false;
}

/* remove a named property */
bool GemState::remove(const char*key) {
  return false;
}



// --------------------------------------------------------------
/* legacy functions */
float GemState::texCoordX(int num) const {
  if (texture && numTexCoords > num)
    return texCoords[num].s;
  else return 0.;
}

float GemState::texCoordY(int num) const {
  if (texture && numTexCoords > num)
    return texCoords[num].t;
  else return 0.;
}
