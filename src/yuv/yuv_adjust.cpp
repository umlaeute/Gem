/*
 *  yuv_adjust.cpp
 *  gem_darwin
 *
 *  Created by chris clepper on Mon Oct 07 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#include "yuv_adjust.h"
#define clamp(x) ((x < 0) ? 0 : (x > 255) ? 255 : x)
CPPEXTERN_NEW(yuv_adjust)

/////////////////////////////////////////////////////////
//
// yuv_adjust
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
yuv_adjust :: yuv_adjust()
{
//post("yuv_adjust: i am being constructed");
Y=0;
U=0;
V=0;
inletY = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("Yvalue"));
inletU = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("Uvalue"));
inletV = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("Vvalue"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
yuv_adjust :: ~yuv_adjust()
{

}

/////////////////////////////////////////////////////////
// messages
//
/////////////////////////////////////////////////////////

void yuv_adjust :: yMess(float size)
{
    Y = (short)size;
    //Y = size;
    setModified();
}

void yuv_adjust :: uMess(float size)
{
    U = (short)size;
   // U = size;
    setModified();
}

void yuv_adjust :: vMess(float size)
{
    V = (short)size;
   // V = size;
    setModified();
}

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void yuv_adjust :: processImage(imageStruct &image)
{
}


/////////////////////////////////////////////////////////
// do the YUV processing here
//
/////////////////////////////////////////////////////////
void yuv_adjust :: processYUVImage(imageStruct &image)
{
    int h,w;
    long src, width;

src = 0;

#ifdef ALTIVEC
width = image.xsize/8; //for altivec
//width = image.xsize/2; //for scalar
//format is U Y V Y
/* start of working altivec function */
    union
    {
        //unsigned int	i;
        short	elements[8];
        //vector signed char v;
        vector	short v;
    }transferBuffer;
    
    //vector unsigned char c;
    vector signed short c, hi, lo;
    vector unsigned char zero = vec_splat_u8(0);
    vector unsigned char *inData = (vector unsigned char*) image.data;

    //Write the pixel (pair) to the transfer buffer
    //transferBuffer.i = (U << 24) | (Y << 16) | (V << 8 ) | Y;
    transferBuffer.elements[0] = U;
    transferBuffer.elements[1] = Y;
    transferBuffer.elements[2] = V;
    transferBuffer.elements[3] = Y;
    transferBuffer.elements[4] = U;
    transferBuffer.elements[5] = Y;
    transferBuffer.elements[6] = V;
    transferBuffer.elements[7] = Y;

    //Load it into the vector unit
    c = transferBuffer.v;

    //Splat the pixel (pair) across the entire vector
    //c =  vec_splat( c, 0 );
    //c = (vector unsigned char) vec_splat( (vector unsigned int) c, 0 );
    //c = vec_mergeh( vec_splat_u8(0), (vector unsigned char) c );

    //Do saturated addition between c and the pixel buffer
    //for (h=0; h<image.ysize; h++){
    //    for(w=0; w<image.xsize/8; w++)
    //    {
    //        inData[0] = vec_adds( inData[0], c );
    //        inData++;
    //    }
   // }
   
   	UInt32			prefetchSize = GetPrefetchConstant( 16, 1, 256 );
	vec_dst( inData, prefetchSize, 0 );
        
    for ( h=0; h<image.ysize; h++){
        for (w=0; w<width; w++)
        {
        
	vec_dst( inData, prefetchSize, 0 );
        
            //expand the UInt8's to short's
            hi = (vector signed short) vec_mergeh( zero, inData[0] );
            lo = (vector signed short) vec_mergel( zero, inData[0] );
            
            //add the constant to it
            hi = vec_add( hi, c );
            lo = vec_add( lo, c );
            
            //pack the result back down, with saturation
            inData[0] = vec_packsu( hi, lo );
            
            inData++;
        }
        vec_dss( 0 );
}  /*end of working altivec function */

#else 
/* start of scalar */
for (h=0; h<image.ysize; h++){
    for(w=0; w<image.xsize/2; w++){
        image.data[src] = clamp( image.data[src] + U );
        image.data[src+1] = clamp( image.data[src+1] + Y );
        image.data[src+2] = clamp( image.data[src+2] + V );
        image.data[src+3] = clamp( image.data[src+3] + Y );

        src+=4;
    }
} 
#endif
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void yuv_adjust :: obj_setupCallback(t_class *classPtr)
{
class_addmethod(classPtr, (t_method)&yuv_adjust::yMessCallback,
    	    gensym("Yvalue"), A_FLOAT, A_NULL);
            
class_addmethod(classPtr, (t_method)&yuv_adjust::uMessCallback,
    	    gensym("Uvalue"), A_FLOAT, A_NULL);

class_addmethod(classPtr, (t_method)&yuv_adjust::vMessCallback,
    	    gensym("Vvalue"), A_FLOAT, A_NULL);

}

void yuv_adjust :: yMessCallback(void *data, t_floatarg size)
{
    GetMyClass(data)->yMess((float)size);
}

void yuv_adjust :: uMessCallback(void *data, t_floatarg size)
{
    GetMyClass(data)->uMess((float)size);
}

void yuv_adjust :: vMessCallback(void *data, t_floatarg size)
{
    GetMyClass(data)->vMess((float)size);
}

