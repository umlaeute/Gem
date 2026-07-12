/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, FUKUCHI Kentaro
 * SPDX-FileCopyrightText: © 2003, James Tittle II and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

/*-----------------------------------------------------------------

    random dot stereogram
    This object is an based on the RandomDotStereoTV effect from EffecTV
    Originally written by Fukuchi Kentaro
    ported by tigital@mac.com

-----------------------------------------------------------------*/

#ifndef _INCLUDE__GEM_PIXES_PIX_RDS_H_
#define _INCLUDE__GEM_PIXES_PIX_RDS_H_

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  pix_rds

  creates random dot stereogram from luminance

  KEYWORDS
  pix

  DESCRIPTION

   send a "toggle" to input for switching between methods
   method 0 = crosseyed
   method 1 = walleyed
  -----------------------------------------------------------------*/
class GEM_EXTERN pix_rds : public GemPixObj
{
  CPPEXTERN_HEADER(pix_rds, GemPixObj);

public:

  //////////
  // Constructor
  pix_rds(t_floatarg);

protected:

  //////////
  // Destructor
  virtual ~pix_rds();

  //////////
  // Do the processing
  virtual void processRGBAImage(imageStruct &image);
  virtual void processGrayImage(imageStruct &image);
  virtual void processYUVImage(imageStruct &image);

  imageStruct    myImage;
  int   doDots;
  int   stride;
  int   method;
  int   fastrand_val;

private:

  //////////
  // static member functions
  static void methMessCallback(void *data, t_float state);
  static void strideMessCallback(void *data, t_float state);
  static void seedMessCallback(void *data, t_float state);
};

#endif  // for header file
