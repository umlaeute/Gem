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
//  this file has been generated...
////////////////////////////////////////////////////////

#include "GEMglPopClientAttrib.h"

CPPEXTERN_NEW ( GEMglPopClientAttrib)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglPopClientAttrib :: GEMglPopClientAttrib	(){
#ifndef GL_VERSION_1_1
        error("GEMglPopClientAttrib: GEM was compiled without GL_VERSION_1_1");
        error("GEMglPopClientAttrib: therefore this object will do nothing");
#endif
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglPopClientAttrib :: ~GEMglPopClientAttrib () {}

/////////////////////////////////////////////////////////
// Render
//
void GEMglPopClientAttrib :: render(GemState *state) {
#ifdef GL_VERSION_1_1
	glPopClientAttrib ();
#endif // GL_VERSION_1_1
}


void GEMglPopClientAttrib :: obj_setupCallback(t_class *classPtr) {}
