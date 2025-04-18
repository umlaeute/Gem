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
//  this file has been generated...
////////////////////////////////////////////////////////

#include "GEMglReportError.h"
#include "Utils/GLUtil.h"


CPPEXTERN_NEW( GEMglReportError);
using namespace gem::utils::gl;

/////////////////////////////////////////////////////////
//
// GEMglReportError
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglReportError :: GEMglReportError    (void)
{
  m_outlet = outlet_new(this->x_obj, &s_float);
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglReportError :: ~GEMglReportError ()
{
  outlet_free(m_outlet);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglReportError :: render(GemState *state)
{
  GLenum result=0, err;
  while ((err = glReportError(false))) {
    const char*errStr = glErrorString(err);
    error("GL[0x%X]: %s", err, errStr?errStr:"generic error");
    if (err)
      result = err;
  }
  outlet_float(m_outlet, static_cast<t_float>(result));
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglReportError :: obj_setupCallback(t_class *classPtr)
{
  class_addanything(classPtr, GEMglReportError::bangMessCallback);

};
void GEMglReportError :: bangMessCallback (void* data)
{
  GetMyClass(data)->render(NULL);
}
