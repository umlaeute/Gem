 /* ------------------------------------------------------------------
  * GEM - Graphics Environment for Multimedia
  *
  *  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM
  *	zmoelnig@iem.kug.ac.at
  *  For information on usage and redistribution, and for a DISCLAIMER
  *  OF ALL WARRANTIES, see the file, "GEM.LICENSE.TERMS"
  *
  * ------------------------------------------------------------------
  */

#ifndef INCLUDE_GEM_GLGETPOINTERV_H_
#define INCLUDE_GEM_GLGETPOINTERV_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"


#include "math.h"

#ifdef __ppc__
#include "Base/GemFuncUtil.h"
#undef sqrt
#define sqrt fast_sqrtf
#endif

/*
 CLASS
	gemlist_info
 KEYWORDS
	openGL	0
 DESCRIPTION
	get information (scale, shear, rotation, translation) about a gemlist
 */

class GEM_EXTERN gemlist_info : public GemBase
{
	CPPEXTERN_HEADER(gemlist_info, GemBase)

	public:
	  // Constructor
	  gemlist_info (t_floatarg);	// CON
	protected:
	  // Destructor
	  virtual ~gemlist_info ();
	  // Do the rendering
	  virtual void	render (GemState *state);
	// variable
	  GLenum pname;
	  virtual void	pnameMess(t_floatarg);	// FUN

	private:
	// we need one inlet
	  t_inlet *m_inlet;
    // The outlet
      t_outlet    	*m_outletScale;
      t_outlet    	*m_outletRotation;
      t_outlet    	*m_outletPosition;
      t_outlet    	*m_outletShear;

      t_atom m_alist[12];

	// static member functions
         static void    pnameMessCallback (void*,t_floatarg);
};
#endif // for header file
