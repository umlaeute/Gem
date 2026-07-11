/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2000, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

/////////////////////////////////////////////////////////
// added the gluDisk
/////////////////////////////////////////////////////////

#ifndef _INCLUDE__GEM_GEOS_DISK_H_
#define _INCLUDE__GEM_GEOS_DISK_H_

#include "Base/GemGluObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
        disk

    Creates a disk

KEYWORD
    geo

DESCRIPTION

-----------------------------------------------------------------*/
class GEM_EXTERN disk : public GemGluObj
{
  CPPEXTERN_HEADER(disk, GemGluObj);

public:

  //////////
  // Constructor
  disk(int argc, t_atom *argv);

protected:

  //////////
  // Destructor
  virtual ~disk(void);

  //////////
  // Do the rendering
  virtual void    renderShape(GemState *state);

  //////////
  // Set the inner radius
  void                      innerRadius(float radius);

  //////////
  // Inner radius of the disk
  float                     m_innerRadius;
};

#endif  // for header file
