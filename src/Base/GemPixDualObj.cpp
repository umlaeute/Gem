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

/////////////////////////////////////////////////////////
//
// GemPixDualObj
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GemPixDualObj :: GemPixDualObj()
    	       : m_cacheRight(NULL), m_pixRight(NULL)
{
    m_inlet = inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("gem_state"), gensym("gem_right"));
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
    if (!m_pixRight) return;
    
    if (image.xsize != m_pixRight->image.xsize ||
    	image.ysize != m_pixRight->image.ysize)
    {
    	error("GEM: GemPixDualObj: two images do not have equal dimensions");
    	m_pixRight = NULL;
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
    m_cacheRight = NULL;
    m_pixRight = NULL;
}

/////////////////////////////////////////////////////////
// rightRender
//
/////////////////////////////////////////////////////////
void GemPixDualObj :: rightRender(GemState *statePtr)
{
    m_pixRight = statePtr->image;
    if (!statePtr->image) return;
    if (statePtr->image->newimage) setPixModified();
}

/////////////////////////////////////////////////////////
// rightDagCacheMess
//
/////////////////////////////////////////////////////////
void GemPixDualObj :: rightDagCacheMess(GemDag *dagPtr, GemCache *cachePtr)
{
    dagPtr->addChild(this, &GemPixDualObj::rightRenderCallback, &GemPixDualObj::rightPostrenderCallback);
    
    m_cacheRight = cachePtr;

    // do not send the message any farther
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
