/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    A 3D-scope

    Copyright (c) 1997-2000 Mark Danks. mark@danks.org
    Copyright (c) Günther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_SCOPEXYZ_H_
#define INCLUDE_SCOPEXYZ_H_

#include "Base/GemShape.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    scopeXYZ
    
    Creates a scopeXYZ

KEYWORDS
    geo
    
DESCRIPTION

    Inlet~ for signal X
    Inlet~ for signal Y
    Inlet~ for signal Z
     
-----------------------------------------------------------------*/
class GEM_EXTERN scopeXYZ : public GemShape
{
    CPPEXTERN_HEADER(scopeXYZ, GemShape)

    public:

        //////////
        // Constructor
    	scopeXYZ(t_floatarg len=0);
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~scopeXYZ();

    	//////////
    	// Do the rendering
    	virtual void 	render(GemState *state);

    	//////////
    	// The width of the lines in line draw mode
    	void	    	linewidthMess(float linewidth);
    	
    	//////////
    	// How the object should be drawn
    	virtual void	typeMess(t_symbol *type);

	//-----------------------------------
	// GROUP:	Member variables
	//-----------------------------------
    
    	//////////
    	// the number of points
    	int 	    	m_length;
    	virtual void	lengthMess(int length);

	int m_position; 
    	
    	//////////
    	// The vertices
	float  *m_vertarray;
    	float **m_vert;

        //////////
        // The drawing style (GL_LINE, GL_SCOPEXYZ, etc)
        GLenum	    	m_drawType;

	t_inlet*m_inX, *m_inY, *m_inZ;


    private:
    	    
    	//////////
    	// Static member functions
    	static void 	linewidthMessCallback(void *data, t_floatarg linewidth);
    	static void 	typeMessCallback(void *data, t_symbol *type);
    	static void 	lengthMessCallback(void *data, t_floatarg linewidth);

    	static void 	dspCallback(void *data, t_signal** sp);
	static t_int*   perform(t_int* w);

};

#endif	// for header file
