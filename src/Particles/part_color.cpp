////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
//    Copyright (c) 1997-2000 Mark Danks.
//    Copyright (c) G�nther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f�r::uml�ute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "part_color.h"

#include "papi.h"

CPPEXTERN_NEW(part_color)

/////////////////////////////////////////////////////////
//
// part_color
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
part_color :: part_color()
{
	color1Mess(1.f, 1.f, 1.f, 1.f);
	color2Mess(1.f, 1.f, 1.f, 1.f);
	
    // create the new inlet
    inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("list"), gensym("color1"));

    // create the new inlet
    inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("list"), gensym("color2"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
part_color :: ~part_color()
{ }

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void part_color :: render(GemState *state)
{
	if (state->stereo == 0 ||
		state->stereo == 1)
	{
		pColorD(1.0f, PDLine, m_color1[0], m_color1[1], m_color1[2],
							  m_color2[0], m_color2[1], m_color2[2]);
	}
}

/////////////////////////////////////////////////////////
// color1Mess
//
/////////////////////////////////////////////////////////
void part_color :: color1Mess(float red, float green, float blue, float alpha)
{
    m_color1[0] = red;
    m_color1[1] = green;
    m_color1[2] = blue;
    m_color1[3] = alpha;
    setModified();
}

/////////////////////////////////////////////////////////
// color2Mess
//
/////////////////////////////////////////////////////////
void part_color :: color2Mess(float red, float green, float blue, float alpha)
{
    m_color2[0] = red;
    m_color2[1] = green;
    m_color2[2] = blue;
    m_color2[3] = alpha;
    setModified();
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void part_color :: obj_setupCallback(t_class *classPtr)
{
    class_addmethod(classPtr, (t_method)&part_color::color1MessCallback,
    	    gensym("color1"), A_GIMME, A_NULL); 
    class_addmethod(classPtr, (t_method)&part_color::color2MessCallback,
    	    gensym("color2"), A_GIMME, A_NULL); 
}
void part_color :: color1MessCallback(void *data, t_symbol*s, int argc, t_atom*argv)
{
  if (argc==3 || argc==4)
    GetMyClass(data)->color1Mess(atom_getfloat(argv+0),
				 atom_getfloat(argv+1),
				 atom_getfloat(argv+2),
				 (argc==4)?atom_getfloat(argv+3):1.0f);
  else {
    error("GEM: part_color:: only 3 or 4 arguments are accepted as colours");
  }
}
void part_color :: color2MessCallback(void *data, t_symbol*s, int argc, t_atom*argv)
{
  if (argc==3 || argc==4)
    GetMyClass(data)->color2Mess(atom_getfloat(argv+0),
				 atom_getfloat(argv+1),
				 atom_getfloat(argv+2),
				 (argc==4)?atom_getfloat(argv+3):1.0f);
  else {
    error("GEM: part_color:: only 3 or 4 arguments are accepted as colours");
  }
}
