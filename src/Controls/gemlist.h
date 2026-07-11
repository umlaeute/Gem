/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2008, Cyrille Henry and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

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
