/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

set the Alpha-channel of a pixBlock depending on the difference between the current and the last pixBlock
  
Copyright (c) 1997-1999 Mark Danks. mark@danks.org
Copyright (c) Günther Geiger. geiger@epy.co.at
Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
	
-----------------------------------------------------------------*/

/*-----------------------------------------------------------------
tv_movement

  movement-transform a series of pictures (especially movies, videos,...)
  you have to use pix_film instead of pix_movie to make this work with prerecorded videos
 
  2803:forum::für::umläute:2000
  0409:forum::für::umläute:2000
  1801:forum::für::umläute:2001  added the second mode
  IOhannes m zmoelnig
  mailto:zmoelnig@iem.kug.ac.at
  
  this code is published under the Gnu GeneralPublicLicense that should be distributed with gem & pd
	  
-----------------------------------------------------------------*/

#ifndef INCLUDE_TV_MOVEMENT_H_
#define INCLUDE_TV_MOVEMENT_H_

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
tv_movement

  KEYWORDS
  pix
  
	DESCRIPTION
	
-----------------------------------------------------------------*/
class GEM_EXTERN tv_movement : public GemPixObj
{
    CPPEXTERN_HEADER(tv_movement, GemPixObj)
		
public:
	
	//////////
	// Constructor
	tv_movement();
	
protected:
	
	//////////
	// Destructor
	virtual ~tv_movement();
	
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
	// the huge double buffer and other tx-formation
	imageStruct    buffer;
	int            bufcount;    // where to read/write
	
	//////////
	// the movement-mode
	unsigned char  treshold;
	
	//////////
	// the methods
	static void treshMessCallback(void *data, t_floatarg ftresh);
	
};

#endif	// for header file
