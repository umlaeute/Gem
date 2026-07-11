/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * this file has been generated...
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLREPORTERROR_H_
#define _INCLUDE__GEM_OPENGL_GEMGLREPORTERROR_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglReportError
 KEYWORDS
        openGL  0
 DESCRIPTION
        wrapper for the openGL defines
 */

class GEM_EXTERN GEMglReportError : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglReportError, GemGLBase);

public:
  // Constructor
  GEMglReportError (void);      // CON

protected:
  // Destructor
  virtual ~GEMglReportError ();
  // Do the rendering

  virtual void render(GemState *state);


private:
  t_outlet *m_outlet;

  // static member functions
  static void    bangMessCallback (void*);
};
#endif // for header file
