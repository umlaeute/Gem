/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2003, James Tittle II and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_PIXES_PIX_REFRACTION_H_
#define _INCLUDE__GEM_PIXES_PIX_REFRACTION_H_

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_refraction


KEYWORDS
    pix

DESCRIPTION

    View pix thru glass blocks
    ported from pete's_plugins (www.petewarden.com)


-----------------------------------------------------------------*/
class GEM_EXTERN pix_refraction : public GemPixObj
{
  CPPEXTERN_HEADER(pix_refraction, GemPixObj);

public:

  //////////
  // Constructor
  pix_refraction();

protected:

  //////////
  // Destructor
  virtual ~pix_refraction();

  //////////
  // Do the processing
  virtual void    processRGBAImage(imageStruct &image);
  virtual void    processYUVImage(imageStruct &image);

  imageStruct     myImage;
  int             nHeight;
  int             nWidth;
  int             init;

  U32*            pSource;
  U32*            pOutput;

  float           m_Refraction;
  float           m_CellWidth;
  float           m_CellHeight;
  float           m_DoAllowMagnification;

private:

  //////////
  // Static member functions
  static void     refractCallback(void *data, t_float m_Refraction);
  static void     widthCallback(void *data, t_float m_CellWidth);
  static void     heightCallback(void *data, t_float m_CellHeight);
  static void     magCallback(void *data, t_float m_DoAllowMagnification);
};

#endif  // for header file
