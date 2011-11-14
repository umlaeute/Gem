/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

Detects red, green, blue, yellow, white, black in an image

Copyright (c) 2011 Ricardo Fabbri. labmacambira.sf.net rfabbri@gmail.com
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
	 
-----------------------------------------------------------------*/
#ifndef _INCLUDE__GEM_PIXES_PIX_COLOR_DETECT_H_
#define _INCLUDE__GEM_PIXES_PIX_COLOR_DETECT_H_

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_colorclassify
    
    Detects red, green, blue, yellow, white, black in an image

KEYWORDS
    pix, color
    
DESCRIPTION
    This algorithm uses interval logic in the the HSV colorspace to detect
    colors in 6 clases: red, green, blue, yellow, white, and black, together
    with a 7th label 'not sure'. The 7th label can be useful for skin detection.
    The algorithm was tuned to maximize performance on a ground-truth database
    of over 200 exemplars from color paper shown to a consumer webcam under
    different lighting conditions. The philosophy of the algorithm is to provide
    high precision at moderate recall, meaning it will only output a color class
    if it is sure of the result, and will output a flag if unsure, so that a
    fallback algorithm can be useful (e.g. lookup the color from previous video frames).  

TODO
  - Enable the output of guesses for the 'not sure' class.
  - Allow further manipulation of the intervals / sensitivity from Pd

SEE ALSO
    General-purpose techniques such as statistical classifiers will allow one to
    detect the color classes in an image. Pd already has this, but it is usually
    not as effective as the present data-driven hand-tuned approach where highly
    nonlinear and nonuniform aspects human percetion of a given number of
    classes is in play. Another advantage of the present approach

    We also compared to a 'naive' algorithm which blindly computes the minimum
    distance of a given color to a representative of the color classes. On our
    color database, the present approach outperformed the naive one by a large
    factor.

AUTHOR
  Ricardo Fabbri. labmacambira.sf.net rfabbri@gmail.com
-----------------------------------------------------------------*/
class pix_colorclassify : public GemPixObj
{
    CPPEXTERN_HEADER(pix_colorclassify, GemPixObj);

    public:

	    //////////
	    // Constructor
    	pix_colorclassify();
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~pix_colorclassify();

    	//////////
    	// Do the processing
    	virtual void 	processRGBAImage(imageStruct &image);
      // virtual void 	processGrayImage(imageStruct &image);
    	// virtual void 	processYUVImage(imageStruct &image);
	
    private:
};

#endif	// for header file
