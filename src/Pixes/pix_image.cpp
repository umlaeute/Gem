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
#include "Base/GemConfig.h"
#include "pix_image.h"
#include "Base/GemState.h"

#ifdef _WIN32
# include <io.h>
# define close _close
# define snprintf _snprintf
#endif

#if defined(__unix__) || defined(__APPLE__) 
# include <unistd.h>
# include <strings.h>
#endif

#include <stdio.h>
#include "Base/GemCache.h"

CPPEXTERN_NEW_WITH_ONE_ARG(pix_image, t_symbol *, A_DEFSYM);



/////////////////////////////////////////////////////////
//
// pix_image
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_image :: pix_image(t_symbol *filename) :
  m_wantThread(true),
  m_loadedImage(NULL),
  m_id(gem::image::load::INVALID)
{
  m_pixBlock.image = m_imageStruct;

  if(filename!=&s_)openMess(filename);

}

////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_image :: ~pix_image()
{
  cleanImage();
  gem::image::load::cancel(m_id);
  m_id=gem::image::load::INVALID;
}

void pix_image :: threadMess(bool onoff)
{
  m_wantThread=onoff;
}

/////////////////////////////////////////////////////////
// openMess
//
/////////////////////////////////////////////////////////
void pix_image :: openMess(t_symbol *filename)
{
  if(NULL==filename || NULL==filename->s_name || 0==*filename->s_name)return;

  gem::image::load::cancel(m_id);
 
  m_filename = findFile(filename->s_name);

  gem::image::load::callback cb = loadCallback;
  void*userdata=reinterpret_cast<void*>(this);

  m_id = gem::image::load::INVALID;

  bool success=false;
  if(m_wantThread) {
    success=gem::image::load::async(cb, userdata, m_filename, m_id);
  } else {
    success=gem::image::load:: sync(cb, userdata, m_filename, m_id);
  }
  if(gem::image::load::INVALID == m_id)
    success=false;

  if(!success) {
    error("loading of '%s' failed", m_filename.c_str());
  }
}


void    pix_image:: loaded(const gem::image::load::id_t ID, 
			   imageStruct*img,
			   const gem::Properties&props) {

  if(ID!=m_id || ID == gem::image::load::INVALID) {
    verbose(0, "discarding image with ID %d", ID);
    return;
  }

  cleanImage();
  if(img) {
    m_loadedImage=img;
    m_loadedImage->copy2Image(&m_pixBlock.image);
    m_pixBlock.newimage = 1;
    verbose(0, "loaded image '%s'", m_filename.c_str());
  } else {
    error("failed to load image '%s'", m_filename.c_str());
  }
}
void    pix_image:: loadCallback(void*data,
				 gem::image::load::id_t ID, 
				 imageStruct*img,
				 const gem::Properties&props) {
  pix_image*me=reinterpret_cast<pix_image*>(data);
  me->loaded(ID, img, props);
}
    	    	

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void pix_image :: render(GemState *state)
{
  // if we don't have an image, just return
  if (!m_loadedImage){
    return;
  }

  // do we need to reload the image?    
  if (m_cache&&m_cache->resendImage)
    {
      m_loadedImage->refreshImage(&m_pixBlock.image);
      m_pixBlock.newimage = 1;
      m_cache->resendImage = 0;
    }
  state->set(GemState::_PIX, &m_pixBlock);
}

/////////////////////////////////////////////////////////
// postrender
//
/////////////////////////////////////////////////////////
void pix_image :: postrender(GemState *state)
{
  m_pixBlock.newimage = 0;
  state->set(GemState::_PIX, static_cast<pixBlock*>(NULL));
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
  class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_image::openMessCallback),
                  gensym("open"), A_SYMBOL, A_NULL);
  class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_image::threadMessCallback),
                  gensym("thread"), A_FLOAT, A_NULL);
}
void pix_image :: openMessCallback(void *data, t_symbol *filename)
{
  GetMyClass(data)->openMess(filename);
}
void pix_image :: threadMessCallback(void *data, t_floatarg f)
{
  GetMyClass(data)->threadMess(f>0);
}
