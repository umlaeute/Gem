/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2004, James Tittle II and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_MANIPS_FRAGMENT_PROGRAM_H_
#define _INCLUDE__GEM_MANIPS_FRAGMENT_PROGRAM_H_

#include "Manips/vertex_program.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  fragment_program

  Loads in an ARB fragment program/shader

  KEYWORDS


  DESCRIPTION

  -----------------------------------------------------------------*/
class GEM_EXTERN fragment_program : public vertex_program
{
  CPPEXTERN_HEADER(fragment_program, vertex_program);

public:

  //////////
  // Constructor
  fragment_program(t_symbol* filename);

protected:

  //////////
  // Destructor
  virtual ~fragment_program(void);

  //////////
  // which Program do we have (ARB, NV,...)
  virtual GLint queryProgramtype(char*program);

  //////////
  // Print Info about Hardware limits
  virtual void printInfo(void);

  //////////
  // extension check
  virtual bool isRunnable(void);
};

#endif  // for header file
