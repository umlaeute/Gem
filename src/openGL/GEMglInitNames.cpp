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

#include "GEMglInitNames.h"

CPPEXTERN_NEW( GEMglInitNames)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglInitNames :: GEMglInitNames(){
#ifndef GL_VERSION_1_1
        error("GEMglInitNames: GEM was compiled without GL_VERSION_1_1");
        error("GEMglInitNames: therefore this object will do nothing");
#endif
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglInitNames :: ~GEMglInitNames () {}

/////////////////////////////////////////////////////////
// Render
//
void GEMglInitNames :: render(GemState *state) {
#ifdef GL_VERSION_1_1
	glInitNames ();
#endif // GL_VERSION_1_1
}

/////////////////////////////////////////////////////////
// static member functions
//

void GEMglInitNames :: obj_setupCallback(t_class *classPtr) {}
