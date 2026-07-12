/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2004, Tim Blechmann and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _PIX_MEAN_COLOR_H
#define _PIX_MEAN_COLOR_H

#include "Base/GemPixObj.h"

class GEM_EXTERN pix_mean_color : public GemPixObj
{
  CPPEXTERN_HEADER(pix_mean_color, GemPixObj);

public:
  // constructor
  pix_mean_color(int argc, t_atom *argv);

protected:
  // destructor
  virtual ~pix_mean_color();

  // processing routine
  virtual void processRGBImage(imageStruct &image);
  virtual void processRGBAImage(imageStruct &image);
  virtual void processGrayImage(imageStruct &image);
  virtual void processYUVImage(imageStruct &image);

  t_outlet * m_list;
};


#endif /* _PIX_MEAN_COLOR_H */
