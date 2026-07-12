/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_MANIPS_ALPHA_H_
#define _INCLUDE__GEM_MANIPS_ALPHA_H_

#include "Base/GemBase.h"
#include "Gem/GemGL.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    alpha

    Turn on alpha blending

DESCRIPTION

    "alphastate" - whether to use alpha blending

-----------------------------------------------------------------*/
class GEM_EXTERN alpha : public GemBase
{
  CPPEXTERN_HEADER(alpha, GemBase);

public:

  //////////
  // Constructor
  alpha(t_floatarg);

protected:

  //////////
  // Destructor
  virtual ~alpha();

  //////////
  // Do the rendering
  virtual void    render(GemState *state);

  //////////
  // Turn off alpha blending
  virtual void    postrender(GemState *state);

  //////////
  // alpha blending state
  int                 m_alphaState;

  //////////
  // alpha test state
  int                 m_alphaTest;

  /////////
  // depthtest
  int                 m_depthtest;

  //////////
  // the blending function
  GLenum              m_function;
  void            funMess(int fun);


  //////////
  // Alpha state changed
  void            alphaMess(int alphaState);

  //////////
  // Alpha test changed
  void            testMess(int alphaTest);

  //////////
  // Disable Depthtest
  void            depthtestMess(int i);

  t_inlet *m_inlet;
};

#endif  // for header file
