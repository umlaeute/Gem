/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia



    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyleft  (l) 2001 IOhannes m zmölnig. zmoelnig@iem.kug.ac.at
    Copyleft (l) 2002 James Tittle & Chris Clepper
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_PIX_GET_H_
#define INCLUDE_PIX_GET_H_

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  pix_get
  
  change the pixBuf into dots
  
  KEYWORDS
  pix
    
  DESCRIPTION
   
  -----------------------------------------------------------------*/
class GEM_EXTERN pix_get : public GemPixObj
{
  CPPEXTERN_HEADER(pix_get, GemPixObj)

    public:

  //////////
  // Constructor
  pix_get(t_symbol *s);
    	
 protected:
    	
  //////////
  // Destructor
  virtual ~pix_get();

  //////////
  // Do the processing
  virtual void  setMess(t_symbol*);
  virtual void  frameMess(int);

  virtual void render(GemState*state);

  int m_frame;
  t_symbol *m_bindname;

 private:
  
  //////////
  // static member functions
  static void setMessCallback  (void*data, t_symbol*s);
  static void frameMessCallback(void*data, t_float  f);
};

#endif	// for header file
