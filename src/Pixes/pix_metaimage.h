/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    Copyright (c) 2003 James Tittle
    ported from pete's_plugins (www.petewarden.com)
    
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_PIX_METAIMAGE_H_
#define INCLUDE_PIX_METAIMAGE_H_

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_metaimage
    
    

KEYWORDS
    pix
    
DESCRIPTION

    
   
-----------------------------------------------------------------*/
class GEM_EXTERN pix_metaimage : public GemPixObj
{
    CPPEXTERN_HEADER(pix_metaimage, GemPixObj)

    public:

	    //////////
	    // Constructor
    	pix_metaimage();
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~pix_metaimage();

    	//////////
    	// Do the processing
    	virtual void 	processRGBAImage(imageStruct &image);
        virtual void	processYUVImage(imageStruct &image);

	imageStruct	myImage;
	int		nHeight;
	int		nWidth;
	int		init;

	U32*		pSource;
	U32*		pOutput;

	float 		m_Size;
	float 		m_DoDistanceBased;
	float 		m_DoCheapAndNasty;
	
	SPete_MemHandle 	hSubImage;

	int Pete_MetaImage_Init();
	void Pete_MetaImage_DeInit();
	U32 Pete_MetaImage_CreateSubImage(U32* pInput,U32* pSubImage,float SubWidth,float SubHeight);
	void Pete_MetaImage_DrawSubImages(U32* pSubImage,U32 AverageColour,float SubWidth,float SubHeight);
	void Pete_MetaImage_DrawSubImage(U32* pSource, U32* pShrunkBuffer,U32* pOutput, int nLeftX,int nTopY,int nRightX,int nBottomY,U32 WholeImageAverage,int nClippedLeftX,int nClippedTopY,int nClippedRightX,int nClippedBottomY,U32 SubImageAverage);
	U32 Pete_MetaImage_GetAreaAverage(U32* pImage,int nLeftX,int nTopY,int nRightX,int nBottomY,int nStride);
	U32 Pete_MetaImage_ShrinkSourceImage(U32* pSource, U32* pOutput, float SubWidth,float SubHeight);
	U32 Pete_MetaImage_ShrinkSourceImageFast(U32* pSource, U32* pOutput, float SubWidth,float SubHeight);
	
    private:
    
    	//////////
    	// Static member functions
    	static void 	sizeCallback(void *data, t_floatarg sz);
	static void 	distanceCallback(void *data, t_floatarg m_DoDistanceBased);
	static void 	cheapCallback(void *data, t_floatarg m_DoCheapAndNasty);
};

#endif	// for header file
