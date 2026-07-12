/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2003, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

/*-----------------------------------------------------------------
  pix_tIIR

  time-domain filtering:
  apply a 2p2z-filter on a sequence of pixBlocks

  arguments: number of feedforward-taps, number of feedback-taps

  -----------------------------------------------------------------*/

#ifndef _INCLUDE__GEM_PIXES_PIX_TIIR_H_
#define _INCLUDE__GEM_PIXES_PIX_TIIR_H_

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  pix_tIIR

  KEYWORDS
  pix

  DESCRIPTION

  -----------------------------------------------------------------*/
class GEM_EXTERN pix_tIIR : public GemPixObj
{
  CPPEXTERN_HEADER(pix_tIIR, GemPixObj);

public:

  //////////
  // Constructor
  pix_tIIR(t_floatarg,t_floatarg);

protected:

  //////////
  // Destructor
  virtual ~pix_tIIR(void);

  //////////
  // Do the processing
  virtual void  processImage(imageStruct &image);

#ifdef __MMX__
  virtual void  processRGBAMMX(imageStruct &image);
  virtual void  processYUVMMX (imageStruct &image);
  virtual void  processGrayMMX(imageStruct &image);
#endif

  //////////
  // set-flag
  bool set;      // set the buffers
  bool set_zero; // and set them to zero

  //////////
  // the filter factors (feed-forward, feed-back)
  t_float *m_ff, *m_fb;
  int ff_count, fb_count;

  t_inlet **m_inlet;
  unsigned int m_inletCount;

  //////////
  // the image-latches
  imageStruct m_buffer;
  // how many images are stored in m_buffer ?
  int         m_bufnum;

  // which buffer-image is the current one ?
  int m_counter;

  //////////
  // the methods
  void setMess(t_symbol*,int,t_atom*);
};

#endif  // for header file
