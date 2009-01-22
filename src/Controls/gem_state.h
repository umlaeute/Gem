/*-----------------------------------------------------------------
  LOG
  GEM - Graphics Environment for Multimedia

  this object resets the gem_state
    
  Copyright (c) 2009 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

  -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_STATE_H_
#define INCLUDE_GEM_STATE_H_

#include "Base/GemBase.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  gem_state

  DESCRIPTION
    
  "gem_state" - reset the Gem->State
    
  -----------------------------------------------------------------*/
class GEM_EXTERN gem_state : public GemBase
{
  CPPEXTERN_HEADER(gem_state, GemBase)

    public:

  //////////
  // Constructor
  gem_state();
    	
 protected:
    	
  //////////
  // Destructor
  virtual ~gem_state();

  //////////
  // Push the current state
  virtual void 	render(GemState *state);

  //////////
  virtual void 	postrender(GemState *state);

  //////////
  // curent state
  GemCache *m_newcache, *m_oldcache;

  virtual void profileMess(int state);
  int m_profiling;

 private:

  static void 	profileMessCallback(void *data, t_floatarg n);

};

#endif	// for header file
