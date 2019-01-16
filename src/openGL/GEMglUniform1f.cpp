////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// Implementation file
//
// Copyright (c) 2002-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//      zmoelnig@iem.at
//  For information on usage and redistribution, and for a DISCLAIMER
//  *  OF ALL WARRANTIES, see the file, "GEM.LICENSE.TERMS"
//
////////////////////////////////////////////////////////

#include "GEMglUniform1f.h"

CPPEXTERN_NEW_WITH_TWO_ARGS ( GEMglUniform1f, t_floatarg, A_DEFFLOAT,
                              t_floatarg, A_DEFFLOAT);

/////////////////////////////////////////////////////////
//
// GEMglUniform1f
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglUniform1f :: GEMglUniform1f  (t_floatarg arg0,
    t_floatarg arg1) :
  location(static_cast<GLint>(arg0)),
  val(static_cast<GLfloat>(arg1))
{
  m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float,
                         gensym("location"));
  m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float,
                         gensym("val"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglUniform1f :: ~GEMglUniform1f ()
{
  inlet_free(m_inlet[0]);
  inlet_free(m_inlet[1]);
}

//////////////////
// extension check
bool GEMglUniform1f :: isRunnable(void)
{
  if(GLEW_ARB_shader_objects) {
    return true;
  }
  error("ARB shader_objects not supported by this system");
  return false;
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglUniform1f :: render(GemState *state)
{
  glUniform1f (location, val);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglUniform1f :: locMess (t_float arg1)        // FUN
{
  location = static_cast<GLint>(arg1);
  setModified();
}

void GEMglUniform1f :: valMess (t_float arg1)        // FUN
{
  val = static_cast<GLfloat>(arg1);
  setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglUniform1f :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&GEMglUniform1f::locMessCallback),
                  gensym("location"), A_DEFFLOAT, A_NULL);
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&GEMglUniform1f::valMessCallback),
                  gensym("val"), A_DEFFLOAT, A_NULL);
};

void GEMglUniform1f :: locMessCallback (void* data, t_float arg0)
{
  GetMyClass(data)->locMess ( static_cast<t_float>(arg0));
}
void GEMglUniform1f :: valMessCallback (void* data, t_float arg0)
{
  GetMyClass(data)->valMess ( static_cast<t_float>(arg0));
}
