////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "shearZY.h"

CPPEXTERN_NEW_WITH_GIMME(shearZY);

/////////////////////////////////////////////////////////
//
// shearZY
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
shearZY :: shearZY(int argc, t_atom *argv)
{
    if (argc == 1)
    {
        shear = atom_getfloat(&argv[0]);
    }
    else if (argc == 0)
    {
        shear = 0.f;
    }

    // create the new inlets
    inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("shearVal"));

}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
shearZY :: ~shearZY()
{ }

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void shearZY :: render(GemState *)
{
    GLfloat matrix[16];

	matrix[0]=matrix[5]=matrix[10]=matrix[15]=1;
	matrix[1]=matrix[2]=matrix[3]=matrix[4]=0.f;
	matrix[6]=matrix[7]=matrix[8]=matrix[9]=0.f;
	matrix[11]=matrix[12]=matrix[13]=matrix[14]=0.f;

	matrix[6]=shear;

	glMultMatrixf(matrix);

}

/////////////////////////////////////////////////////////
// shear
//
/////////////////////////////////////////////////////////
void shearZY :: shearMess(float val)
{
    shear = val;
    setModified();
}


/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void shearZY :: obj_setupCallback(t_class *classPtr)
{
    class_addmethod(classPtr, reinterpret_cast<t_method>(&shearZY::shearMessCallback),
    	    gensym("shearVal"), A_FLOAT, A_NULL);
}
void shearZY :: shearMessCallback(void *data, t_floatarg val)
{
    GetMyClass(data)->shearMess((float)val);
}

