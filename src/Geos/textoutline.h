/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_GEOS_TEXTOUTLINE_H_
#define _INCLUDE__GEM_GEOS_TEXTOUTLINE_H_

#include "Base/TextBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    textoutline

    Creates a textoutline string

DESCRIPTION

-----------------------------------------------------------------*/
class GEM_EXTERN textoutline : public TextBase
{
  CPPEXTERN_HEADER(textoutline, TextBase);

public:

  //////////
  // Constructor
  textoutline(int argc, t_atom *argv);

protected:

  //////////
  // Destructor
  virtual ~textoutline();

#ifdef FTGL
  virtual FTFont*makeFont(const char*fontname);
#endif
};

#endif  // for header file
