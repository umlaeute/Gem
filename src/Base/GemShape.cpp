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
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute
//
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "GemShape.h"

/////////////////////////////////////////////////////////
//
// square
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GemShape :: GemShape(t_floatarg size)
    	  : m_linewidth(1.0f), m_size((float)size)
{
	if (m_size == 0.f)
		m_size = 1.f;

	// the size inlet
    m_inlet = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("ft1"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GemShape :: ~GemShape()
{
    inlet_free(m_inlet);
}

/////////////////////////////////////////////////////////
// linewidthMess
//
/////////////////////////////////////////////////////////
void GemShape :: linewidthMess(float linewidth)
{
    m_linewidth = (linewidth < 0.0f) ? 0.0f : linewidth;
    setModified();
}

/////////////////////////////////////////////////////////
// sizeMess
//
/////////////////////////////////////////////////////////
void GemShape :: sizeMess(float size)
{
    m_size = size;
    setModified();
}

/////////////////////////////////////////////////////////
// static member functions
//
/////////////////////////////////////////////////////////
void GemShape :: obj_setupCallback(t_class *classPtr)
{
    class_addmethod(classPtr, (t_method)&GemShape::linewidthMessCallback,
    	    gensym("width"), A_FLOAT, A_NULL);
    class_addmethod(classPtr, (t_method)&GemShape::typeMessCallback,
    	    gensym("draw"), A_SYMBOL, A_NULL);
    class_addmethod(classPtr, (t_method)&GemShape::sizeMessCallback,
    	    gensym("ft1"), A_FLOAT, A_NULL);
}
void GemShape :: linewidthMessCallback(void *data, t_floatarg linewidth)
{
    GetMyClass(data)->linewidthMess((float)linewidth);
}
void GemShape :: typeMessCallback(void *data, t_symbol *type)
{
    GetMyClass(data)->typeMess(type);
}
void GemShape :: sizeMessCallback(void *data, t_floatarg size)
{
    GetMyClass(data)->sizeMess((float)size);
}

