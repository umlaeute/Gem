/*
 *  yuv_pixel_data.cpp
 *  gem_darwin
 *
 *  Created by chris clepper on Mon Oct 07 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#include "yuv_pixel_data.h"

CPPEXTERN_NEW(yuv_pixel_data)

/////////////////////////////////////////////////////////
//
// yuv_pixel_data
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
yuv_pixel_data :: yuv_pixel_data()
{
    m_processOnOff=0;
    m_width =1;
    m_height = 1;
    m_YOut = outlet_new(this->x_obj, 0);
    m_UOut = outlet_new(this->x_obj, 0);
    m_VOut = outlet_new(this->x_obj, 0);
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
yuv_pixel_data :: ~yuv_pixel_data()
{
    outlet_free(m_YOut);
    outlet_free(m_UOut);
    outlet_free(m_VOut);
}

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void yuv_pixel_data :: processImage(imageStruct &image)
{

}

/////////////////////////////////////////////////////////
// do the YUV processing here
//
/////////////////////////////////////////////////////////
void yuv_pixel_data :: processYUVImage(imageStruct &image)
{
    long pos;
    
//check for odd or even pixel
    if (m_height > image.ysize){
        m_height = image.ysize;
        }
    if (m_height < 1){
        m_height = 1;
        }
    if (m_width > image.xsize){
        m_width = image.xsize;
        }
    if (m_width < 1){
        m_width = 1;
        }
    pos = ((m_height * image.xsize * 2) + m_width * 2) ;
    
    if ( m_width % 2) { //odd
        YVal = image.data[pos + 1];
        UVal = image.data[pos - 2];
        VVal = image.data[pos];
    }else{
        YVal = image.data[pos + 1];
        UVal = image.data[pos];
        VVal = image.data[pos + 2];
    }
    
/*
    YVal = image.data[(m_height*m_width)+1];
    UVal = image.data[m_height*m_width];
    VVal = image.data[(m_height*m_width)+2];
*/
    outlet_float(m_YOut, (t_float)YVal);
    outlet_float(m_UOut, (t_float)UVal);
    outlet_float(m_VOut, (t_float)VVal);
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void yuv_pixel_data :: obj_setupCallback(t_class *classPtr)
{
    class_addmethod(classPtr, (t_method)&yuv_pixel_data::locationCallback,
		  gensym("location"), A_DEFFLOAT, A_DEFFLOAT, A_NULL);
}

void yuv_pixel_data :: locationCallback(void *data, t_floatarg Wval, t_floatarg Hval)
{
  GetMyClass(data)->m_height=((long)Hval);
  GetMyClass(data)->m_width=((long)Wval);

}
