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

#ifndef INCLUDE_PART_VELOCITY_H_
#define INCLUDE_PART_VELOCITY_H_

#include "papi.h"

#include "Base/GemBase.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS

  part_velocity
    
  Set the velocity domain for particles

  DESCRIPTION

  -----------------------------------------------------------------*/
class GEM_EXTERN part_velocity : public GemBase
{
  CPPEXTERN_HEADER(part_velocity, GemBase)

    public:

  //////////
  // Constructor
  part_velocity(int,t_atom*);
    	
  //////////
  virtual void 	render(GemState *state);

 protected:
    	
  //////////
  // Destructor
  virtual ~part_velocity();

  //////////
  void		vectorMess(int argc, t_atom*argv);
  void		domainMess(t_symbol*s);

  //////////
  float		m_arg[9];
  PDomainEnum	m_domain;

 private:

  //////////
  // static member functions
  static void	domainMessCallback(void *data, t_symbol*s);
  static void	vectorMessCallback(void *data, t_symbol*, int, t_atom*);
};

#endif	// for header file
