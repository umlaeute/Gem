/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * this file has been generated...
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GLDEFINE_H_
#define _INCLUDE__GEM_OPENGL_GLDEFINE_H_

#include "Base/CPPExtern.h"
#include "Utils/GLUtil.h"


/*
 CLASS
        GLdefine
 KEYWORDS
        openGL  0
 DESCRIPTION
        wrapper for the openGL defines
 */

class GEM_EXTERN GLdefine : public CPPExtern
{
  CPPEXTERN_HEADER(GLdefine, CPPExtern);

public:
  // Constructor
  GLdefine (int, t_atom*);      // CON

protected:
  // Destructor
  virtual ~GLdefine ();
  // Do the rendering

  int     m_argc;
  t_atom *m_argv;

  virtual void  symMess(t_symbol*);     // FUN
  virtual void bangMess(void);  // FUN
  virtual void listMess(int,t_atom*);   // FUN


private:
  t_outlet *m_outlet;

  // static member functions
  static void    symMessCallback (void*, t_symbol*);
  static void    anyMessCallback (void*, t_symbol*, int, t_atom*);
  static void    bangMessCallback (void*);
  static void    listMessCallback (void*, t_symbol*, int, t_atom*);
};
#endif // for header file
