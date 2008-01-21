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

#include "GEMglPrioritizeTextures.h"

CPPEXTERN_NEW_WITH_ONE_ARG ( GEMglPrioritizeTextures , t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglPrioritizeTextures :: GEMglPrioritizeTextures	(t_floatarg arg0=16) :
		n((GLsizei)arg0) {
#ifndef GL_VERSION_1_1
        error("GEMglPrioritizeTextures: GEM was compiled without GL_VERSION_1_1");
        error("GEMglPrioritizeTextures: therefore this object will do nothing");
#endif
	if (n>0) t_len=p_len=n;
	else t_len=p_len=16;

	textures=new GLuint[t_len];
	priorities=new GLclampf[p_len];

	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("n"));
	m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("textures"));
	m_inlet[2] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("priorities"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglPrioritizeTextures :: ~GEMglPrioritizeTextures () {
inlet_free(m_inlet[0]);
inlet_free(m_inlet[1]);
inlet_free(m_inlet[2]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglPrioritizeTextures :: render(GemState *state) {
#ifdef GL_VERSION_1_1
  int N=n;
  if (t_len<N)N=t_len;
  if (p_len<N)N=p_len;
  glPrioritizeTextures (N, textures, priorities);
#endif // GL_VERSION_1_1
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglPrioritizeTextures :: nMess (t_float arg1) {	// FUN
	n = (GLsizei)arg1;
	setModified();
}

void GEMglPrioritizeTextures :: texturesMess (int argc,t_atom*argv) {	// FUN
  if (argc>t_len){
    t_len=argc;
    delete [] textures;
    textures = new GLuint[t_len];
  }
  while(argc--)textures[argc]=(GLuint)atom_getint(argv+argc);
  setModified();
}

void GEMglPrioritizeTextures :: prioritiesMess (int argc, t_atom*argv) {	// FUN
  if (argc>p_len){
    p_len=argc;
    delete [] priorities;
    priorities = new GLclampf[p_len];
  }
  while(argc--)priorities[argc]=(GLclampf)atom_getfloat(argv+argc);
  setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglPrioritizeTextures :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglPrioritizeTextures::nMessCallback,  	gensym("n"), A_DEFFLOAT, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglPrioritizeTextures::texturesMessCallback,  	gensym("textures"), A_GIMME, A_NULL);
	 class_addmethod(classPtr, (t_method)&GEMglPrioritizeTextures::prioritiesMessCallback,  	gensym("priorities"), A_GIMME, A_NULL);
}

void GEMglPrioritizeTextures :: nMessCallback (void* data, t_floatarg arg0){
	GetMyClass(data)->nMess (arg0);
}
void GEMglPrioritizeTextures :: texturesMessCallback (void* data, t_symbol*, int argc, t_atom*argv){
	GetMyClass(data)->texturesMess (argc,argv);
}
void GEMglPrioritizeTextures :: prioritiesMessCallback (void* data, t_symbol*, int argc, t_atom*argv){
	GetMyClass(data)->prioritiesMess (argc,argv);
}
