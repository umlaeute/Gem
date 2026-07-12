/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2000, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

/*-----------------------------------------------------------------

-----------------------------------------------------------------*/

#ifndef _INCLUDE__GEM_PIXES_PIX_BLOB_H_
#define _INCLUDE__GEM_PIXES_PIX_BLOB_H_

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
  pix_blob

KEYWORDS
  pix

DESCRIPTION
  Calculate the center of gravity of a pixBlock.

-----------------------------------------------------------------*/
class GEM_EXTERN pix_blob : public GemPixObj
{
  CPPEXTERN_HEADER(pix_blob, GemPixObj);

public:

  //////////
  // Constructor
  pix_blob(int argc, t_atom *argv);

protected:

  //////////
  // Destructor
  virtual ~pix_blob();

  //////////
  // All we want is the pixel information, so this is a complete override.
  virtual void    processRGBAImage(imageStruct &image);
  virtual void    processGrayImage(imageStruct &image);
  virtual void    processYUVImage(imageStruct &image);

  //////////
  void            ChannelMess(int  channel);
  void            GainMess(int argc, t_atom *argv);

  //////////
  // The color outlet
  t_outlet        *m_xOut, *m_yOut;
  t_outlet        *m_zOut;

  //////////
  // user settings
  int              m_method;

  //////////
  //
  t_float          m_gain[4];

private:

  //////////
  // Static member callbacks
  static void gainMessCallback(void *dump, t_symbol*, int argc,
                               t_atom *argv);
  static void channelMessCallback(void *dump, t_float channel);
};

#endif  // for header file
