/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2003, Chris Clepper and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_PIXES_PIX_MIX_H_
#define _INCLUDE__GEM_PIXES_PIX_MIX_H_

#include "Base/GemPixDualObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_mix

    Blends two images together.

KEYWORDS
    pix

DESCRIPTION

    gain $1 $2 - sets the gain for the left and right images.

-----------------------------------------------------------------*/
class GEM_EXTERN pix_mix : public GemPixDualObj
{
  CPPEXTERN_HEADER(pix_mix, GemPixDualObj);

public:

  //////////
  // Constructor
  pix_mix(int,t_atom*);

protected:

  //////////
  // Destructor
  virtual ~pix_mix();

  //////////
  // Do the processing
  virtual void    processRGBA_RGBA(imageStruct &image, imageStruct &right);
  virtual void    processGray_Gray(imageStruct &image, imageStruct &right);
  virtual void    processYUV_YUV(imageStruct &image, imageStruct &right);
#ifdef __MMX__
  virtual void    processRGBA_MMX(imageStruct &image, imageStruct &right);
  virtual void    processGray_MMX(imageStruct &image, imageStruct &right);
  virtual void    processYUV_MMX(imageStruct &image, imageStruct &right);
#endif
#ifdef __VEC__
  virtual void    processYUV_Altivec (imageStruct &image,
                                      imageStruct &right);
#endif

  virtual void    gainMess (float X, float Y);

  long imageGain,rightGain;


private:

  //////////
  // Static member functions

  //static void gainCallback       (void *data, t_float X, t_float Y);
  static void gainCallback       (void *data, t_symbol*,int,t_atom*);



};

#endif  // for header file
