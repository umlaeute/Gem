/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_PIXES_PIX_ALPHA_H_
#define _INCLUDE__GEM_PIXES_PIX_ALPHA_H_

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_alpha

    Set the alpha values of a pix

KEYWORDS
    pix

DESCRIPTION

    Inlet for a float - "ft2"
    Inlet for a float - "ft1"
    Inlet for a list - "high_val"
    Inlet for a list - "low_val"

    "ft2" - Alpha setting for pixels outside range
    "ft1" - Alpha setting for pixels inside range
    "high_val" - The high value for pixels
    "low_val" - The low value for pixels

-----------------------------------------------------------------*/
class GEM_EXTERN pix_alpha : public GemPixObj
{
  CPPEXTERN_HEADER(pix_alpha, GemPixObj);

public:

  //////////
  // Constructor
  pix_alpha();

protected:

  //////////
  // Destructor
  virtual ~pix_alpha();

  //////////
  // Do the processing
  virtual void    processRGBAImage(imageStruct &image);

  //////////
  // Set the high threshold vector
  void            highThreshMess(float red, float green, float blue);

  //////////
  // Set the low threshold vector
  void            lowThreshMess(float red, float green, float blue);

  //////////
  // Set the new alpha value
  void            alphaMess(float alpha);

  //////////
  // Set the other alpha value
  void            otheralphaMess(float alpha);

  //////////
  // The low threshold
  unsigned char   m_lowThresh[3];

  //////////
  // The high threshold
  unsigned char   m_highThresh[3];

  //////////
  // The alpha value
  unsigned char   m_alpha;

  //////////
  // The other alpha value
  unsigned char   m_otheralpha;

private:

  //////////
  // Static member functions
  static void     highThreshMessCallback(void *data, t_float red,
                                         t_float green, t_float blue);
  static void     lowThreshMessCallback(void *data, t_float red,
                                        t_float green, t_float blue);
  static void     alphaMessCallback(void *data, t_float alpha);
  static void     otheralphaMessCallback(void *data, t_float alpha);
};

#endif  // for header file
