////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "part_size.h"

#include "papi.h"

CPPEXTERN_NEW_WITH_ONE_ARG(part_size, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// part_size
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
part_size :: part_size(t_floatarg num)
		   : m_size(1.f)
{
	if (num > 0)
		m_size = num;
    inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"), gensym("size"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
part_size :: ~part_size()
{ }

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void part_size :: render(GemState *state)
{
	if (state->stereo == 0 ||
		state->stereo == 1)
	{
		pSize(m_size);
	}
}

/////////////////////////////////////////////////////////
// static member functions
//
/////////////////////////////////////////////////////////
void part_size :: obj_setupCallback(t_class *classPtr)
{
    class_addmethod(classPtr, (t_method)&part_size::numberMessCallback,
    	    gensym("size"), A_FLOAT, A_NULL);
}
void part_size :: numberMessCallback(void *data, t_floatarg num)
{
    GetMyClass(data)->numberMess(num);
}

