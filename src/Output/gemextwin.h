/*-----------------------------------------------------------------
  LOG
  GEM - Graphics Environment for Multimedia

  Interface for the window manager

  Copyright (c) 1997-200 Mark Danks.
  Copyright (c) Günther Geiger. geiger@epy.co.at
  Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

  -----------------------------------------------------------------*/

#ifndef INCLUDE_GEMEXTWIN_H_
#define INCLUDE_GEMEXTWIN_H_

#ifndef HELPSYMBOL_BASE
# define HELPSYMBOL_BASE "Gem/"
#endif

#include "Base/GemOutput.h"
#include "GemExtContext.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  gemextwin
    
  output to an external openGL-context

  DESCRIPTION

  -----------------------------------------------------------------*/
class GEM_EXTERN gemextwin : public GemOutput
{
  CPPEXTERN_HEADER(gemextwin, GemOutput)
    
    public:
    
  //////////
  // Constructor
  gemextwin();
    	
 private:

  //////////
  // Destructor
  virtual     	~gemextwin();

  // do pure virtual functions
  virtual void  makeCurrent();
  virtual void  postRender (GemState);


  void	contextMess(GemExtContext*);
  GemExtContext *m_context;

 private:
  static void	contextMessCallback(void *, t_gpointer*);


};

#endif	// for header file
