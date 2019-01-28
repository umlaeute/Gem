/*-----------------------------------------------------------------

GEM - Graphics Environment for Multimedia

Load an GLSL geometry shader

 *  Created by tigital on 10/04/2005.
 *  Copyright 2005 James Tittle.

Copyright (c) 1997-1999 Mark Danks. mark@danks.org
Copyright (c) Günther Geiger. geiger@epy.co.at
Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef _INCLUDE__GEM_MANIPS_GLSL_GEOMETRY_H_
#define _INCLUDE__GEM_MANIPS_GLSL_GEOMETRY_H_

#include "Manips/glsl_vertex.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  glsl_geometry

  Loads in a geometry shader

  KEYWORDS

  DESCRIPTION

  -----------------------------------------------------------------*/
class GEM_EXTERN glsl_geometry : public glsl_vertex
{
  CPPEXTERN_HEADER(glsl_geometry, glsl_vertex);

public:

  //////////
  // Constructor
  glsl_geometry(void);
  glsl_geometry(t_symbol *filename);

protected:

  //////////
  // Destructor
  virtual ~glsl_geometry();

  ////////
  // extension check
  virtual bool isRunnable(void);

  //////////
  // Print Info about Hardware limits
  virtual void printInfo();
};

#endif  // for header file
