////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// mark@danks.org
//
// Implementation file
//
//    Copyright (c) 1997-1999 Mark Danks.
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "CPPExtern.h"

GEM_EXTERN void *operator new(size_t, void *location, void *) {return(location);}


GemException::GemException(char *error){
  ErrorString=error;
}
GemException::GemException():ErrorString(NULL){}

void GemException::report(){
  if(ErrorString!=NULL)
    error("GemException: %s", ErrorString);
}


t_object * CPPExtern::m_holder;
char* CPPExtern::m_holdname;

/////////////////////////////////////////////////////////
//
// CPPExtern
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
CPPExtern :: CPPExtern()
           : x_obj(m_holder) 
{
    m_canvas = canvas_getcurrent();
    m_objectname=gensym(m_holdname);
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
CPPExtern :: ~CPPExtern()
{ }

