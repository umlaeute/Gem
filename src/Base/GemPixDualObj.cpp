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
//    Copyright (c) 2002 James Tittle & Chris Clepper
//
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "GemPixDualObj.h"

#include "GemCache.h"

#include <string.h>
#include <stdio.h>

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
void GemPixDualObj :: render(GemState *state)
{
  GemPixObj::render(state);
}

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
#define PROCESS_DUALIMAGE_SIMD(CS) \
  switch(m_simd){		   \
  case (GEM_SIMD_MMX):				\
    process##CS ##_MMX(image, m_pixRight->image);	\
    break;					\
  case(GEM_SIMD_SSE2):				\
    process##CS ##_SSE2(image, m_pixRight->image);	\
    break;					\
  case(GEM_SIMD_ALTIVEC):				\
    process##CS ##_Altivec(image, m_pixRight->image);		\
    break;						\
  default:						\
    process##CS ##_##CS(image, m_pixRight->image);		\
  }
  
#define PROCESS_DUALIMAGE(CS1, CS2) \
  process##CS1 ##_##CS2 (image, m_pixRight->image);

#define PROCESS_COLORSPACE(FUN_RGBA, FUN_YUV, FUN_GRAY)			\
  switch (m_pixRight->image.format) {					\
  case GL_RGBA: case GL_BGRA_EXT:					\
    found=true; FUN_RGBA; break;		\
  case GL_LUMINANCE:							\
    found=true; FUN_GRAY; break;		\
  case GL_YCBCR_422_GEM:						\
    found=true; FUN_YUV ; break;		\
  default:break;}

void GemPixDualObj :: processImage(imageStruct &image)
{
  if (!m_cacheRight || m_cacheRight->m_magic!=GEMCACHE_MAGIC){
    m_cacheRight=NULL;
    return;
  }

  //if (!m_cacheRight || !&image || !&m_pixRight || !&m_pixRight->image) return;
  if (!m_pixRightValid || !&image || !&m_pixRight || !&m_pixRight->image) return;
  
    if (image.xsize != m_pixRight->image.xsize ||
    	image.ysize != m_pixRight->image.ysize)    {
      error("GEM: GemPixDualObj [%s]: two images do not have equal dimensions", m_objectname->s_name);
      m_pixRightValid = 0;
      
    	return;
    }
    bool found = false;
    switch (image.format) {
    case GL_RGBA:
    case GL_BGRA_EXT:
      PROCESS_COLORSPACE(PROCESS_DUALIMAGE_SIMD(RGBA),
			 PROCESS_DUALIMAGE(RGBA, YUV),
			 PROCESS_DUALIMAGE(RGBA, Gray));
      break;
    case GL_LUMINANCE:
      PROCESS_COLORSPACE(PROCESS_DUALIMAGE(Gray, RGBA),
			 PROCESS_DUALIMAGE(Gray, YUV),
			 PROCESS_DUALIMAGE_SIMD(Gray));
      break;
    case GL_YCBCR_422_GEM:
      PROCESS_COLORSPACE(PROCESS_DUALIMAGE(YUV, RGBA),
			 PROCESS_DUALIMAGE_SIMD(YUV),
			 PROCESS_DUALIMAGE(YUV, Gray));
      break;
    default: break;
    }
    if (!found)processDualImage(image, m_pixRight->image);
}

/////////////////////////////////////////////////////////
// process
//
/////////////////////////////////////////////////////////

void GemPixDualObj :: processDualImage(imageStruct &left, imageStruct &right){
  char *lformat, *rformat;
  switch (left.format) {
  case GL_RGBA:
  case GL_BGRA_EXT:
    lformat ="RGBA";break;
  case GL_LUMINANCE:
    lformat ="Gray";break;
  case GL_YCBCR_422_GEM:
    lformat ="YUV";break;
  default:
    lformat = new char[6];
    sprintf(lformat,"0x%04X", (unsigned int)left.format);
  }
  switch (right.format) {
  case GL_RGBA:
  case GL_BGRA_EXT:
    rformat ="RGBA";break;
  case GL_LUMINANCE:
    rformat ="Gray";break;
  case GL_YCBCR_422_GEM:
    rformat ="YUV";break;
  default:
    rformat = new char[6];
    sprintf(rformat, "0x%04X", (unsigned int)left.format);
  }
  
  error("processDualImage [%s]: no method to combine (%s) and (%s)", m_objectname->s_name,
	lformat, rformat);
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

/////////////////////////////////////////////////////////
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
  } else error("GEM: [%s] wrong righthand arguments....", GetMyClass(data)->m_objectname->s_name);
}
