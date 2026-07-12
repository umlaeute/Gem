/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_PIXES_PIX_RGB_HSV_H_
#define _INCLUDE__GEM_PIXES_PIX_RGB_HSV_H_

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  pix_rgb2hsv

  Change pix to greyscale

  KEYWORDS
  pix

  DESCRIPTION

  -----------------------------------------------------------------*/
class GEM_EXTERN pix_rgb2hsv : public GemPixObj
{
  CPPEXTERN_HEADER(pix_rgb2hsv, GemPixObj);

public:

  //////////
  // Constructor
  pix_rgb2hsv();

protected:

  //////////
  // Destructor
  virtual ~pix_rgb2hsv();

  //////////
  // Do the processing
  virtual void  processRGBAImage(imageStruct &image);
};

#endif  // for header file
