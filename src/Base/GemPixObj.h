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
#include "stdlib.h"
#include "string.h"
#include "math.h"

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
#ifndef __APPLE__
  int             m_mmx;
#endif // __APPLE__
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

// utility functions from PeteHelpers.h
typedef unsigned long U32; 
typedef unsigned short U16;

#define SHIFT_ALPHA	(24)
#define SHIFT_RED	(16)
#define SHIFT_GREEN	(8)
#define SHIFT_BLUE	(0)

inline int GateInt(int nValue,int nMin,int nMax) {

	if (nValue<nMin) {
		return nMin;
	} else if (nValue>nMax) {
		return nMax;
	} else {
		return nValue;
	}
}

    inline float GateFlt(float nValue,float nMin,float nMax) {
	if (nValue<nMin) {
	    return nMin;
	} else if (nValue>nMax) {
	    return nMax;
	} else {
	    return nValue;
	}
    }

inline void* GatePtr(void* pValue,void* pMin,void* pMax) {

	if (pValue<pMin) {
		return pMin;
	} else if (pValue>pMax) {
		return pMax;
	} else {
		return pValue;
	}
}

inline int AbsInt(int inValue) {
	if (inValue>0) {
		return inValue;
	} else {
		return -inValue;
	}
}

inline int GetTiled(int inValue,const int nMax) {
	int nOutValue=(inValue%nMax);

	if (nOutValue<0) {
		nOutValue=((nMax-1)+nOutValue);
	}

	return nOutValue;
}

inline int GetMirrored(int inValue,const int nMax) {

	const int nTwoMax=(nMax*2);

	int nOutValue=GetTiled(inValue,nTwoMax);

	if (nOutValue>=nMax) {
		nOutValue=((nTwoMax-1)-nOutValue);
	}

	return nOutValue;
}

inline float GetRandomFloat(void) {
	return rand()/static_cast<float>(RAND_MAX);
}

static inline int GetSign(int inValue) {

	if (inValue<0) {
		return -1;
	} else {
		return 1;
	}
}

static inline void Get2dTangent(float inX,float inY,float* poutX,float* poutY) {
	*poutX=inY;
	*poutY=-inX;
}

static inline float Dot2d(float Ax,float Ay,float Bx,float By) {
	return ((Ax*Bx)+(Ay*By));
}

static inline void Normalise2d(float* pX,float* pY) {
	const float MagSqr=Dot2d(*pX,*pY,*pX,*pY);
	float Magnitude=(float)sqrt(MagSqr);
	if (Magnitude<=0.0f) {
		Magnitude=0.001f;
	}
	const float RecipMag=1.0f/Magnitude;

	*pX*=RecipMag;
	*pY*=RecipMag;
}

const int cnBiggestSignedInt=0x7fffffff;

const float Pete_Pi=3.141582f;
const float Pete_TwoPi=(2.0f*Pete_Pi);
const float Pete_HalfPi=(0.5f*Pete_Pi);

static inline void Pete_ZeroMemory(void* pMemory,int nCount) {

	char* pCurrent=(char*)pMemory;
	char* pEnd=(pCurrent+nCount);
	while (pCurrent<pEnd) {
		*pCurrent=0;
		pCurrent+=1;
	}
//	memset(pMemory,0,nCount);
}

typedef U32		PETE_PIXELDATA32;
#define SIZEOF_PETE_PIXELDATA32 (4)

typedef U32		PETE_PIXELDATA24;
#define SIZEOF_PETE_PIXELDATA24 (3)

typedef U16		PETE_PIXELDATA16;
#define SIZEOF_PETE_PIXELDATA16 (2)

static inline void Pete_CopyAndConvert24BitTo32Bit(PETE_PIXELDATA24* pSource,PETE_PIXELDATA32* pOutput,int nPixelCount) {

	char* pSourceEnd=((char*)pSource)+(nPixelCount*SIZEOF_PETE_PIXELDATA24);
	char* pCurrentSource=((char*)pSource);
	char* pCurrentOutput=((char*)pOutput);

	while (pCurrentSource<pSourceEnd) {

		*((PETE_PIXELDATA32*)pCurrentOutput)=
		*((PETE_PIXELDATA24*)pCurrentSource);

		pCurrentSource+=SIZEOF_PETE_PIXELDATA24;
		pCurrentOutput+=SIZEOF_PETE_PIXELDATA32;

	}

}

static inline void Pete_CopyAndConvert32BitTo24Bit(PETE_PIXELDATA32* pSource,PETE_PIXELDATA24* pOutput,int nPixelCount) {

	char* pSourceEnd=((char*)pSource)+(nPixelCount*SIZEOF_PETE_PIXELDATA32);
	char* pCurrentSource=((char*)pSource);
	char* pCurrentOutput=((char*)pOutput);

	while (pCurrentSource<pSourceEnd) {

		*((PETE_PIXELDATA24*)pCurrentOutput)=
		*((PETE_PIXELDATA32*)pCurrentSource);

		pCurrentSource+=SIZEOF_PETE_PIXELDATA32;
		pCurrentOutput+=SIZEOF_PETE_PIXELDATA24;

	}

}

