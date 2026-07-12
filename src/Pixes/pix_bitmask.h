/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_PIXES_PIX_BITMASK_H_
#define _INCLUDE__GEM_PIXES_PIX_BITMASK_H_

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_bitmask

KEYWORDS
    pix

DESCRIPTION
    Apply a bit mask to each color

    Inlet for a list - "vec_gain"
    Inlet for a float - "ft1"

    "vec_gain" - The gain vector to set to
    "ft1" - Which bit to use as a mask (converted to an int)

-----------------------------------------------------------------*/
class GEM_EXTERN pix_bitmask : public GemPixObj
{
  CPPEXTERN_HEADER(pix_bitmask, GemPixObj);

public:

  //////////
  // Constructor
  pix_bitmask();

protected:

  //////////
  // Destructor
  virtual ~pix_bitmask();

  //////////
  // Do the processing
  virtual void    processRGBAImage(imageStruct &image);
  virtual void    processGrayImage(imageStruct &image);
  virtual void    processYUVImage(imageStruct &image);

#ifdef __MMX__
  virtual void    processRGBAMMX(imageStruct &image);
  virtual void    processGrayMMX(imageStruct &image);
  virtual void    processYUVMMX(imageStruct &image);
#endif

  //////////
  // Set the new gain
  void            vecMaskMess(int argc, t_atom *argv);

  //////////
  // Set the new mask
  void            intMaskMess(int gain);

  //////////
  // The new gain
  unsigned char   m_mask[4];

  int m_mode; // whether we have only 1 value or separate values; interesting for YUV

private:

  //////////
  // Static member functions
  static void     vecMaskMessCallback(void *data, t_symbol*, int argc,
                                      t_atom *argv);
  static void     floatMaskMessCallback(void *data, t_float gain);
};

#endif  // for header file
