/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    A polygon

    Copyright (c) 1997-2000 Mark Danks. mark@danks.org
    Copyright (c) Günther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_POLYGON_H_
#define INCLUDE_POLYGON_H_

#include "Base/GemBase.h"
#include "Base/GemShape.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    polygon
    
    Creates a polygon

KEYWORDS
    geo
    
DESCRIPTION

    Inlet for a list - "vert_1"
    ...
    Inlet for a list - "vert_9"

    "list" - The first vertex
    "vert_1" - The second vertex
    ...
    "vert_9" - The tenth vertex
     
-----------------------------------------------------------------*/
class GEM_EXTERN polygon : public GemShape
{
    CPPEXTERN_HEADER(polygon, GemShape)

    public:

        //////////
        // Constructor
    	polygon(t_floatarg numInputs);
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~polygon();

    	//////////
    	// Do the rendering
    	virtual void 	render(GemState *state);

    	//////////
    	// The width of the lines in line draw mode
    	void	    	linewidthMess(float linewidth);
    	
    	//////////
    	// How the object should be drawn
    	virtual void	typeMess(t_symbol *type);

    	//////////
    	// Set the vertices
    	void	    	setVert(int whichOne, float x, float y, float z);

	    //-----------------------------------
	    // GROUP:	Member variables
	    //-----------------------------------
    
    	//////////
    	// The line width for GL_LINE mode
		GLfloat	    	m_linewidth;

    	//////////
    	// The number of inputs (the number of points)
    	int 	    	m_numInputs;
    	
    	//////////
    	// The vertices
    	float	    	m_vert[10][3];

        //////////
        // The drawing style (GL_LINE, GL_POLYGON, etc)
        GLenum	    	m_drawType;

    private:
    	    
    	//////////
    	// Static member functions
    	static void 	linewidthMessCallback(void *data, t_floatarg linewidth);
    	static void 	typeMessCallback(void *data, t_symbol *type);
    	static void 	setVert0Callback(void *data, t_floatarg x, t_floatarg y, t_floatarg z);
    	static void 	setVert1Callback(void *data, t_floatarg x, t_floatarg y, t_floatarg z);
    	static void 	setVert2Callback(void *data, t_floatarg x, t_floatarg y, t_floatarg z);
    	static void 	setVert3Callback(void *data, t_floatarg x, t_floatarg y, t_floatarg z);
    	static void 	setVert4Callback(void *data, t_floatarg x, t_floatarg y, t_floatarg z);
    	static void 	setVert5Callback(void *data, t_floatarg x, t_floatarg y, t_floatarg z);
    	static void 	setVert6Callback(void *data, t_floatarg x, t_floatarg y, t_floatarg z);
    	static void 	setVert7Callback(void *data, t_floatarg x, t_floatarg y, t_floatarg z);
    	static void 	setVert8Callback(void *data, t_floatarg x, t_floatarg y, t_floatarg z);
    	static void 	setVert9Callback(void *data, t_floatarg x, t_floatarg y, t_floatarg z);
};

#endif	// for header file
