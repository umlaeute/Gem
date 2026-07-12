/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */
#ifndef _INCLUDE__GEM_PIXES_PIX_RGBA_H_
#define _INCLUDE__GEM_PIXES_PIX_RGBA_H_

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_rgba

KEYWORDS
    pix

DESCRIPTION

    Change pix from "any" color-space to GL_RGBA

-----------------------------------------------------------------*/

class GEM_EXTERN pix_rgba : public GemPixObj
{
  CPPEXTERN_HEADER(pix_rgba, GemPixObj);

public:

  //////////
  // Constructor
  pix_rgba();

protected:

  //////////
  // Destructor
  virtual ~pix_rgba();

  //////////
  // Do the processing
  void        processImage(imageStruct &image);

  imageStruct m_image;
private:
};

#endif  // for header file
