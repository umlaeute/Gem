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

#include "GemPixDualObj.h"

#include "GemCache.h"

#include <string.h>

/////////////////////////////////////////////////////////
//
// GemPixDualObj
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GemPixDualObj :: GemPixDualObj()
   	       : m_cacheRight(NULL), m_pixRightValid(-1) //, m_pixRight(NULL) changed DH 8/5/02
{
    m_inlet = inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("gem_state"), gensym("gem_right"));
    memset(&m_pixRight, 0, sizeof(m_pixRight));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GemPixDualObj :: ~GemPixDualObj()
{
    inlet_free(m_inlet);
}

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void GemPixDualObj :: processImage(imageStruct &image)
{
  if (!m_pixRightValid || !m_cacheRight || !&image || !&m_pixRight || !&m_pixRight->image) return;
  //if (!m_cacheRight || !&image || !&m_pixRight || !&m_pixRight->image) return;

    if (image.xsize != m_pixRight->image.xsize ||
    	image.ysize != m_pixRight->image.ysize)
    {
    	error("GEM: GemPixDualObj: two images do not have equal dimensions");
     	m_pixRightValid = 0;

    	return;
    }
	if (image.csize == 1)
	{
		if (m_pixRight->image.csize == 1)
			processBothGray(image, m_pixRight->image);
		else
			processLeftGray(image, m_pixRight->image);
	}
	else
	{
		if (m_pixRight->image.csize == 1)
			processRightGray(image, m_pixRight->image);
		else
			processDualImage(image, m_pixRight->image);
	}
}

/////////////////////////////////////////////////////////
// processBothGray
//
/////////////////////////////////////////////////////////
void GemPixDualObj :: processBothGray(imageStruct &, imageStruct &)
{
	error("GEM: GemPixDualObj: cannot handle gray image");
}

/////////////////////////////////////////////////////////
// processLeftGray
//
/////////////////////////////////////////////////////////
void GemPixDualObj :: processLeftGray(imageStruct &, imageStruct &)
{
	error("GEM: GemPixDualObj: cannot handle gray image");
}

/////////////////////////////////////////////////////////
// processRightGray
//
/////////////////////////////////////////////////////////
void GemPixDualObj :: processRightGray(imageStruct &, imageStruct &)
{
	error("GEM: GemPixDualObj: cannot handle gray image");
}


/////////////////////////////////////////////////////////
// postrender
//
/////////////////////////////////////////////////////////
void GemPixDualObj :: postrender(GemState *state)
{
  if (org_pixRightValid != m_pixRightValid)setPixModified();

  org_pixRightValid = m_pixRightValid;

  m_pixRightValid = 0;
}

/////////////////////////////////////////////////////////
// stopRendering
//
/////////////////////////////////////////////////////////
void GemPixDualObj :: stopRendering()
{
  m_pixRightValid = 0;
}

/////////////////////////////////////////////////////////
// rightRender
//
/////////////////////////////////////////////////////////
void GemPixDualObj :: rightRender(GemState *statePtr)
{
  if (!statePtr || !statePtr->image) {
    m_pixRightValid = 0;
    m_pixRight = 0;
    return;
  }
  
  m_pixRightValid = 1;
  m_pixRight = statePtr->image;
 
  if (statePtr->image->newimage)setPixModified(); // force the left arm to create a new image
}

/////////////////////////^////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void GemPixDualObj :: obj_setupCallback(t_class *classPtr)
{
    class_addmethod(classPtr, (t_method)&GemPixDualObj::gem_rightMessCallback,
    	    gensym("gem_right"), A_GIMME, A_NULL);
}
void GemPixDualObj :: gem_rightMessCallback(void *data, t_symbol *s, int argc, t_atom *argv)
{
  if (argc==1 && argv->a_type==A_FLOAT){
  } else if (argc==2 && argv->a_type==A_POINTER && (argv+1)->a_type==A_POINTER){
    GetMyClass(data)->m_cacheRight = (GemCache*)argv->a_w.w_gpointer;
    GetMyClass(data)->rightRender((GemState *)(argv+1)->a_w.w_gpointer);
  } else error("GEM: wrong righthand arguments....");
}
