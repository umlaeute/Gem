/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2000, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_PIXES_PIX_A__GREY_H_
#define _INCLUDE__GEM_PIXES_PIX_A__GREY_H_

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_a_2grey

KEYWORDS
    pix

DESCRIPTION
    Change pix to greyscale with respect to alpha

-----------------------------------------------------------------*/
class GEM_EXTERN pix_a_2grey : public GemPixObj
{
  CPPEXTERN_HEADER(pix_a_2grey, GemPixObj);

public:

  //////////
  // Constructor
  pix_a_2grey(t_floatarg alpha);

protected:

  //////////
  // Destructor
  virtual ~pix_a_2grey();

  //////////
  // Do the processing
  virtual void    processRGBAImage(imageStruct &image);

  //////////
  // Do the processing - this is a no-op
  virtual void    processGrayImage(imageStruct &image) { }

  //////////
  // alpha influences process
  // == 0 no influence
  // <  0 change pixes with alpha < -mode
  // >  0 change pixes with alpha >  mode
  int                             m_mode;

  //////////
  // alpha setting
  void            alphaMess(float alphaval);

private:

  //////////
  // static member functions
  static void     alphaMessCallback(void *data, t_float alphaval);
};

#endif  // for header file
