////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-2000 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
#include "Base/config.h"
#include "gemwindow_qtExt.h"

CPPEXTERN_NEW(gemwindow_qtExt)

/////////////////////////////////////////////////////////
//
// gemwindow_qtExt
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
gemwindow_qtExt :: gemwindow_qtExt() : GemOutput(),
				       m_qglContext(NULL)
{}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
gemwindow_qtExt :: ~gemwindow_qtExt()
{
  m_qglContext=NULL;
}

void gemwindow_qtExt :: qglMess(QGLContext*context){
  if(context && context->isValid()){
    m_qglContext=context;
    m_outputState=1;
  } else {
    m_qglContext=NULL;
    m_outputState=0;
    error("GEM: external qt window not valid !!!");
  }
}


/////////////////////////////////////////////////////////
// renderMess
//
/////////////////////////////////////////////////////////
void gemwindow_qtExt :: makeCurrent(){
  m_qglContext->makeCurrent();
}

void gemwindow_qtExt :: postRender(GemState){
  m_qglContext->swapBuffers();
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void gemwindow_qtExt :: obj_setupCallback(t_class *classPtr){
  class_addmethod(classPtr, (t_method)&gemwindow_qtExt::qglMessCallback,
		  gensym("qglContext"), A_POINTER, A_NULL);
}


void gemwindow_qtExt :: qglMessCallback(void *data, t_gpointer*p){
  GetMyClass(data)->qglMess((QGLContext*)p);
}
