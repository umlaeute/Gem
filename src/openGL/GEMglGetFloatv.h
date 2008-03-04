/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 *  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM
 *	zmoelnig@iem.kug.ac.at
 *  For information on usage and redistribution, and for a DISCLAIMER
 *  OF ALL WARRANTIES, see the file, "GEM.LICENSE.TERMS"
 *
 *  this file has been generated...
 * ------------------------------------------------------------------
 */

#ifndef INCLUDE_GEM_GLGETPOINTERV_H_
#define INCLUDE_GEM_GLGETPOINTERV_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*
  CLASS
  GEMglGetFloatv
  KEYWORDS
  openGL	0
  DESCRIPTION
  wrapper for the openGL-function
  "glGetFloatv( GLenum pname, GLfloat *params)"
*/

class GEM_EXTERN GEMglGetFloatv : public GemBase
{
  CPPEXTERN_HEADER(GEMglGetFloatv, GemBase)

    public:
  // Constructor
  GEMglGetFloatv (t_floatarg);	// CON
 protected:
  // Destructor
  virtual ~GEMglGetFloatv ();
          // check extensions
          virtual bool isRunnable(void);

  // Do the rendering
  virtual void	render (GemState *state);
  // variable
  GLenum pname;
  virtual void	pnameMess(t_floatarg);	// FUN

  t_atom m_alist[16];

 private:
  // we need one inlet
  t_inlet *m_inlet;
  // The outlet
  t_outlet    	*m_outlet;

  // static member functions
  static void    pnameMessCallback (void*,t_floatarg);
};
#endif // for header file