static inline void Pete_InPlaceConvert24BitTo32Bit(PETE_PIXELDATA24* pBuffer,int nPixelCount) {

	char* pBufferStart=(char*)pBuffer;
	
	char* pBuffer32Current=(pBufferStart+((nPixelCount-1)*SIZEOF_PETE_PIXELDATA32));
	char* pBuffer24Current=(pBufferStart+((nPixelCount-1)*SIZEOF_PETE_PIXELDATA24));

	while (pBuffer32Current>=pBufferStart) {

		*((PETE_PIXELDATA32*)pBuffer32Current)=
		*((PETE_PIXELDATA24*)pBuffer24Current);

		pBuffer32Current-=SIZEOF_PETE_PIXELDATA32;
		pBuffer24Current-=SIZEOF_PETE_PIXELDATA24;

	}

}

static inline void Pete_CopyAndConvert16Bit565To32Bit(PETE_PIXELDATA16* pSource,PETE_PIXELDATA32* pOutput,int nPixelCount) {

	char* pSourceEnd=((char*)pSource)+(nPixelCount*SIZEOF_PETE_PIXELDATA16);
	char* pCurrentSource=((char*)pSource);
	char* pCurrentOutput=((char*)pOutput);

	while (pCurrentSource<pSourceEnd) {

		PETE_PIXELDATA16 SourceColour=
			*((PETE_PIXELDATA16*)pCurrentSource);

		const int nMaskedRed=(SourceColour>>11)&31;
		const int nMaskedGreen=(SourceColour>>5)&63;
		const int nMaskedBlue=(SourceColour>>0)&31;

		const int nNormalizedRed=(nMaskedRed<<3)|(nMaskedRed>>2);
		const int nNormalizedGreen=(nMaskedGreen<<2)|(nMaskedGreen>>4);
		const int nNormalizedBlue=(nMaskedBlue<<3)|(nMaskedBlue>>2);

		const PETE_PIXELDATA32 OutputColour=
			(nNormalizedRed<<16)|
			(nNormalizedGreen<<8)|
			(nNormalizedBlue<<0);

		*((PETE_PIXELDATA32*)pCurrentOutput)=OutputColour;

		pCurrentSource+=SIZEOF_PETE_PIXELDATA16;
		pCurrentOutput+=SIZEOF_PETE_PIXELDATA32;

	}

}

static inline void Pete_CopyAndConvert32BitTo16Bit565(PETE_PIXELDATA32* pSource,PETE_PIXELDATA16* pOutput,int nPixelCount) {

	char* pSourceEnd=((char*)pSource)+(nPixelCount*SIZEOF_PETE_PIXELDATA32);
	char* pCurrentSource=((char*)pSource);
	char* pCurrentOutput=((char*)pOutput);

	while (pCurrentSource<pSourceEnd) {

		PETE_PIXELDATA32 SourceColour=
			*((PETE_PIXELDATA32*)pCurrentSource);

		const int nSourceRed=(SourceColour>>16)&0xff;
		const int nSourceGreen=(SourceColour>>8)&0xff;
		const int nSourceBlue=(SourceColour>>0)&0xff;

		const int nMaskedRed=(nSourceRed>>3);
		const int nMaskedGreen=(nSourceGreen>>2);
		const int nMaskedBlue=(nSourceBlue>>3);

		PETE_PIXELDATA16 OutputColour=
			(nMaskedRed<<11)|
			(nMaskedGreen<<5)|
			(nMaskedBlue<<0);

		*((PETE_PIXELDATA16*)pCurrentOutput)=OutputColour;

		pCurrentSource+=SIZEOF_PETE_PIXELDATA32;
		pCurrentOutput+=SIZEOF_PETE_PIXELDATA16;

	}

}

inline int GetLuminance(const U32 inColour) {
	const int nRed=(inColour&(0xff<<SHIFT_RED))>>16;
	const int nGreen=(inColour&(0xff<<SHIFT_GREEN))>>8;
	const int nBlue=(inColour&(0xff<<SHIFT_BLUE))>>0;

	const int nLuminance = 
		((90 * nRed)+
		(115 * nGreen)+
		(51 * nBlue));

	return nLuminance;
}
typedef void* SPete_MemHandle;

inline SPete_MemHandle Pete_NewHandle(int nBytesToAlloc) {
	
	return malloc(nBytesToAlloc);

}
	
inline void Pete_FreeHandle(SPete_MemHandle InHandle) {

	free(InHandle);
	
}

inline void* Pete_LockHandle(SPete_MemHandle InHandle) {

	return InHandle;
	
}

inline void Pete_UnLockHandle(SPete_MemHandle InHandle) {

	// do nothing

}
// end of PeteHelpers.h stuff

#ifdef __APPLE__
//Ian Ollman's function to determine the cache prefetch for altivec vec_dst()
inline UInt32 GetPrefetchConstant( int blockSizeInVectors, int blockCount, int blockStride )
{
	return ((blockSizeInVectors << 24) & 0x1F000000) | ((blockCount << 16) & 0x00FF0000) | (blockStride & 0xFFFF);
} 
#endif

#endif	// for header file
