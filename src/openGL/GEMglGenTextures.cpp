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

#include "GEMglGenTextures.h"

CPPEXTERN_NEW_WITH_GIMME ( GEMglGenTextures)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglGenTextures :: GEMglGenTextures	(int argc, t_atom*argv) {
#ifndef GL_VERSION_1_1
        error("GEMglGenTextures: GEM was compiled without GL_VERSION_1_1");
        error("GEMglGenTextures: therefore this object will do nothing");
#endif
	texturesMess(argc, argv);
	
	m_inlet = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("textures"));

}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglGenTextures :: ~GEMglGenTextures () {
inlet_free(m_inlet);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglGenTextures :: render(GemState *state) {
#ifdef GL_VERSION_1_1
	glGenTextures (n, textures);
#endif // GL_VERSION_1_1
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglGenTextures :: texturesMess (int argc, t_atom*argv) {	// FUN
  n=0;
  delete [] textures;
  textures = new GLuint[argc];
  while(argc--){
    if(argv->a_type == A_FLOAT)textures[n++] = (GLuint)atom_getint(argv);
    argv++;
  }
  setModified();
}



/////////////////////////////////////////////////////////
// static member functions
//

void GEMglGenTextures :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglGenTextures::texturesMessCallback,  	gensym("textures"), A_GIMME, A_NULL);
}
void GEMglGenTextures :: texturesMessCallback (void* data, int argc, t_atom*argv){
	GetMyClass(data)->texturesMess (argc,argv);
}
