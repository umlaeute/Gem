////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// Implementation file
//
// Copyright (c) 2002 IOhannes m zmoelnig. forum::f�r::uml�ute. IEM
//	zmoelnig@iem.kug.ac.at
//  For information on usage and redistribution, and for a DISCLAIMER
//  *  OF ALL WARRANTIES, see the file, "GEM.LICENSE.TERMS"
//
//  this file has been generated...
////////////////////////////////////////////////////////

#include "GEMglAreTexturesResident.h"

CPPEXTERN_NEW_WITH_GIMME ( GEMglAreTexturesResident )

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglAreTexturesResident :: GEMglAreTexturesResident	(int argc, t_atom*argv) {
#ifndef GL_VERSION_1_1
        error("GEMglAreTexturesResident: GEM was compiled without GL_VERSION_1_1");
        error("GEMglAreTexturesResident: therefore this object will do nothing");
#endif
	len=32;
	textures  =new GLuint   [len];
	residences=new GLboolean[len];
	m_buffer  =new t_atom   [len];
	texturesMess(argc, argv);
	
	m_inlet = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_list, gensym("textures"));
	m_out1 = outlet_new(this->x_obj, 0);
	m_out2 = outlet_new(this->x_obj, 0);
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglAreTexturesResident :: ~GEMglAreTexturesResident () {
  inlet_free(m_inlet);
  outlet_free(m_out1);
  outlet_free(m_out2);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglAreTexturesResident :: render(GemState *state) {
#ifdef GL_VERSION_1_1
  GLboolean ok = glAreTexturesResident (n, textures, residences);
  int i=n;
  while(i--){
    t_float f = residences[i]?1.0:0.0;
    SETFLOAT(m_buffer+i, f);
  }
  outlet_list(m_out2, &s_list, n, m_buffer);
  outlet_float(m_out1, (ok?1.0:0.0));
#endif // GL_VERSION_1_1
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglAreTexturesResident :: texturesMess (int argc, t_atom*argv) {
  if (argc>len){
    len=argc;
    delete[]textures;   textures  =new GLuint   [len];
    delete[]residences; residences=new GLboolean[len];
    delete[]m_buffer;   m_buffer  =new t_atom   [len];
  }
  n=argc;
  while(argc--)textures[argc]=atom_getint(argv+argc);
  setModified();
}

/////////////////////////////////////////////////////////
// static member functions
//

void GEMglAreTexturesResident :: obj_setupCallback(t_class *classPtr) {
	 class_addmethod(classPtr, (t_method)&GEMglAreTexturesResident::texturesMessCallback,  	gensym("textures"), A_GIMME, A_NULL);
}
void GEMglAreTexturesResident :: texturesMessCallback (void* data, t_symbol*,int argc, t_atom*argv){
	GetMyClass(data)->texturesMess (argc,argv);
}
