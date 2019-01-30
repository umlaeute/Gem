/*-----------------------------------------------------------------

GEM - Graphics Environment for Multimedia

Load an GLSL tesselation evaluation shader

 *  Copyright 2019 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef _INCLUDE__GEM_MANIPS_GLSL_TESSEVAL_H_
#define _INCLUDE__GEM_MANIPS_GLSL_TESSEVAL_H_

#include "Manips/glsl_vertex.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  glsl_tesseval

  Loads in a tesseval shader

  KEYWORDS

  DESCRIPTION

  -----------------------------------------------------------------*/
class GEM_EXTERN glsl_tesseval : public glsl_vertex
{
  CPPEXTERN_HEADER(glsl_tesseval, glsl_vertex);

public:

  //////////
  // Constructor
  glsl_tesseval(void);
  glsl_tesseval(t_symbol *filename);

protected:

  //////////
  // Destructor
  virtual ~glsl_tesseval();

  ////////
  // extension check
  virtual bool isRunnable(void);

  //////////
  // Print Info about Hardware limits
  virtual void printInfo();
};

#endif  // for header file
