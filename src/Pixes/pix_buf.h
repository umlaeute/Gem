/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2000, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_PIXES_PIX_BUF_H_
#define _INCLUDE__GEM_PIXES_PIX_BUF_H_

#include "Base/GemBase.h"
#include "Gem/Image.h"

class GemCache;

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_buf

KEYWORDS
    pix

DESCRIPTION
    buffer a pixBlock

    This makes an assumption that the size of the image doesn't
        change once the rendering has started

-----------------------------------------------------------------*/
class GEM_EXTERN pix_buf : public GemBase
{
  CPPEXTERN_HEADER(pix_buf, GemBase);

public:

  //////////
  // Constructor
  pix_buf(t_floatarg);

protected:

  //////////
  // Destructor
  virtual ~pix_buf();

  //////////
  // Do the rendering
  virtual void    render(GemState *state);

  //////////
  // After the rendering
  virtual void    postrender(GemState *state);

  //////////
  // Clean everything up
  void            cleanImage();

  //////////
  // the pixBlock-cache
  pixBlock    cachedPixBlock;
  pixBlock    *orgPixBlock;

  //////////
  // force output of the buffer:
  void            bangMess();
  bool            m_banged;

  void            autoMess(int);
  bool            m_auto;

};

#endif  // for header file
