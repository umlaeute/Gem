////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-2000 Mark Danks.
//    Copyright (c) G�nther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f�r::uml�ute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

// 1307:forum::f�r::uml�ute:2000

#include "shininess.h"

CPPEXTERN_NEW_WITH_GIMME(shininess)

/////////////////////////////////////////////////////////
//
// shininess
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
shininess :: shininess(int argc, t_atom *argv)
{
    if (argc == 1) shininessMess(atom_getfloat(&argv[0]));
    else if (argc == 0)  shininessMess(0.f);
	else
    {
        error("GEM: shininess: needs 0 or 1 arguments");
        shininessMess(0.f);
    }

    // create the new inlet
    inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("shininess"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
shininess :: ~shininess()
{ }

/////////////////////////////////////////////////////////
// postrender
//
/////////////////////////////////////////////////////////
void shininess :: postrender(GemState *)
{
	glEnable(GL_COLOR_MATERIAL);
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void shininess :: render(GemState *)
{
	glDisable(GL_COLOR_MATERIAL);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &m_shininess);
}

/////////////////////////////////////////////////////////
// shininessMess
//
/////////////////////////////////////////////////////////
void shininess :: shininessMess(float val)
{
    m_shininess = val;
	if (m_shininess < 0.f)
		m_shininess = 0.f;
	else if (m_shininess > 128.f)
		m_shininess = 128.f;

    setModified();
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void shininess :: obj_setupCallback(t_class *classPtr)
{
    class_addmethod(classPtr, (t_method)&shininess::shininessMessCallback,
    	    gensym("shininess"), A_FLOAT, A_NULL); 
}
void shininess :: shininessMessCallback(void *data, t_floatarg val)
{
    GetMyClass(data)->shininessMess(val);
}

