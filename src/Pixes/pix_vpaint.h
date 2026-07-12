/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2005, James Tittle II and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_PIXES_PIX_VPAINT_H_
#define _INCLUDE__GEM_PIXES_PIX_VPAINT_H_

#include "Base/GemPixObj.h"
#include "Gem/Image.h"
#include "Gem/PBuffer.h"
#include "Utils/GemMath.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_vpaint

    Creates voronoi regions based on pixel color

KEYWORDS
    pix

DESCRIPTION

    Inlet for a list - "vert_size"

-----------------------------------------------------------------*/
class GEM_EXTERN pix_vpaint : public GemPixObj
{
  CPPEXTERN_HEADER(pix_vpaint, GemPixObj);

public:

  //////////
  // Constructor
  pix_vpaint();

protected:

  //////////
  // Destructor
  virtual ~pix_vpaint();

  // extension check
  bool isRunnable(void);

  //////////
  // Do the processing
  virtual void    processImage(imageStruct &image);

  //////////
  // When a size message is received
  virtual void    sizeMess(int width, int height);


  //////////
  //
  void    makepoints();
  void    makecone();
  void    init();
  int             m_initialized;
  int             maxPoints, numPoints, viewImage, useStrokes, drawEdges,
                  moving, canDrawEdges;

  /*
  * Point structure
  */
  typedef struct {
    int x, y;
    unsigned char r, g, b;
  } cPoint;
  cPoint *points;

  //////////
  // The imageStruct with the current image
  imageStruct     m_imageStruct;
  //////////
  // PBuffer
  PBuffer         *m_pbuffer;
  //////////
  // The x position
  int             m_x;

  //////////
  // The y position
  int             m_y;

  //////////
  // The width
  int             m_w;

  //////////
  // The height
  int             m_h;

  //////////
  // recalc the random points
  bool            m_banged;

private:

  t_inlet *m_sizinlet;

  //////////
  // static member functions
  static void     bangMessCallback(void *data);
  static void         sizeMessCallback(void *data, t_float width,
                                       t_float height );
};

#endif  // for header file
