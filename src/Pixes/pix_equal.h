/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

Marks the pixels equal to given values/range

Copyright (c) 2011 Ricardo Fabbri. labmacambira.sf.net rfabbri@gmail.com
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef _INCLUDE__GEM_PIXES_PIX_EQUAL_H_
#define _INCLUDE__GEM_PIXES_PIX_EQUAL_H_

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_equal

    mark pixels nearly equal to a given color, within given bounds

KEYWORDS
    pix, color

DESCRIPTION

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
