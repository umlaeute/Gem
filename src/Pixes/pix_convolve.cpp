////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-1998 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    Copyright (c) 2002 James Tittle & Chris Clepper
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "pix_convolve.h"

CPPEXTERN_NEW_WITH_TWO_ARGS(pix_convolve, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// pix_convolve
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_convolve :: pix_convolve(t_floatarg fRow, t_floatarg fCol)
  : m_imatrix(NULL)
{
  int row = (int)fRow;
  int col = (int)fCol;

    if (!row || !col )
    {
    	error("GEM: pix_convolve: matrix must have some dimension");
    	return;
    }
    
    if (!(row % 2) || !(col % 2) )
    {
    	error("GEM: pix_convolve: matrix must have odd dimensions");
    	return;
    }
    
    m_rows = row;
    m_cols = col;
    m_irange = 255;
    m_imatrix = new signed short[m_rows * m_cols];

    // zero out the matrix
    int i;
    for (i = 0; i < m_cols * m_rows; i++) m_imatrix[i] = 0;
    // insert a one for the default center value (identity matrix)
    m_imatrix[ ((m_cols / 2 + 1) * m_rows) + (m_rows / 2 + 1) ] = 255;
    
    inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"), gensym("ft1"));
    inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("list"), gensym("matrix"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_convolve :: ~pix_convolve()
{
    if (m_imatrix)delete [] m_imatrix;
    post("done...");
  
}

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////


void pix_convolve :: calculateRGBA3x3(imageStruct &image,imageStruct &tempImg)
{
  int i;
  int j;
  int k;
  int xsize =  tempImg.xsize;
  int ysize =  tempImg.ysize;
  int size = xsize*ysize - xsize-1;
  int csize = tempImg.csize;

  int* src = (int*) tempImg.data;
  int* dest = (int*)image.data;
  
  //  MMXSTART;
//unroll this to do R G B in one pass?? (too many registers?)
  i = xsize;
  int* val1 = 0;
  int* val2 = src+i-xsize;
  int* val3 = src+i-xsize+1;
  int* val4 = src+i-1;
  int* val5 = src+i;
  int* val6 = src+i+1;
  int* val7 = src+i+xsize-1;
  int* val8 = src+i+xsize;
  int* val9 = src+i+xsize+1;
  int res;
  for (i=xsize+1;i<size;i++) {
    val1 = val2;
    val2 = val3;
    val3 = src+i-xsize+1;
    val4 = val5;
    val5 = val6;
    val6 = src+i+1;
    val7 = val8;
    val8 = val9;
    val9 = src+i+xsize+1;
    if (i%xsize == 0 || i%xsize == xsize-1) continue;
    #ifndef MACOSX
    for (j=0;j<3;j++) 
    #else
    for (j=1;j<4;j++)
    #endif
    {
      res = m_imatrix[0]*(int)((unsigned char*)val1)[j];
      res += m_imatrix[1]*(int)((unsigned char*)val2)[j];
      res += m_imatrix[2]*(int)((unsigned char*)val3)[j];
      res += m_imatrix[3]*(int)((unsigned char*)val4)[j];
      res += m_imatrix[4]*(int)((unsigned char*)val5)[j];
      res += m_imatrix[5]*(int)((unsigned char*)val6)[j];
      res += m_imatrix[6]*(int)((unsigned char*)val7)[j];
      res += m_imatrix[7]*(int)((unsigned char*)val8)[j];
      res += m_imatrix[8]*(int)((unsigned char*)val9)[j];
      res*=m_irange;
      res>>=16;
      ((unsigned char*)dest)[i*csize+j] = CLAMP(res);
    }

  }

  
}

void pix_convolve :: processImage(imageStruct &image)
{
    image.copy2Image(&tempImg);
    int initX = m_rows / 2;
    int initY = m_cols / 2;
    int maxX = tempImg.xsize - initX;
    int maxY = tempImg.ysize - initY;
    int xTimesc = tempImg.xsize * tempImg.csize;
    int initOffset = initY * xTimesc + initX * tempImg.csize;



    if (m_rows == 3 && m_cols == 3 && tempImg.csize == 4) {
      calculateRGBA3x3(image,tempImg);
      return;
    }

    for (int y = initY; y < maxY; y++)
    {
        int realY = y * xTimesc;
        int offsetY = realY - initOffset;

    	for (int x = initX; x < maxX; x++)
    	{
	    int csize = tempImg.csize;
    	    int realPos = x * csize + realY;
            int offsetXY = x * csize + offsetY;

    	    // skip the alpha value

	    for (int c = 1; c < csize; c++)
    	    {
    		    int new_val = 0;
		    int offsetXYC = offsetXY + c;
    		    for (int matY = 0; matY < m_cols; matY++)
    		    {
    		        int offsetXYCMat = matY * xTimesc + offsetXYC;
    		        int realMatY = matY * m_rows;
    	    	    for (int matX = 0; matX < m_rows; matX++)
    	    	    {
                        new_val += (tempImg.data[offsetXYCMat + matX * csize] *
                                        m_imatrix[realMatY + matX])>>8;
    	    	    }
    		    }
                    image.data[realPos + c] = CLAMP(new_val);  
		    //removes insult from injury ??
		    // we do not use the m_irange anymore ...  remove it ??

    	    }
    	}
    }

}


void pix_convolve :: processYUVImage(imageStruct &image)
{
     image.copy2Image(&tempImg);
     //float range = 1;
    int initX = m_rows / 2;
    int initY = m_cols / 2;
    int maxX = tempImg.xsize - initX;
    int maxY = tempImg.ysize - initY;
    int xTimesc = tempImg.xsize * tempImg.csize;
    int initOffset = initY * xTimesc + initX * tempImg.csize;
    
 //   calculate3x3YUV(image,tempImg);
    if (m_rows == 3 && m_cols == 3) {
      calculate3x3YUV(image,tempImg);
      return;
    }
    if (m_chroma) {
    
    for (int y = initY; y < maxY; y++)
    {
        int realY = y * xTimesc;
        int offsetY = realY - initOffset;

    	for (int x = initX; x < maxX; x++)
    	{
    	    int realPos = x * tempImg.csize + realY;
            int offsetXY = x * tempImg.csize + offsetY;

    	    // skip the UV
    	    for (int c = 1; c < 3; c+=2)
    	    {
    		    int new_val = 0;
                int offsetXYC = offsetXY + c;
    		    for (int matY = 0; matY < m_cols; matY++)
    		    {
    		        int offsetXYCMat = matY * xTimesc + offsetXYC;
    		        int realMatY = matY * m_rows;
    	    	    for (int matX = 0; matX < m_rows; matX++)
    	    	    {
                      new_val += (tempImg.data[offsetXYCMat + matX * tempImg.csize] *
                                        m_imatrix[realMatY + matX])>>8;
    	    	    }
    		    }
                   image.data[realPos + c] = CLAMP(new_val);
                   // image.data[realPos + c-1] = 128;  //remove the U+V
    	    }
    	}
    }
    }else{
    for (int y = initY; y < maxY; y++)
    {
        int realY = y * xTimesc;
        int offsetY = realY - initOffset;

    	for (int x = initX; x < maxX; x++)
    	{
    	    int realPos = x * tempImg.csize + realY;
            int offsetXY = x * tempImg.csize + offsetY;

    	    // skip the UV
    	    for (int c = 1; c < 3; c+=2)
    	    {
    		    int new_val = 0;
                int offsetXYC = offsetXY + c;
    		    for (int matY = 0; matY < m_cols; matY++)
    		    {
    		        int offsetXYCMat = matY * xTimesc + offsetXYC;
    		        int realMatY = matY * m_rows;
    	    	    for (int matX = 0; matX < m_rows; matX++)
    	    	    {
                      new_val += (tempImg.data[offsetXYCMat + matX * tempImg.csize] *
                                        m_imatrix[realMatY + matX])>>8;
    	    	    }
    		    }
                   image.data[realPos + c] = CLAMP(new_val);
                    image.data[realPos + c-1] = 128;  //remove the U+V
    	    }
    	}
    }
    }
   
}

void pix_convolve :: calculate3x3YUV(imageStruct &image,imageStruct &tempImg)
{
  int i;
  int j;
  int k;
  int xsize =  tempImg.xsize;
  int ysize =  tempImg.ysize;
  int size = xsize*ysize - xsize-1;

  short* src = (short*) tempImg.data;
  short* dest = (short*)image.data;
  
 
if (m_chroma){
//post("pix_convolve: new YUV method");
  i = xsize;
  //make these temp register vars rather than pointers?
  /*short* val1 = 0;  
  short* val2 = src+i-xsize; //val2 = src[i-xsize];
  short* val3 = src+i-xsize+1; //val3 = src[i-xsize+1];
  short* val4 = src+i-1; //val4 = src[i-1];
  short* val5 = src+i; //val5 = src[i];
  short* val6 = src+i+1; //val6 = src[i+1];
  short* val7 = src+i+xsize-1; //val7 = src[i+xsize-1];
  short* val8 = src+i+xsize; //val8 = src[i+xsize];
  short* val9 = src+i+xsize+1; //val9 = src[i+xsize+1];*/
  register unsigned char val1 = 0;  
  register unsigned char val2 = src[i-xsize+1]; 
  register unsigned char val3 = src[i-xsize+3];
  register unsigned char val4 = src[i-1];
  register unsigned char val5 = src[i+1];
  register unsigned char val6 = src[i+3];
  register unsigned char val7 = src[i+xsize-1];
  register unsigned char val8 = src[i+xsize+1];
  register unsigned char val9 = src[i+xsize+3];
  int res;

  
  //unroll this 2x to fill the registers? (matrix*y1*y2= 9*9*9 =27)
  for (i=xsize+1;i<size;i++) {
  //load furthest value first...the rest should be in cache
    val7 = val8;
    val8 = val9;
    val9 = src[i+xsize+3]; //should be in cache from previous pass
    val1 = val2;
    val2 = val3;
    val3 = src[i-xsize+3]; //should be in cache from previous pass
    val4 = val5;
    val5 = val6;
    val6 = src[i+3];
    
    if (i%xsize == 0 || i%xsize == xsize-1) continue;
   /* #ifndef MACOSX
    for (j=0;j<3;j++) 
    #else
    for (j=1;j<3;j+=2)
    #endif
    { */
    //use separate temp vars here??
    //register the matrix values as separate vars??
      res = m_imatrix[0]*(int)((unsigned char)val1);
      res += m_imatrix[1]*(int)((unsigned char)val2);
      res += m_imatrix[2]*(int)((unsigned char)val3);
      res += m_imatrix[3]*(int)((unsigned char)val4);
      res += m_imatrix[4]*(int)((unsigned char)val5);
      res += m_imatrix[5]*(int)((unsigned char)val6);
      res += m_imatrix[6]*(int)((unsigned char)val7);
      res += m_imatrix[7]*(int)((unsigned char)val8);
      res += m_imatrix[8]*(int)((unsigned char)val9);
      res*=m_irange;
      res>>=16;
      ((unsigned char*)dest)[i*2+1] = CLAMP(res);
   // }
  } 
  }else{
  
  i = xsize;
  //make these temp register vars rather than pointers?
  short* val1 = 0;  
  short* val2 = src+i-xsize; //val2 = src[i-xsize];
  short* val3 = src+i-xsize+1; //val3 = src[i-xsize+1];
  short* val4 = src+i-1; //val4 = src[i-1];
  short* val5 = src+i; //val5 = src[i];
  short* val6 = src+i+1; //val6 = src[i+1];
  short* val7 = src+i+xsize-1; //val7 = src[i+xsize-1];
  short* val8 = src+i+xsize; //val8 = src[i+xsize];
  short* val9 = src+i+xsize+1; //val9 = src[i+xsize+1];
  int res;
  for (i=xsize+1;i<size;i++) {
    val1 = val2;
    val2 = val3;
    val3 = src+i-xsize+1;
    val4 = val5;
    val5 = val6;
    val6 = src+i+1;
    val7 = val8;
    val8 = val9;
    val9 = src+i+xsize+1;
    if (i%xsize == 0 || i%xsize == xsize-1) continue;
    #ifndef MACOSX
    for (j=0;j<3;j++) 
    #else
    for (j=1;j<3;j+=2)
    #endif
    {
      res = m_imatrix[0]*(int)((unsigned char*)val1)[j];
      res += m_imatrix[1]*(int)((unsigned char*)val2)[j];
      res += m_imatrix[2]*(int)((unsigned char*)val3)[j];
      res += m_imatrix[3]*(int)((unsigned char*)val4)[j];
      res += m_imatrix[4]*(int)((unsigned char*)val5)[j];
      res += m_imatrix[5]*(int)((unsigned char*)val6)[j];
      res += m_imatrix[6]*(int)((unsigned char*)val7)[j];
      res += m_imatrix[7]*(int)((unsigned char*)val8)[j];
      res += m_imatrix[8]*(int)((unsigned char*)val9)[j];
      res*=m_irange;
      res>>=16;
      ((unsigned char*)dest)[i*2] = 128;
      ((unsigned char*)dest)[i*2+2] = 128;
      ((unsigned char*)dest)[i*2+j] = CLAMP(res);
    }
  }
  }
}


/////////////////////////////////////////////////////////
// rangeMess
//
/////////////////////////////////////////////////////////
void pix_convolve :: rangeMess(float range)
{
    m_irange = (int)(range*255.f);
    setPixModified();
}

/////////////////////////////////////////////////////////
// matrixMess
//
/////////////////////////////////////////////////////////
void pix_convolve :: matrixMess(int argc, t_atom *argv)
{
    if (argc != m_cols * m_rows)
    {
    	error("GEM: pix_convolve: matrix size not correct");
    	return;
    }

    int i;
    for (i = 0; i < argc; i++) m_imatrix[i] = (int)(atom_getfloat(&argv[i])*255.);


    setPixModified();
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_convolve :: obj_setupCallback(t_class *classPtr)
{
    class_addmethod(classPtr, (t_method)&pix_convolve::matrixMessCallback,
    	    gensym("matrix"), A_GIMME, A_NULL);
    class_addmethod(classPtr, (t_method)&pix_convolve::rangeMessCallback,
    	    gensym("ft1"), A_FLOAT, A_NULL);
    class_addmethod(classPtr, (t_method)&pix_convolve::chromaMessCallback,
    	    gensym("chroma"), A_FLOAT, A_NULL);
}
void pix_convolve :: matrixMessCallback(void *data, t_symbol *, int argc, t_atom *argv)
{
    GetMyClass(data)->matrixMess(argc, argv);
}
void pix_convolve :: rangeMessCallback(void *data, t_floatarg range)
{
    GetMyClass(data)->rangeMess((float)range);
}

void pix_convolve :: chromaMessCallback(void *data, t_floatarg value)
{
    GetMyClass(data)->m_chroma=(int)value;
}
