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

#ifndef INCLUDE_GEM_GLBEGIN_H_
#define INCLUDE_GEM_GLBEGIN_H_

#include "Base/CPPExtern.h"
#include "Base/GemGLUtil.h"


/*
 CLASS
	GLdefine
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glBegin( GLenum mode)"
 */

class GEM_EXTERN GLdefine : public CPPExtern
{
	CPPEXTERN_HEADER(GLdefine, CPPExtern)

	public:
	  // Constructor
	  GLdefine (t_symbol*);	// CON

	protected:
	  // Destructor
	  virtual ~GLdefine ();
	  // Do the rendering

	  t_symbol *m_symbol	;		// VAR
	  virtual void	symMess(t_symbol*);	// FUN
	  virtual void bangMess();	// FUN


	private:
	  t_outlet *m_outlet;

	// static member functions
	  static void	 symMessCallback (void*, t_symbol*);
	  static void	 anyMessCallback (void*, t_symbol*, int, t_atom*);
	  static void	 bangMessCallback (void*);
};
#endif // for header file
