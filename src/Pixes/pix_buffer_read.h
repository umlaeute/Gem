/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia



    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyleft  (l) 2001 IOhannes m zmölnig. zmoelnig@iem.kug.ac.at
    Copyleft (l) 2002 James Tittle & Chris Clepper
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_PIX_BUFFER_READ_H_
#define INCLUDE_PIX_BUFFER_READ_H_

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  pix_buffer_read
  
  reads from a pix_buffer
  
  KEYWORDS
  pix
    
  DESCRIPTION
   
  -----------------------------------------------------------------*/
class GEM_EXTERN pix_buffer_read : public GemPixObj
{
  CPPEXTERN_HEADER(pix_buffer_read, GemPixObj)

    public:

  //////////
  // Constructor
  pix_buffer_read(t_symbol *s);
    	
 protected:
    	
  //////////
  // Destructor
  virtual ~pix_buffer_read();

  //////////
  // Do the processing
  virtual void  setMess(t_symbol*);
  virtual void  frameMess(t_float);
  virtual void  autoMess(t_float);
  virtual void  loopMess(int);

  virtual void render(GemState*state);

  //////////
  // the frame to read in the next render-cycle
  t_float m_frame;
  //////////
  // an auto-increment value for proceeding to the next value
  t_float m_auto;
  //////////
  // should we loop when we reach the end of the buffer ?
  bool m_loop;

  //////////
  // the name of the buffer we bind to
  t_symbol *m_bindname;

 private:
  
  //////////
  // static member functions
  static void setMessCallback  (void*data, t_symbol*s);
  static void frameMessCallback(void*data, t_float  f);
  static void autoMessCallback(void*data, t_float  f);
  static void loopMessCallback(void*data, t_float  f);
};

#endif	// for header file
