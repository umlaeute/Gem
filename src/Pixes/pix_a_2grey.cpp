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
//    Copyright (c) 2002 James Tittle & Chris Clepper
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////
//
//  pix_a_2grey
//
//  2002:forum::für::umläute:2000
//  iohannes m zmoelnig
//  mailto:zmoelnig@iem.mhsg.ac.at
//
/////////////////////////////////////////////////////////

#include "pix_a_2grey.h"

CPPEXTERN_NEW_WITH_ONE_ARG(pix_a_2grey, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// pix_a_2grey
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_a_2grey :: pix_a_2grey(t_floatarg alpha)
{
	m_mode = (int)(alpha * 255.f);
    inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"), gensym("ft1"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_a_2grey :: ~pix_a_2grey()
{ }

/////////////////////////////////////////////////////////
// alphaMess
//
/////////////////////////////////////////////////////////
void pix_a_2grey :: alphaMess(float alphaval)
{
	if (alphaval > 1.f)
		alphaval =  1.f;
	if (alphaval < -1.f)
		alphaval = -1.f;

	m_mode = (int)(alphaval*256.f);

	setPixModified();
}

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void pix_a_2grey :: processImage(imageStruct &image)
{
	if (!m_mode)
		return;

    unsigned char *pixels = image.data;
	int count  = image.ysize * image.xsize;

	if (m_mode < 0)
	{
		const int realVal = -m_mode;
		while (count--)
		{
			if (pixels[chAlpha] < realVal)
			{
   				const float grey = pixels[chRed] * 0.3086f + pixels[chGreen] * 0.6094f
							+ pixels[chBlue] * 0.0820f;
				pixels[chRed] = pixels[chGreen] = pixels[chBlue] = (unsigned char)grey;
			}
			pixels += 4;
		}
	}
	else
	{
		while (count--)
		{
			if (pixels[chAlpha] > m_mode)
			{
   				const float grey = pixels[chRed] * 0.3086f + pixels[chGreen] * 0.6094f
							+ pixels[chBlue] * 0.0820f;
				pixels[chRed] = pixels[chGreen] = pixels[chBlue] = (unsigned char)grey;
			}
			pixels += 4;
		}
	}
}

/////////////////////////////////////////////////////////
// processYUVImage
//
/////////////////////////////////////////////////////////
void pix_a_2grey :: processYUVImage(imageStruct &image)
{
    post("pix_a_2grey:  YUV not yet implemented :-(");
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_a_2grey :: obj_setupCallback(t_class *classPtr)
{
    class_addmethod(classPtr, (t_method)&pix_a_2grey::alphaMessCallback,
    	    gensym("ft1"), A_FLOAT, A_NULL);
    class_addmethod(classPtr, (t_method)&pix_a_2grey::alphaMessCallback,
    	    gensym("alpha"), A_FLOAT, A_NULL);
}

void pix_a_2grey :: alphaMessCallback(void *data, t_floatarg alphaval)
{
    GetMyClass(data)->alphaMess((float)alphaval);
}
