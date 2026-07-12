/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2000, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_PIXES_PIX_DUMP_H_
#define _INCLUDE__GEM_PIXES_PIX_DUMP_H_

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS

 pix_dump

  Get pixel information

 KEYWORDS

  pix

 DESCRIPTION

  dumps the pix-data as a list of floats

-----------------------------------------------------------------*/
class GEM_EXTERN pix_dump : public GemPixObj
{
  CPPEXTERN_HEADER(pix_dump, GemPixObj);

public:

  //////////
  // Constructor
  pix_dump();

protected:

  //////////
  // Destructor
  virtual ~pix_dump();

  //////////
  // All we want is the pixel information, so this is a complete override.
  virtual void processImage(imageStruct &image);
  virtual void processFloat32(imageStruct &image);
  virtual void processFloat64(imageStruct &image);

  //////////
  void trigger();
  void bytemodeMess(bool);
  void modeMess(std::string);

  //////////
  // Set to RGBA-mode
  void RGBAMess(void);
  //////////
  // Set to RGB-mode
  void RGBMess(void);

  //////////
  // The color outlet
  t_outlet     *m_dataOut;

  //////////
  // the buffer
  imageStruct   m_image;
  unsigned int  m_bufsize;
  t_atom       *m_buffer;
  // whether we output byte values (0..255) or normalized values (0..1.f)
  // defaults to FALSE
  bool m_bytemode;

  ////////
  // dump mode
  int           m_mode;
};

#endif  // for header file
