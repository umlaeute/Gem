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
  : m_matrix(NULL), m_imatrix(NULL)
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
    m_range = 1.0;
    m_irange = 255;
    m_matrix = new float[m_rows * m_cols];
    m_imatrix = new signed short[m_rows * m_cols];

    // zero out the matrix
    for (int i = 0; i < m_cols * m_rows; i++) m_matrix[i] = 0.0;
    for (int i = 0; i < m_cols * m_rows; i++) m_imatrix[i] = 0;
    // insert a one for the default center value (identity matrix)
    m_matrix[ ((m_cols / 2 + 1) * m_rows) + (m_rows / 2 + 1) ] = 1.0;
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
  post("convolution...%x %x", m_matrix, m_imatrix);
    if (m_matrix)delete [] m_matrix;
    if (m_imatrix)delete [] m_imatrix;
    post("done...");
  
}

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////

#ifndef MMX
#else
void pix_convolve :: calculate3x3(imageStruct &image,imageStruct &tempImg)
{
  int i;
  int j;
  int k;
  int xsize =  tempImg.xsize;
  int ysize =  tempImg.ysize;
  int size = xsize*ysize - xsize-1;

  int* src = (int*) tempImg.data;
  int* dest = (int*)image.data;
  
  //  MMXSTART;

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
    for (j=0;j<3;j++) {
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
      ((unsigned char*)dest)[i*4+j] = CLAMP(res);
    }

  }

  //  MMXDONE;
}
#endif


#define MMULT(a,b) (a*b>>8)
void pix_convolve :: processImage(imageStruct &image)
{
    image.copy2Image(&tempImg);
    int initX = m_rows / 2;
    int initY = m_cols / 2;
    int maxX = tempImg.xsize - initX;
    int maxY = tempImg.ysize - initY;
    int xTimesc = tempImg.xsize * tempImg.csize;
    int initOffset = initY * xTimesc + initX * tempImg.csize;
#ifndef MMX
    for (int y = initY; y < maxY; y++)
    {
        int realY = y * xTimesc;
        int offsetY = realY - initOffset;

    	for (int x = initX; x < maxX; x++)
    	{
    	    int realPos = x * tempImg.csize + realY;
            int offsetXY = x * tempImg.csize + offsetY;

    	    // skip the alpha value
    	    for (int c = 0; c < 3; c++)
    	    {
    		    int new_val = 0;
                int offsetXYC = offsetXY + c;
    		    for (int matY = 0; matY < m_cols; matY++)
    		    {
    		        int offsetXYCMat = matY * xTimesc + offsetXYC;
    		        int realMatY = matY * m_rows;
    	    	    for (int matX = 0; matX < m_rows; matX++)
    	    	    {
                        new_val += tempImg.data[offsetXYCMat + matX * tempImg.csize] *
                                        m_matrix[realMatY + matX];
    	    	    }
    		    }
    		    image.data[realPos + c] = CLAMP(new_val/m_range);
    	    }
    	}
    }
    //delete [] tempImg.data;
#else
    if (m_rows == 3 && m_cols == 3) {
      calculate3x3(image,tempImg);
    }
    else {
      for (int y = initY; y < maxY; y++)
	{
	  int realY = y * xTimesc;
	  int offsetY = realY - initOffset;
	  
	  for (int x = initX; x < maxX; x++)
	    {
	      int realPos = x * tempImg.csize + realY;
	      int offsetXY = x * tempImg.csize + offsetY;
	      
	      // skip the alpha value
	      for (int c = 0; c < 3; c++)
		{
		  int new_val = 0;
		  int offsetXYC = offsetXY + c;
		  for (int matY = 0; matY < m_cols; matY++)
		    {
		      int offsetXYCMat = matY * xTimesc + offsetXYC;
		      int realMatY = matY * m_rows;
		      for (int matX = 0; matX < m_rows; matX++)
			{
			  new_val += MMULT(tempImg.data[offsetXYCMat + matX * tempImg.csize],
					   m_imatrix[realMatY + matX]);
			}
		    }
		  image.data[realPos + c] = CLAMP(new_val/m_range);
		}
	    }
	}
    }
#endif
}

void pix_convolve :: processYUVImage(imageStruct &image)
{
    post("pix_convolve: YUV not yet implemented :-(");
}
/////////////////////////////////////////////////////////
// rangeMess
//
/////////////////////////////////////////////////////////
void pix_convolve :: rangeMess(float range)
{
    m_range = range;
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
    
    for (int i = 0; i < argc; i++) m_matrix[i] = atom_getfloat(&argv[i]);
    for (int i = 0; i < argc; i++) m_imatrix[i] = (int)(atom_getfloat(&argv[i])*255.);

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
}
void pix_convolve :: matrixMessCallback(void *data, t_symbol *, int argc, t_atom *argv)
{
    GetMyClass(data)->matrixMess(argc, argv);
}
void pix_convolve :: rangeMessCallback(void *data, t_floatarg range)
{
    GetMyClass(data)->rangeMess((float)range);
}
