/*-----------------------------------------------------------------
  LOG
  GEM - Graphics Environment for Multimedia

  Convert 4 signals (R~, G~, B~, A~) into a pixBuf

  Copyright (c) 2000 Günther Geiger. geiger@epy.co.at
  Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

  -----------------------------------------------------------------*/

#ifndef _INCLUDE__GEM_PIXES_PIX_SIG_PIX_H_
#define _INCLUDE__GEM_PIXES_PIX_SIG_PIX_H_

#include "Base/GemBase.h"
#include "Gem/Image.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  pix_sig2pix

  Interprete 4 signals as RGBA-Values of a pixel-image

  KEYWORDS
  pix

  DESCRIPTION

  "dimen" -- change pix_buf dimension
  4 inlets eat signals : R~, G~, B~, A~
  creation: width, height in pixels

  -----------------------------------------------------------------*/
class GEM_EXTERN pix_sig2pix : public GemBase
{
  CPPEXTERN_HEADER(pix_sig2pix, GemBase);

public:

  //////////
  // Constructor
  pix_sig2pix(t_floatarg width, t_floatarg height);

protected:

  //////////
  // Destructor
  virtual      ~pix_sig2pix();

  //////////

  // DSP perform
  void perform(t_sample**signals, size_t count);

  //////////
  // Do the rendering
  virtual void  render(GemState *state);

  //////////
  // Clear the dirty flag on the pixBlock
  virtual void  postrender(GemState *state);

  //////////
  virtual void  startRendering();

  //////////
  // setting dimension and colourspace
  virtual void  dimenMess(int w, int h);
  virtual void typeMess(std::string);

  virtual void  csMess(std::string);

  //////////
  // DSP-Message
  virtual void  dspMess(t_signal**);

  //-----------------------------------
  // GROUP:     Image data
  //-----------------------------------

  //////////
  // The pixBlock with the current image
  pixBlock      m_pixBlock;

  int           m_width, m_height;

  //////////////
  // which colorspace do we want ? currently only GL_RGBA
  int         m_reqFormat;
  unsigned int m_reqType;


  virtual void filltypeMess(std::string);
  enum  {CLEAR, FILL, LINE, WATERFALL} m_fillType;
  size_t m_offset;

  virtual void upsidedownMess(bool);
  bool m_upsidedown;
};

#endif  // for header file
