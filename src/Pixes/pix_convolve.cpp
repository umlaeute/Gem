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
    m_matrix = new float[m_rows * m_cols];

    // zero out the matrix
    for (int i = 0; i < m_cols * m_rows; i++) m_matrix[i] = 0.0;
    // insert a one for the default center value (identity matrix)
    m_matrix[ ((m_cols / 2 + 1) * m_rows) + (m_rows / 2 + 1) ] = 1.0;
    
    inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"), gensym("ft1"));
    inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("list"), gensym("matrix"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_convolve :: ~pix_convolve()
{
    delete [] m_matrix;
}

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void pix_convolve :: processImage(imageStruct &image)
{
    imageStruct tempImg;
    copy2Image(&tempImg, &image);
    
    int initX = m_rows / 2;
    int initY = m_cols / 2;
    int maxX = tempImg.xsize - initX;
    int maxY = tempImg.ysize - initY;
    int xTimesc = tempImg.xsize * tempImg.csize;
    int initOffset = initY * xTimesc + initX * tempImg.csize;

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
    		    float new_val = 0.0;
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
    delete [] tempImg.data;
}

/////////////////////////////////////////////////////////
// rangeMess
//
/////////////////////////////////////////////////////////
void pix_convolve :: rangeMess(float range)
{
    m_range = range;
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
