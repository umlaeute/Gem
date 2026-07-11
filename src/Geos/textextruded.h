/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2003, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_GEOS_TEXTEXTRUDED_H_
#define _INCLUDE__GEM_GEOS_TEXTEXTRUDED_H_

#include "Base/TextBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    textextruded

    Creates a textextruded string

DESCRIPTION

-----------------------------------------------------------------*/
class GEM_EXTERN textextruded : public TextBase
{
  CPPEXTERN_HEADER(textextruded, TextBase);

public:

  //////////
  // Constructor
  textextruded(int argc, t_atom *argv);

protected:

  //////////
  // Destructor
  virtual ~textextruded();

#ifdef FTGL
  virtual FTFont*makeFont(const char*fontname);
#else
  virtual void    destroyFont() {}
  virtual int     makeFontFromFace()
  {
    return 0;
  }
  virtual void    render(GemState*) {}
#endif

  //////////
  // Set the precision for rendering
  virtual void        setDepth(float depth);
};

#endif  // for header file
