/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2003, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_PIXES_PIX_CONVERT_H_
#define _INCLUDE__GEM_PIXES_PIX_CONVERT_H_

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_convert

KEYWORDS
    pix

DESCRIPTION
    Change pix from "any" color-space to *any* other

-----------------------------------------------------------------*/

class GEM_EXTERN pix_convert : public GemPixObj
{
  CPPEXTERN_HEADER(pix_convert, GemPixObj);

public:

  //////////
  // Constructor
  pix_convert(t_symbol*);

protected:

  //////////
  // Destructor
  virtual ~pix_convert();

  //////////
  // Do the processing
  void        processImage(imageStruct &image);
  void colorMess(t_symbol*s);

  imageStruct m_image;
};

#endif  // for header file
