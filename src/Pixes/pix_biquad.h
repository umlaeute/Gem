/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2003, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_PIXES_PIX_BIQUAD_H_
#define _INCLUDE__GEM_PIXES_PIX_BIQUAD_H_

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  pix_biquad

  KEYWORDS
  pix

  DESCRIPTION
  apply a 2p2z-filter on a sequence of pixBlocks

  -----------------------------------------------------------------*/
class GEM_EXTERN pix_biquad : public GemPixObj
{
  CPPEXTERN_HEADER(pix_biquad, GemPixObj);

public:

  //////////
  // Constructor
  pix_biquad(int, t_atom*);

protected:

  //////////
  // Destructor
  virtual ~pix_biquad();

  //////////
  // Do the processing
  virtual void  processRGBAImage(imageStruct &image);
  virtual void  processYUVImage(imageStruct &image);
#ifdef __MMX__
  virtual void  processRGBAMMX(imageStruct &image);
  virtual void  processYUVMMX (imageStruct &image);
  virtual void  processGrayMMX(imageStruct &image);
#endif
#ifdef __VEC__
  virtual void  processYUVAltivec(imageStruct &image);
#endif

  //////////
  // the methods
  void setMess(void);
  void modeMess(int mode);
  void faktorMess(t_symbol*, int, t_atom*);

  //////////
  // the image-latches
  imageStruct  prev;
  imageStruct    last;

  //////////
  // set-flag: if "set", the buffers (prev&last) are set to the current image
  bool set;

  //////////
  // the biquad-factors
  t_float fb0, fb1, fb2, ff1, ff2, ff3;

  // 0..integer-processing(fast) [default]
  // 1..float-processing(slow)
  int m_mode;


};

#endif  // for header file
