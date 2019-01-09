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
//    Copyright (c) 2002 James Tittle & Chris Clepper
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "pix_write.h"

#include "Gem/Manager.h"
#include "Gem/Cache.h"
#include "Gem/ImageIO.h"

#include "Gem/Files.h"
#include <stdio.h>
#include <string.h>

#ifdef _MSC_VER  /* This is only for Microsoft's compiler, not cygwin, e.g. */
# define snprintf _snprintf
# define vsnprintf _vsnprintf
#endif


CPPEXTERN_NEW_WITH_GIMME(pix_write);

/////////////////////////////////////////////////////////
//
// pix_write
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_write :: pix_write(int argc, t_atom *argv)
  : m_originalImage(NULL), m_color(3)
{
  m_xoff = m_yoff = 0;
  m_width = m_height = 0;
  if (argc == 5) {
    m_color = atom_getint(&argv[4]);
    if (m_color != 1 && m_color != 3 && m_color != 4) {
      error("color argument could be 1, 3 or 4");
      m_color = 3;
    }
    argc--;
  }
  if (argc == 4) {
    m_xoff = atom_getint(&argv[0]);
    m_yoff = atom_getint(&argv[1]);
    m_width = atom_getint(&argv[2]);
    m_height = atom_getint(&argv[3]);
  } else if (argc == 2) {
    m_width = atom_getint(&argv[0]);
    m_height = atom_getint(&argv[1]);
  } else if (argc != 0) {
    error("needs 0, 2, 4 or 5 values");
    m_xoff = m_yoff = 0;
    m_width = m_height = 128;
  }

  inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("list"),
            gensym("vert_pos"));
  inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("list"),
            gensym("vert_size"));

  m_automatic = false;
  m_autocount = 0;
  m_filetype=0;
  m_pathname="gem";

  m_banged = false;

  m_originalImage = new imageStruct();
  m_originalImage->xsize=m_width;
  m_originalImage->ysize=m_height;
  m_originalImage->setCsizeByFormat(m_color);
  m_originalImage->allocate();

}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_write :: ~pix_write(void)
{
  cleanImage();
}


/////////////////////////////////////////////////////////
// extension checks
//
/////////////////////////////////////////////////////////
bool pix_write :: isRunnable(void)
{
  if(GLEW_VERSION_1_1 || GLEW_EXT_texture_object) {
    return true;
  }

  error("your system lacks texture support");
  return false;
}


/////////////////////////////////////////////////////////
// writeMess
//
/////////////////////////////////////////////////////////
void pix_write :: doWrite(void)
{
  int width  = m_width;
  int height = m_height;

  GemMan::getDimen(((m_width >0)?NULL:&width ),
                   ((m_height>0)?NULL:&height));

  m_originalImage->xsize = width;
  m_originalImage->ysize = height;

#ifndef __APPLE__
  m_originalImage->setCsizeByFormat(m_color);
#else
  m_originalImage->setCsizeByFormat(GEM_RGBA);
#endif /* APPLE */

  m_originalImage->reallocate();

  /* the orientation is always correct, since we get it from openGL */
  /* if we do need flipping, this must be handled in mem2image() */
  // FIXXXME: upsidedown should default be 'true'
  m_originalImage->upsidedown=false;


  glReadPixels(m_xoff, m_yoff, width, height,
               m_originalImage->format, m_originalImage->type, m_originalImage->data);

#if 0 // asynchronous texture fetching idea sketch
  /* Enable AGP storage hints */
  glPixelStorei( GL_UNPACK_CLIENT_STORAGE_APPLE, 1 );
  glTextureRangeAPPLE(...);
  glTexParameteri(..., GL_TEXTURE_STORAGE_HINT_APPLE,
                  GL_STORAGE_SHARED_APPLE );

  /* Copy from Frame Buffer */
  glCopyTexSubImage2d(...);

  /* Flush into AGP */
  glFlush(...);

  /* Pull out of AGP */
  glGetTexImage(...);
#endif


  mem2image(m_originalImage, m_filename, m_filetype);
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void pix_write :: render(GemState *state)
{
  if (m_automatic || m_banged) {
    char *extension;
    if (m_filetype<0) {
      m_filetype=0;
    }
    if (m_filetype==0) {
      extension=(char*)"tif";
    } else {
      extension=(char*)"jpg";
    }

    snprintf(m_filename, (size_t)(MAXPDSTRING), "%s%05d.%s",
             m_pathname.c_str(), m_autocount, extension);
    m_filename[MAXPDSTRING-1]=0;

    m_autocount++;
    m_banged = false;
    doWrite();
  }
}


/////////////////////////////////////////////////////////
// sizeMess
//
/////////////////////////////////////////////////////////
void pix_write :: sizeMess(int width, int height)
{
  m_width = width;
  m_height = height;
}

/////////////////////////////////////////////////////////
// posMess
//
/////////////////////////////////////////////////////////
void pix_write :: posMess(int x, int y)
{
  m_xoff = x;
  m_yoff = y;
}

void pix_write :: fileMess(t_symbol*s, int argc, t_atom *argv)
{
  if (argc) {
    if (argv->a_type == A_SYMBOL) {
      char tmp[MAXPDSTRING];
      atom_string(argv++, tmp, MAXPDSTRING);
      argc--;

      m_pathname=gem::files::getFullpath(tmp, this);
    }
    if (argc>0) {
      m_filetype = atom_getint(argv);
    }
  }

  m_autocount = 0;
}

/////////////////////////////////////////////////////////
// cleanImage
//
/////////////////////////////////////////////////////////
void pix_write :: cleanImage(void)
{
  // release previous data
  if (m_originalImage) {
    delete m_originalImage;
    m_originalImage = NULL;
  }
}

/////////////////////////////////////////////////////////
// static member functions
//
/////////////////////////////////////////////////////////
void pix_write :: obj_setupCallback(t_class *classPtr)
{
  CPPEXTERN_MSG (classPtr, "file", fileMess);
  CPPEXTERN_MSG1(classPtr, "auto", autoMess, bool);
  CPPEXTERN_MSG0(classPtr, "bang", bangMess);
  CPPEXTERN_MSG1(classPtr, "color_format", colorFormatMess, int);

  CPPEXTERN_MSG2(classPtr, "vert_size", sizeMess, int, int);
  CPPEXTERN_MSG2(classPtr, "vert_pos",  posMess, int, int);
}

void pix_write :: autoMess(bool on)
{
  m_automatic=on;
}
void pix_write :: bangMess(void)
{
  m_banged=true;
}
void pix_write :: colorFormatMess(int format)
{
  m_color = format;
  if (m_color != 1 && m_color != 3 && m_color != 4) {
    error("color argument could be 1, 3 or 4");
    m_color = 3;
  }
}
