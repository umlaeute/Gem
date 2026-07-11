/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_CONTROLS_GEMHEAD_H_
#define _INCLUDE__GEM_CONTROLS_GEMHEAD_H_

#include "Base/CPPExtern.h"
#include "gemreceive.h"

class GemState;
class GemCache;

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  gemhead

  The starting point for all graphics trees

  DESCRIPTION

  "bang" - sends out a state list

  -----------------------------------------------------------------*/
class GEM_EXTERN gemhead : public gemreceive
{
  CPPEXTERN_HEADER(gemhead, CPPExtern);

public:

  //////////
  // Constructor
  gemhead(int, t_atom*);

  //////////
  // A render message
  void          renderGL(GemState *state);

  //////////
  // Set up the modifying flags
  virtual void          startRendering();

  //////////
  // Clean up the modifying flags
  virtual void          stopRendering();


  //////////
  // change the priority
  void          setMess(t_float priority);
  void        setContext(const std::string&contextName);

  virtual void receive(t_symbol*s, int argc, t_atom*argv);

  /* make sur that the named context (of any) is valid */
  bool activateContext(void);

protected:

  //////////
  // Destructor
  virtual ~gemhead();

  std::string m_contextname;

private:

  GemCache      *m_cache;                   // The cache information

  void            outputRenderOnOff(int state);

  void          renderOnOff(int state);     // Turn rendering on and off
  int           m_renderOn;

  void          bangMess();

  bool m_contextActive; // whether our selected context is currently active
  t_symbol*m_contextsym;
};

#endif  // for header file
