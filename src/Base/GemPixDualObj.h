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

#ifndef INCLUDE_GEMPIXDUALOBJ_H_
#define INCLUDE_GEMPIXDUALOBJ_H_

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GemPixDualObj
    
    An object which accepts two pixes.

DESCRIPTION

    Inlet for a gem - "gem_right"
    
    "gem_right" - The second gem list
   
    -----------------------------------------------------------------*/
class GEM_EXTERN GemPixDualObj : public GemPixObj
{
 public:

  //////////
  // Constructor
  GemPixDualObj();
    	
 protected:
    	
  //////////
  // Destructor
  virtual ~GemPixDualObj();

  //////////
  // Derived classes should NOT override this!
  // This makes sure that the images are the same size.
  // This calls the other process functions based on the input images.
  virtual void 	processImage(imageStruct &image);

  //////////
  // The derived class HAS TO override this.
  // This is called whenever a new image comes through and
  //		both of the image structs are RGBA
  virtual void 	processDualImage(imageStruct &image, imageStruct &right) = 0;
    	
  //////////
  // The derived class CAN override this.
  // This is called whenever a new image comes through and both
  //		of the image structs are gray8.
  // The default behavior is to output an error.
  virtual void 	processBothGray(imageStruct &image, imageStruct &right);
    	
  //////////
  // The derived class CAN override this.
  // This is called whenever a new image comes through and 
  //		the left image is an RGBA while the right is a gray8.
  // The default behavior is to output an error.
  virtual void 	processRightGray(imageStruct &image, imageStruct &right);
    	
  //////////
  // The derived class CAN override this.
  // This is called whenever a new image comes through and
  //		the left image is a gray8, the right is an RGBA
  // The default behavior is to output an error.
  virtual void 	processLeftGray(imageStruct &image, imageStruct &right);
    	

  //////////
  virtual void	postrender(GemState *);

  //////////
  virtual void	stopRendering();

  //////////
  // If you care about the stop of rendering
  virtual void	rightstopRendering()            { ; }

  //////////
  virtual void  rightRender(GemState *state);

  //////////
  virtual void   	rightPostrender(GemState *)    	{ ; }
 		
  // Called when rendering stops on the right input
  virtual void   	rightStoprender()    			{ ; }

  //////////
  GemCache    	*m_cacheRight;

  //////////
  pixBlock    	*m_pixRight;

  int		m_pixRightValid;
  int           org_pixRightValid;

  //////////
  t_inlet         *m_inlet;

  //////////
  // creation callback
  static void 	real_obj_setupCallback(t_class *classPtr)
    { GemPixObj::real_obj_setupCallback(classPtr); GemPixDualObj::obj_setupCallback(classPtr); }
    	
 private:
    
  static inline GemPixDualObj *GetMyClass(void *data) {return((GemPixDualObj *)((Obj_header *)data)->data);}
	
  //////////
  // Static member functions
  static void 	obj_setupCallback(t_class *classPtr);
  static void 	gem_rightMessCallback(void *x, t_symbol *s, int argc, t_atom *argv);

};
#endif	// for header file
