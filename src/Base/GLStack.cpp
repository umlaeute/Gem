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
//    Copyright (c) 2002 tigital
//
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "GLStack.h"
#include "Gem/RTE.h"

using namespace gem;

namespace gem {
  class GLStack::Data {
public:
  Data(void) {
    int i=0;
    for(i=0; i<4; i++) {
      stackDepth[i]=0;
      maxDepth[i]=0;
      orgDepth[i]=0;
    }
  }
  int stackDepth[4];
  int maxDepth[4];
  int orgDepth[4];
};
};

static GLenum id2maxdepth(enum GLStack::GemStackId id) {
  GLenum result=0;
  switch(id) {
  case GLStack::MODELVIEW:
    result=GL_MAX_MODELVIEW_STACK_DEPTH; 
    break;
  case GLStack::PROJECTION: 
    result=GL_MAX_PROJECTION_STACK_DEPTH; 
    break;
  case GLStack::TEXTURE: 
    result=GL_MAX_TEXTURE_STACK_DEPTH; 
    break;
  case GLStack::COLOR: 
    result=GL_MAX_COLOR_MATRIX_STACK_DEPTH; 
    break;
  default:
    //    ::error("GLStack: illegal stack matrix: %d");
    break;
  }
  return result;
}


static GLenum id2depth(enum GLStack::GemStackId id) {
  GLenum result=0;
  switch(id) {
  case GLStack::MODELVIEW:
    result=GL_MODELVIEW_STACK_DEPTH; 
    break;
  case GLStack::PROJECTION: 
    result=GL_PROJECTION_STACK_DEPTH; 
    break;
  case GLStack::TEXTURE: 
    result=GL_TEXTURE_STACK_DEPTH; 
    break;
  case GLStack::COLOR: 
    result=GL_COLOR_MATRIX_STACK_DEPTH; 
    break;
  default:
    //    ::error("GLStack: illegal stack matrix: %d");
    break;
  }
  return result;
}


static GLenum id2mode(enum GLStack::GemStackId id) {
  GLenum result=0;
  switch(id) {
  case GLStack::MODELVIEW:
    result=GL_MODELVIEW; 
    break;
  case GLStack::PROJECTION: 
    result=GL_PROJECTION; 
    break;
  case GLStack::TEXTURE: 
    result=GL_TEXTURE; 
    break;
  case GLStack::COLOR: 
    result=GL_COLOR; 
    break;
  default:
    //    ::error("GLStack: illegal stack matrix: %d");
    break;
  }
  return result;
}


GLStack:: GLStack(bool haveValidContext) : data(new Data()) {
  if(haveValidContext) {
    reset();
  }
}
GLStack::~GLStack() {
}

#ifdef __GNUC__
# warning push/pop texture matrix has to be done per texunit
  // each texunit has it's own matrix to be pushed/popped
  // changing the texunit (e.g. in [pix_texture]) makes the 
  // local depthcounter a useless, and we get a lot of 
  // stack under/overflows
#endif  


/** push the given matrix to the stack if the maximum has not been reached 
 *   returns true on success and false otherwise (stack overflow)
 * NOTE: needs valid openGL context
 */
bool GLStack::push(enum GemStackId id) {
  GLenum mode=id2mode(id);
  if(!mode)return false;
  if(data->stackDepth[id]<data->maxDepth[id]) {
    glMatrixMode(mode);
    glPushMatrix();
    data->stackDepth[id]++;
    return true;
  }

  data->stackDepth[id]++;
  return false;
}

void GLStack::push() {
  push(COLOR);
  push(TEXTURE);
  push(PROJECTION);
  push(MODELVIEW);
}


/** pop the given matrix from the stack if the maximum has not been reached 
 *   returns true on success and false otherwise (stack underlow)
 * NOTE: needs valid openGL context
 */
bool GLStack::pop(enum GemStackId id) {
  GLenum mode=id2mode(id);
  if(!mode)return false;

  data->stackDepth[id]--;
  if(data->stackDepth[id]<data->maxDepth[id]) {
    glMatrixMode(mode);
    glPopMatrix();
    return true;
  }
  return false;
}

void GLStack::pop() {
  pop(COLOR);
  pop(TEXTURE);
  pop(PROJECTION);
  pop(MODELVIEW);
}
/** 
 * reset the maximum stack depth of the given stack 
 * NOTE: needs valid openGL context
 */
void GLStack::reset() {
  reset(MODELVIEW);
  reset(PROJECTION);
  reset(TEXTURE);
  reset(COLOR);
}

/** 
 * reset the maximum stack depth of all stacks
 * NOTE: needs valid openGL context
 */
int GLStack::reset(enum GemStackId id) {
  GLenum maxmode=id2maxdepth(id);
  GLenum mode=id2depth(id);

  if(maxmode && mode) {
    if(GL_COLOR != mode || GLEW_ARB_imaging) {
      glGetIntegerv(maxmode, data->maxDepth+id);
      glGetIntegerv(mode, data->stackDepth+id);
    }
    data->orgDepth[id]=data->stackDepth[id];
    return data->stackDepth[id];
  }
  return -1;
}

void GLStack::print() {
  post("MODELVIEW: %02d/%02d",  data->stackDepth[MODELVIEW], data->maxDepth[MODELVIEW]);
  post("PROJECTION: %02d/%02d",  data->stackDepth[PROJECTION], data->maxDepth[PROJECTION]);
  post("TEXTURE: %02d/%02d",  data->stackDepth[TEXTURE], data->maxDepth[TEXTURE]);
  post("COLOR: %02d/%02d",  data->stackDepth[COLOR], data->maxDepth[COLOR]);
}
