/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    A disk

    Copyright (c) 1997-2000 Mark Danks. mark@danks.org
    Copyright (c) G�nther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f�r::uml�ute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

/////////////////////////////////////////////////////////
// 1905:forum::f�r::uml�ute:2000
/////////////////////////////////////////////////////////
// added the gluDisk
/////////////////////////////////////////////////////////

#ifndef INCLUDE_DISK_H_
#define INCLUDE_DISK_H_

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
    CPPEXTERN_HEADER(disk, GemGluObj)

    public:

	    //////////
	    // Constructor
    	disk(int argc, t_atom *argv);
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~disk();

    	//////////
		// Inner radius of the disk
		float			m_innerRadius;

    	//////////
		// Set the inner radius
		void			innerRadius(float radius);

    	//////////
    	// Do the rendering
    	virtual void 	render(GemState *state);
	
	private:

    	//////////
    	// Static member functions
    	static void 	innerRadiusCallback(void *data, t_floatarg radius);
};

#endif	// for header file
