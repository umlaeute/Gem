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

#include "GEMglPopName.h"

CPPEXTERN_NEW ( GEMglPopName)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglPopName :: GEMglPopName	(){
#ifndef GL_VERSION_1_1
        error("GEMglPopName: GEM was compiled without GL_VERSION_1_1");
        error("GEMglPopName: therefore this object will do nothing");
#endif
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglPopName :: ~GEMglPopName () {}

/////////////////////////////////////////////////////////
// Render
//
void GEMglPopName :: render(GemState *state) {
#ifdef GL_VERSION_1_1
	glPopName ();
#endif // GL_VERSION_1_1
}

/////////////////////////////////////////////////////////
// static member functions
//
void GEMglPopName :: obj_setupCallback(t_class *classPtr) {}
