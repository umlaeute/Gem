/*
 *  yuv_posterize.cpp
 *  gem_darwin
 *  tigital@mac.com
 *
 *  Created by Jamie Tittle on Fri Oct 18 2002.
 *  Copyleft (l) 2002 tigital. All rights reserved.
 *
 */

#include <stdint.h>
#include "yuv_posterize.h"

CPPEXTERN_NEW(yuv_posterize)

/////////////////////////////////////////////////////////
//
// yuv_posterize
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
yuv_posterize :: yuv_posterize()
{
    inletF = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("factor"));
    inletL = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("limit"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
yuv_posterize :: ~yuv_posterize()
{

}

void yuv_posterize :: factorMess(float f)
{
    factor = (int)f;
    setModified();
}

void yuv_posterize :: limitMess(float l)
{
    limit = (int)l;
    setModified();
}

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void yuv_posterize :: processImage(imageStruct &image)
{
    post("yuv_posterize: RGB not supported :-P");
}


/////////////////////////////////////////////////////////
// do the YUV processing here
//
/////////////////////////////////////////////////////////
void yuv_posterize :: processYUVImage(imageStruct &image)
{
    int h,w;
    long src;
    src = 0;
//format is U Y V Y

    if (factor <= 0 || factor > 255) factor = 1;
    switch (limit) {
        case 1:
            for (h=0; h<image.ysize; h++){
                for(w=0; w<image.xsize/2; w++){
                image.data[src+1] = image.data[src+1] - ( image.data[src+1] % (256/factor));
                image.data[src+3] = image.data[src+3] - ( image.data[src+3] % (256/factor));
                if ( image.data[src+1] > 100 && image.data[src+1] < 150) image.data[src+1]=235;
                if ( image.data[src+3] > 100 && image.data[src+3] < 150) image.data[src+3]=235;
                image.data[src] = image.data[src] - ( image.data[src] % (256/factor));
                    image.data[src+2] = image.data[src+2] - ( image.data[src+2] % (256/factor));
                src += 4;
                }
            }
            break;
        case 2:
            for (h=0; h<image.ysize; h++){
                for(w=0; w<image.xsize/2; w++){
                image.data[src+1] = image.data[src+1] - ( image.data[src+1] % (256/factor));
                image.data[src+3] = image.data[src+3] - ( image.data[src+3] % (256/factor));
                if ( image.data[src+1] > 100 && image.data[src+1] < 150) image.data[src+1]=235;
                if ( image.data[src+3] > 100 && image.data[src+3] < 150) image.data[src+3]=235;
                src += 4;
                }
            }
            break;
        case 3:
            for (h=0; h<image.ysize; h++){
                for(w=0; w<image.xsize/2; w++){
                image.data[src+1] = image.data[src+1] - ( image.data[src+1] % (256/factor));
                image.data[src+3] = image.data[src+3] - ( image.data[src+3] % (256/factor));
                if ( image.data[src+1] > 100 && image.data[src+1] < 150) image.data[src+1]=235;
                if ( image.data[src+3] > 100 && image.data[src+3] < 150) image.data[src+3]=235;
                image.data[src+3] = image.data[src+3] - ( image.data[src+3] % (256/factor));             
                src += 4;
                }
            }
            break;
        case 4:
            for (h=0; h<image.ysize; h++){
                for(w=0; w<image.xsize/2; w++){
                image.data[src+1] = image.data[src+1] - ( image.data[src+1] % (256/factor));
                image.data[src+3] = image.data[src+3] - ( image.data[src+3] % (256/factor));
                if ( image.data[src+1] > 100 && image.data[src+1] < 150) image.data[src+1]=235;
                if ( image.data[src+3] > 100 && image.data[src+3] < 150) image.data[src+3]=235;
                image.data[src] = image.data[src] - ( image.data[src] % (256/factor));              
                src += 4;
                }
            }
            break;
        break;
        default:
            for (h=0; h<image.ysize; h++){
                for(w=0; w<image.xsize/2; w++){
                image.data[src+1] = image.data[src+1] - ( image.data[src+1] % (256/factor));
                image.data[src+3] = image.data[src+3] - ( image.data[src+3] % (256/factor));
                if ( image.data[src+1] > 100 && image.data[src+1] < 150) image.data[src+1]=235;
                if ( image.data[src+3] > 100 && image.data[src+3] < 150) image.data[src+3]=235;
                src += 4;
                }
            } 
            break;                                                                
    }	                    
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void yuv_posterize :: obj_setupCallback(t_class *classPtr)
{
class_addmethod(classPtr, (t_method)&yuv_posterize::factorMessCallback,
    	    gensym("factor"), A_FLOAT, A_NULL);
            
class_addmethod(classPtr, (t_method)&yuv_posterize::limitMessCallback,
    	    gensym("limit"), A_FLOAT, A_NULL);

}

void yuv_posterize :: factorMessCallback(void *data, t_floatarg size)
{
    GetMyClass(data)->factorMess((float)size);
}

void yuv_posterize :: limitMessCallback(void *data, t_floatarg size)
{
    GetMyClass(data)->limitMess((float)size);
}
