/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_PIXES_PIX_ZOOM_H_
#define _INCLUDE__GEM_PIXES_PIX_ZOOM_H_

#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_zoom

    Change the pixel zooming for glDrawPixels

KEYWORDS
    pix

DESCRIPTION

    Inlet for float, float - "zoom"

    "zoom" - the x and y mag

-----------------------------------------------------------------*/
class GEM_EXTERN pix_zoom : public GemBase
{
  CPPEXTERN_HEADER(pix_zoom, GemBase);

public:

  //////////
  // Constructor
  pix_zoom(int, t_atom*);

protected:

  //////////
  // Destructor
  virtual ~pix_zoom();

  //////////
  // Do the rendering
  virtual void    render(GemState *state);

  //////////
  // Turn back on pix_zoom test
  virtual void    postrender(GemState *state);

  //////////
  // x & y zoom
  t_float           m_xZoom;
  t_float           m_yZoom;

  //////////
  // Zoom mag
  void            zoomMess(t_float xMag, t_float yMag);

private:
};

#endif  // for header file
