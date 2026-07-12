/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_PIXES_PIX_CLEARBLOCK_H_
#define _INCLUDE__GEM_PIXES_PIX_CLEARBLOCK_H_

#include "Base/GemBase.h"
#include "Gem/Image.h"

class GemCache;

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  pix_clearblock

  KEYWORDS
  pix

  DESCRIPTION
  Clear an image without destroying glTexture

  -----------------------------------------------------------------*/

class GEM_EXTERN pix_clearblock : public GemBase {
  CPPEXTERN_HEADER(pix_clearblock, GemBase);

public:

  //////////
  // Constructor
  pix_clearblock();

protected:

  //////////
  // Destructor
  virtual ~pix_clearblock();

  //////////
  // Do the rendering
  virtual void  render(GemState *state);

  //////////
  // After the rendering
  virtual void  postrender(GemState *state);

  //////////
  virtual void  startRendering();

  //////////
  virtual void  stopRendering();

  //////////
  // The old cache
  GemCache      *m_oldcache;
};

#endif  // for header file
