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
//   
////////////////////////////////////////////////////////

#include "gemlist_info.h"

#define rad2deg -57.2957795132

CPPEXTERN_NEW_WITH_ONE_ARG ( gemlist_info , t_floatarg, A_DEFFLOAT )

/////////////////////////////////////////////////////////
//
// gemlist_info
//
/////////////////////////////////////////////////////////
// Constructor
//
gemlist_info :: gemlist_info	(t_floatarg arg0=0) {
#ifndef GL_VERSION_1_1
        error("gemlist_info: GEM was compiled without GL_VERSION_1_1");
        error("gemlist_info: therefore this object will do nothing");
#endif
	pnameMess(arg0);
    m_outletRotation = outlet_new(this->x_obj, 0);
	m_outletShear = outlet_new(this->x_obj, 0);
    m_outletScale = outlet_new(this->x_obj, 0);
    m_outletPosition = outlet_new(this->x_obj, 0);
}
/////////////////////////////////////////////////////////
// Destructor
//
gemlist_info :: ~gemlist_info () {
	outlet_free(m_outletScale);
	outlet_free(m_outletPosition);
	outlet_free(m_outletRotation);
	outlet_free(m_outletShear);

}

/////////////////////////////////////////////////////////
// Render
//
void gemlist_info :: render(GemState *state) {
#ifdef GL_VERSION_1_1
	float mi[16]={0};

	float X, Y, Z, ScaleX, ScaleY, ScaleZ, shearYX, shearZX, shearZY;

	glGetFloatv(GL_MODELVIEW_MATRIX,mi);

	// test de syngularité a effectuer

	// normalisation
//	for (i=0; i<16; i++) mi[i] /= mi[15];
	// not usefull because I never saw mi[15]!=1; if this change, un-comment this normalisation procedure
	ScaleX = sqrt (mi[0] * mi[0] + mi[4] * mi[4] + mi[8] * mi[8]);
	mi[0] /= ScaleX; // Normalise X
	mi[4] /= ScaleX;
	mi[8] /= ScaleX;

	shearYX = mi[0]*mi[1] + mi[4]*mi[5] + mi[8]*mi[9];

	mi[1] -= shearYX * mi[0]; //make X and Y orthogonal
	mi[5] -= shearYX * mi[4]; 
	mi[9] -= shearYX * mi[8]; 

	ScaleY = sqrt (mi[1] * mi[1] + mi[5] * mi[5] + mi[9] * mi[9]);

	mi[1] /= ScaleY; // Normalise Y
	mi[5] /= ScaleY;
	mi[9] /= ScaleY;
	shearYX /= ScaleY;

	shearZX = mi[0]*mi[2] + mi[4]*mi[6] + mi[8]*mi[10];

	mi[2] -= shearZX * mi[0]; //make X and Z orthogonal
	mi[6] -= shearZX * mi[4]; 
	mi[10] -= shearZX * mi[8]; 

	shearZY = mi[1]*mi[2] + mi[5]*mi[6] + mi[9]*mi[10];

	mi[2] -= shearZY * mi[1]; //make X and Z orthogonal
	mi[6] -= shearZY * mi[5]; 
	mi[10] -= shearZY * mi[9]; 

	ScaleZ = sqrt (mi[2] * mi[2] + mi[6] * mi[6] + mi[10] * mi[10]);

	mi[2] /= ScaleZ; // Normalise Y
	mi[6] /= ScaleZ;
	mi[10] /= ScaleZ;
	shearZX /= ScaleZ;
	shearZY /= ScaleZ;

// maybee some test could be inserted here.
// The matrix can only be decomposed if it's determinent is not 0.

	Y = asin(-mi[8]);
	if ( cos(Y) != 0 ) {
		X = atan2(mi[9], mi[10]);
		Z = atan2(mi[4], mi[0]);
	} else {
		X = atan2(mi[1], mi[5]);
		Z = 0;
	}

	X *= rad2deg;
	Y *= rad2deg;
	Z *= rad2deg;

	SETFLOAT(m_alist+0, ScaleX);
	SETFLOAT(m_alist+1, ScaleY);
	SETFLOAT(m_alist+2, ScaleZ);

	SETFLOAT(m_alist+3, X);
	SETFLOAT(m_alist+4, Y);
	SETFLOAT(m_alist+5, Z);

	SETFLOAT(m_alist+6, mi[12]); 
	SETFLOAT(m_alist+7, mi[13]);
	SETFLOAT(m_alist+8, mi[14]);

	SETFLOAT(m_alist+9, shearYX); 
	SETFLOAT(m_alist+10, shearZX);
	SETFLOAT(m_alist+11, shearZY);

    outlet_list (m_outletPosition, &s_list, 3, m_alist+6);
	outlet_list (m_outletScale, &s_list, 3, m_alist+0);
    outlet_list (m_outletShear, &s_list, 3, m_alist+9);
    outlet_list (m_outletRotation, &s_list, 3, m_alist+3);

#endif // GL_VERSION_1_1
}

/////////////////////////////////////////////////////////
// variable
//
void gemlist_info :: pnameMess (t_float arg0) {	// FUN
  pname=(GLenum)arg0;
  setModified();
}

/////////////////////////////////////////////////////////
// static member functions
//

void gemlist_info :: obj_setupCallback(t_class *classPtr) {
  class_addmethod(classPtr, (t_method)&gemlist_info::pnameMessCallback,  	gensym("pname"), A_DEFFLOAT, A_NULL);
}

void gemlist_info :: pnameMessCallback (void* data, t_floatarg arg0) {
	GetMyClass(data)->pnameMess ( arg0 );
}
