////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-2000 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////
//
//  pix_rectangle
//
//  2002:forum::für::umläute:2000
//  iohannes m zmoelnig
//  mailto:zmoelnig@iem.mhsg.ac.at
//
/////////////////////////////////////////////////////////


#include "pix_rectangle.h"

CPPEXTERN_NEW(pix_rectangle)

/////////////////////////////////////////////////////////
//
// pix_rectangle
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_rectangle :: pix_rectangle()
{
    inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("list"), gensym("coord"));
    inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("list"), gensym("color"));

    m_color[chRed] = m_color[chGreen] = m_color[chBlue] = m_color[chAlpha] = 255;
	m_lower_left[0] = m_lower_left[1] = m_upper_right[0] = m_upper_right[1] = 0;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_rectangle :: ~pix_rectangle()
{}

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void pix_rectangle :: processRGBAImage(imageStruct &image)
{
	int bytesize = 4 * sizeof(unsigned char); // 4 for RGBA
	int rowsize  = image.xsize * bytesize;
    unsigned char *pixels = image.data;
	int col, row;

	if (m_upper_right[0] > image.xsize)
	{
		m_upper_right[0] = image.xsize;
		if (m_lower_left[0] > image.xsize) 
			m_lower_left[0] = image.xsize;
	}

	if (m_upper_right[1] > image.ysize)
	{
		m_upper_right[1] = image.ysize;
		if (m_lower_left[1] > image.ysize) 
			m_lower_left[1] = image.ysize;
	}

	row = (m_upper_right[1] - m_lower_left[1]);

	while (row--)
	{
		pixels = image.data + rowsize * (m_lower_left[1] + row) + m_lower_left[0] * bytesize;
		col = (m_upper_right[0] - m_lower_left[0]);
		
		while (col--)
		{
			pixels[chRed]   = m_color[chRed];
			pixels[chGreen] = m_color[chGreen];
			pixels[chBlue]  = m_color[chBlue];
			pixels[chAlpha] = m_color[chAlpha];

			pixels += 4;
		}
	}
}

/////////////////////////////////////////////////////////
// processGrayImage
//
/////////////////////////////////////////////////////////
void pix_rectangle :: processGrayImage(imageStruct &image)
{
    int bytesize = 1 * sizeof(unsigned char); // 1 for grayscale
    int rowsize  = image.xsize * bytesize;
    unsigned char *pixels = image.data;
    int col, row;
	
    if (m_upper_right[0] > image.xsize)
    {
        m_upper_right[0] = image.xsize;
        if (m_lower_left[0] > image.xsize) 
                m_lower_left[0] = image.xsize;
    }

    if (m_upper_right[1] > image.ysize)
    {
        m_upper_right[1] = image.ysize;
        if (m_lower_left[1] > image.ysize) 
                m_lower_left[1] = image.ysize;
    }

    row = (m_upper_right[1] - m_lower_left[1]);

    while (row--)
    {
        pixels = image.data + rowsize * (m_lower_left[1] + row) + m_lower_left[0] * bytesize;
        col = (m_upper_right[0] - m_lower_left[0]);
		
        while (col--)
        {
            pixels[chGray] = m_color[chGray];
            pixels++;
        }
    }
}

/////////////////////////////////////////////////////////
// vecColorMess
//
/////////////////////////////////////////////////////////
void pix_rectangle :: vecColorMess(int argc, t_atom *argv)
{
	float alpha, red, green, blue;

    if (argc >= 4)
		alpha = atom_getfloat(&argv[3]);
    else if (argc == 3)
		alpha = 1.f;
    else
    {
    	error("GEM: pix_rectangle: not enough color values");
    	return;
    }
    red = atom_getfloat(&argv[0]);
    green = atom_getfloat(&argv[1]);
    blue = atom_getfloat(&argv[2]);

	m_color[chRed]   = (unsigned char)(255.*red);
	m_color[chGreen] = (unsigned char)(255.*green);
	m_color[chBlue]  = (unsigned char)(255.*blue);
	m_color[chAlpha] = (unsigned char)(255.*alpha);

    setPixModified();
}

/////////////////////////////////////////////////////////
// vecCoordMess
//
/////////////////////////////////////////////////////////
void pix_rectangle :: vecCoordMess(int argc, t_atom *argv)
{
	int X1, Y1, X2, Y2;

    if (argc < 4)
	{
    	error("GEM: pix_rectangle: not enough coordinates");
    	return;
    }
    X1 = (int)atom_getfloat(&argv[0]);
    Y1 = (int)atom_getfloat(&argv[1]);
    X2 = (int)atom_getfloat(&argv[2]);
	Y2 = (int)atom_getfloat(&argv[3]);

	// check if within range
	if (X1 < 0)
		X1 = 0;
	if (X2 < 0)
		X2 = 0;
	if (Y1 < 0)
		Y1 = 0;
	if (Y2 < 0)
		Y2 = 0;

	// set
	m_lower_left [0] = (X1<X2)?X1:X2;
	m_lower_left [1] = (Y1<Y2)?Y1:Y2;
	m_upper_right[0] = (X1>X2)?X1:X2;
	m_upper_right[1] = (Y1>Y2)?Y1:Y2;

    setPixModified();
}


/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_rectangle :: obj_setupCallback(t_class *classPtr)
{
    class_addmethod(classPtr, (t_method)&pix_rectangle::vecCoordMessCallback,
    	    gensym("coord"), A_GIMME, A_NULL);
    class_addmethod(classPtr, (t_method)&pix_rectangle::vecColorMessCallback,
    	    gensym("color"), A_GIMME, A_NULL);

}
void pix_rectangle :: vecCoordMessCallback(void *data, t_symbol *, int argc, t_atom *argv)
{
    GetMyClass(data)->vecCoordMess(argc, argv);
}
void pix_rectangle :: vecColorMessCallback(void *data, t_symbol *, int argc, t_atom *argv)
{
    GetMyClass(data)->vecColorMess(argc, argv);
}
