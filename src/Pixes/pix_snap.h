/*-----------------------------------------------------------------
  LOG
  GEM - Graphics Environment for Multimedia

  Snap a pix of the render buffer

  Copyright (c) 1997-1999 Mark Danks. mark@danks.org
  Copyright (c) Günther Geiger. geiger@epy.co.at
  Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

  -----------------------------------------------------------------*/

#ifndef _INCLUDE__GEM_PIXES_PIX_SNAP_H_
#define _INCLUDE__GEM_PIXES_PIX_SNAP_H_

#include "Base/GemBase.h"
#include "Gem/GemGL.h"
#include "Gem/Image.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  pix_snap

  Snaps a pix of the render buffer

  KEYWORDS
  pix

  DESCRIPTION

  Inlet for a list - "vert_size"
  Inlet for a list - "vert_pos"

  "snap" - Snap a pix
  "vert_size" - Set the size of the pix
  "vert_pos" - Set the position of the pix

  -----------------------------------------------------------------*/
class GEM_EXTERN pix_snap : public GemBase
{
  CPPEXTERN_HEADER(pix_snap, GemBase);

public:

  //////////
  // Constructor
  pix_snap(int argc, t_atom *argv);

  //////////
  // Destructor
  virtual ~pix_snap(void);

  //////////
  // Do the rendering
  virtual void  render(GemState *state);

  //////////
  // Clear the dirty flag on the pixBlock
  virtual void  postrender(GemState *state);

  virtual void  snapMess(void);

  //////////
  // Clean up the image
  void          cleanImage(void);

  //////////
  // The original pix_snap
  imageStruct   *m_originalImage;

  //////////
  // The pixBlock with the current image
  pixBlock      m_pixBlock;
  imageStruct   m_imageStruct;

  //////////
  // The position & dimension
  virtual void  posMess(int x, int y);
  int m_x, m_y;
  virtual void  sizeMess(int width, int height);
  int m_width, m_height;

  /* using PBOs for (hopefully) optimized pixel transfers */
  void pboMess(int num_pbos);
  GLint m_numPbo, m_curPbo;
  GLuint *m_pbo;                   // IDs of PBO
};

#endif  // for header file
