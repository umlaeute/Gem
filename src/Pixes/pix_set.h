/*-----------------------------------------------------------------

 GEM - Graphics Environment for Multimedia

 interprete a (long) list of floats as a pixBlock

 Copyright (c) 1997-1999 Mark Danks. mark@danks.org
 Copyright (c) Günther Geiger. geiger@epy.co.at
 Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
 For information on usage and redistribution, and for a DISCLAIMER OF ALL
 WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

/*-----------------------------------------------------------------
pix_set

  0409:forum::für::umläute:2000
  IOhannes m zmoelnig
  mailto:zmoelnig@iem.kug.ac.at
-----------------------------------------------------------------*/

#ifndef _INCLUDE__GEM_PIXES_PIX_SET_H_
#define _INCLUDE__GEM_PIXES_PIX_SET_H_

#include "Base/GemPixObj.h"

#include "Base/GemBase.h"
#include "Gem/Image.h"
#include "Gem/Cache.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
pix_set

 set the pixels via a float-package

  KEYWORDS
  pix

   DESCRIPTION

    "RGB"
    "RGBA"
    "GREY"

-----------------------------------------------------------------*/

class GEM_EXTERN pix_set : public GemPixObj
{
  CPPEXTERN_HEADER(pix_set, GemPixObj);

public:

  //////////
  // Constructor
  pix_set(t_floatarg xsize, t_floatarg ysize);

protected:

  //////////
  // Destructor
  virtual ~pix_set(void);

  //////////
  // Do the rendering
  virtual void    render(GemState *state);

  //////////
  // Clear the dirty flag on the pixBlock
  virtual void    postrender(GemState *state);

  //////////
  virtual void    startRendering(void);

  //////////
  // Clean up the pixBlock
  void            cleanPixBlock(void);

  //////////
  // Set to RGBA-mode
  void            RGBAMess(void);
  //////////
  // Set to RGB-mode
  void            RGBMess(void);
  //////////
  // Set to GREYSCALE-mode
  void            GREYMess(void);
  //////////
  // Set a new image size
  void            SETMess(int xsize, int ysize);
  //////////
  // Set ROI offset
  void            ROIOFFSETMess(int xsize, int ysize);
  //////////
  // Set ROI size
  void            ROISIZEMess(int xsize, int ysize);
  // Set clear the whole image
  void            FILLMess(t_symbol*s, int argc, t_atom *argv);
  // Bang message
  void            BANGMess(void);
  //////////
  // Pass the data
  void            DATAMess(t_symbol*s, int argc, t_atom *argv);
  //////////
  // set input data mode
  void bytemodeMess(bool);


  //-----------------------------------
  // GROUP:       Paint data
  //-----------------------------------

  //////////
  // paint mode
  int             m_mode;
  /////////
  // input data scaling
  float m_inputScale;

  //////////
  // The pixBlock with the current image
  pixBlock        m_pixBlock;
  pixBlock     *m_pixels;
};

#endif  // for header file
