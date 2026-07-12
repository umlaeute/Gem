/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2005, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_PIXES_PIX_THRESHOLD_BERNSEN_H_
#define _INCLUDE__GEM_PIXES_PIX_THRESHOLD_BERNSEN_H_

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_threshold_bernsen

    binarizing adaptive tiled thresholder

KEYWORDS
    pix

DESCRIPTION

    Inlet for a list - "vec_thresh"
    Inlet for a float - "ft1"

    "vec_thresh" - The threshold_bernsen vector
    "ft1" - Set all threshold_bernsens to one value

-----------------------------------------------------------------*/
class GEM_EXTERN pix_threshold_bernsen : public GemPixObj
{
  CPPEXTERN_HEADER(pix_threshold_bernsen, GemPixObj);

public:

  //////////
  // Constructor
  pix_threshold_bernsen();

protected:

  //////////
  // Destructor
  virtual ~pix_threshold_bernsen();

  //////////
  // Do the processing
  static void processGraySub_getMinMax(imageStruct&image,
                                       int fromX, int toX,
                                       int fromY, int toY,
                                       unsigned char*resultMin,
                                       unsigned char*resultMax);
  virtual void    processGrayImage(imageStruct &image);

  //////////
  // set the number of tiles
  void            tilesMess(int w, int h);
  void            contrastMess(int c);

  int m_xtiles, m_ytiles;

  int m_contrast;

  unsigned char*m_minVals;
  unsigned char*m_maxVals;


private:

  //////////
  // Static member functions
  static void     tilesMessCallback(void *data, t_float w, t_float h);
  static void     contrastMessCallback(void *data, t_float c);
};

#endif  // for header file
