////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// tigital@mac.com
// ported from pete's_plugins (www.petewarden.com)
//
// Implementation file
//
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "pix_metaimage.h"
	
CPPEXTERN_NEW(pix_metaimage)

/////////////////////////////////////////////////////////
//
// pix_refraction
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_metaimage :: pix_metaimage()
{ 
    m_Size = 0.2f;
    m_DoDistanceBased = 0.0f;
    m_DoCheapAndNasty = 0.0f;
    hSubImage = NULL;

    init =0;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_metaimage :: ~pix_metaimage()
{ 
    Pete_MetaImage_DeInit();
}

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void pix_metaimage :: processRGBAImage(imageStruct &image)
{
    nWidth = image.xsize;
    nHeight = image.ysize;
    if (!init) {
	Pete_MetaImage_Init();
	init = 1;
    }
    pSource = (U32*)image.data;
    
    if ( myImage.xsize*myImage.ysize*myImage.csize != image.xsize*image.ysize*image.csize ){
	int dataSize = image.xsize * image.ysize * image.csize;
	myImage.clear();

	myImage.allocate(dataSize);
    }

    myImage.xsize = image.xsize;
    myImage.ysize = image.ysize;
    myImage.csize = image.csize;
    myImage.type  = image.type;
    pOutput = (U32*)myImage.data;

    float SubWidth;
    float SubHeight;
    
    m_Size = GateFlt(m_Size,0.0f,1.0f);

    if (m_DoDistanceBased>0.0f) {

		const float Distance=1.0f+(m_Size*(nHeight-1.0f));

		SubWidth=nWidth/Distance;
		SubHeight=nHeight/Distance;

    } else {

		SubWidth=1+(m_Size*(nWidth-1));
		SubHeight=1+(m_Size*(nHeight-1));
    }

    U32* pSubImageData=(U32*)Pete_LockHandle(hSubImage);
    if (pSubImageData==NULL) {
		return;
    } 

    U32 AverageColour=Pete_MetaImage_CreateSubImage(pSource,pSubImageData,SubWidth,SubHeight);

    Pete_MetaImage_DrawSubImages(pSubImageData,AverageColour,SubWidth,SubHeight);

    image.data = myImage.data;
}

/////////////////////////////////////////////////////////
// do the other processing here
//
/////////////////////////////////////////////////////////
int pix_metaimage :: Pete_MetaImage_Init() {

	Pete_MetaImage_DeInit();

	const int nNumPixels=(nWidth*nHeight);
	const int nNumBytes=(nNumPixels*sizeof(U32));
	hSubImage = Pete_NewHandle(nNumBytes);
	if (hSubImage==NULL) {
		Pete_MetaImage_DeInit();
		return 0;
	}

	return 1;

}

void pix_metaimage :: Pete_MetaImage_DeInit() {

	if (hSubImage!=NULL) {
		Pete_FreeHandle(hSubImage);
		hSubImage=NULL;
	}

}

U32 pix_metaimage :: Pete_MetaImage_CreateSubImage(U32* pInput,U32* pSubImage,float SubWidth,float SubHeight) {

	U32 AverageColour;

	if (m_DoCheapAndNasty>0.0f) {
		AverageColour=
			Pete_MetaImage_ShrinkSourceImageFast(pInput,pSubImage,SubWidth,SubHeight);
	} else {
		AverageColour=
			Pete_MetaImage_ShrinkSourceImage(pInput,pSubImage,SubWidth,SubHeight);
	}

	return AverageColour;

}

void pix_metaimage :: Pete_MetaImage_DrawSubImages(U32* pSubImage,U32 AverageColour,float SubWidth,float SubHeight) {
	
	const int nHalfWidth=nWidth/2;
	const int nHalfHeight=nHeight/2;

	float CentreX=nHalfWidth+(SubWidth/2);
	float CentreY=nHalfHeight+(SubHeight/2);

	int nSubImageCountX=static_cast<int>(CentreX/SubWidth);
	int nSubImageCountY=static_cast<int>(CentreY/SubHeight);

	const float StartX=(nHalfWidth-(SubWidth/2))-(nSubImageCountX*SubWidth);
	const float EndX=(nHalfWidth+(SubWidth/2))+(nSubImageCountX*SubWidth);

	const float StartY=(nHalfHeight-(SubHeight/2))-(nSubImageCountY*SubHeight);
	const float EndY=(nHalfHeight+(SubHeight/2))+(nSubImageCountY*SubHeight);

	float CurrentY;
	for (CurrentY=StartY; CurrentY<EndY; CurrentY+=SubHeight) {

		float CurrentX;
		for (CurrentX=StartX; CurrentX<EndX; CurrentX+=SubWidth) {

			const int nLeftX=static_cast<int>(CurrentX);
			const int nTopY=static_cast<int>(CurrentY);
			const int nRightX=static_cast<int>(CurrentX+SubWidth);
			const int nBottomY=static_cast<int>(CurrentY+SubHeight);

			const int nClippedLeftX=GateInt(nLeftX,0,(nWidth-1));
			const int nClippedTopY=GateInt(nTopY,0,(nHeight-1));
			const int nClippedRightX=GateInt(nRightX,0,(nWidth-1));
			const int nClippedBottomY=GateInt(nBottomY,0,(nHeight-1));

			U32 SubImageAverage=Pete_MetaImage_GetAreaAverage(
				pSource,
				nClippedLeftX,
				nClippedTopY,
				nClippedRightX,
				nClippedBottomY,
				4);

			Pete_MetaImage_DrawSubImage(
				pSource,
				pSubImage,
				pOutput,
				nLeftX,
				nTopY,
				nRightX,
				nBottomY,
				AverageColour,
				nClippedLeftX,
				nClippedTopY,
				nClippedRightX,
				nClippedBottomY,
				SubImageAverage);

		}

	}

}

void pix_metaimage :: Pete_MetaImage_DrawSubImage(U32* pSource, U32* pShrunkBuffer,U32* pOutput,int nLeftX,int nTopY,int nRightX,int nBottomY,U32 WholeImageAverage,int nClippedLeftX,int nClippedTopY,int nClippedRightX,int nClippedBottomY,U32 SubImageAverage) {

	//const int nWidth=pInstanceData->nWidth;
	//const int nHeight=pInstanceData->nHeight;

	const int nSubRed=(SubImageAverage>>SHIFT_RED)&0xff;
	const int nSubGreen=(SubImageAverage>>SHIFT_GREEN)&0xff;
	const int nSubBlue=(SubImageAverage>>SHIFT_BLUE)&0xff;

	const int nWholeRed=(WholeImageAverage>>SHIFT_RED)&0xff;
	const int nWholeGreen=(WholeImageAverage>>SHIFT_GREEN)&0xff;
	const int nWholeBlue=(WholeImageAverage>>SHIFT_BLUE)&0xff;

	const int nRedDelta=(nSubRed-nWholeRed);
	const int nGreenDelta=(nSubGreen-nWholeGreen);
	const int nBlueDelta=(nSubBlue-nWholeBlue);

	const int nXDelta=nClippedRightX-nClippedLeftX;
	const int nYDelta=nClippedBottomY-nClippedTopY;

	if ((nXDelta<=0)||
		(nYDelta<=0)) {
		return;
	}


	U32* pCurrentSource=pShrunkBuffer;

	pCurrentSource+=((nClippedTopY-nTopY)*nWidth);
	pCurrentSource+=(nClippedLeftX-nLeftX);

	U32* pCurrentOutput=pOutput+(nClippedTopY*nWidth)+nClippedLeftX;
	U32*const pOutputEnd=(pCurrentOutput+(nYDelta*nWidth)+nXDelta);

	while (pCurrentOutput<pOutputEnd) {
		
		U32*const pOutputLineStart=pCurrentOutput;
		U32*const pOutputLineEnd=pCurrentOutput+nXDelta;

		U32* pSourceLineStart=pCurrentSource;

		while (pCurrentOutput<pOutputLineEnd) {

			const U32 SourceColour=*pCurrentSource;

			const U32 nSourceRed=(SourceColour>>SHIFT_RED)&0xff;
			const U32 nSourceGreen=(SourceColour>>SHIFT_GREEN)&0xff;
			const U32 nSourceBlue=(SourceColour>>SHIFT_BLUE)&0xff;

			const U32 nOutputRed=GateInt(nSourceRed+nRedDelta,0,255);
			const U32 nOutputGreen=GateInt(nSourceGreen+nGreenDelta,0,255);
			const U32 nOutputBlue=GateInt(nSourceBlue+nBlueDelta,0,255);

			const U32 OutputColour=
				((nOutputRed&0xff)<<SHIFT_RED)|
				((nOutputGreen&0xff)<<SHIFT_GREEN)|
				((nOutputBlue&0xff)<<SHIFT_BLUE);

			*pCurrentOutput=OutputColour;

			pCurrentOutput+=1;
			pCurrentSource+=1;

		}

		pCurrentOutput=pOutputLineStart+nWidth;
		pCurrentSource=pSourceLineStart+nWidth;

	}

}

U32 pix_metaimage :: Pete_MetaImage_GetAreaAverage(U32* pImage,int nLeftX,int nTopY,int nRightX,int nBottomY,int nStride) {

	//const int nWidth=pInstanceData->nWidth;
	//const int nHeight=pInstanceData->nHeight;

	const int nXDelta=nRightX-nLeftX;
	const int nYDelta=nBottomY-nTopY;

	if ((nXDelta<=0)||
		(nYDelta<=0)) {
		return 0x00000000;
	}
	
	U32* pCurrentImage=pImage+(nTopY*nWidth)+nLeftX;
	U32*const pImageEnd=(pCurrentImage+(nYDelta*nWidth)+nXDelta);

	int nRedTotal=0;
	int nGreenTotal=0;
	int nBlueTotal=0;
	int nSampleCount=0;

	while (pCurrentImage<pImageEnd) {
		
		U32*const pImageLineStart=pCurrentImage;
		U32*const pImageLineEnd=pCurrentImage+nXDelta;

		while (pCurrentImage<pImageLineEnd) {

			const U32 ImageColour=*pCurrentImage;

			const U32 nImageRed=(ImageColour>>SHIFT_RED)&0xff;
			const U32 nImageGreen=(ImageColour>>SHIFT_GREEN)&0xff;
			const U32 nImageBlue=(ImageColour>>SHIFT_BLUE)&0xff;

			nRedTotal+=nImageRed;
			nGreenTotal+=nImageGreen;
			nBlueTotal+=nImageBlue;

			nSampleCount+=1;
			
			pCurrentImage+=nStride;

		}

		pCurrentImage=pImageLineStart+(nStride*nWidth);

	}

	const int nAverageRed=(nRedTotal/nSampleCount);
	const int nAverageGreen=(nGreenTotal/nSampleCount);
	const int nAverageBlue=(nBlueTotal/nSampleCount);

	U32 Average=
		(nAverageRed<<SHIFT_RED)|
		(nAverageGreen<<SHIFT_GREEN)|
		(nAverageBlue<<SHIFT_BLUE);

	return Average;
	
}

U32 pix_metaimage :: Pete_MetaImage_ShrinkSourceImage(U32* pSource, U32* pOutput, float SubWidth,float SubHeight) {

	if (SubWidth>(float)(nWidth)) {
		SubWidth=(float)(nWidth);
	}

	if (SubHeight>(float)(nHeight)) {
		SubHeight=(float)(nHeight);
	}

	const float SourceYInc=(nHeight/SubHeight);
	const float SourceXInc=(nWidth/SubWidth);

	int nRedTotal=0;
	int nGreenTotal=0;
	int nBlueTotal=0;
	int nSampleCount=0;

	U32* pCurrentOutput=pOutput;
	
	float SourceY;
	for (SourceY=0.0f; SourceY<nHeight; SourceY+=SourceYInc) {

		U32* pOutputLineStart=pCurrentOutput;
		const int nTopY=static_cast<int>(SourceY);
		int nBottomY=static_cast<int>(SourceY+SourceYInc);
		nBottomY=GateInt(nBottomY,0,(nHeight-1));

		float SourceX;
		for (SourceX=0.0f; SourceX<nWidth; SourceX+=SourceXInc) {

			const int nLeftX=static_cast<int>(SourceX);
			int nRightX=static_cast<int>(SourceX+SourceXInc);
			nRightX=GateInt(nRightX,0,(nWidth-1));

			const U32 OutputColour=
				Pete_MetaImage_GetAreaAverage(pSource,nLeftX,nTopY,nRightX,nBottomY,1);

			const U32 nOutputRed=(OutputColour>>SHIFT_RED)&0xff;
			const U32 nOutputGreen=(OutputColour>>SHIFT_GREEN)&0xff;
			const U32 nOutputBlue=(OutputColour>>SHIFT_BLUE)&0xff;

			nRedTotal+=nOutputRed;
			nGreenTotal+=nOutputGreen;
			nBlueTotal+=nOutputBlue;

			nSampleCount+=1;

			*pCurrentOutput=OutputColour;

			pCurrentOutput+=1;

		}

		pCurrentOutput=pOutputLineStart+nWidth;

	}

	const int nAverageRed=(nRedTotal/nSampleCount);
	const int nAverageGreen=(nGreenTotal/nSampleCount);
	const int nAverageBlue=(nBlueTotal/nSampleCount);

	U32 Average=
		(nAverageRed<<SHIFT_RED)|
		(nAverageGreen<<SHIFT_GREEN)|
		(nAverageBlue<<SHIFT_BLUE);

	return Average;

}

U32 pix_metaimage :: Pete_MetaImage_ShrinkSourceImageFast(U32* pSource, U32* pOutput, float SubWidth,float SubHeight) {

	if (SubWidth>(float)(nWidth)) {
		SubWidth=(float)(nWidth);
	}

	if (SubHeight>(float)(nHeight)) {
		SubHeight=(float)(nHeight);
	}

	const float SourceYInc=(nHeight/SubHeight);
	const float SourceXInc=(nWidth/SubWidth);

	int nRedTotal=0;
	int nGreenTotal=0;
	int nBlueTotal=0;
	int nSampleCount=0;

	U32* pCurrentOutput=pOutput;
	
	float SourceY;
	for (SourceY=0.0f; SourceY<nHeight; SourceY+=SourceYInc) {

		U32* pOutputLineStart=pCurrentOutput;
		const int nTopY=static_cast<int>(SourceY);

		U32* pSourceLineStart=pSource+(nTopY*nWidth);

		float SourceX;
		for (SourceX=0.0f; SourceX<nWidth; SourceX+=SourceXInc) {

			const int nLeftX=static_cast<int>(SourceX);

			const U32 OutputColour=*(pSourceLineStart+nLeftX);

			const U32 nOutputRed=(OutputColour>>SHIFT_RED)&0xff;
			const U32 nOutputGreen=(OutputColour>>SHIFT_GREEN)&0xff;
			const U32 nOutputBlue=(OutputColour>>SHIFT_BLUE)&0xff;

			nRedTotal+=nOutputRed;
			nGreenTotal+=nOutputGreen;
			nBlueTotal+=nOutputBlue;

			nSampleCount+=1;

			*pCurrentOutput=OutputColour;

			pCurrentOutput+=1;

		}

		pCurrentOutput=pOutputLineStart+nWidth;

	}

	const int nAverageRed=(nRedTotal/nSampleCount);
	const int nAverageGreen=(nGreenTotal/nSampleCount);
	const int nAverageBlue=(nBlueTotal/nSampleCount);

	U32 Average=
		(nAverageRed<<SHIFT_RED)|
		(nAverageGreen<<SHIFT_GREEN)|
		(nAverageBlue<<SHIFT_BLUE);

	return Average;

}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_metaimage :: obj_setupCallback(t_class *classPtr)
{
    class_addmethod(classPtr, (t_method)&pix_metaimage::sizeCallback,
		  gensym("size"), A_DEFFLOAT, A_NULL);
    class_addmethod(classPtr, (t_method)&pix_metaimage::distanceCallback,
		  gensym("distance"), A_DEFFLOAT, A_NULL);
    class_addmethod(classPtr, (t_method)&pix_metaimage::cheapCallback,
		  gensym("cheap"), A_DEFFLOAT, A_NULL);
}
void pix_metaimage :: sizeCallback(void *data, t_floatarg sz)
{
  GetMyClass(data)->m_Size=(sz);
}
void pix_metaimage :: distanceCallback(void *data, t_floatarg m_DoDistanceBased)
{
  GetMyClass(data)->m_DoDistanceBased=(m_DoDistanceBased);
}
void pix_metaimage :: cheapCallback(void *data, t_floatarg m_DoCheapAndNasty)
{
  GetMyClass(data)->m_DoCheapAndNasty=(m_DoCheapAndNasty);  
}
