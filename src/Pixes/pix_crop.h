/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2003, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_PIXES_PIX_CROP_H_
#define _INCLUDE__GEM_PIXES_PIX_CROP_H_

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_crop

KEYWORDS
    pix

DESCRIPTION
    Extract a part of an image

-----------------------------------------------------------------*/

class GEM_EXTERN pix_crop : public GemPixObj
{
  CPPEXTERN_HEADER(pix_crop, GemPixObj);

public:

  //////////
  // Constructor
  pix_crop(t_floatarg,t_floatarg,t_floatarg,t_floatarg);

protected:

  //////////
  // Destructor
  virtual ~pix_crop();

  //////////
  // set dimension
  void dimenMess(int, int);
  void dimXMess(int);
  void dimYMess(int);

  //////////
  // set offset
  void offsetMess(int, int);
  void offXMess(int);
  void offYMess(int);

  //////////
  // Do the processing
  void    processImage(imageStruct &image);

  unsigned char *m_data;
  int            m_size;


  int sizeX, sizeY, sizeC;
  int offsetX, offsetY;
  int wantSizeX, wantSizeY;

private:
  //////////
  // Static member functions
  static void offsetMessCallback(void *data, t_float x, t_float y);
  static void dimenMessCallback(void *data, t_float x, t_float y);
  static void dimXMessCallback(void *data, t_float x);
  static void dimYMessCallback(void *data, t_float x);
  static void offXMessCallback(void *data, t_float x);
  static void offYMessCallback(void *data, t_float x);
};

#endif  // for header file
