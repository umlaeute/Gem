////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.at
//
// Implementation file
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "pix_snap.h"

#include "Gem/Manager.h"
#include "Gem/Cache.h"
#include "Gem/State.h"
#include "Gem/Settings.h"

CPPEXTERN_NEW_WITH_GIMME(pix_snap);

/////////////////////////////////////////////////////////
//
// pix_snap
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_snap :: pix_snap(int argc, t_atom *argv) :
  m_originalImage(NULL),
  m_x(0), m_y(0), m_width(0), m_height(0),
  m_numPbo(0), m_curPbo(0), m_pbo(NULL)

{
  m_pixBlock.image = m_imageStruct;
  m_pixBlock.image.data = NULL;
  if (argc == 4) {
    m_x = atom_getint(&argv[0]);
    m_y = atom_getint(&argv[1]);
    m_width = atom_getint(&argv[2]);
    m_height = atom_getint(&argv[3]);
  } else if (argc == 2) {
    m_x = m_y = 0;
    m_width = atom_getint(&argv[0]);
    m_height = atom_getint(&argv[1]);
  } else if (argc == 0) {
    m_x = m_y = 0;
    m_width = m_height = 128;
  } else {
    error("needs 0, 2, or 4 values");
    m_x = m_y = 0;
    m_width = m_height = 128;
  }

  gem::Settings::get("snap.pbo", m_numPbo);

  inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("list"),
            gensym("vert_pos"));
  inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("list"),
            gensym("vert_size"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_snap :: ~pix_snap(void)
{
  cleanImage();
}


#ifdef DEBUG_TIME
#include <sys/time.h>

# define START_TIMING() float mseconds=0.f;     \
  timeval startTime, endTime;                   \
  gettimeofday(&startTime, 0)
# define STOP_TIMING(x) gettimeofday(&endTime, 0);      \
  mseconds = (endTime.tv_sec - startTime.tv_sec)*1000 + \
    (endTime.tv_usec - startTime.tv_usec) * 0.001;      \
  post("%d PBO time = %f ms", x, mseconds)

#else
# define START_TIMING()
# define STOP_TIMING(x)
#endif

/////////////////////////////////////////////////////////
// snapMess
//
/////////////////////////////////////////////////////////
void pix_snap :: snapMess(void)
{
  if(getState()==INIT) {
    verbose(0, "not initialized yet with a valid context");
    return;
  }
  if(!GLEW_VERSION_1_1 && !GLEW_EXT_texture_object) {
    return;
  }

  if (m_cache&&m_cache->m_magic!=GEMCACHE_MAGIC) {
    m_cache=NULL;
  }

  if (m_width <= 0 || m_height <= 0) {
    error("Illegal size");
    return;
  }
  // do we need to remake the data?
  bool makeNew = false;
  bool makePbo = false;

  // release previous data
  if (m_originalImage)  {
    if (m_originalImage->xsize != m_width ||
        m_originalImage->ysize != m_height) {
      m_originalImage->clear();
      delete m_originalImage;
      m_originalImage = NULL;
      makeNew = true;
    }
  }       else {
    makeNew = true;
  }
  if (makeNew) {
    m_originalImage = new imageStruct;
    m_originalImage->xsize = m_width;
    m_originalImage->ysize = m_height;
    m_originalImage->setCsizeByFormat(GEM_RGBA);
    // FIXXXME: upsidedown should default be 'true'
    m_originalImage->upsidedown = false;

    m_originalImage->allocate(m_originalImage->xsize * m_originalImage->ysize *
                              m_originalImage->csize);

    makePbo=true;
  }


  if(m_numPbo>0 && !m_pbo) {
    makePbo=true;
  } else if(m_numPbo<=0) {
    makePbo=false;
  }

  /* FIXXME */
  if(makePbo) {
    if(m_pbo) {
      delete[]m_pbo;
      m_pbo=NULL;
    }
    if(GLEW_ARB_pixel_buffer_object) {
      m_pbo=new GLuint[m_numPbo];
      glGenBuffersARB(m_numPbo, m_pbo);
      int i=0;
      for(i=0; i<m_numPbo; i++) {
        glBindBufferARB(GL_PIXEL_PACK_BUFFER_ARB, m_pbo[i]);
        glBufferDataARB(GL_PIXEL_PACK_BUFFER_ARB,
                        m_originalImage->xsize*m_originalImage->ysize*m_originalImage->csize,
                        0, GL_STREAM_READ_ARB);
      }
      glBindBufferARB(GL_PIXEL_PACK_BUFFER_ARB, 0);
    } else {
      verbose(1, "PBOs not supported! disabling");
      m_numPbo=0;
    }
  }

  if(m_pbo) {
    START_TIMING();
    m_curPbo=(m_curPbo+1)%m_numPbo;
    int index=m_curPbo;
    int nextIndex=(m_curPbo+1)%m_numPbo;

    glBindBufferARB(GL_PIXEL_PACK_BUFFER_ARB, m_pbo[index]);

    glReadPixels(m_x, m_y, m_width, m_height,
                 m_originalImage->format, m_originalImage->type, 0);


    glBindBufferARB(GL_PIXEL_PACK_BUFFER_ARB, m_pbo[nextIndex]);
    GLubyte* src = (GLubyte*)glMapBufferARB(GL_PIXEL_PACK_BUFFER_ARB,
                                            GL_READ_ONLY_ARB);
    if(src) {
      m_originalImage->fromRGBA(src);
      glUnmapBufferARB(
        GL_PIXEL_PACK_BUFFER_ARB);     // release pointer to the mapped buffer
    }
    glBindBufferARB(GL_PIXEL_PACK_BUFFER_ARB, 0);
    STOP_TIMING(m_numPbo);
  } else {
    START_TIMING();
    glFinish();
    glPixelStorei(GL_PACK_ALIGNMENT, 4);
    glPixelStorei(GL_PACK_ROW_LENGTH, 0);
    glPixelStorei(GL_PACK_SKIP_ROWS, 0);
    glPixelStorei(GL_PACK_SKIP_PIXELS, 0);

    glReadPixels(m_x, m_y, m_width, m_height,
                 m_originalImage->format, m_originalImage->type, m_originalImage->data);
    STOP_TIMING(-1);
  }

  if (m_cache) {
    m_cache->resendImage = 1;
  }
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void pix_snap :: render(GemState *state)
{
  // if we don't have an image, just return
  if (!m_originalImage) {
    return;
  }

  // do we need to reload the image?
  if (m_cache&&m_cache->resendImage) {
    m_originalImage->refreshImage(&m_pixBlock.image);
    m_pixBlock.newimage = 1;
    m_cache->resendImage = 0;
  }

  state->set(GemState::_PIX, &m_pixBlock);
}

/////////////////////////////////////////////////////////
// postrender
//
/////////////////////////////////////////////////////////
void pix_snap :: postrender(GemState *state)
{
  m_pixBlock.newimage = 0;
  state->set(GemState::_PIX, static_cast<pixBlock*>(NULL));
}

/////////////////////////////////////////////////////////
// sizeMess
//
/////////////////////////////////////////////////////////
void pix_snap :: sizeMess(int width, int height)
{
  m_width = width;
  m_height = height;
}

/////////////////////////////////////////////////////////
// posMess
//
/////////////////////////////////////////////////////////
void pix_snap :: posMess(int x, int y)
{
  m_x = x;
  m_y = y;
}

/////////////////////////////////////////////////////////
// cleanImage
//
/////////////////////////////////////////////////////////
void pix_snap :: cleanImage(void)
{
  // release previous data
  if (m_originalImage) {
    delete m_originalImage;
    m_originalImage = NULL;

    m_pixBlock.image.clear();
  }
}



////////////////////////////////////////////////////////
// Pixel Buffer Object message
//
/////////////////////////////////////////////////////////
void pix_snap :: pboMess(int num)
{
  if(num<0) {
    return;
  }

  if(m_pbo) {
    glDeleteBuffersARB(m_numPbo, m_pbo);
    delete[]m_pbo;
    m_pbo=NULL;
    m_numPbo=0;
  }

  m_numPbo=num;
  setModified();
}

/////////////////////////////////////////////////////////
// static member functions
//
/////////////////////////////////////////////////////////
void pix_snap :: obj_setupCallback(t_class *classPtr)
{
  CPPEXTERN_MSG0(classPtr, "snap", snapMess);
  CPPEXTERN_MSG0(classPtr, "bang", snapMess);

  CPPEXTERN_MSG2(classPtr, "vert_size", sizeMess, int, int);
  CPPEXTERN_MSG2(classPtr, "vert_pos",  posMess, int, int);

  CPPEXTERN_MSG1(classPtr, "pbo",  pboMess, int);
}
