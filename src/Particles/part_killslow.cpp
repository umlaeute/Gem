////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) G�nther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f�r::uml�ute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "part_killslow.h"

#include "papi.h"

CPPEXTERN_NEW_WITH_ONE_ARG(part_killslow, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// part_killslow
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
part_killslow :: part_killslow(t_floatarg num)
			   : m_killSpeed(.01f)
{
	if (num > 0)
		m_killSpeed = num;
    inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"), gensym("speed"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
part_killslow :: ~part_killslow()
{ }

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void part_killslow :: render(GemState *state)
{
	if (state->stereo == 0 ||
		state->stereo == 1)
	{
		pKillSlow(m_killSpeed);
	}
}

/////////////////////////////////////////////////////////
// static member functions
//
/////////////////////////////////////////////////////////
void part_killslow :: obj_setupCallback(t_class *classPtr)
{
    class_addmethod(classPtr, (t_method)&part_killslow::numberMessCallback,
    	    gensym("speed"), A_FLOAT, A_NULL);
}
void part_killslow :: numberMessCallback(void *data, t_floatarg num)
{
    GetMyClass(data)->numberMess(num);
}

