/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2000, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

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
  // set colorspace to RGBA, RGB or Grey
  void            csMess(std::string type);

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
  //////////
  // Set data type (BYTE, FLOAT, DOUBLE)
  void            typeMess(std::string type);


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
  // data type
  int             m_reqType;

  //////////
  // The pixBlock with the current image
  pixBlock        m_pixBlock;
  pixBlock     *m_pixels;
};

#endif  // for header file
