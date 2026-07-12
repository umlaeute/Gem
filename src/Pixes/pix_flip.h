/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_PIXES_PIX_FLIP_H_
#define _INCLUDE__GEM_PIXES_PIX_FLIP_H_

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_flip

KEYWORDS
    pix

DESCRIPTION

    Flip a pix

    "horizontal" - Flip horizontally
    "vertical" - Flip vertically
    "both" - Flip in both directiosn
    "none" - Do nothing

-----------------------------------------------------------------*/
class GEM_EXTERN pix_flip : public GemPixObj
{
  CPPEXTERN_HEADER(pix_flip, GemPixObj);

public:

  //////////
  // Constructor
  pix_flip(t_symbol*s);

protected:

  //////////
  // Destructor
  virtual ~pix_flip();

  //////////
  // Do the processing
  virtual void    processRGBAImage(imageStruct &image);
  virtual void    processGrayImage(imageStruct &image);
  virtual void    processYUVImage(imageStruct &image);

  //////////
  enum FlipType   { HORIZONTAL, VERTICAL, BOTH, NONE };

  //////////
  void            flipMess(FlipType type);
  void            flipMess(t_symbol*s);

  //////////
  // The flip type
  FlipType            m_flip;

private:

  //////////
  // Static member functions
  static void     horMessCallback(void *data);
  static void     vertMessCallback(void *data);
  static void     bothMessCallback(void *data);
  static void     noneMessCallback(void *data);
};

#endif  // for header file
