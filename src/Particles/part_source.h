/*-----------------------------------------------------------------
  LOG
  GEM - Graphics Environment for Multimedia

  Generate particles

  Copyright (c) 1997-1999 Mark Danks. mark@danks.org
  Copyright (c) Günther Geiger. geiger@epy.co.at
  Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

  -----------------------------------------------------------------*/

#ifndef INCLUDE_PART_SOURCE_H_
#define INCLUDE_PART_SOURCE_H_

#include "Base/GemBase.h"
#include "papi.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS

  part_source
    
  Generate particles

  DESCRIPTION

  -----------------------------------------------------------------*/
class GEM_EXTERN part_source : public GemBase
{
  CPPEXTERN_HEADER(part_source, GemBase)

    public:

  //////////
  // Constructor
  part_source(t_floatarg num);
    	
  //////////
  virtual void 	render(GemState *state);

 protected:
    	
  //////////
  // Destructor
  virtual ~part_source();
		
  //////////
  // Number to add
  void			numberMess(int num)		{ m_numberToAdd = num; }
  int			m_numberToAdd;

  //////////
  // vel.domain
  PDomainEnum           m_domain;
  void                  domainMess(t_symbol *s);
  float			m_arg[9];
  void			vectorMess(int argc, t_atom*argv);
	
 private:

  //////////
  // static member functions
  static void		numberMessCallback(void *data, t_floatarg num);
  static void		domainMessCallback(void *data, t_symbol*s);
  static void		vectorMessCallback(void *data, t_symbol*, int, t_atom*);
};

#endif	// for header file
