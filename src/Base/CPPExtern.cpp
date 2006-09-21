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


#include <stdio.h>
#include <stdarg.h>


GEM_EXTERN void *operator new(size_t, void *location, void *) {return(location);}


GemException::GemException(char *error) throw()
  : ErrorString(error)
{}

GemException::GemException() throw() 
  : ErrorString(NULL) 
{}
GemException::~GemException() throw() 
{}
const char *GemException::what() const throw() {
  return ErrorString;
}

void GemException::report() const throw() {
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


void CPPExtern :: PDerror(const char*fmt,...)
{
  char buf[MAXPDSTRING];
  va_list ap;
  va_start(ap, fmt);
  vsnprintf(buf, MAXPDSTRING-1, fmt, ap);
  va_end(ap);
  if(x_obj)
    pd_error(x_obj, "%s", buf);
  else if (m_holder)
    pd_error(m_holder, "%s", buf);
  else
    error("%s", buf);    
}
