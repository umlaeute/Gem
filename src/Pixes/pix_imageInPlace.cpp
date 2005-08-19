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
//    Copyright (c) 2002 James Tittle & Chris Clepper
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "pix_imageInPlace.h"

#include "Base/GemCache.h"

CPPEXTERN_NEW_WITH_FOUR_ARGS(pix_imageInPlace, t_symbol *, A_DEFSYM, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// pix_imageInPlace
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_imageInPlace :: pix_imageInPlace(t_symbol *filename, t_floatarg baseImage, t_floatarg topImage, t_floatarg skipRate)
  : pix_multiimage(filename, baseImage, topImage, skipRate),
    mInPreload(0)
{ }

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_imageInPlace :: ~pix_imageInPlace()
{ }

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void pix_imageInPlace :: render(GemState *state)
{
  // if we don't have an image, just return
  if (!m_numImages)
    return;

  // if nothing has been bound yet
  if (!m_loadedCache->textBind[m_curImage])
    return;

  state->texture = 1;
    
  glEnable(GL_TEXTURE_2D);

#ifdef GL_VERSION_1_1
  glBindTexture   (GL_TEXTURE_2D, m_loadedCache->textBind[m_curImage]);
#elif GL_EXT_texture_object
  glBindTextureEXT(GL_TEXTURE_2D, m_loadedCache->textBind[m_curImage]);
#else
#error No OpenGL bind mechanism
#endif
}

/////////////////////////////////////////////////////////
// postrender
//
/////////////////////////////////////////////////////////
void pix_imageInPlace :: postrender(GemState *state)
{
  state->texture = 0;

  glDisable(GL_TEXTURE_2D);
}

/////////////////////////////////////////////////////////
// startRendering
//
/////////////////////////////////////////////////////////
void pix_imageInPlace :: startRendering()
{
  // if we don't have any images or if we are going to delay loading
  if (!m_numImages || mInPreload)
    return;

  downloadMess();
}

/////////////////////////////////////////////////////////
// stopRendering
//
/////////////////////////////////////////////////////////
void pix_imageInPlace :: stopRendering()
{
  purgeMess();
}

/////////////////////////////////////////////////////////
// preloadMess
//
/////////////////////////////////////////////////////////
void pix_imageInPlace :: preloadMess(t_symbol *filename, int baseImage, int topImage, int skipRate)
{
  openMess(filename, baseImage, topImage, skipRate);
  if (m_loadedCache)mInPreload = 1;
  else mInPreload = 0;
}

/////////////////////////////////////////////////////////
// downloadMess
//
/////////////////////////////////////////////////////////
void pix_imageInPlace :: downloadMess()
{
  if (!mInPreload)return;
  if (!m_loadedCache->textBind[0])
    {
      glGenTextures(m_numImages, (GLuint *)m_loadedCache->textBind);//__APPLE__
		
      for (int i = 0; i < m_numImages; ++i)
	{
#ifdef GL_VERSION_1_1
	  glBindTexture(GL_TEXTURE_2D, m_loadedCache->textBind[i]);
#elif GL_EXT_texture_object
	  glBindTextureEXT(GL_TEXTURE_2D, m_loadedCache->textBind[i]);
#else
#error No OpenGL bind mechanism
#endif

	  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	  glTexImage2D(GL_TEXTURE_2D, 0,
		       m_loadedCache->images[i]->csize,
		       m_loadedCache->images[i]->xsize,
		       m_loadedCache->images[i]->ysize, 0,
		       m_loadedCache->images[i]->format,
		       m_loadedCache->images[i]->type,
		       m_loadedCache->images[i]->data);
	}
    }
}

/////////////////////////////////////////////////////////
// purgeMess
//
/////////////////////////////////////////////////////////
void pix_imageInPlace :: purgeMess()
{
  if (!m_numImages)
    return;

  if (m_loadedCache->textBind[0])
    {
      glDeleteTextures(m_numImages, (GLuint *)m_loadedCache->textBind);//__APPLE__
      for (int i = 0; i < m_numImages; ++i)
	{
	  m_loadedCache->textBind[i] = 0;
	}
    }
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_imageInPlace :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr, (t_method)&pix_imageInPlace::preloadMessCallback,
		  gensym("preload"), A_SYMBOL, A_FLOAT, A_DEFFLOAT, A_DEFFLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_imageInPlace::downloadImageCallback,
		  gensym("download"), A_NULL);
  class_addmethod(classPtr, (t_method)&pix_imageInPlace::purgeImageCallback,
		  gensym("purge"), A_NULL);
}

void pix_imageInPlace :: preloadMessCallback(void *data, t_symbol *filename, t_floatarg baseImage,
					     t_floatarg topImage, t_floatarg skipRate)
{
  if ((int)skipRate == 0)
    {
      if ((int)topImage == 0)
	GetMyClass(data)->preloadMess(filename, 0, (int)baseImage, 0);
      else
	GetMyClass(data)->preloadMess(filename, (int)baseImage, (int)topImage, 0);
    }
  else
    GetMyClass(data)->preloadMess(filename, (int)baseImage, (int)topImage, (int)skipRate);
}
void pix_imageInPlace :: downloadImageCallback(void *data)
{
  GetMyClass(data)->downloadMess();
}
void pix_imageInPlace :: purgeImageCallback(void *data)
{
  GetMyClass(data)->purgeMess();
}
