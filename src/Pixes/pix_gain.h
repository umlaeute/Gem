/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_PIXES_PIX_GAIN_H_
#define _INCLUDE__GEM_PIXES_PIX_GAIN_H_

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_gain

    Change the overall gain of a pix

KEYWORDS
    pix

DESCRIPTION

    Inlet for a list - "vec_gain"
    Inlet for a float - "ft1"

    "vec_gain" - The gain vector to set to
    "ft1" - The gain amount (all gains are equal)

-----------------------------------------------------------------*/
class GEM_EXTERN pix_gain : public GemPixObj
{
  CPPEXTERN_HEADER(pix_gain, GemPixObj);

public:

  //////////
  // Constructor
  pix_gain(int,t_atom*);

protected:

  //////////
  // Destructor
  virtual ~pix_gain();

  //////////
  // Do the processing
  virtual void    processRGBAImage(imageStruct &image);
  virtual void    processGrayImage(imageStruct &image);
  virtual void    processYUVImage(imageStruct &image);

  virtual void    processFloat32(imageStruct &image);

#ifdef __MMX__
  //////////
  // MMX
  virtual void    processRGBAMMX(imageStruct &image);
#endif
#ifdef __VEC__
  //////////
  // altivec
  virtual void    processYUVAltivec(imageStruct &image);
#endif
  //////////
  // Set the new gain
  void            vecGainMess(int argc, t_atom *argv);

  //////////
  // Set the new gain
  void            floatGainMess(float gain);

  //////////
  // The new gain
  float                   m_gain[4];

  //////////
  // Set the new gain
  void            saturateMess(int sat);

  //////////
  bool  m_saturate;

private:

  //////////
  // Static member functions
  static void     vecGainMessCallback(void *data, t_symbol*, int argc,
                                      t_atom *argv);
  static void     floatGainMessCallback(void *data, t_float gain);
  static void     saturateMessCallback(void *data, t_float saturate);
};

#endif  // for header file
