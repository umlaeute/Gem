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

#include "pix_buf.h"

#include "Base/GemCache.h"

CPPEXTERN_NEW(pix_buf)

/////////////////////////////////////////////////////////
//
// pix_buf
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_buf :: pix_buf()
    	 : m_oldcache(NULL)
{
    m_pixBlock.image.data = NULL;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_buf :: ~pix_buf()
{
    if (m_oldcache) stopRendering();
    cleanImage();
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void pix_buf :: render(GemState *state)
{
    // the cache and image should have been set
    if (!m_oldcache || !state->image ) return;
    
    // if it is a new image or a dirty image, reprocess
    if ( state->image->newimage || m_cache->resendImage )
    {
    	if (!m_pixBlock.image.data)
    	    state->image->image.copy2Image(&(m_pixBlock.image));
    	else
    	    state->image->image.refreshImage(&(m_pixBlock.image));
    	m_pixBlock.newimage = 1;
    	m_cache->resendImage = 0;
    }

    state->image = &m_pixBlock;
}

/////////////////////////////////////////////////////////
// postrender
//
/////////////////////////////////////////////////////////
void pix_buf :: postrender(GemState *state)
{
    m_pixBlock.newimage = 0;
    state->image = NULL;
}

/////////////////////////////////////////////////////////
// startRendering
//
/////////////////////////////////////////////////////////
void pix_buf :: startRendering()
{
    m_oldcache = m_cache;
    m_cache = new GemCache(m_oldcache->m_parent);
    cleanImage();
}

/////////////////////////////////////////////////////////
// stopRendering
//
/////////////////////////////////////////////////////////
void pix_buf :: stopRendering()
{
    if (m_cache) delete m_cache;
    cleanImage();
}

/////////////////////////////////////////////////////////
// cleanImage
//
/////////////////////////////////////////////////////////
void pix_buf :: cleanImage()
{
    if (m_pixBlock.image.data)
    {
     	m_pixBlock.image.clear();
    }
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_buf :: obj_setupCallback(t_class *)
{ }
