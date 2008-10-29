/*-----------------------------------------------------------------
  LOG
  GEM - Graphics Environment for Multimedia

  this object work with gemlist exactly like pd [float] work with float
    
  Copyright (c) 1997-1999 Mark Danks. mark@danks.org
  Copyright (c) Günther Geiger. geiger@epy.co.at
  Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

  -----------------------------------------------------------------*/

#ifndef INCLUDE_GEMLIST_H_
#define INCLUDE_GEMLIST_H_

#include "Base/GemBase.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  gemlist

  DESCRIPTION
    
  "gemlist" - send the curent gemlist when banged.
	The curent gemlist can be set via the hot or cold inlet
    
  -----------------------------------------------------------------*/
class GEM_EXTERN gemlist : public GemBase
{
  CPPEXTERN_HEADER(gemlist, GemBase)

    public:

  //////////
  // Constructor
  gemlist();
    	
 protected:
    	
  //////////
  // Destructor
  virtual ~gemlist();

  //////////
  // Push the current state
  virtual void 	render(GemState *state);

  //////////
  virtual void 	postrender(GemState *state);

  //////////
  // send a valid state to the outlet
  void sendCacheState(GemCache*cache, GemState*state);

  //////////
  void			trigger();

  //////////
  // curent state
  GemState* 	m_current_state;

  ////////
  bool		m_valide_state;

  //////////
  t_inlet         *m_inlet;

  virtual void   	rightRender(GemState *state);

 private:

    //////////
    // Static member callbacks
		static void		triggerMessCallback(void *data);
    static void 	gem_rightMessCallback(void *x, t_symbol *s, int argc, t_atom *argv);
};

#endif	// for header file
