/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

Get pixel information

Copyright (c) 1997-1998 Mark Danks. mark@danks.org
Copyright (c) Günther Geiger. geiger@epy.co.at
Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
Copyright (c) 2002 James Tittle & Chris Clepper
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

/*-----------------------------------------------------------------
pix_dump

  0409:forum::für::umläute:2000
  IOhannes m zmoelnig
  mailto:zmoelnig@iem.at
-----------------------------------------------------------------*/

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

         dumps the pix-data as a float-package

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
