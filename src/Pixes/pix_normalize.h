/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

/*-----------------------------------------------------------------

    normalize a pixBuf, so that the values are not between min..max but between 0..255
    MIN=min{R,G,B} and MAX=max{R,G,B} of all the pixBuf

-----------------------------------------------------------------*/

#ifndef _INCLUDE__GEM_PIXES_PIX_NORMALIZE_H_
#define _INCLUDE__GEM_PIXES_PIX_NORMALIZE_H_

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_normalize

    Change the overall gain of a pix

KEYWORDS
    pix

DESCRIPTION

    Inlet for a list - "vec_gain"
    Inlet for a float - "ft1"

    "vec_gain" - The gain vector to set to
    "ft1" - Which bit to use as a mask (converted to an int)

-----------------------------------------------------------------*/
class GEM_EXTERN pix_normalize : public GemPixObj
{
  CPPEXTERN_HEADER(pix_normalize, GemPixObj);

public:

  //////////
  // Constructor
  pix_normalize();

protected:

  //////////
  // Destructor
  virtual ~pix_normalize();

  //////////
  // Do the processing
  virtual void        processRGBAImage(imageStruct &image);
  virtual void        processGrayImage(imageStruct &image);
  virtual void        processYUVImage(imageStruct &image);

private:

  //////////
  // Static member functions
  static void         vecMaskMessCallback(void *data, t_symbol*, int argc,
                                          t_atom *argv);
  static void         floatMaskMessCallback(void *data, t_float gain);
};

#endif  // for header file
