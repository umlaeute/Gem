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
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "pix_image.h"

#ifdef unix
#include <unistd.h>
#include <strings.h>
#endif

#include "Base/GemCache.h"

CPPEXTERN_NEW_WITH_ONE_ARG(pix_image, t_symbol *, A_DEFSYM)

/////////////////////////////////////////////////////////
//
// pix_image
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_image :: pix_image(t_symbol *filename) :
  m_loadedImage(NULL)
{
  m_pixBlock.image = m_imageStruct;
  openMess(filename);
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_image :: ~pix_image()
{
    cleanImage();
}

/////////////////////////////////////////////////////////
// openMess
//
/////////////////////////////////////////////////////////
void pix_image :: openMess(t_symbol *filename)
{
  if(NULL==filename || NULL==filename->s_name || 0==*filename->s_name)return;

  cleanImage();

  char buf[MAXPDSTRING];
  canvas_makefilename(getCanvas(), filename->s_name, buf, MAXPDSTRING);

  if ( !(m_loadedImage = image2mem(buf)) )
    {
      return;
    }

  m_loadedImage->copy2Image(&m_pixBlock.image);
  m_pixBlock.newimage = 1;
  post("GEM: loaded image: %s", buf);
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void pix_image :: render(GemState *state)
{
    // if we don't have an image, just return
    if (!m_loadedImage) return;
    // do we need to reload the image?    
    if (m_cache&&m_cache->resendImage)
    {
      m_loadedImage->refreshImage(&m_pixBlock.image);
      m_pixBlock.newimage = 1;
      m_cache->resendImage = 0;
    }
    state->image = &m_pixBlock;
}

/////////////////////////////////////////////////////////
// postrender
//
/////////////////////////////////////////////////////////
void pix_image :: postrender(GemState *state)
{
  m_pixBlock.newimage = 0;
  state->image = NULL;
}

/////////////////////////////////////////////////////////
// startRendering
//
/////////////////////////////////////////////////////////
void pix_image :: startRendering()
{
    if (!m_loadedImage) return;
    m_loadedImage->refreshImage(&m_pixBlock.image);
    m_pixBlock.newimage = 1;
}

/////////////////////////////////////////////////////////
// cleanImage
//
/////////////////////////////////////////////////////////
void pix_image :: cleanImage()
{
    // release previous data
    if (m_loadedImage)
    {
      delete m_loadedImage;
      m_loadedImage = NULL;
      m_pixBlock.image.clear();
      m_pixBlock.image.data = NULL;
    }
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void pix_image :: obj_setupCallback(t_class *classPtr)
{
    class_addmethod(classPtr, (t_method)&pix_image::openMessCallback,
    	    gensym("open"), A_SYMBOL, A_NULL);
}
void pix_image :: openMessCallback(void *data, t_symbol *filename)
{
    GetMyClass(data)->openMess(filename);
}
