/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2000, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_PIXES_PIX_RECTANGLE_H_
#define _INCLUDE__GEM_PIXES_PIX_RECTANGLE_H_

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_rectangle

    create a pix-rectangle with given RGB(A)

KEYWORDS
    pix

DESCRIPTION

    Inlet for a list - "coord"
    Inlet for a list - "color"

    "coord" - The coordinate-vector that defines the rect
        "color"  - the RGB(A)-color for the rectangle

-----------------------------------------------------------------*/
class GEM_EXTERN pix_rectangle : public GemPixObj
{
  CPPEXTERN_HEADER(pix_rectangle, GemPixObj);

public:

  //////////
  // Constructor
  pix_rectangle();

protected:

  //////////
  // Destructor
  virtual ~pix_rectangle();

  //////////
  // Do the processing
  virtual void    processRGBAImage(imageStruct &image);
  virtual void    processGrayImage(imageStruct &image);
  virtual void    processYUVImage (imageStruct &image);

  //////////
  // Set the new color
  void            vecColorMess(int argc, t_atom *argv);

  //////////
  // Set the new coordinates
  void            vecCoordMess(int argc, t_atom *argv);

  //////////
  // The new color
  unsigned char   m_color[4];

  //////////
  // The two corners
  int                             m_lower_left[2];
  //////////
  int                             m_upper_right[2];

private:
  //////////
  // Static member functions
  static void     vecColorMessCallback(void *data, t_symbol*, int argc,
                                       t_atom *argv);
  static void     vecCoordMessCallback(void *data, t_symbol*, int argc,
                                       t_atom *argv);
};

#endif  // for header file
