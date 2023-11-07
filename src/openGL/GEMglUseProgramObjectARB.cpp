////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// Implementation file
//
// Copyright (c) 2005 tigital@mac.com
//  For information on usage and redistribution, and for a DISCLAIMER
//  *  OF ALL WARRANTIES, see the file, "GEM.LICENSE.TERMS"
//
////////////////////////////////////////////////////////

#include "GEMglUseProgramObjectARB.h"

CPPEXTERN_NEW ( GEMglUseProgramObjectARB );

/////////////////////////////////////////////////////////
//
// GEMglUseProgramObjectARB
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglUseProgramObjectARB :: GEMglUseProgramObjectARB()
  : m_program(0)
{
  m_inlet = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float,
                      gensym("program"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglUseProgramObjectARB :: ~GEMglUseProgramObjectARB ()
{
  inlet_free(m_inlet);
}

//////////////////
// extension check
bool GEMglUseProgramObjectARB :: isRunnable(void)
{
#ifndef __APPLE__
  if(GLEW_ARB_shader_objects) {
    return true;
  }
#endif
  error("ARB shader_objects not supported by this system");
  return false;
}


/////////////////////////////////////////////////////////
// Render
//
void GEMglUseProgramObjectARB :: render(GemState *state)
{
  glUseProgramObjectARB ( m_program );
}

/////////////////////////////////////////////////////////
// postrender
//
void GEMglUseProgramObjectARB :: postrender(GemState *state)
{
  glUseProgramObjectARB (0);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglUseProgramObjectARB :: programMess (int program)      // FUN
{
#ifndef __APPLE__
  m_program = static_cast<GLhandleARB>(program);
  setModified();
#endif
}

/////////////////////////////////////////////////////////
// static member functions
//

void GEMglUseProgramObjectARB :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&GEMglUseProgramObjectARB::programMessCallback),
                  gensym("program"), A_FLOAT, A_NULL);
}

void GEMglUseProgramObjectARB :: programMessCallback (void* data,
    t_float program)
{
  GetMyClass(data)->programMess (static_cast<int>(program));
}
