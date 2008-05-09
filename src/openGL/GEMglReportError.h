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

#ifndef INCLUDE_GEM_GLREPORTERROR_H_
#define INCLUDE_GEM_GLREPORTERROR_H_

#ifndef HELPSYMBOL_BASE
# define HELPSYMBOL_BASE "Gem/"
#endif

#include "Base/GemBase.h"

/*
 CLASS
	GEMglReportError
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL defines
 */

class GEM_EXTERN GEMglReportError : public GemBase
{
	CPPEXTERN_HEADER(GEMglReportError, GemBase)

	public:
	  // Constructor
	  GEMglReportError (void);	// CON

	protected:
	  // Destructor
	  virtual ~GEMglReportError ();
	  // Do the rendering

	  virtual void render(GemState *state);


	private:
	  t_outlet *m_outlet;

	// static member functions
	  static void	 bangMessCallback (void*);
};
#endif // for header file
