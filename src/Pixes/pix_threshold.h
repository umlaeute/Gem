/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_PIXES_PIX_THRESHOLD_H_
#define _INCLUDE__GEM_PIXES_PIX_THRESHOLD_H_

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_threshold

    Clamp pixel values to a threshold

KEYWORDS
    pix

DESCRIPTION

    Inlet for a list - "vec_thresh"
    Inlet for a float - "ft1"

    "vec_thresh" - The threshold vector
    "ft1" - Set all thresholds to one value

-----------------------------------------------------------------*/
class GEM_EXTERN pix_threshold : public GemPixObj
{
  CPPEXTERN_HEADER(pix_threshold, GemPixObj);

public:

  //////////
  // Constructor
  pix_threshold(int, t_atom*);

protected:

  //////////
  // Destructor
  virtual ~pix_threshold();

  //////////
  // Do the processing
  virtual void    processRGBAImage(imageStruct &image);

  //////////
  // Do the processing
  virtual void    processGrayImage(imageStruct &image);

  //////////
  // Do the processing
  virtual void    processYUVImage(imageStruct &image);

#ifdef __VEC__
  //////////
  // Do the processing
  virtual void    processYUVAltivec(imageStruct &image);
#endif
  //////////
  // Set the new threshold vector
  void            vecThreshMess(t_symbol*, int argc, t_atom *argv);

  //////////
  // Set the new threshold value
  void            floatThreshMess(float thresh);

  //////////
  // The new color
  unsigned char   m_thresh[4];
  unsigned char   m_Y;
};

#endif  // for header file
