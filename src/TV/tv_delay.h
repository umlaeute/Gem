/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    play puzzle with a sequence of pixBufs

    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyleft  (l) 2001 IOhannes m zmölnig. zmoelnig@iem.kug.ac.at
    Copyleft (l) 2002 James Tittle & Chris Clepper
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_TV_DELAY_H_
#define INCLUDE_TV_DELAY_H_

#include "Base/GemPixObj.h"

#define DEFAULT_MAX_FRAMES 256
/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  tv_delay
  
  change the pixBuf into dots
  
  KEYWORDS
  pix
    
  DESCRIPTION
   
  -----------------------------------------------------------------*/
class GEM_EXTERN tv_delay : public GemPixObj
{
  CPPEXTERN_HEADER(tv_delay, GemPixObj)

    public:

  //////////
  // Constructor
  tv_delay(t_float &f);
    	
 protected:
    	
  //////////
  // Destructor
  virtual ~tv_delay();

  //////////
  // Do the processing
  virtual void 	processImage(imageStruct &image);

  imageStruct    myImage;

  virtual void  delayMess(int frames);

  int m_maxframes, m_curframe;
  int m_frame;
   

 private:
  
  //////////
  // static member functions
  static void delayMessCallback(void *data, t_floatarg);
};

#endif	// for header file
