/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

	Draw a part_info group

    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyright (c) G�nther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f�r::uml�ute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_PART_INFO_H_
#define INCLUDE_PART_INFO_H_

#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS

	part_info
    
	Draw a part_info group

DESCRIPTION

-----------------------------------------------------------------*/
class GEM_EXTERN part_info : public GemBase
{
  CPPEXTERN_HEADER(part_info, GemBase)

    public:

  //////////
  // Constructor
  part_info();
    	
  //////////
  virtual void 	render(GemState *state);

 protected:
    	
  //////////
  // Destructor
  virtual ~part_info();

  // How the object should be drawn
  float        *m_pos;
  float        *m_colors;
  float        *m_sizes;
  float        *m_velo;
  float        *m_ages;

  int           m_number;

  t_outlet *out_num, *out_pos, *out_col, *out_vel, *out_siz, *out_age;
  t_atom m_alist[13];
};

#endif	// for header file
