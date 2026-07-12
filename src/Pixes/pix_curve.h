/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2001, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_PIXES_PIX_CURVE_H_
#define _INCLUDE__GEM_PIXES_PIX_CURVE_H_

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  pix_curve

  KEYWORDS
  pix

  DESCRIPTION
  map colour values onto others (via tables)

  -----------------------------------------------------------------*/
class GEM_EXTERN pix_curve : public GemPixObj
{
  CPPEXTERN_HEADER(pix_curve, GemPixObj);

public:

  //////////
  // Constructor
  pix_curve(int argc, t_atom *argv);

protected:

  //////////
  // Destructor
  virtual ~pix_curve();

  //////////
  // Do the processing
  virtual void  processRGBAImage(imageStruct &image);
  virtual void  processGrayImage(imageStruct &image);
  virtual void  processYUVImage (imageStruct &image);

  //////////
  // tables to hold the curves
  t_symbol* name_R, *name_G, *name_B, *name_A;

  //////////
  // mode
  int m_mode;

  //////////
  // Set new arrays
  void          setMess(t_symbol*s,int argc, t_atom *argv);

};

#endif  // for header file
