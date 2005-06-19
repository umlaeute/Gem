/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia



    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyleft  (l) 2001 IOhannes m zmölnig. zmoelnig@iem.kug.ac.at
    Copyleft (l) 2002 James Tittle & Chris Clepper
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_PIX_BUFFER_H_
#define INCLUDE_PIX_BUFFER_H_

#ifndef HELPSYMBOL_BASE
# define HELPSYMBOL_BASE "Gem/"
#endif
#include "Base/CPPExtern.h"
#include "Base/GemPixUtil.h"

#define DEFAULT_NUM_FRAMES 100


/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  pix_buffer
  
  this is a buffer object for storing frames of video
  
  KEYWORDS
  pix
    
  DESCRIPTION
   
  -----------------------------------------------------------------*/
class GEM_EXTERN pix_buffer : public CPPExtern
{
  CPPEXTERN_HEADER(pix_buffer, CPPExtern)

    public:

  //////////
  // Constructor
  pix_buffer(t_symbol *s,t_float f);
  virtual bool  putMess(imageStruct*img,int pos);
  virtual imageStruct* getMess(int pos);
  virtual int numFrames(void);
    	
 protected:
    	
  //////////
  // Destructor
  virtual ~pix_buffer();

  //////////
  // Do the processing

  imageStruct    *m_buffer;

  virtual void  allocateMess(int,int,int);
  virtual void  bangMess();

  int m_numframes;
  t_symbol *m_bindname;
   

 private:
  
  //////////
  // static member functions
  static void allocateMessCallback(void *data, t_floatarg x, t_floatarg y, t_floatarg c);
  static void bangMessCallback(void *data);
};

#endif	// for header file
