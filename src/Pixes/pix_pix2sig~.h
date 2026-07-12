/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2000, Günther Geiger
 * SPDX-FileCopyrightText: © 2001, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

/*-----------------------------------------------------------------

  Output a pixel-buffer as 4 signals

  -----------------------------------------------------------------*/

#ifndef _INCLUDE__GEM_PIXES_PIX_PIX_SIG_H_
#define _INCLUDE__GEM_PIXES_PIX_PIX_SIG_H_

#include "Base/GemBase.h"
#include "Gem/Image.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  pix_pix2sig

  Interprete a pixel-image as 4 RGBA-signals

  KEYWORDS
  pix

  DESCRIPTION

  4 outlets with signals : R~, G~, B~, A~

  -----------------------------------------------------------------*/
class GEM_EXTERN pix_pix2sig : public GemBase
{
  CPPEXTERN_HEADER(pix_pix2sig, GemBase);

public:

  //////////
  // Constructor
  pix_pix2sig();

protected:

  //////////
  // Destructor
  virtual      ~pix_pix2sig();

  //////////
  // DSP perform
  void perform(t_sample** signals, size_t n);

  //////////
  // processImage
  virtual void  render(GemState*);

  //////////
  // DSP-Message
  virtual void  dspMess(t_signal** sp);

  //-----------------------------------
  // GROUP:     Image data
  //-----------------------------------

  //////////
  // the current image
  imageStruct    m_image;

  //////////
  // the outlets~
  t_outlet *o_col[4];

  /* current read position within the image (internal state) */
  size_t m_offsetX, m_offsetY;

  virtual void filltypeMess(t_symbol*, int, t_atom*);
  typedef enum {CLEAR, FILL, LINE, WATERFALL, INVALID}  filltype_t;
  filltype_t m_fillType;
  int m_line; /* desired line (for waterfall mode) */
};

#endif  // for header file
