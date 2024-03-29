/*-----------------------------------------------------------------
  LOG
  GEM - Graphics Environment for Multimedia

  this object work with gemlist exactly like pd [float] work with float

  Copyright (c) 1997-1999 Mark Danks. mark@danks.org
  Copyright (c) Günther Geiger. geiger@epy.co.at
  Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

  -----------------------------------------------------------------*/

#ifndef _INCLUDE__GEM_CONTROLS_GEMLIST_H_
#define _INCLUDE__GEM_CONTROLS_GEMLIST_H_

#include "Base/GemBase.h"
#include "Gem/State.h"

class GemCache;

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  gemlist

  DESCRIPTION

  "gemlist" - send the current gemlist when banged.
  The current gemlist can be set via the hot or cold inlet

  -----------------------------------------------------------------*/
class GEM_EXTERN gemlist : public GemBase
{
  CPPEXTERN_HEADER(gemlist, GemBase);

public:

  //////////
  // Constructor
  gemlist(void);

protected:

  //////////
  // Destructor
  virtual ~gemlist(void);

  //////////
  // Push the current state
  virtual void  render(GemState *state);

  //////////
  virtual void  postrender(GemState *state);

  //////////
  // send a valid state to the outlet
  void sendCacheState(GemCache*cache, GemState*state);

  //////////
  void                  trigger(void);
  virtual void rightRender(GemCache*cache, GemState *state);
  void         rightMess(t_symbol* s, int argc, t_atom *argv);

  virtual void ticktimeMess(t_float ticktime);
  virtual void lightingMess(bool state);
  virtual void drawMess(t_atom&arg);
  virtual void drawMess(t_symbol*s, int argc, t_atom*argv);


  //////////
  // current state
  GemState      m_state;
  bool          m_validState;


  float m_tickTime;
  bool m_lightState;
  GLenum m_drawType;

  GemState  m_mystate;
  GemCache* m_mycache;


  t_inlet   *m_inlet;
};

#endif  // for header file
