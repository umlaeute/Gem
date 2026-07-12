/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2008, Jaime Oliver and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

////////////////////////////////////////////////////////
//
// pix_mano - an object to track a hand and its fingers
//
// Jaime Oliver,
//
// jaime.oliver2@gmail.com
// this is still a testing version, no guarantees...
//
// for more information: www.jaimeoliver.pe
// Silent Percussion Project
//
/////////////////////////////////////////////////////////

#ifndef INCLUDE_pix_mano_H_
#define INCLUDE_pix_mano_H_

#include "Base/GemPixObj.h"
#include "Utils/Functions.h"

class GEM_EXPORT pix_mano : public GemPixObj
{
  CPPEXTERN_HEADER(pix_mano, GemPixObj)

public:

  //////////
  // Constructor
  pix_mano();

protected:

  //////////
  // Destructor
  virtual ~pix_mano();

  //////////
  // Do the processing
  virtual void 	processGrayImage(imageStruct &image);

  //////////
  // Set the new threshold vector
  void	    	vecBoundsMess(t_symbol*,int argc, t_atom *argv);

  //////////
  // Set the new threshold value
  void	    	vecThreshMess(t_symbol*,int argc, t_atom *argv);

  //////////
  // Set the new threshold value
  void	    	vecParamsMess(t_symbol*,int argc, t_atom *argv);

  //////////
  // The new color
  t_outlet *outlet1;
  t_outlet *outlet2;
  t_outlet *outlet3;
  t_outlet *outlet4;
  t_outlet *outlet5;
  t_outlet *outlet6;
  t_outlet *outlet7;
  int head, bottom, mode, left, right, pixtip, min_entry_size, min_perim, pixsamp;
  unsigned int pixavg;
  float thresh, tip_scalar;
  float partialx_prev[10000], partialy_prev[10000];
  int prev_tip;
  float tp_i[50], tp_x[50], tp_y[50], tp_m[50], tp_a[50], tp_s[50];

private:
  class PIMPL;
  PIMPL*m_pimpl;
};

#endif	// for header file
