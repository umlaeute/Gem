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

#include "vertex_matrix.h"

#include "Base/GemState.h"

CPPEXTERN_NEW_WITH_GIMME(vertex_matrix)

/////////////////////////////////////////////////////////
//
// vertex_matrix
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
vertex_matrix :: vertex_matrix(int argc, t_atom*argv) : vertex_scale(argc, argv)
{
  if(argc==0){
    m_00=m_10=m_20=m_30=0.f;
    m_01=m_11=m_21=m_31=0.f;
    m_02=m_12=m_22=m_32=0.f;
    m_03=m_13=m_23=m_33=0.f;

    m_00=m_11=m_22=m_33=1.f;
  } 
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
vertex_matrix :: ~vertex_matrix()
{ }

void vertex_matrix :: paramMess(int argc, t_atom*argv){
  m_03=m_13=m_23=m_30=m_31=m_32=0.f;
  m_33=1.f;

  switch (argc){
  case 16:
    m_00=atom_getfloat(argv+0+0);
    m_01=atom_getfloat(argv+1+0);
    m_02=atom_getfloat(argv+2+0);
    m_03=atom_getfloat(argv+3+0);

    m_10=atom_getfloat(argv+0+4);
    m_11=atom_getfloat(argv+1+4);
    m_12=atom_getfloat(argv+2+4);
    m_13=atom_getfloat(argv+3+4);

    m_20=atom_getfloat(argv+0+8);
    m_21=atom_getfloat(argv+1+8);
    m_22=atom_getfloat(argv+2+8);
    m_23=atom_getfloat(argv+3+8);

    m_30=atom_getfloat(argv+0+12);
    m_31=atom_getfloat(argv+1+12);
    m_32=atom_getfloat(argv+2+12);
    m_33=atom_getfloat(argv+3+12);
    break;
  default:
    error("vertex_matrix: offset must be 4x4 values!");
    break;
  }
  setModified();
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void vertex_matrix :: vertexProcess(int size, GLfloat*array){
  int count;
   
  if (m_offset < 0) m_offset = 0;
  if (m_offset > size) m_offset = size;
  count = m_count;
  
  if (count < 1) count = 1;
  if ((count + m_offset-1) > size) count = size - m_offset;// -1;
  
  //needs some altivec 
  if (m_offset){
    int src = (m_offset-1) * 4;
    for (int i=0; i< count; i++){
      const GLfloat x = array[src+0];
      const GLfloat y = array[src+1];
      const GLfloat z = array[src+2];
      const GLfloat w = array[src+3];

      array[src]   = m_00*x+m_01*y+m_02*z+m_03*w;
      array[src+1] = m_10*x+m_11*y+m_12*z+m_13*w;
      array[src+2] = m_20*x+m_21*y+m_22*z+m_23*w;
      array[src+3] = m_30*x+m_31*y+m_32*z+m_33*w;
      src+=4;
    }
  }else{
    int src=0;
    for (int i=0; i< size; i++){
      const GLfloat x = array[src+0];
      const GLfloat y = array[src+1];
      const GLfloat z = array[src+2];
      const GLfloat w = array[src+3];

      array[src]   = m_00*x+m_01*y+m_02*z+m_03*w;
      array[src+1] = m_10*x+m_11*y+m_12*z+m_13*w;
      array[src+2] = m_20*x+m_21*y+m_22*z+m_23*w;
      array[src+3] = m_30*x+m_31*y+m_32*z+m_33*w;
      src+=4;
    }
  }
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void vertex_matrix :: obj_setupCallback(t_class *classPtr)
{ 
  class_addmethod(classPtr, (t_method)&vertex_matrix::paramMessCallback,
		  gensym("offset"), A_GIMME, A_NULL);
  class_addmethod(classPtr, (t_method)&vertex_matrix::paramMessCallback,
		  gensym("param"), A_GIMME, A_NULL);
}
