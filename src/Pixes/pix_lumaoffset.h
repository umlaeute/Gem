/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2003, James Tittle II and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

/*-----------------------------------------------------------------

  ported from pete's_plugins (www.petewarden.com)

-----------------------------------------------------------------*/

#ifndef _INCLUDE__GEM_PIXES_PIX_LUMAOFFSET_H_
#define _INCLUDE__GEM_PIXES_PIX_LUMAOFFSET_H_

#include "Base/GemPixObj.h"
/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_lumaoffset

KEYWORDS
    pix

DESCRIPTION


-----------------------------------------------------------------*/
class GEM_EXTERN pix_lumaoffset : public GemPixObj
{
  CPPEXTERN_HEADER(pix_lumaoffset, GemPixObj);

public:

  //////////
  // Constructor
  pix_lumaoffset();

protected:

  //////////
  // Destructor
  virtual ~pix_lumaoffset();

  //////////
  // Do the processing
  virtual void    processRGBAImage(imageStruct &image);
  virtual void    processYUVImage(imageStruct &image);
  virtual void    processGrayImage(imageStruct &image);

  imageStruct     myImage;
  int             init;

  float   m_OffsetScale;
  float   m_LineGap;
  bool    m_DoFilledLines;
  bool    m_DoSmoothFill;

  SPete_MemHandle         hPreviousLineHeights;
  SPete_MemHandle         hPreviousLineHeights2;
  int hPreviousLineHeights_size;
private:

  //////////
  // Static member functions
  static void     offsetCallback(void *data, t_float m_OffsetScale);
  static void     gapCallback(void *data, t_float m_LineGap);
  static void     fillCallback(void *data, t_float m_DoFilledLines);
  static void     smoothCallback(void *data, t_float m_DoSmoothFill);
};

#endif  // for header file
