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

#include "color.h"

CPPEXTERN_NEW_WITH_GIMME(color)

/////////////////////////////////////////////////////////
//
// color
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
color :: color(int argc, t_atom *argv)
{
    if (argc == 4) colorMess(atom_getfloat(&argv[0]), atom_getfloat(&argv[1]),
    	    	    	     atom_getfloat(&argv[2]), atom_getfloat(&argv[3]));
    else if (argc == 3) colorMess(atom_getfloat(&argv[0]), atom_getfloat(&argv[1]),
    	    	    	          atom_getfloat(&argv[2]), 1.f);
    else if (argc == 0) colorMess(1.f, 1.f, 1.f, 1.f);
    else
    {
        error("GEM: color: needs 0, 3, or 4 arguments");
        colorMess(1.f, 1.f, 1.f, 1.f);
    }

    // create the new inlet
    inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_list, gensym("color"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
color :: ~color()
{ }

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void color :: render(GemState *)
{
    glColor4fv(m_color);
}

/////////////////////////////////////////////////////////
// colorMess
//
/////////////////////////////////////////////////////////
void color :: colorMess(float red, float green, float blue, float alpha)
{
    m_color[0] = red;
    m_color[1] = green;
    m_color[2] = blue;
    m_color[3] = alpha;
    setModified();
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void color :: obj_setupCallback(t_class *classPtr)
{
  class_addcreator((t_newmethod)_classcolor, 
		   gensym("colour"), A_GIMME, A_NULL);
    class_addmethod(classPtr, (t_method)&color::colorMessCallback,
    	    gensym("color"), A_GIMME, A_NULL); 
}
void color :: colorMessCallback(void *data, t_symbol *, int argc, t_atom *argv)
{
    float alpha = 1;
    if (argc == 4) alpha = atom_getfloat(&argv[3]);
    GetMyClass(data)->colorMess(atom_getfloat(&argv[0]), atom_getfloat(&argv[1]),
    	    	    	       atom_getfloat(&argv[2]), alpha);
}

