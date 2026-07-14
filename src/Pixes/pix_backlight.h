/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, Pete Warden
 * SPDX-FileCopyrightText: © 2003, James Tittle II and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

/*-----------------------------------------------------------------

    ported from pete's_plugins (www.petewarden.com)

-----------------------------------------------------------------*/

#ifndef _INCLUDE__GEM_PIXES_PIX_BACKLIGHT_H_
#define _INCLUDE__GEM_PIXES_PIX_BACKLIGHT_H_

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_backlight

KEYWORDS
    pix

DESCRIPTION



-----------------------------------------------------------------*/
class GEM_EXTERN pix_backlight : public GemPixObj
{
  CPPEXTERN_HEADER(pix_backlight, GemPixObj);

public:

  //////////
  // Constructor
  pix_backlight(int, t_atom*);

protected:

  //////////
  // Destructor
  virtual ~pix_backlight();

  //////////
  // Do the processing
  virtual void    processRGBAImage(imageStruct &image);
  virtual void    processYUVImage(imageStruct &image);
  //virtual void  processGrayImage(imageStruct &image);

  imageStruct     myImage;
  int             nHeight;
  int             nWidth;
  int             init;

  U32*            pSource;
  U32*            pOutput;

  float m_SpikeScale;
  float m_SpikeFloor;
  float m_SpikeCeiling;

  //int Pete_BackLight_Init();
  //void Pete_BackLight_DeInit();

  //////////
  void     scaleMess(t_float scale);
  void     floorMess(t_float floor);
  void     ceilingMess(t_float ceil);
};

#endif  // for header file
