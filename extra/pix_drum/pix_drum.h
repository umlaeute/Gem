/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2008, Jaime Oliver and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef INCLUDE_pix_drum_H_
#define INCLUDE_pix_drum_H_

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_drum

    Clamp pixel values to a threshold

KEYWORDS
    pix

DESCRIPTION

    Inlet for a list - "vec_thresh"
    Inlet for a float - "ft1"

    "vec_thresh" - The threshold vector
    "ft1" - Set all thresholds to one value

-----------------------------------------------------------------*/
class GEM_EXPORT pix_drum : public GemPixObj
{
  CPPEXTERN_HEADER(pix_drum, GemPixObj)

public:

  //////////
  // Constructor
  pix_drum();

protected:

  //////////
  // Destructor
  virtual ~pix_drum();
  virtual void 	processGrayImage(imageStruct &image);

  void	    	vecBoundsMess(t_symbol*,int argc, t_atom *argv);
  void	    	vecThreshMess(t_symbol*,int argc, t_atom *argv);

  t_outlet *outlet1;
  t_outlet *outlet2;
  t_outlet *outlet3;
  t_outlet *outlet4;
  unsigned char  	m_thresh[4];
  int head, bottom, right, left, mode, pix_dist, min_width, pix_dist_ctr;
  float thresh, min_height;

private:

};

#endif	// for header file
