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

#include "pix_lumaoffset.h"
	
CPPEXTERN_NEW(pix_lumaoffset)

/////////////////////////////////////////////////////////
//
// pix_lumaoffset
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_lumaoffset :: pix_lumaoffset()
{ 
    m_OffsetScale = 12.0f; 	// -127 to 127
    m_LineGap = 1.2f;		// 0 to 32
    m_DoFilledLines = false;	// 0 or 1
    m_DoSmoothFill = false;	// 0 or 1

    init =0;

    inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"), gensym("offset"));
    inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"), gensym("gap"));

    hPreviousLineHeights_size=0;
    hPreviousLineHeights=NULL;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_lumaoffset :: ~pix_lumaoffset()
{ 
    
}

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void pix_lumaoffset :: processRGBAImage(imageStruct &image)
{
    nWidth = image.xsize;
    nHeight = image.ysize;
    if (!init) {
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

    const int nNumPixels=nWidth*nHeight;

    const int nOffsetScale=static_cast<int>(m_OffsetScale);
    const int nLineGap=static_cast<int>(m_LineGap);

    Pete_ZeroMemory(pOutput,(nNumPixels*sizeof(U32)));
	
    U32* pCurrentSource=pSource;
    U32* pCurrentOutput=pOutput;
	
    U32* pSourceEnd=(pSource+nNumPixels);
    U32* pOutputEnd=(pOutput+nNumPixels);

    if (!m_DoFilledLines) {
	while (pCurrentSource<pSourceEnd) {
	    const U32* pSourceLineEnd=pCurrentSource+nWidth;

	    while (pCurrentSource!=pSourceLineEnd) {
		const U32 SourceColour=*pCurrentSource;
		int nLuma=GetLuminance(SourceColour);
		nLuma-=(128*255);
				
		const int nOffset=(nLuma*nOffsetScale)>>16;

		U32*const pOffsetOutput=
			    pCurrentOutput+(nOffset*nWidth);

		if ((pOffsetOutput<pOutputEnd)&& (pOffsetOutput>=pOutput)) {

		    *pOffsetOutput=SourceColour;
		}

		pCurrentSource+=1;
		pCurrentOutput+=1;
	    }
	    pCurrentSource+=(nWidth*nLineGap);
	    pCurrentOutput+=(nWidth*nLineGap);
	}
    } else {
      if (hPreviousLineHeights==NULL || hPreviousLineHeights_size<(int)(nWidth*sizeof(U32*)) ){
	free(hPreviousLineHeights);
	hPreviousLineHeights_size=nWidth*sizeof(U32*);
	hPreviousLineHeights=malloc(hPreviousLineHeights_size);	
      }
	U32** ppPreviousLineHeights=(U32**)Pete_LockHandle(hPreviousLineHeights);
	if (ppPreviousLineHeights==NULL)return;

	Pete_ZeroMemory(ppPreviousLineHeights,(nWidth*sizeof(U32*)));

	//int nCurrentY=0;
	while (pCurrentSource<pSourceEnd) {
	    const U32* pSourceLineEnd=pCurrentSource+nWidth;
	    U32** ppCurrentLineHeight=ppPreviousLineHeights;

	    if (m_DoSmoothFill) {
		while (pCurrentSource!=pSourceLineEnd) {
		    const U32 SourceColour=*pCurrentSource;
		    const int nSourceRed=(SourceColour>>SHIFT_RED)&0xff;
		    const int nSourceGreen=(SourceColour>>SHIFT_GREEN)&0xff;
		    const int nSourceBlue=(SourceColour>>SHIFT_BLUE)&0xff;
		    const int nSourceAlpha=(SourceColour>>SHIFT_ALPHA)&0xff;

		    int nLuma=GetLuminance(SourceColour);
		    nLuma-=(128*255);
		    const int nOffset=(nLuma*nOffsetScale)>>16;

		    U32* pOffsetOutputStart=
			    pCurrentOutput+(nOffset*nWidth);

		    U32* pOffsetOutput=pOffsetOutputStart;
		    U32* pPreviousOffsetOutput=*ppCurrentLineHeight;

		    int nDestRed;
		    int nDestGreen;
		    int nDestBlue;
		    int nDestAlpha;
		    int nDestDistance;
		    if (pPreviousOffsetOutput==NULL) {
			nDestRed=0;
			nDestGreen=0;
			nDestBlue=0;
			nDestAlpha=0;
			nDestDistance=10000;
		    } else {
			const U32 DestColour=*pPreviousOffsetOutput;
			nDestRed=(DestColour>>SHIFT_RED)&0xff;
			nDestGreen=(DestColour>>SHIFT_GREEN)&0xff;
			nDestBlue=(DestColour>>SHIFT_BLUE)&0xff;
			nDestAlpha=(DestColour>>SHIFT_ALPHA)&0xff;
			nDestDistance=(pOffsetOutput-pPreviousOffsetOutput)/nWidth;
			if (nDestDistance==0)nDestDistance=1;
		    }

		    const int nDeltaRed=(nDestRed-nSourceRed);
		    const int nDeltaGreen=(nDestGreen-nSourceGreen);
		    const int nDeltaBlue=(nDestBlue-nSourceBlue);
		    const int nDeltaAlpha=(nDestAlpha-nSourceAlpha);
		    
		    const int nIncRed=(nDeltaRed/nDestDistance);
		    const int nIncGreen=(nDeltaGreen/nDestDistance);
		    const int nIncBlue=(nDeltaBlue/nDestDistance);
		    const int nIncAlpha=(nDeltaAlpha/nDestDistance);

		    int nCurrentRed=nSourceRed;
		    int nCurrentGreen=nSourceGreen;
		    int nCurrentBlue=nSourceBlue;
		    int nCurrentAlpha=nSourceAlpha;

		    while ((pOffsetOutput<pOutputEnd)&&
				(pOffsetOutput>=pOutput)&&
				(pOffsetOutput>pPreviousOffsetOutput)) {
			U32 CurrentColour=
					    (nCurrentRed<<SHIFT_RED)|
					    (nCurrentGreen<<SHIFT_GREEN)|
					    (nCurrentBlue<<SHIFT_BLUE)|
					    (nCurrentAlpha<<SHIFT_ALPHA);

			*pOffsetOutput=CurrentColour;

			nCurrentRed+=nIncRed;
			nCurrentGreen+=nIncGreen;
			nCurrentBlue+=nIncBlue;
			nCurrentAlpha+=nIncAlpha;
					
			pOffsetOutput-=nWidth;
		    }

		    *ppCurrentLineHeight=pOffsetOutputStart;

		    pCurrentSource+=1;
		    pCurrentOutput+=1;
		    ppCurrentLineHeight+=1;
		}
	    } else {
		while (pCurrentSource!=pSourceLineEnd) {
		    const U32 SourceColour=*pCurrentSource;
		    int nLuma=GetLuminance(SourceColour);
		    nLuma-=(128*255);
		    const int nOffset=(nLuma*nOffsetScale)>>16;
		    U32* pOffsetOutputStart=pCurrentOutput+(nOffset*nWidth);
		    U32* pOffsetOutput=pOffsetOutputStart;
		    U32* pPreviousOffsetOutput=*ppCurrentLineHeight;

		    while ((pOffsetOutput<pOutputEnd)&&
				(pOffsetOutput>=pOutput)&&
				(pOffsetOutput>pPreviousOffsetOutput)) {
			*pOffsetOutput=SourceColour;
			pOffsetOutput-=nWidth;
		    }

		    *ppCurrentLineHeight=pOffsetOutputStart;

		    pCurrentSource+=1;
		    pCurrentOutput+=1;
		    ppCurrentLineHeight+=1;
		}
	    }
	    pCurrentSource+=(nWidth*nLineGap);
	    pCurrentOutput+=(nWidth*nLineGap);
	}

	U32* pFinalLineStart=pOutputEnd-nWidth;
	U32* pFinalLineEnd=pOutputEnd;
	U32** ppCurrentLineHeight=ppPreviousLineHeights;
	U32* pCurrentOutput=pFinalLineStart;

	while (pCurrentOutput<pFinalLineEnd) {
	    U32* pPreviousOffsetOutput=*ppCurrentLineHeight;
	    const U32 SourceColour=*pPreviousOffsetOutput;
	    U32* pOffsetOutput=pCurrentOutput;

	    while ((pOffsetOutput<pOutputEnd)&&
		    (pOffsetOutput>=pOutput)&&
		    (pOffsetOutput>pPreviousOffsetOutput)) {

		*pOffsetOutput=SourceColour;
		pOffsetOutput-=nWidth;
	    }
	    pCurrentOutput+=1;
	    ppCurrentLineHeight+=1;
	}
    }
    image.data = myImage.data;
}

/////////////////////////////////////////////////////////
// processYUVImage
//
/////////////////////////////////////////////////////////
void pix_lumaoffset :: processYUVImage(imageStruct &image)
{
	int nLuma1, nLuma2;
	
    nWidth = image.xsize/2;
    nHeight = image.ysize;
    if (!init) {
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
	myImage.setBlack();
    pOutput = (U32*)myImage.data;

    const int nNumPixels=nWidth*nHeight;

    const int nOffsetScale=static_cast<int>(m_OffsetScale);
    const int nLineGap=static_cast<int>(m_LineGap);

    //Pete_ZeroMemory(pOutput,(nNumPixels*sizeof(U32)));
	
    U32* pCurrentSource=pSource;
    U32* pCurrentOutput=pOutput;
	
    U32* pSourceEnd=(pSource+nNumPixels);
    U32* pOutputEnd=(pOutput+nNumPixels);

    if (!m_DoFilledLines) {
		while (pCurrentSource<pSourceEnd) {
			const U32* pSourceLineEnd=pCurrentSource+nWidth;

			while (pCurrentSource!=pSourceLineEnd) {
				const U32 SourceColour=*pCurrentSource;
				nLuma1 = ((SourceColour&(0xff<<16))>>16)<<8;
				nLuma1 -= 32640;								// 128*255
				nLuma2 = ((SourceColour&(0xff<<0))>>0)<<8;
				nLuma2 -= 32640;
				
				const int nOffset1=(nLuma1*nOffsetScale)>>16;
				const int nOffset2=(nLuma2*nOffsetScale)>>16;

				U32*const pOffsetOutput1=pCurrentOutput+(nOffset1*nWidth);

				if ((pOffsetOutput1<pOutputEnd) && (pOffsetOutput1>=pOutput)) {
					*pOffsetOutput1=( (SourceColour&(0xff<<24)) |
									 (SourceColour&(0xff<<16)) |
									 (SourceColour&(0xff<<8)) );
				}
				U32*const pOffsetOutput2=(pCurrentOutput)+(nOffset2*nWidth);

				if ((pOffsetOutput2<pOutputEnd) && (pOffsetOutput2>=pOutput)) {
					*pOffsetOutput2=( (SourceColour&(0xff<<24)) |
									 (SourceColour&(0xff<<8)) |
									 (SourceColour&(0xff<<0)) );
				}
				
				pCurrentSource+=1;
				pCurrentOutput+=1;
			}
			pCurrentSource+=(nWidth*nLineGap);
			pCurrentOutput+=(nWidth*nLineGap);
		}
    } else {
		if (hPreviousLineHeights==NULL || hPreviousLineHeights_size<(int)(nWidth*sizeof(U32*)) ){
			free(hPreviousLineHeights);
			hPreviousLineHeights_size=nWidth*sizeof(U32*);
			hPreviousLineHeights=malloc(hPreviousLineHeights_size);	
		}
		U32** ppPreviousLineHeights=(U32**)Pete_LockHandle(hPreviousLineHeights);
		if (ppPreviousLineHeights==NULL)return;

		Pete_ZeroMemory(ppPreviousLineHeights,(nWidth*sizeof(U32*)));

		//int nCurrentY=0;
		while (pCurrentSource<pSourceEnd) {
			const U32* pSourceLineEnd=pCurrentSource+nWidth;
			U32** ppCurrentLineHeight=ppPreviousLineHeights;

			if (m_DoSmoothFill) {
				while (pCurrentSource!=pSourceLineEnd) {
					const U32 SourceColour=*pCurrentSource;
					const int nSourceRed=(SourceColour>>SHIFT_RED)&0xff;
					const int nSourceGreen=(SourceColour>>SHIFT_GREEN)&0xff;
					const int nSourceBlue=(SourceColour>>SHIFT_BLUE)&0xff;
					const int nSourceAlpha=(SourceColour>>SHIFT_ALPHA)&0xff;

					//int nLuma=GetLuminance(SourceColour);
					int nLuma=pCurrentSource[1];
					//int nLuma = (SourceColour&(0xff << 0))>>0;
					//nLuma-=(128*255);
					const int nOffset=(nLuma*nOffsetScale)>>16;

					U32* pOffsetOutputStart=pCurrentOutput+(nOffset*nWidth);

					U32* pOffsetOutput=pOffsetOutputStart;
					U32* pPreviousOffsetOutput=*ppCurrentLineHeight;

					int nDestRed;
					int nDestGreen;
					int nDestBlue;
					int nDestAlpha;
					int nDestDistance;
					if (pPreviousOffsetOutput==NULL) {
						nDestRed=0;
						nDestGreen=0;
						nDestBlue=0;
						nDestAlpha=0;
						nDestDistance=10000;
					} else {
						const U32 DestColour=*pPreviousOffsetOutput;
						nDestRed=(DestColour>>SHIFT_RED)&0xff;
						nDestGreen=(DestColour>>SHIFT_GREEN)&0xff;
						nDestBlue=(DestColour>>SHIFT_BLUE)&0xff;
						nDestAlpha=(DestColour>>SHIFT_ALPHA)&0xff;
						nDestDistance=(pOffsetOutput-pPreviousOffsetOutput)/nWidth;
						if (nDestDistance==0)nDestDistance=1;
					}

					const int nDeltaRed=(nDestRed-nSourceRed);
					const int nDeltaGreen=(nDestGreen-nSourceGreen);
					const int nDeltaBlue=(nDestBlue-nSourceBlue);
					const int nDeltaAlpha=(nDestAlpha-nSourceAlpha);
		    
					const int nIncRed=(nDeltaRed/nDestDistance);
					const int nIncGreen=(nDeltaGreen/nDestDistance);
					const int nIncBlue=(nDeltaBlue/nDestDistance);
					const int nIncAlpha=(nDeltaAlpha/nDestDistance);

					int nCurrentRed=nSourceRed;
					int nCurrentGreen=nSourceGreen;
					int nCurrentBlue=nSourceBlue;
					int nCurrentAlpha=nSourceAlpha;

					while ((pOffsetOutput<pOutputEnd)&&(pOffsetOutput>=pOutput)&&
										(pOffsetOutput>pPreviousOffsetOutput)) {
						U32 CurrentColour=
							(nCurrentRed<<SHIFT_RED)|
							(nCurrentGreen<<SHIFT_GREEN)|
							(nCurrentBlue<<SHIFT_BLUE)|
							(nCurrentAlpha<<SHIFT_ALPHA);

						*pOffsetOutput=CurrentColour;

						nCurrentRed+=nIncRed;
						nCurrentGreen+=nIncGreen;
						nCurrentBlue+=nIncBlue;
						nCurrentAlpha+=nIncAlpha;
					
						pOffsetOutput-=nWidth;
					}

				*ppCurrentLineHeight=pOffsetOutputStart;

				pCurrentSource+=1;
				pCurrentOutput+=1;
				ppCurrentLineHeight+=1;
				}
			} else {
				while (pCurrentSource!=pSourceLineEnd) {
					const U32 SourceColour=*pCurrentSource;
					int nLuma=GetLuminance(SourceColour);
					//int nLuma = (SourceColour&(0xff << 0))>>0;
					nLuma-=(128*255);
					const int nOffset=(nLuma*nOffsetScale)>>16;
					U32* pOffsetOutputStart=pCurrentOutput+(nOffset*nWidth);
					U32* pOffsetOutput=pOffsetOutputStart;
					U32* pPreviousOffsetOutput=*ppCurrentLineHeight;

					while ((pOffsetOutput<pOutputEnd)&&(pOffsetOutput>=pOutput)&&
										(pOffsetOutput>pPreviousOffsetOutput)) {
						*pOffsetOutput=SourceColour;
						pOffsetOutput-=nWidth;
					}

					*ppCurrentLineHeight=pOffsetOutputStart;

					pCurrentSource+=1;
					pCurrentOutput+=1;
					ppCurrentLineHeight+=1;
				}
			}
			pCurrentSource+=(nWidth*nLineGap);
			pCurrentOutput+=(nWidth*nLineGap);
		}

		U32* pFinalLineStart=pOutputEnd-nWidth;
		U32* pFinalLineEnd=pOutputEnd;
		U32** ppCurrentLineHeight=ppPreviousLineHeights;
		U32* pCurrentOutput=pFinalLineStart;

		while (pCurrentOutput<pFinalLineEnd) {
			U32* pPreviousOffsetOutput=*ppCurrentLineHeight;
			const U32 SourceColour=*pPreviousOffsetOutput;
			U32* pOffsetOutput=pCurrentOutput;

			while ((pOffsetOutput<pOutputEnd)&&(pOffsetOutput>=pOutput)&&
								(pOffsetOutput>pPreviousOffsetOutput)) {
				*pOffsetOutput=SourceColour;
				pOffsetOutput-=nWidth;
			}
			pCurrentOutput+=1;
			ppCurrentLineHeight+=1;
		}
    }
	image.data = myImage.data;
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_lumaoffset :: obj_setupCallback(t_class *classPtr)
{
    class_addmethod(classPtr, (t_method)&pix_lumaoffset::offsetCallback,
		  gensym("offset"), A_DEFFLOAT, A_NULL);
    class_addmethod(classPtr, (t_method)&pix_lumaoffset::gapCallback,
		  gensym("gap"), A_DEFFLOAT, A_NULL);
    class_addmethod(classPtr, (t_method)&pix_lumaoffset::fillCallback,
		  gensym("fill"), A_DEFFLOAT, A_NULL);
    class_addmethod(classPtr, (t_method)&pix_lumaoffset::smoothCallback,
		  gensym("smooth"), A_DEFFLOAT, A_NULL);
}
void pix_lumaoffset :: offsetCallback(void *data, t_floatarg m_OffsetScale)
{
  GetMyClass(data)->m_OffsetScale=(m_OffsetScale);
  GetMyClass(data)->setPixModified();
}

void pix_lumaoffset :: gapCallback(void *data, t_floatarg m_LineGap)
{
  if(m_LineGap<0)m_LineGap=0.f;
  GetMyClass(data)->m_LineGap=(m_LineGap);
  GetMyClass(data)->setPixModified();
}
void pix_lumaoffset :: fillCallback(void *data, t_floatarg m_DoFilledLines)
{
  GetMyClass(data)->m_DoFilledLines=(m_DoFilledLines!=0.0);  
  GetMyClass(data)->setPixModified();
}

void pix_lumaoffset :: smoothCallback(void *data, t_floatarg m_DoSmoothFill)
{
  GetMyClass(data)->m_DoSmoothFill=(m_DoSmoothFill!=0.0);  
  GetMyClass(data)->setPixModified();
}
