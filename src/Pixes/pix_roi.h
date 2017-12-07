/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    select a region of interest (ROI)

    Copyright (c) 2012 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef _INCLUDE__GEM_PIXES_PIX_ROI_H_
#define _INCLUDE__GEM_PIXES_PIX_ROI_H_

#include "Base/GemPixObj.h"
#include "Gem/Rectangle.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_roi

    set the region of interest

KEYWORDS
    pix

DESCRIPTION

-----------------------------------------------------------------*/

class GEM_EXTERN pix_roi : public GemPixObj
{
  CPPEXTERN_HEADER(pix_roi, GemPixObj);

public:
  //////////
  // Constructor/Destructor
  pix_roi(int argc, t_atom*argv);
  virtual ~pix_roi(void);

  //////////
  // Do the rendering
  virtual void     render(GemState *state);
  virtual void postrender(GemState *state);
  void roiMess(float x1, float y1, float x2, float y2);

protected:
  gem::Rectangle m_rectangle; /* the ROI as given from the user */
  gem::Rectangle
  m_staterect; /* this is the (normalized) rectangle we pass to state */
  gem::Rectangle*m_oldrect;   /* the rectangle we retrieved from upstream (to be restored on postrender */

  static int s_ID; /* the key to retrieve the ROI from the GemState */
};

#endif  // for header file
