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
#include "GLStack.h"

#include <map>
#include <memory>

#include <iostream>

using namespace gem;

class GemStateData {
  friend class GemState;
 public:
  GemStateData(void) : stacks(new GLStack()){}

  ~GemStateData(void) {
    if (NULL==stacks.get()){
      post("ouch");
      //      const GLStack*dummy=new GLStack();
      //stacks=dummy;
      stacks.reset();
      post("yaroooo!");
    }
  }

  GemStateData& copyFrom(const GemStateData*org) {
    data=org->data;
    stacks->reset();
    return (*this);
  }

 protected:
  // dictionary for setting values
  std::map <t_symbol*, any> data;

  std::auto_ptr<GLStack>stacks;
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
        TexCoordArray(0), HaveTexCoordArray(0),
        data(new GemStateData())
{

  //  std::cout << "GemState" << std::endl;

  stackDepth[GemMan::STACKMODELVIEW]=
    stackDepth[GemMan::STACKCOLOR]=
    stackDepth[GemMan::STACKTEXTURE]=
    stackDepth[GemMan::STACKPROJECTION]=
    1; // 1 is the current matrix

  set("dirty", false);
  set("inDisplayList", false);

  set("gl.lighting", false); 
  set("gl.smooth", false); 
  set("gl.tex.type", 0);
  //  set("pix", 0);
  set("gl.tex.coords", 0); 
  set("gl.tex.units", 0);
  set("timing.tick", 50.f); 
  set("gl.drawtype", 0);

  set("gl.stacks", data->stacks.get());

  /*
    set("vertex.array.vertex", 0); 
    set("vertex.array.color", 0); 
    set("vertex.array.normal", 0); 
    set("vertex.array.texcoord", 0); 
  */
}

GemState& GemState::operator=(const GemState&org) {
  dirty=org.dirty;
  inDisplayList=org.inDisplayList;
  lighting=org.lighting;
  smooth=org.smooth;
  texture=org.texture;
  image=org.image;
  texCoords=org.texCoords;
  numTexCoords=org.numTexCoords;
  multiTexUnits=org.multiTexUnits;
  tickTime=org.tickTime;
  drawType=org.drawType;
  VertexArray=org.VertexArray;
  VertexArraySize=org.VertexArraySize;
  VertexArrayStride=org.VertexArrayStride;
  ColorArray=org.ColorArray;
  HaveColorArray=org.HaveColorArray;
  NormalArray=org.NormalArray;
  HaveNormalArray=org.HaveNormalArray;
  TexCoordArray=org.TexCoordArray;
  HaveTexCoordArray=org.HaveTexCoordArray;

  data->copyFrom(org.data);
  return (*this);
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

  if(GemMan::windowExists()) {
    GLStack *stacks;
    get("gl.stacks", stacks);
    stacks->reset();
  }
}

GemState :: ~GemState() {  
  if(data)delete data;data=NULL;
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


/* real properties */


/* get a named property */
bool GemState::get(t_symbol*key, any&value) {
  std::map<t_symbol*,any>::iterator it = data->data.find(key);

  if(it==data->data.end()) {
    return false;
  }

  value=it->second;
  return true;
}

/* set a named property */
bool GemState::set(t_symbol*key, any value) {
  if(value.empty()) {
    data->data.erase(key);
    return false;
  }

  /* wrapper for DEPRECATED access to member variables */
  // ...

  data->data[key]=value;
  return true;
}

/* remove a named property */
bool GemState::remove(t_symbol*key) {
  return (0!=data->data.erase(key));
}

