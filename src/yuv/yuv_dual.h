/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    An object which accepts two pixes.

    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyright (c) Günther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_YUV_DUAL_H_
#define INCLUDE_YUV_DUAL_H_

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    yuv_dual
    
    An object which accepts two pixes.

DESCRIPTION

    Inlet for a gem - "gem_right"
    
    "gem_right" - The second gem list
   
-----------------------------------------------------------------*/
class GEM_EXTERN yuv_dual : public GemPixObj
{
    public:

        //////////
        // Constructor
    	yuv_dual();
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~yuv_dual();

    	//////////
		// Derived classes should NOT override this!
		// This makes sure that the images are the same size.
    	// This calls the other process functions based on the input images.
    	virtual void 	processImage(imageStruct &image);

    	//////////
    	// The derived class HAS TO override this.
    	// This is called whenever a new image comes through and
        //		both of the image structs are RGBA or YUV
    	virtual void 	processDualImage(imageStruct &image, imageStruct &right) = 0;
        
        virtual void 	processYUVImage(imageStruct &image);
    	
    	//////////
    	// The derived class CAN override this.
    	// This is called whenever a new image comes through and both
    	//		of the image structs are YUV.
        // The default behavior is to output an error.
        virtual void 	processDualYUV(imageStruct &image, imageStruct &right);
    	
    	//////////
    	// The derived class CAN override this.
    	// This is called whenever a new image comes through and 
		//		the left image is an RGBA while the right is a YUV.
		// The default behavior is to output an error.
    	virtual void 	processRightYUV(imageStruct &image, imageStruct &right);
    	
    	//////////
    	// The derived class CAN override this.
    	// This is called whenever a new image comes through and
		//		the left image is a YUV, the right is an RGBA
		// The default behavior is to output an error.
    	virtual void 	processLeftYUV(imageStruct &image, imageStruct &right);
        
        //////////
        virtual void	postrender(GemState *);
    	
    	//////////
    	virtual void	stopRendering();
        
        //////////
        virtual void	rightstopRendering()		{ ; }

    	//////////
    	virtual void   	rightRender(GemState *state);

    	//////////
    	virtual void   	rightPostrender(GemState *)    	{ ; }
        
        //////////
        virtual void	rightStoprender()		{ ; }

    	//////////
    	GemCache    	*m_cacheRight;

    	//////////
    	pixBlock    	*m_pixRight;

    	//////////
        int		m_pixRightValid;
        int		org_pixRightValid;

        //////////
        t_inlet         *m_inlet;

    	//////////
    	// creation callback
    	static void 	real_obj_setupCallback(t_class *classPtr)
    	    { GemPixObj::real_obj_setupCallback(classPtr); yuv_dual::obj_setupCallback(classPtr); }
    	
  private:
    
     	static inline yuv_dual *GetMyClass(void *data) {return((yuv_dual *)((Obj_header *)data)->data);}

    	//////////
    	// Static member functions
    	static void 	obj_setupCallback(t_class *classPtr);
    	static void 	gem_rightMessCallback(void *x, t_symbol *s, int argc, t_atom *argv);
};

#endif	// for header file
