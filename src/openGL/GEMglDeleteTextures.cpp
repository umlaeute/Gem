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

#include "GEMglDeleteTextures.h"

CPPEXTERN_NEW_WITH_GIMME ( GEMglDeleteTextures )

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglDeleteTextures :: GEMglDeleteTextures	(int argc, t_atom* argv) :
  n(0), textures(NULL), m_inlet(NULL)
{
#ifndef GL_VERSION_1_1
        error("GEMglDeleteTextures: GEM was compiled without GL_VERSION_1_1");
        error("GEMglDeleteTextures: therefore this object will do nothing");
#endif
	texturesMess(argc, argv);
	m_inlet = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("textures"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglDeleteTextures :: ~GEMglDeleteTextures () {
inlet_free(m_inlet);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglDeleteTextures :: render(GemState *state) {
#ifdef GL_VERSION_1_1
	glDeleteTextures (n, textures);
#endif // GL_VERSION_1_1
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglDeleteTextures :: texturesMess (int argc, t_atom*argv) {	// FUN
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

void GEMglDeleteTextures :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglDeleteTextures::texturesMessCallback,  	gensym("textures"), A_GIMME, A_NULL);
}
void GEMglDeleteTextures :: texturesMessCallback (void* data, int argc, t_atom*argv){
	GetMyClass(data)->texturesMess (argc, argv);
}
