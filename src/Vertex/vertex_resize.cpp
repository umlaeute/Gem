////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-2000 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "vertex_resize.h"

#include "Base/GemState.h"

CPPEXTERN_NEW_WITH_ONE_ARG(vertex_resize, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// vertex_resize
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
vertex_resize :: vertex_resize(t_floatarg f) : GemVertex(),
					       m_size(0),
					       m_vertexArray(NULL),
					       m_colorArray(NULL),
					       m_normalArray(NULL),
					       m_texCoordArray(NULL)
{
  m_sizeIn=inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("size"));
  m_size=(f>0.f)?(int)f:0;
  sizeMess(m_size);
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
vertex_resize :: ~vertex_resize()
{
  inlet_free(m_sizeIn);
  if(m_vertexArray)  delete[]m_vertexArray;
  if(m_colorArray)   delete[]m_colorArray;
  if(m_normalArray)  delete[]m_normalArray;
  if(m_texCoordArray)delete[]m_texCoordArray;
}


void vertex_resize :: sizeMess(int size){
  post("vertex_resize: %d", size);
  m_size=(size>0)?size:0;
  if(m_size==0)return;

  if(m_vertexArray)delete[]m_vertexArray;
  m_vertexArray  =new GLfloat[m_size*4];

  if(m_colorArray)delete[]m_colorArray;
  m_colorArray   =new GLfloat[m_size*4];

  if(m_normalArray)delete[]m_normalArray;
  m_normalArray  =new GLfloat[m_size*4];

  if(m_texCoordArray)delete[]m_texCoordArray;
  m_texCoordArray=new GLfloat[m_size*4];

  setModified();
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void vertex_resize :: vertexProcess(const int oldsize, GLfloat*oldarray, 
				    const int newsize, GLfloat*newarray){
  float f_ind=0.f; // the org f_index
  float f_inc=(float)oldsize/(float)newsize; // the org.increment

  //post("vertex_resize: (%d, %X) -> (%d, %X)", oldsize, oldarray, newsize, newarray);

  for(int i=0; i<newsize; i++){

    const int I=4*i;
    const int J=4*(int)f_ind; // i know that this is expensive
    newarray[I+0]=oldarray[J+0];
    newarray[I+1]=oldarray[J+1];
    newarray[I+2]=oldarray[J+2];
    newarray[I+3]=oldarray[J+3];
    f_ind+=f_inc;
    //    post("vertex_resized %d %d", i, (int)f_ind);
  }

}


void vertex_resize :: render(GemState *state)
{
  const int orgsize=state->VertexArraySize;
  const int newsize=m_size;

  m_sizeCache=orgsize;
  m_vertexCache=state->VertexArray;
  m_colorCache=state->ColorArray;
  m_normalCache=state->NormalArray;
  m_texCoordCache=state->TexCoordArray;

  if(orgsize<=0) return;
  if(newsize==0) return;
  if(orgsize==newsize)return;

  if(state->VertexArray!=NULL && m_vertexArray!=NULL){
    vertexProcess(orgsize, state->VertexArray, newsize, m_vertexArray);
    state->VertexArray=m_vertexArray;
  }
  if(state->ColorArray!=NULL && m_colorArray!=NULL){
    vertexProcess(orgsize, state->ColorArray, newsize, m_colorArray);
    state->ColorArray=m_colorArray;
  }

#if 0
  if(state->NormalArray!=NULL && m_normalArray!=NULL){
    vertexProcess(orgsize, state->NormalArray, newsize, m_normalArray);
    state->NormalArray=m_normalArray;
  }

  if(state->TexCoordArray!=NULL && m_texCoordArray!=NULL){
    vertexProcess(orgsize, state->TexCoordArray, newsize, m_texCoordArray);
    state->TexCoordArray=m_texCoordArray;
  }
#endif
  state->VertexArraySize=newsize;
}

void vertex_resize :: postrender(GemState *state){
  state->VertexArraySize=   m_sizeCache;
  state->VertexArray    =  m_vertexCache;
  state->ColorArray     =  m_colorCache;
  state->NormalArray    =  m_normalCache;
  state->TexCoordArray  =  m_texCoordCache;
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void vertex_resize :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr, (t_method)&vertex_resize::sizeMessCallback,
		  gensym("size"), A_FLOAT, A_NULL);
}

void vertex_resize :: sizeMessCallback(void *data,  t_floatarg num)
{
  GetMyClass(data)->sizeMess((int)num);
}
