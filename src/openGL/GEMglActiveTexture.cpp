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

CPPEXTERN_NEW_WITH_GIMME ( GEMglActiveTexture );

/////////////////////////////////////////////////////////
//
// GEMglActiveTexture
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglActiveTexture :: GEMglActiveTexture  (int argc, t_atom*argv)
  : texUnit(0)
  , maxTexUnits(0)
{
  switch(argc) {
  case 0: break;
  case 1:
    texUnitMess(argv[0]);
    break;
  default:
    throw(GemException("invalid number of arguments"));
  }
  m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd,
                         gensym("list"), gensym("texUnit"));
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
  if (texUnit < GL_TEXTURE0) {
    /* the normal usage is 'glActiveTexture (GL_TEXTURE0 + n)'
     * but the user might be inclined to just use 'glActiveTexture(n)'
     */
    texUnit += GL_TEXTURE0;
  }
  glActiveTexture (texUnit);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglActiveTexture :: texUnitMess (t_atom&arg)        // FUN
{
  texUnit = static_cast<GLenum>(gem::utils::gl::getGLdefine(&arg));
  setModified();
}

/////////////////////////////////////////////////////////
// static member functions
//

void GEMglActiveTexture :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&GEMglActiveTexture::texUnitMessCallback),
                  gensym("texUnit"), A_GIMME, A_NULL);
}

void GEMglActiveTexture :: texUnitMessCallback (void* data,
                                                t_symbol*, int argc, t_atom*argv)
{
  if(argc==1) {
    GetMyClass(data)->texUnitMess(argv[0]);
  }
}
