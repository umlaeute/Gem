////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
//    Copyright (c) 1997-2000 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "part_targetsize.h"

#include "papi.h"

CPPEXTERN_NEW_WITH_TWO_ARGS(part_targetsize, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// part_targetsize
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
part_targetsize :: part_targetsize(t_floatarg size, t_floatarg scale)
{
 	if (size != 0.f)
		sizeMess(size);
	else
		sizeMess(1.f);

 	if (scale != 0.f)
		scaleMess(scale);
	else
		scaleMess(.05f);

    // create the new inlet
    inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("ft1"));
    inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("ft2"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
part_targetsize :: ~part_targetsize()
{ }

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void part_targetsize :: render(GemState *state)
{
	if (state->stereo == 0 ||
		state->stereo == 1)
	{
		pTargetSize(m_size, m_scale);
	}
}

/////////////////////////////////////////////////////////
// sizeMess
//
/////////////////////////////////////////////////////////
void part_targetsize :: sizeMess(float size)
{
    m_size = size;
    setModified();
}

/////////////////////////////////////////////////////////
// scaleMess
//
/////////////////////////////////////////////////////////
void part_targetsize :: scaleMess(float scale)
{
    m_scale = scale;
    setModified();
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void part_targetsize :: obj_setupCallback(t_class *classPtr)
{
    class_addmethod(classPtr, (t_method)&part_targetsize::sizeMessCallback,
    	    gensym("ft1"), A_FLOAT, A_NULL); 
    class_addmethod(classPtr, (t_method)&part_targetsize::scaleMessCallback,
    	    gensym("ft2"), A_FLOAT, A_NULL); 
}
void part_targetsize :: sizeMessCallback(void *data, t_floatarg size)
{
    GetMyClass(data)->sizeMess((float)size);
}
void part_targetsize :: scaleMessCallback(void *data, t_floatarg scale)
{
    GetMyClass(data)->scaleMess((float)scale);
}
