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

#include "pix_gain.h"

CPPEXTERN_NEW(pix_gain)

/////////////////////////////////////////////////////////
//
// pix_gain
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_gain :: pix_gain()
{
    inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"), gensym("ft1"));
    inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("list"), gensym("vec_gain"));
    m_gain[chRed] = m_gain[chGreen] = m_gain[chBlue] = m_gain[chAlpha] = 1.0f;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_gain :: ~pix_gain()
{ }

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void pix_gain :: processRGBAImage(imageStruct &image)
{
    int datasize =  image.xsize * image.ysize;
    unsigned char *pixels = image.data;

	while(datasize--)
	{
		int red   = (int)(pixels[chRed] * m_gain[chRed]);
		int green = (int)(pixels[chGreen] * m_gain[chGreen]);
		int blue  = (int)(pixels[chBlue] * m_gain[chBlue]);
		int alpha = (int)(pixels[chAlpha] * m_gain[chAlpha]);
		pixels[chRed]	= CLAMP(red);
		pixels[chGreen] = CLAMP(green);
		pixels[chBlue]	= CLAMP(blue);
		pixels[chAlpha] = CLAMP(alpha);
		pixels += 4;
	}
}

/////////////////////////////////////////////////////////
// processGrayImage
//
/////////////////////////////////////////////////////////
void pix_gain :: processGrayImage(imageStruct &image)
{
    int datasize =  image.xsize * image.ysize;
    unsigned char *pixels = image.data;

	while (datasize--)
	{
		int gray = (int)(pixels[chGray] * m_gain[chRed]);
		pixels[chGray] = CLAMP(gray);
		pixels++;
	}
}
/////////////////////////////////////////////////////////
// do the YUV processing here
//
/////////////////////////////////////////////////////////
void pix_gain :: processYUVImage(imageStruct &image)
{
  post("1111");
    int h,w,width;
    long src;
    float y1,y2,u,v;

    float Y=m_gain[0];
    float U=m_gain[1];
    float V=m_gain[2];
    post("---");
    
    width = image.xsize/2;
    for (h=0; h<image.ysize; h++){
      for(w=0; w<width; w++){
	post("w=%d\th=%d", w, h);
	//U
        if (image.data[src] > 128)
	  u = (image.data[src] - ((image.data[src] - 128) * (1 - U)));
        if (image.data[src] < 128)
	  u = ((128 - image.data[src]) * (1 - U)) + image.data[src];
        image.data[src] = (unsigned char)CLAMP(u);
	
        //Y1
        y1 = image.data[src+1] * Y;
        image.data[src+1] = (unsigned char)CLAMP(y1);
	post("v");
	
	//V
	//v = image.data[src+2] * G;
	if (image.data[src+2] > 128)
	  v = ((float)image.data[src+2] - ((image.data[src+2] - 128) * (1 - V)));
	if (image.data[src+2] < 128)
	  v = (((128 - image.data[src+2]) * (1 - V)) + image.data[src+2]);
        image.data[src+2] = (unsigned char)CLAMP(v);
	post("y2");
        //Y2
        y2 = image.data[src+3] * Y;
	image.data[src+3] = (unsigned char)CLAMP(y2);
        src+=4;
	
      }
    }
    post("::::");
}

/////////////////////////////////////////////////////////
// vecGainMess
//
/////////////////////////////////////////////////////////
void pix_gain :: vecGainMess(int argc, t_atom *argv)
{
    if (argc >= 4) m_gain[chAlpha] = atom_getfloat(&argv[3]);
    else if (argc == 3) m_gain[chAlpha] = 1.0;
    else if (argc == 1) m_gain[chRed] = m_gain[chGreen] = m_gain[chBlue] = m_gain[chAlpha] = atom_getfloat(argv);
    else {
      error("GEM: pix_gain: not enough gain values");
      return;
    }
    m_gain[chRed] = atom_getfloat(&argv[0]);
    m_gain[chGreen] = atom_getfloat(&argv[1]);
    m_gain[chBlue] = atom_getfloat(&argv[2]);
    setPixModified();
}

/////////////////////////////////////////////////////////
// floatGainMess
//
/////////////////////////////////////////////////////////
void pix_gain :: floatGainMess(float gain)
{
    // assumption that the alpha should be one
    m_gain[chAlpha] = 1.0f;
    m_gain[chRed] = m_gain[chGreen] = m_gain[chBlue] = FLOAT_CLAMP(gain);
    setPixModified();
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_gain :: obj_setupCallback(t_class *classPtr)
{
    class_addmethod(classPtr, (t_method)&pix_gain::vecGainMessCallback,
    	    gensym("vec_gain"), A_GIMME, A_NULL);
    class_addmethod(classPtr, (t_method)&pix_gain::floatGainMessCallback,
    	    gensym("ft1"), A_FLOAT, A_NULL);
}
void pix_gain :: vecGainMessCallback(void *data, t_symbol *, int argc, t_atom *argv)
{
    GetMyClass(data)->vecGainMess(argc, argv);
}
void pix_gain :: floatGainMessCallback(void *data, t_floatarg gain)
{
    GetMyClass(data)->floatGainMess((float)gain);
}
