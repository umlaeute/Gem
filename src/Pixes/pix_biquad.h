/*-----------------------------------------------------------------
  LOG
  GEM - Graphics Environment for Multimedia

  apply a 2p2z-filter on a sequence of pixBlocks
  
  Copyright (c) 1997-1999 Mark Danks. mark@danks.org
  Copyright (c) Günther Geiger. geiger@epy.co.at
  Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
	
  -----------------------------------------------------------------*/

/*-----------------------------------------------------------------
  tv_biquad

  IOhannes m zmoelnig
  mailto:zmoelnig@iem.kug.ac.at
	
  this code is published under the Gnu GeneralPublicLicense that should be distributed with gem & pd
	  
  -----------------------------------------------------------------*/

#ifndef INCLUDE_PIX_BIQUAD_H_
#define INCLUDE_PIX_BIQUAD_H_

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  pix_biquad

  KEYWORDS
  pix
  
  DESCRIPTION
	
  -----------------------------------------------------------------*/
class GEM_EXTERN pix_biquad : public GemPixObj
{
  CPPEXTERN_HEADER(pix_biquad, GemPixObj)
		
    public:
	
  //////////
  // Constructor
  pix_biquad();
	
 protected:
	
  //////////
  // Destructor
  virtual ~pix_biquad();
	
  //////////
  // create a buffer that fits to the current imageSize
  virtual void   create_buffer(imageStruct image);
	
  //////////
  // delete the buffer
  virtual void   delete_buffer();
	
  //////////
  // clear the buffer
  virtual void   clear_buffer();

  //////////
  // Do the processing
  virtual void 	processImage(imageStruct &image);

  //////////
  // set-flag
  bool set;
	
  //////////
  // the image-latches
  imageStruct    prev;
  imageStruct	 last;
	
  //////////
  // the biquad-factors
  t_float ff1, ff2, ff3, fb1, fb2, fb0;
  
  int m_mode;
	
  //////////
  // the methods
  static void setMessCallback(void *data);
  static void modeMessCallback(void *data,float value);
  static void faktorMessCallback(void *data, t_symbol *s, int argc, t_atom* argv);

};

#endif	// for header file
