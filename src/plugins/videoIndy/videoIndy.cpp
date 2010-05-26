////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-1998 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#ifdef __sgi
/* but i guess, this does not work for windof either */

#include "pix_indycam.h"

#include <unistd.h>
#include <dmedia/vl_vino.h>

CPPEXTERN_NEW(pix_indycam)

/////////////////////////////////////////////////////////
//
// video
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_indycam :: pix_indycam()
{ }

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_indycam :: ~pix_indycam()
{ }

/////////////////////////////////////////////////////////
// zoomMess
//
/////////////////////////////////////////////////////////
void pix_indycam :: zoomMess(int num, int denom)
{
    if (!m_haveVideo)
    {
    	error("Connect to video first");
    	return;
    }
    VLControlValue value;
    value.fractVal.numerator = num;
    value.fractVal.denominator = denom;
    if ( vlSetControl(m_svr, m_path, m_drn, VL_ZOOM, &value) )
    	error("zoom error");
}

/////////////////////////////////////////////////////////
// brightMess
//
/////////////////////////////////////////////////////////
void pix_indycam :: brightMess(int val)
{
    if (!m_haveVideo)
    {
    	error("Connect to video first");
    	return;
    }
    VLControlValue value;
    value.intVal = val;
    if ( vlSetControl(m_svr, m_path, m_drn, VL_BRIGHTNESS, &value) )
    	error("problem setting brightness");
}

/////////////////////////////////////////////////////////
// contrastMess
//
/////////////////////////////////////////////////////////
void pix_indycam :: contrastMess(int val)
{
    if (!m_haveVideo)
    {
    	error("Connect to video first");
    	return;
    }
    VLControlValue value;
    value.intVal = val;
    if ( vlSetControl(m_svr, m_path, m_drn, VL_CONTRAST, &value) )
    	error("problem setting contrast");
}

/////////////////////////////////////////////////////////
// hueMess
//
/////////////////////////////////////////////////////////
void pix_indycam :: hueMess(int val)
{
    if (!m_haveVideo)
    {
    	error("Connect to video first");
    	return;
    }
    VLControlValue value;
    value.intVal = val;
    if ( vlSetControl(m_svr, m_path, m_drn, VL_HUE, &value) )
    	error("problem setting hue");
}

/////////////////////////////////////////////////////////
// satMess
//
/////////////////////////////////////////////////////////
void pix_indycam :: satMess(int val)
{
    if (!m_haveVideo)
    {
    	error("Connect to video first");
    	return;
    }
    VLControlValue value;
    value.intVal = val;
    if ( vlSetControl(m_svr, m_path, m_src, VL_VINO_INDYCAM_SATURATION, &value) )
    	error("problem setting saturation");
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_indycam :: obj_setupCallback(t_class *classPtr)
{
    class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_indycam::zoomMessCallback),
    	    gensym("zoom"), A_FLOAT, A_FLOAT, A_NULL);
    class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_indycam::brightMessCallback),
    	    gensym("bright"), A_FLOAT, A_NULL);
    class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_indycam::contrastMessCallback),
    	    gensym("contrast"), A_FLOAT, A_NULL);
    class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_indycam::hueMessCallback),
    	    gensym("hue"), A_FLOAT, A_NULL);
    class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_indycam::satMessCallback),
    	    gensym("sat"), A_FLOAT, A_NULL);
}
void pix_indycam :: zoomMessCallback(void *data, t_floatarg num, t_floatarg denom)
{
    GetMyClass(data)->zoomMess(num, denom);
}
void pix_indycam :: brightMessCallback(void *data, t_floatarg val)
{
    GetMyClass(data)->brightMess(val);
}
void pix_indycam :: contrastMessCallback(void *data, t_floatarg val)
{
    GetMyClass(data)->contrastMess(val);
}
void pix_indycam :: hueMessCallback(void *data, t_floatarg val)
{
    GetMyClass(data)->hueMess(val);
}
void pix_indycam :: satMessCallback(void *data, t_floatarg val)
{
    GetMyClass(data)->satMess(val);
}
#endif
