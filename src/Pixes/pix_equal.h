/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2011, Ricardo Fabbri and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_PIXES_PIX_EQUAL_H_
#define _INCLUDE__GEM_PIXES_PIX_EQUAL_H_

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_equal

KEYWORDS
    pix, color

DESCRIPTION

    mark pixels nearly equal to a given color, within given bounds

    Inlet for a list - "vec_low"
    Inlet for a float  - "vec_up"

    "vec_low" - Lower bound in RGBA to search for
    "vec_up" - Upper bound in RGBA to search for

-----------------------------------------------------------------*/
class GEM_EXTERN pix_equal : public GemPixObj
{
  CPPEXTERN_HEADER(pix_equal, GemPixObj);

public:

  //////////
  // Constructor
  pix_equal();

protected:

  //////////
  // Destructor
  virtual ~pix_equal();

  //////////
  // Do the processing
  virtual void    processRGBAImage(imageStruct &image);

  //////////
  // Set the new color range vectors
  void            vecLowerBoundMess(t_symbol*,int argc, t_atom *argv);
  void            vecUpperBoundMess(t_symbol*,int argc, t_atom *argv);

  //////////
  // The bounds
  unsigned char   m_upper[4];
  unsigned char   m_lower[4];
};

#endif  // for header file
