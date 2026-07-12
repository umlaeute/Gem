/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_PIXES_PIX_MOVIE_H_
#define _INCLUDE__GEM_PIXES_PIX_MOVIE_H_

#include "Pixes/pix_film.h"
#include "Pixes/pix_texture.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  pix_movie

  Load an digital video (like AVI, Mpeg, Quicktime) into a texture

  KEYWORDS
  pix

  DESCRIPTION

  -----------------------------------------------------------------*/
class GEM_EXTERN pix_movie : public pix_film
{
  CPPEXTERN_HEADER(pix_movie, pix_film);

public:

  //////////
  // Constructor
  pix_movie(t_symbol* filename);

protected:

  //////////
  // Destructor
  virtual ~pix_movie();

  //////////
  // Do the rendering
  virtual void render(GemState *state);
  //////////
  // Clear the dirty flag on the pixBlock
  virtual void postrender(GemState *state);
  //////////
  virtual void startRendering();
  //////////
  // Delete texture object
  virtual void stopRendering();

  pix_texture    m_pixtexture;
  //////////
  // Set the texture quality
  // [in] type - if == 0, then GL_NEAREST, else GL_LINEAR
  void          textureQuality(int type)
  {
    m_pixtexture.textureQuality(type);
  }
  void          repeatMess(int type)
  {
    m_pixtexture.repeatMess(type);
  }
  void          modeMess(int mode)
  {
    m_pixtexture.m_rectangle=mode;
  }

protected:

  //////////
  // static member functions
  static void   textureMessCallback(void *data, t_float n);
  static void   modeCallback(void *data, t_float n);
  static void   rectangleCallback(void *data, t_float n);
  static void   repeatMessCallback(void *data, t_float n);

};

#endif  // for header file
