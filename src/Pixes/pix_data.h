/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_PIXES_PIX_DATA_H_
#define _INCLUDE__GEM_PIXES_PIX_DATA_H_

#include "Base/GemPixDualObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS

    pix_data

KEYWORDS
    pix

DESCRIPTION

    Get pixel information

    Inlet for a float - "xPos"
    Inlet for a float - "yPos"

        The x and y position should be normalized between 0 and 1.

-----------------------------------------------------------------*/
class GEM_EXTERN pix_data : public GemPixDualObj
{
  CPPEXTERN_HEADER(pix_data, GemPixDualObj);

public:

  //////////
  // Constructor
  pix_data(t_floatarg x=0, t_floatarg y=0);

protected:

  //////////
  // Destructor
  virtual ~pix_data();

  //////////
  // All we want is the pixel information, so this is a complete override.
  virtual void    render(GemState *state)                 {}

  //////////
  virtual void    processImage(imageStruct &)             {}

  //////////
  virtual void    processDualImage(imageStruct &, imageStruct &)  {}

  //////////
  void                    trigger(void);

  //////////
  void                    xPos(t_float pos);
  void                    yPos(t_float pos);
  void                    listMess(t_symbol* s, int argc, t_atom* argv);

  //////////
  t_float                 m_position[2];

  //////////
  void                    qualityMess(int q);
  enum QualityType   { NONE, LINEAR2D };
  QualityType m_quality;

  //////////
  void                    normalizeMess(int r);
  enum NormalizeType   { RAW, NORMALIZED };
  NormalizeType m_normalize;

  //////////
  // The color outlet
  t_outlet        *m_colorOut;

  //////////
  // The grayscale outlet
  t_outlet        *m_grayOut;

private:

  //////////
  // Static member callbacks
  static void             triggerMessCallback(void *data);
  static void             xPosCallback(void *data, t_float pos);
  static void             yPosCallback(void *data, t_float pos);
};

#endif  // for header file
