/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glLogicOp(GLenum opcode)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLLOGICOP_H_
#define INCLUDE_GEM_GLLOGICOP_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglLogicOp

    A Wrapper for the openGL-command "glLogicOp(GLenum opcode)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglLogicOp : public GemBase
{
    CPPEXTERN_HEADER(GEMglLogicOp, GemBase)

    public:

        //////////
        // Constructor
        GEMglLogicOp (t_symbol*);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglLogicOp();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLenum opcode;		// VAR
	virtual void	opcodeMess 	(int);		// FUN glLogicOp GLenum


    private:

        //////////
        // Static member functions

	static void	opcodeMessCallback	(void*, t_symbol*);		// CALLBACK glLogicOp

	// we need some inlets
	t_inlet	*m_inlet[ 1 ];
};

#endif  // for header file
