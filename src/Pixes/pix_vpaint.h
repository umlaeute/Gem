/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    Snap a pix of the render buffer

	Copyright (c) 2005 tigital. tigital@mac.com
    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyright (c) Günther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_PIX_VPAINT_H_
#define INCLUDE_PIX_VPAINT_H_

#include "Base/GemPixObj.h"
#include "Base/GemPixUtil.h"
#include "Base/GemPBuffer.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_vpaint
    
    Creates voronoi regions based on pixel color
    
KEYWORDS
    pix
    
DESCRIPTION

    Inlet for a list - "vert_size"
    Inlet for a list - "vert_pos"
    
-----------------------------------------------------------------*/
class GEM_EXTERN pix_vpaint : public GemPixObj
{
    CPPEXTERN_HEADER(pix_vpaint, GemPixObj)

    public:

        //////////
        // Constructor
    	pix_vpaint();
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~pix_vpaint();
    	
    	//////////
    	// Do the processing
    	virtual void 	processImage(imageStruct &image);
//		virtual void 	processRGBAImage(imageStruct &image);
//      virtual void 	processGrayImage(imageStruct &image);
//    	virtual void 	processYUVImage(imageStruct &image);

    	//////////
    	// When a size message is received
    	virtual void	sizeMess(int width, int height);
    	
    	//////////
    	// When a position message is received
    	virtual void	posMess(int x, int y);
		
		//////////
		//
		void	makepoints();
		void	makecone();
		void	init();
		int		m_initialized;
		int		maxPoints, numPoints, viewImage, useStrokes, drawEdges, moving, canDrawEdges;
		int		mouseX, mouseY, lastX, lastY, sampleMenu, strokeMenu;
		
		/*
		* Point structure 
		*/
		typedef struct {
			int x, y;
			unsigned char r, g, b;
		} cPoint;
		cPoint *points;
    	
    	//////////
    	// The imageStruct with the current image
		imageStruct     m_imageStruct;
    	//////////
		// PBuffer
		PBuffer		*m_pbuffer;
    	//////////
    	// The x position
    	int     	m_x;
    	
    	//////////
    	// The y position
    	int     	m_y;
    	
    	//////////
    	// The width
    	int     	m_w;
    	
    	//////////
    	// The height
    	int     	m_h;
		
		//////////
		// recalc the random points
		bool		m_banged;
    	
    private:
    	
    	//////////
    	// static member functions
    	static void 	vpaintCallback(void *data);
		static void 	bangMessCallback(void *data);
    	static void 	sizeMessCallback(void *data, t_floatarg width, t_floatarg height );
    	static void 	posMessCallback(void *data, t_floatarg x, t_floatarg y);
};

#endif	// for header file
