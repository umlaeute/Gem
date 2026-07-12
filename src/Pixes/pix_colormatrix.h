/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_PIXES_PIX_COLORMATRIX_H_
#define _INCLUDE__GEM_PIXES_PIX_COLORMATRIX_H_

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_colormatrix

KEYWORDS
    pix

DESCRIPTION

    Apply a color matrix kernel

    Inlet for a list - "matrix"

    "matrix" - The matrix for the color conversion

-----------------------------------------------------------------*/
class GEM_EXTERN pix_colormatrix : public GemPixObj
{
  CPPEXTERN_HEADER(pix_colormatrix, GemPixObj);

public:

  //////////
  // Constructor
  pix_colormatrix();

protected:

  //////////
  // Destructor
  virtual ~pix_colormatrix();

  //////////
  // Do the processing
  virtual void    processRGBAImage(imageStruct &image);

  //////////
  // Set the matrix
  void            matrixMess(int argc, t_atom *argv);

  //////////
  // The matrix
  float           m_matrix[16];

private:

  //////////
  // Static member functions
  static void     matrixMessCallback(void *data, t_symbol*, int argc,
                                     t_atom *argv);
};

#endif  // for header file
