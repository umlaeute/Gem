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
#include "GemDag.h"

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
   	       : m_cacheRight(NULL), m_pixRightValid(0) //, m_pixRight(NULL) changed DH 8/5/02
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
    if (m_cacheRight)
        m_cacheRight->breakDAG();
    inlet_free(m_inlet);
}

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void GemPixDualObj :: processImage(imageStruct &image)
{
    if (!m_pixRightValid) return;
    
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
// stopRendering
//
/////////////////////////////////////////////////////////
void GemPixDualObj :: stopRendering()
{
  //    m_pixRightValid = 0;
}

/////////////////////////////////////////////////////////
// rightRender
//
/////////////////////////////////////////////////////////
void GemPixDualObj :: rightRender(GemState *statePtr)
{
     if (!statePtr->image) 
 	{
 		m_pixRightValid = 0;
 		return;
 	}
     if (statePtr->image->newimage)
 	{
	  // 	    m_pixRight = *statePtr->image;
	  m_pixRight = statePtr->image;

	    m_pixRightValid = 1;
	    setPixModified(); // force the left arm to create a new image
 	}
}

/////////////////////////////////////////////////////////
// rightDagCacheMess
//
/////////////////////////////////////////////////////////
void GemPixDualObj :: rightDagCacheMess(GemDag *dagPtr, GemCache *cachePtr)
{
  dagPtr->addChild(this, &GemPixDualObj::rightRenderCallback, &GemPixDualObj::rightPostrenderCallback, &GemPixDualObj::rightStoprenderCallback);
    
    m_cacheRight = cachePtr;

    // do not send the message any farther
}

/////////////////////////////////////////////////////////
// rightrealStopRendering
//
/////////////////////////////////////////////////////////
void GemPixDualObj :: rightrealStopRendering()
{
  rightstopRendering();
  m_pixRightValid = 0;
  m_cacheRight = NULL;
}
/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void GemPixDualObj :: obj_setupCallback(t_class *classPtr)
{
    class_addmethod(classPtr, (t_method)&GemPixDualObj::rightDagCacheMessCallback,
    	    gensym("gem_right"), A_POINTER, A_POINTER, A_NULL);
}
void GemPixDualObj :: rightDagCacheMessCallback(void *data, void *gem_dag, void *gem_cache)
{
    GetMyClass(data)->rightDagCacheMess((GemDag *)gem_dag, (GemCache *)gem_cache);
}
void GemPixDualObj :: rightRenderCallback(GemBase *data, GemState *state)
{
    ((GemPixDualObj *)data)->rightRender(state);
}
void GemPixDualObj :: rightPostrenderCallback(GemBase *data, GemState *state)
{
    ((GemPixDualObj *)data)->rightPostrender(state);
}
void GemPixDualObj :: rightStoprenderCallback(GemBase *data)
{
    ((GemPixDualObj *)data)->rightStoprender();
}
