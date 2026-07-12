/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, Chris Clepper and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_PIXES_PIX_BACKGROUND_H_
#define _INCLUDE__GEM_PIXES_PIX_BACKGROUND_H_

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  pix_background

  KEYWORDS
  pix
  yuv

  DESCRIPTION


  -----------------------------------------------------------------*/

class GEM_EXTERN pix_background : public GemPixObj
{
  CPPEXTERN_HEADER(pix_background, GemPixObj);

public:

  //////////
  // Constructor
  pix_background(int argc, t_atom*argv);

protected:

  //////////
  // Destructor
  virtual ~pix_background();

  //////////
  // Do the processing
  virtual void  processRGBAImage(imageStruct &image);
  virtual void  processGrayImage(imageStruct &image);
  virtual void  processYUVImage (imageStruct &image);
#ifdef __MMX__
  virtual void processRGBAMMX   (imageStruct &image);
  virtual void processYUVMMX    (imageStruct &image);
  virtual void processGrayMMX   (imageStruct &image);
#endif
#ifdef __VEC__
  //////////
  // Do the YUV Altivec processing
  virtual void  processYUVAltivec(imageStruct &image);
#endif

  virtual void rangeNMess(int argc, t_atom*argv);

  imageStruct   m_savedImage;
  int           m_Yrange,m_Urange,m_Vrange, m_Arange;
  t_inlet      *inletRange;
  int           m_reset;


private:

  //////////
  // Static member functions

  static void rangeCallback       (void *data, t_float Y, t_float U,
                                   t_float V);
  //  static void rangeNCallback      (void *data, t_float Y, t_float U, t_float V);
  static void rangeNCallback      (void *data, t_symbol*,int,t_atom*);
  static void resetCallback       (void *data);


};

#endif
