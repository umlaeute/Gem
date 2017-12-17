/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    Copyright (c) 2003 James Tittle
    ported from pete's_plugins (www.petewarden.com)
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef _INCLUDE__GEM_PIXES_PIX_LUMAOFFSET_H_
#define _INCLUDE__GEM_PIXES_PIX_LUMAOFFSET_H_

#include "Base/GemPixObj.h"
/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_lumaoffset



KEYWORDS
    pix

DESCRIPTION



-----------------------------------------------------------------*/
class GEM_EXTERN pix_lumaoffset : public GemPixObj
{
  CPPEXTERN_HEADER(pix_lumaoffset, GemPixObj);

public:

  //////////
  // Constructor
  pix_lumaoffset();

protected:

  //////////
  // Destructor
  virtual ~pix_lumaoffset();

  //////////
  // Do the processing
  virtual void    processRGBAImage(imageStruct &image);
  virtual void    processYUVImage(imageStruct &image);
  virtual void    processGrayImage(imageStruct &image);

  imageStruct     myImage;
  int             init;

  float   m_OffsetScale;
  float   m_LineGap;
  bool    m_DoFilledLines;
  bool    m_DoSmoothFill;

  SPete_MemHandle         hPreviousLineHeights;
  SPete_MemHandle         hPreviousLineHeights2;
  int hPreviousLineHeights_size;
private:

  //////////
  // Static member functions
  static void     offsetCallback(void *data, t_float m_OffsetScale);
  static void     gapCallback(void *data, t_float m_LineGap);
  static void     fillCallback(void *data, t_float m_DoFilledLines);
  static void     smoothCallback(void *data, t_float m_DoSmoothFill);
};

#endif  // for header file
