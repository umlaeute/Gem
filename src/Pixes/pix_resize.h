/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_PIXES_PIX_RESIZE_H_
#define _INCLUDE__GEM_PIXES_PIX_RESIZE_H_

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_resize

    Resizes an image

KEYWORDS
    pix

-----------------------------------------------------------------*/
class GEM_EXTERN pix_resize : public GemPixObj
{
  CPPEXTERN_HEADER(pix_resize, GemPixObj);

public:

  //////////
  // Constructor
  pix_resize(t_floatarg w, t_floatarg h);

protected:

  //////////
  // Destructor
  virtual ~pix_resize();

  //////////
  // Do the processing
  void    processImage(imageStruct &image);

  //////////
  // setting dimension and colourspace
  void  dimenMess(int w, int h);
  int           m_width, m_height;
  imageStruct   m_image;

private:

  //////////
  // static member functions
  static void   dimenMessCallback(void *data, t_float w, t_float h);
};

#endif  // for header file
