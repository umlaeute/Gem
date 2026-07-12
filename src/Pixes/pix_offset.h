/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_PIXES_PIX_OFFSET_H_
#define _INCLUDE__GEM_PIXES_PIX_OFFSET_H_

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_offset

    add offset-values to the RGBA-Channels (without clamping to 0..255)

KEYWORDS
    pix

DESCRIPTION

    Inlet for a list - "vec_offset"
    Inlet for a float - "ft1"

    "vec_offset" - The offset vector to set to
    "ft1" - The offset amount (all offsets are equal)

-----------------------------------------------------------------*/
class GEM_EXTERN pix_offset : public GemPixObj
{
  CPPEXTERN_HEADER(pix_offset, GemPixObj);

public:

  //////////
  // Constructor
  pix_offset();

protected:

  //////////
  // Destructor
  virtual ~pix_offset();

  //////////
  // Do the processing
  virtual void    processRGBAImage(imageStruct &image);
  virtual void    processGrayImage(imageStruct &image);
  virtual void    processYUVImage(imageStruct &image);
#ifdef __MMX__
  //////////
  // MMX
  virtual void    processRGBAMMX(imageStruct &image);
  virtual void    processYUVMMX(imageStruct &image);
  virtual void    processGrayMMX(imageStruct &image);
#endif
#ifdef __VEC__
  //////////
  // altivec
  virtual void    processYUVAltivec(imageStruct &image);
#endif

  //////////
  // Set the new offset
  void            vecOffsetMess(int argc, t_atom *argv);

  //////////
  // Set the new offset
  void            floatOffsetMess(float foffset);

  //////////
  // The new offset
  unsigned char   m_offset[4];
  short Y,U,V;

  //////////
  // whether we want saturated logic
  void            saturateMess(int sat);
  bool  m_saturate;

private:

  //////////
  // Static member functions
  static void     vecOffsetMessCallback(void *data, t_symbol*, int argc,
                                        t_atom *argv);
  static void     floatOffsetMessCallback(void *data, t_float offset);
  static void     saturateMessCallback(void *data, t_float saturate);
};

#endif  // for header file
