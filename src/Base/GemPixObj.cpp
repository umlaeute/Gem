////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) G�nther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f�r::uml�ute
//
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "GemPixObj.h"

#include "GemCache.h"

/////////////////////////////////////////////////////////
//
// GemPixObj
//
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
// setPixModified
//
/////////////////////////////////////////////////////////
void GemPixObj :: setPixModified()
{
    if (m_cache) m_cache->resendImage = 1;
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void GemPixObj :: render(GemState *state)
{
    if (!state->image || !state->image->newimage || !m_processOnOff) return;
	if (state->image->image.format == GL_RGBA)
		processImage(state->image->image);
	else
		processGrayImage(state->image->image);
}

/////////////////////////////////////////////////////////
// processGrayImage
//
/////////////////////////////////////////////////////////
void GemPixObj :: processGrayImage(imageStruct &)
{
	error("Gem: pix object cannot handle gray image");
}

/////////////////////////////////////////////////////////
// processOnOff
//
/////////////////////////////////////////////////////////
void GemPixObj :: processOnOff(int on)
{
    m_processOnOff = on;
    setPixModified();
}

/////////////////////////////////////////////////////////
// static member functions
//
/////////////////////////////////////////////////////////
void GemPixObj :: obj_setupCallback(t_class *classPtr)
{
    class_addfloat(classPtr, (t_method)&GemPixObj::floatMessCallback);    
}
void GemPixObj :: floatMessCallback(void *data, float n)
{
    GetMyClass(data)->processOnOff((int)n);
}
