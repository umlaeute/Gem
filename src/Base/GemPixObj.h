/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    Base class for pix class gem objects

    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyright (c) Günther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_GEMPIXOBJ_H_
#define INCLUDE_GEMPIXOBJ_H_

#include "Base/GemBase.h"
#include "Base/GemPixUtil.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GemPixObj
    
    Base class for pix class gem objects

DESCRIPTION

-----------------------------------------------------------------*/
class GEM_EXTERN GemPixObj : public GemBase
{
    public:

  //////////
  // Constructor
  GemPixObj();

    protected:
    	
  //////////
  // Destructor
  virtual ~GemPixObj()				{ }

  //////////
  // The derived class should override this if it provides
  // processing independent of the image.format
  // This is called whenever a new image comes through.
  // The default is to output an error
  virtual void 	processImage(imageStruct &image);

  //////////
  // The derived class should override this.
  // This is called whenever a new RGB image comes through.
  // The default is to call processImage().
  virtual void 	processRGBImage(imageStruct &image);

  //////////
  // The derived class should override this.
  // This is called whenever a new RGBA image comes through.
  // The default is to call processImage().
  virtual void 	processRGBAImage(imageStruct &image);

  //////////
  // The derived class should override this.
  // This is called whenever a new gray8 image comes through.
  // The default is to call processImage().
  virtual void 	processGrayImage(imageStruct &image);
        
  //////////
  // The derived class should override this.
  // This is called whenever a new YUV422 image comes through.
  // The default is to call processImage().
  virtual void 	processYUVImage(imageStruct &image);
    	
  //////////
  // If the derived class needs the image resent.
  //  	This sets the dirty bit on the pixBlock.
  void	    	setPixModified();
  
  //////////
  // Turn on/off processing
  void            processOnOff(int on);

  //////////
  int             m_processOnOff;
#ifndef MACOSX
  int             m_mmx;
#endif // MACOSX
  //////////
  // creation callback
  static void 	real_obj_setupCallback(t_class *classPtr) { 
    GemBase::real_obj_setupCallback(classPtr);
    GemPixObj::obj_setupCallback(classPtr); 
  }

  //////////
  // The derived class should NOT override this unless they have some
  //		very special behavior.
  // Do the rendering, which calls processImage or processGrayImage, etc...
  // save the image-information
  virtual void 	render(GemState *state);
  // turn the pointer back to the old data after rendering
  virtual void postrender(GemState *state);
  
  void startRendering(void) {
    //post("start rendering");
    setPixModified();
  }
  
  //////////
  // the pixBlock-cache
  pixBlock    cachedPixBlock;
  pixBlock    *orgPixBlock;
 private:

  static inline GemPixObj *GetMyClass(void *data) {return((GemPixObj *)((Obj_header *)data)->data);}
    
  //////////
  // static member functions
  static void     obj_setupCallback(t_class *classPtr);
  static void 	floatMessCallback(void *data, float n);
};

static inline int powerOfTwo(int value)
{
    int x = 1;
    //    while(x <= value) x <<= 1;
    while(x < value) x <<= 1;
    return(x);
}

#ifdef MACOSX
//Ian Ollman's function to determine the cache prefetch for altivec vec_dst()
inline UInt32 GetPrefetchConstant( int blockSizeInVectors, int blockCount, int blockStride )
{
	return ((blockSizeInVectors << 24) & 0x1F000000) | ((blockCount << 16) & 0x00FF0000) | (blockStride & 0xFFFF);
} 
#endif

#endif	// for header file
