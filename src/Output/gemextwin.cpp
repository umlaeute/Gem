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
#include "gemextwin.h"

CPPEXTERN_NEW(gemextwin)

/////////////////////////////////////////////////////////
//
// gemextwin
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
gemextwin :: gemextwin() : GemOutput(), m_context(NULL)
{}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
gemextwin :: ~gemextwin()
{
  m_context=NULL;
}

void gemextwin :: contextMess(GemExtContext*context){
  //  if(context)post("magic=%d (%d)", context->magic, GEMEXTCONTEXT_MAGIC);
  if(context && (context->magic==GEMEXTCONTEXT_MAGIC) && context->isValid()){
    m_context=context;
    m_width = context->getWidth();
    m_height= context->getHeight();
    m_buffer= context->bufferMode();

    m_outputState=1;
  } else {
    m_context=NULL;
    m_outputState=0;
    m_width=m_height=m_buffer=0;
    error("GEM: external window not valid !!!");
  }
}


/////////////////////////////////////////////////////////
// renderMess
//
/////////////////////////////////////////////////////////
void gemextwin :: makeCurrent(){
  m_context->makeCurrent();
  if(!m_context->isValid())m_outputState=0;
}

void gemextwin :: postRender(GemState){
  m_context->swapBuffers();
  if(!m_context->isValid())m_outputState=0;
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void gemextwin :: obj_setupCallback(t_class *classPtr){
  class_addmethod(classPtr, (t_method)&gemextwin::contextMessCallback,
		  gensym("context"), A_POINTER, A_NULL);
}


void gemextwin :: contextMessCallback(void *data, t_gpointer*p){
  GetMyClass(data)->contextMess((GemExtContext*)p);
}
