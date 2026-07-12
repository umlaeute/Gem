/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

/*-----------------------------------------------------------------

  Convert the HSV(A)-Channels of a pixBuf into RGB(A)

  -----------------------------------------------------------------*/

#ifndef _INCLUDE__GEM_PIXES_PIX_HSV_RGB_H_
#define _INCLUDE__GEM_PIXES_PIX_HSV_RGB_H_

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  pix_hsv2rgb

  Change pix to greyscale

  KEYWORDS
  pix

  DESCRIPTION

  -----------------------------------------------------------------*/
class GEM_EXTERN pix_hsv2rgb : public GemPixObj
{
  CPPEXTERN_HEADER(pix_hsv2rgb, GemPixObj);

public:

  //////////
  // Constructor
  pix_hsv2rgb();

protected:

  //////////
  // Destructor
  virtual ~pix_hsv2rgb();

  //////////
  // Do the processing
  virtual void  processRGBAImage(imageStruct &image);
};

#endif  // for header file
