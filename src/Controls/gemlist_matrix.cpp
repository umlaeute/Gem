////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// Implementation file
//
// Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//	zmoelnig@iem.kug.ac.at
//  For information on usage and redistribution, and for a DISCLAIMER
//  *  OF ALL WARRANTIES, see the file, "GEM.LICENSE.TERMS"
//
//   
////////////////////////////////////////////////////////

#include "gemlist_matrix.h"

#include "math.h"

#ifdef __ppc__
#include "Base/GemFuncUtil.h"
#undef sqrt
#define sqrt fast_sqrtf
#endif

#define rad2deg -57.2957795132


CPPEXTERN_NEW_WITH_ONE_ARG ( gemlist_matrix , t_floatarg, A_DEFFLOAT )

/////////////////////////////////////////////////////////
//
// gemlist_matrix
//
/////////////////////////////////////////////////////////
// Constructor
//
gemlist_matrix :: gemlist_matrix	(t_floatarg arg0=0) {
#ifndef GL_VERSION_1_1
  error("gemlist_matrix: GEM was compiled without GL_VERSION_1_1");
  error("gemlist_matrix: therefore this object will do nothing");
#endif
  m_outletMatrice = outlet_new(this->x_obj, 0);
}
/////////////////////////////////////////////////////////
// Destructor
//
gemlist_matrix :: ~gemlist_matrix () {
  outlet_free(m_outletMatrice);
}

/////////////////////////////////////////////////////////
// Render
//
void gemlist_matrix :: render(GemState *state) {
#ifdef GL_VERSION_1_1
  float mi[16]={0};
  int i;
  t_atom alist[16];

  glGetFloatv(GL_MODELVIEW_MATRIX,mi);

  for (i=0;i<16;i++)
  { 
    SETFLOAT(alist+i, mi[i]);
  }
  outlet_list (m_outletMatrice, &s_list, 16, alist);
#endif // GL_VERSION_1_1
}

/////////////////////////////////////////////////////////
// static member functions
//
void gemlist_matrix :: obj_setupCallback(t_class *classPtr) {
}
