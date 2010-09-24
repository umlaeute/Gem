////////////////////////////////////////////////////////
//
// pix_mano - an object to track a hand and its fingers
//
// Jaime Oliver,  
//
// jaime.oliver2@gmail.com
// this is still a testing version, no guarantees...
//
// the license for this object is GNU 
//
// for more information: www.jaimeoliver.pe
// Silent Percussion Project
//
// GEM - Graphics Environment for Multimedia
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//    Copyright (c) 1997-1998 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "LICENSE.txt" in this distribution.
//
/////////////////////////////////////////////////////////

#ifndef INCLUDE_pix_mano_H_
#define INCLUDE_pix_mano_H_

#include "Base/GemPixObj.h"

class GEM_EXTERN pix_mano : public GemPixObj
{
    CPPEXTERN_HEADER(pix_mano, GemPixObj)

    public:

        //////////
        // Constructor
    	pix_mano();
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~pix_mano();

    	//////////
    	// Do the processing
    	virtual void 	processRGBAImage(imageStruct &image);
    	
    	//////////
    	// Do the processing
    	virtual void 	processGrayImage(imageStruct &image);
        	
        //////////
    	// Do the processing
    	virtual void 	processYUVImage(imageStruct &image);
		
#ifdef __VEC__
	//////////
    	// Do the processing
    	virtual void 	processYUVAltivec(imageStruct &image);
#endif  
    	//////////
    	// Set the new threshold vector
    	void	    	vecBoundsMess(int argc, t_atom *argv);
    	
    	//////////
    	// Set the new threshold value
    	void	    	vecThreshMess(int argc, t_atom *argv);
		
		//////////
    	// Set the new threshold value
    	void	    	vecParamsMess(int argc, t_atom *argv);
    	
    	//////////
    	// The new color
		t_outlet *outlet1;
		t_outlet *outlet2;
		t_outlet *outlet3;
		t_outlet *outlet4;
		t_outlet *outlet5;
		t_outlet *outlet6;
		int head, bottom, mode, left, right, pixavg, pixtip, min_entry_size, min_perim, pixsamp;
		float thresh, tip_scalar;
		float partialx_prev[10000], partialy_prev[10000];
		int Xsize, Ysize, hop, prev_tip;
		float tp_i[50], tp_x[50], tp_y[50], tp_m[50], tp_a[50], tp_s[50];
    
    private:
    
    	//////////
    	// Static member functions
    	static void 	vecBoundsMessCallback(void *data, t_symbol *, int argc, t_atom *argv);
    	static void 	vecThreshMessCallback(void *data, t_symbol *, int argc, t_atom *argv);
		static void 	vecParamsMessCallback(void *data, t_symbol *, int argc, t_atom *argv);
};

#endif	// for header file
