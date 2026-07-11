////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// Implementation file
//
// SPDX-FileCopyrightText: © 2004, James Tittle II and the GEM contributors
// SPDX-License-Identifier: GPL-2.0-or-later
//
////////////////////////////////////////////////////////

#include "GEMglLoadTransposeMatrixd.h"

CPPEXTERN_NEW_WITH_ONE_ARG ( GEMglLoadTransposeMatrixd, t_floatarg,
                             A_DEFFLOAT );

/////////////////////////////////////////////////////////
//
// GEMglLoadTransposeMatrixf
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglLoadTransposeMatrixd :: GEMglLoadTransposeMatrixd  (t_floatarg arg0)
{
  m_inlet = inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("list"),
                      gensym("list"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglLoadTransposeMatrixd :: ~GEMglLoadTransposeMatrixd ()
{
  inlet_free(m_inlet);
}

//////////////////
// extension check
bool GEMglLoadTransposeMatrixd :: isRunnable(void)
{
  if(GLEW_VERSION_1_3) {
    return true;
  }
  error("your system does not support OpenGL-1.3");
  return false;
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglLoadTransposeMatrixd :: render(GemState *state)
{
  glLoadTransposeMatrixd (m_matrix);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglLoadTransposeMatrixd :: matrixMess (int argc,
    t_atom*argv)    // FUN
{
  if(argc!=16) {
    error("need 16 (4x4) elements");
    return;
  }
  int i;
  for (i=0; i<16; i++) {
    m_matrix[i]=static_cast<GLdouble>(atom_getfloat(argv+i));
  }
  setModified();
}

/////////////////////////////////////////////////////////
// static member functions
//

void GEMglLoadTransposeMatrixd :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&GEMglLoadTransposeMatrixd::matrixMessCallback),
                  gensym("list"), A_GIMME, A_NULL);
}

void GEMglLoadTransposeMatrixd :: matrixMessCallback (void* data,
    t_symbol*, int argc, t_atom*argv)
{
  GetMyClass(data)->matrixMess ( argc, argv);
}
