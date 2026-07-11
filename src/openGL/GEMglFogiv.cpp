////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// Implementation file
//
// SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
// SPDX-License-Identifier: GPL-2.0-or-later
//
// this file has been generated...
////////////////////////////////////////////////////////

#include "GEMglFogiv.h"

CPPEXTERN_NEW_WITH_GIMME ( GEMglFogiv );

/////////////////////////////////////////////////////////
//
// GEMglFogiv
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglFogiv :: GEMglFogiv        (int argc, t_atom *argv)
{
  int i=FOG_ARRAY_LENGTH;
  while(i--) {
    params[i]=0;
  }

  pnameMess(atom_getfloat(argv));
  if (argc>0) {
    paramsMess(argc-1, argv+1);
  }

  m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"),
                         gensym("pname"));
  m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("list"),
                         gensym("params"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglFogiv :: ~GEMglFogiv ()
{
  inlet_free(m_inlet[0]);
  inlet_free(m_inlet[1]);
}

//////////////////
// extension check
bool GEMglFogiv :: isRunnable(void)
{
  if(GLEW_VERSION_1_1) {
    return true;
  }
  error("your system does not support OpenGL-1.1");
  return false;
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglFogiv :: render(GemState *state)
{
  glFogiv (pname, params);
}

/////////////////////////////////////////////////////////
// variable
//
void GEMglFogiv :: pnameMess (t_float arg0)     // FUN
{
  pname=static_cast<GLenum>(arg0);
  setModified();
}
void GEMglFogiv :: paramsMess (int argc, t_atom*argv)   // FUN
{
  int i = (argc<FOG_ARRAY_LENGTH)?argc:FOG_ARRAY_LENGTH;
  while(i--) {
    params[i]=atom_getint(argv+i);
  }
  setModified();
}

/////////////////////////////////////////////////////////
// static member functions
//

void GEMglFogiv :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&GEMglFogiv::pnameMessCallback),
                  gensym("pname"), A_DEFFLOAT, A_NULL);
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&GEMglFogiv::paramsMessCallback),
                  gensym("params"), A_GIMME, A_NULL);
}

void GEMglFogiv :: pnameMessCallback (void* data, t_float arg0)
{
  GetMyClass(data)->pnameMess (arg0);
}
void GEMglFogiv :: paramsMessCallback (void* data, t_symbol*, int argc,
                                       t_atom* argv)
{
  GetMyClass(data)->paramsMess (argc, argv);
}
