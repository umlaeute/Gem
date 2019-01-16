////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// Implementation file
//
// Copyright (c) 2004-2005 tigital@mac.com
//  For information on usage and redistribution, and for a DISCLAIMER
//  *  OF ALL WARRANTIES, see the file, "GEM.LICENSE.TERMS"
//
////////////////////////////////////////////////////////

#include "GEMglActiveTexture.h"

CPPEXTERN_NEW_WITH_ONE_ARG ( GEMglActiveTexture, t_floatarg,
                             A_DEFFLOAT);

/////////////////////////////////////////////////////////
//
// GEMglActiveTexture
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglActiveTexture :: GEMglActiveTexture  (t_floatarg arg0) :
  texUnit(static_cast<GLenum>(arg0))
{
  m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float,
                         gensym("texUnit"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglActiveTexture :: ~GEMglActiveTexture ()
{
  inlet_free(m_inlet[0]);
}
//////////////////
// extension check
bool GEMglActiveTexture :: isRunnable(void)
{
  if(GLEW_ARB_multitexture) {
    return true;
  }
  error("your system does not support the ARB multitexture extension");
  return false;
}
/////////////////////////////////////////////////////////
// Render
//
void GEMglActiveTexture :: render(GemState *state)
{
  glActiveTexture (texUnit);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglActiveTexture :: texUnitMess (t_float arg1)        // FUN
{
  texUnit = static_cast<GLenum>(arg1);
  setModified();
}

/////////////////////////////////////////////////////////
// static member functions
//

void GEMglActiveTexture :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&GEMglActiveTexture::texUnitMessCallback),
                  gensym("texUnit"), A_DEFFLOAT, A_NULL);
}

void GEMglActiveTexture :: texUnitMessCallback (void* data,
    t_float arg0)
{
  GetMyClass(data)->texUnitMess ( static_cast<t_float>(arg0));
}
