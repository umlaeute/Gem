/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

  Change rtx-transform a series of images (eg: a video or a film)
  
    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyright (c) G�nther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f�r::uml�ute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

pix_rtx

  rtx-transform a series of pictures (especially movies, videos,...)
  you have to use pix_film instead of pix_movie to make this work with prerecorded videos
  
  2803:forum::f�r::uml�ute:2000
  0409:forum::f�r::uml�ute:2000
  1801:forum::f�r::uml�ute:2001  added the second mode
  IOhannes m zmoelnig
  mailto:zmoelnig@iem.kug.ac.at
	
  this code is published under the Gnu GeneralPublicLicense that should be distributed with gem & pd
	  
-----------------------------------------------------------------*/

#ifndef INCLUDE_PIX_RTX_H_
#define INCLUDE_PIX_RTX_H_

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
pix_rtx

  KEYWORDS
  pix
  
	DESCRIPTION
	
-----------------------------------------------------------------*/
class GEM_EXTERN pix_rtx : public GemPixObj
{
    CPPEXTERN_HEADER(pix_rtx, GemPixObj)
		
public:
	
	//////////
	// Constructor
	pix_rtx();
	
protected:
	
	//////////
	// Destructor
	virtual ~pix_rtx();
	
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
	// the rtx-mode
	bool			 mode;

	//////////
	// fill buffer with current pixbuff ?
	bool                     set_buffer;
	
	//////////
	// the methods
	static void modeMessCallback(void *data, t_floatarg newmode);
	static void clearMessCallback(void *data);
	static void setMessCallback(void *data);
	
};

#endif	// for header file
