/*-----------------------------------------------------------------
  LOG
  GEM - Graphics Environment for Multimedia

  Set the velocity domain for particles

  Copyright (c) 1997-1999 Mark Danks. mark@danks.org
  Copyright (c) Günther Geiger. geiger@epy.co.at
  Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

  -----------------------------------------------------------------*/

#ifndef INCLUDE_PART_VELSPHERE_H_
#define INCLUDE_PART_VELSPHERE_H_

#include "papi.h"

#include "Base/GemBase.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS

  part_velsphere
    
  Set the velocity domain for particles

  DESCRIPTION

  -----------------------------------------------------------------*/
class GEM_EXTERN part_velsphere : public GemBase
{
  CPPEXTERN_HEADER(part_velsphere, GemBase)

    public:

  //////////
  // Constructor
  part_velsphere(t_floatarg val1, t_floatarg val2, t_floatarg val, t_floatarg radius);
    	
  //////////
  virtual void 	render(GemState *state);

 protected:
    	
  //////////
  // Destructor
  virtual ~part_velsphere();

  //////////
  void			vectorMess(float val1, float val2, float val3)
    { m_pos[0] = val1; m_pos[1] = val2; m_pos[2] = val3; }

  //////////
  void			numberMess(float num)	{ m_radius = num; }

  //////////
  float			m_pos[3];
  //////////
  float			m_radius;

 private:

  //////////
  // static member functions
  static void		numberMessCallback(void *data, t_floatarg num);
  static void		vectorMessCallback(void *data, t_floatarg num1, t_floatarg num2, t_floatarg num3);
};

#endif	// for header file
