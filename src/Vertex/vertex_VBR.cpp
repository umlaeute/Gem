////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-2000 Mark Danks.
//    Copyright (c) G¸nther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f¸r::uml‰ute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "vertex_VBR.h"
#include "Base/GemCache.h"
#include <math.h>

CPPEXTERN_NEW_WITH_GIMME(vertex_VBR)
 
/////////////////////////////////////////////////////////
//
// vertex_VBR
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
vertex_VBR :: vertex_VBR(int argc, t_atom*argv) : vertex_add(argc, argv)
{}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
vertex_VBR :: ~vertex_VBR()
{}


/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
// we assume that "lsize" and "rsize" are >0
// we assume that "larray" and "larray" point somewhere
// checking is done in render()
void vertex_VBR :: vertexProcess(int lsize, float*larray, int rsize, float*rarray){
  float indR=0.f; // the right-hand index
  float incR=(float)rsize/(float)lsize; // the right-hand increment

  for(int i=0; i<lsize; i++){
    const int I=4*i;
    const int J=16*(int)indR; // i know that this is expensive
    const GLfloat x = larray[I+0];
    const GLfloat y = larray[I+1];
    const GLfloat z = larray[I+2];
    const GLfloat w = larray[I+3];
    
    larray[I+0]=x*rarray[J+0]+y*rarray[J+1]+z*rarray[J+2]+w*rarray[J+3];
    larray[I+1]=x*rarray[J+4]+y*rarray[J+5]+z*rarray[J+6]+w*rarray[J+7];
    larray[I+2]=x*rarray[J+8]+y*rarray[J+9]+z*rarray[J+10]+w*rarray[J+11];
    larray[I+3]=x*rarray[J+12]+y*rarray[J+13]+z*rarray[J+14]+w*rarray[J+15];
    indR+=incR;
  }
}

void vertex_VBR :: rightRender(GemState *state, GemCache *cache)
{
  if(m_rightSize!=state->VertexArraySize){
    m_rightSize          = state->VertexArraySize;
    
    if(m_rightVertexArray)delete[]m_rightVertexArray;
    m_rightVertexArray=new GLfloat[m_rightSize*16];
  }
  GLfloat*array=state->VertexArray;

  if((state->VertexDirty) || (cache && cache->m_magic == GEMCACHE_MAGIC && cache->vertexDirty)){
    for(int i=0; i<m_rightSize; i++){
      GLfloat x = array[4*i+0];
      GLfloat y = array[4*i+1];
      GLfloat z = array[4*i+2];
      
      const GLfloat length =sqrtf(x*x+y*y+z*z);
      const GLfloat ilength=(length>0.f)?1.f/length:1.f;
      
      x*=ilength;
      y*=ilength;
      z*=ilength;
      
      const GLfloat c = cosf(length);
      const GLfloat s = sinf(length);
      const GLfloat t = (1.f-c);
      
      //post("%d:\t%f\t%f\t%f\t\t%f %f %f", i, x, y, z, c, s, t);

      m_rightVertexArray[i*16+ 0] =t*x*x+c;
      m_rightVertexArray[i*16+ 1] =t*x*y-s*z;
      m_rightVertexArray[i*16+ 2] =t*x*z+s*y;
      m_rightVertexArray[i*16+ 3] =0.f;

      m_rightVertexArray[i*16+ 4] =t*x*y+s*z;
      m_rightVertexArray[i*16+ 5] =t*y*y+c;
      m_rightVertexArray[i*16+ 6] =t*y*z-s*x;
      m_rightVertexArray[i*16+ 7] =0.f;
      
      m_rightVertexArray[i*16+ 8] =t*x*y-s*y;
      m_rightVertexArray[i*16+ 9] =t*y*z+s*x;
      m_rightVertexArray[i*16+10] =t*z*z+c;
      m_rightVertexArray[i*16+11] =0.f;
      
      m_rightVertexArray[i*16+12] =0.f;
      m_rightVertexArray[i*16+13] =0.f;
      m_rightVertexArray[i*16+14] =0.f;
      m_rightVertexArray[i*16+15] =1.f;
    }

    for(int i=0; i<4; i++){
      for(int j=0; j<4; j++){
      startpost("%f ", m_rightVertexArray[i*4+j]);
      }
      endpost();
    }
    endpost();


    setModified();
  }
} 


/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void vertex_VBR :: obj_setupCallback(t_class *classPtr)
{}
