/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2000, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

/*-----------------------------------------------------------------

pix_rtx

  rtx-transform a series of pictures (especially movies, videos,...)
  you have to use pix_film instead of pix_movie to make this work with prerecorded videos

  2803:forum::für::umläute:2000
  0409:forum::für::umläute:2000
  1801:forum::für::umläute:2001  added the second mode

-----------------------------------------------------------------*/

#ifndef _INCLUDE__GEM_PIXES_PIX_RTX_H_
#define _INCLUDE__GEM_PIXES_PIX_RTX_H_

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
pix_rtx

  KEYWORDS
  pix

        DESCRIPTION

-----------------------------------------------------------------*/
class GEM_EXTERN pix_rtx : public GemPixObj
{
  CPPEXTERN_HEADER(pix_rtx, GemPixObj);

public:

  //////////
  // Constructor
  pix_rtx();

protected:

  //////////
  // Destructor
  virtual ~pix_rtx();

  //////////
  // Do the processing
  virtual void    processImage(imageStruct &image);

  //////////
  // the huge double buffer and other tx-formation
  imageStruct    buffer;
  int            bufcount;    // where to read/write

  //////////
  // the rtx-mode
  bool                     mode;

  //////////
  // fill buffer with current pixbuff ?
  bool                     set_buffer;

  //////////
  // the methods
  void modeMess(int newmode);
  void clearMess(void);
  void setMess(void);

};

#endif  // for header file
