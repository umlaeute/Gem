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

#ifndef INCLUDE_GEMWINDOW_QTEXT_H_
#define INCLUDE_GEMWINDOW_QTEXT_H_

#ifndef HELPSYMBOL_BASE
# define HELPSYMBOL_BASE "Gem/"
#endif

#include "Base/GemOutput.h"
#include <qt/qgl.h>

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  gemwindow_qtExt
    
  output to an external qt-openGL-context

  DESCRIPTION

  -----------------------------------------------------------------*/
class GEM_EXTERN gemwindow_qtExt : public GemOutput
{
  CPPEXTERN_HEADER(gemwindow_qtExt, GemOutput)
    
    public:
    
  //////////
  // Constructor
  gemwindow_qtExt();
    	
 private:

  //////////
  // Destructor
  virtual     	~gemwindow_qtExt();

  // do pure virtual functions
  virtual void  makeCurrent();
  virtual void  postRender (GemState);


  void	qglMess(QGLContext*);
  QGLContext *m_qglContext;

 private:
  static void	qglMessCallback(void *, t_gpointer*);

  

};

#endif	// for header file
