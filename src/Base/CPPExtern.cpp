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


GemException::GemException(const char *error) throw()
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

void GemException::report(const char*origin) const throw() {
  if(ErrorString!=NULL) {
    if (NULL==origin)
      error("GemException: %s", ErrorString);
    else
      error("[%s]: %s", origin, ErrorString);
  }
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
  : x_obj(m_holder),
    m_objectname(NULL),
    m_canvas(NULL),
    m_endpost(true)
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
void CPPExtern :: post(const char*fmt,...)
{
  char buf[MAXPDSTRING];
  va_list ap;
  va_start(ap, fmt);
  vsnprintf(buf, MAXPDSTRING-1, fmt, ap);
  va_end(ap);
  if(m_endpost && NULL!=m_objectname && NULL!=m_objectname->s_name && &s_ != m_objectname){
    ::post("[%s]: %s", m_objectname->s_name, buf);
  } else {
    ::post("%s", buf);
  }
  m_endpost=true;
}
void CPPExtern :: startpost(const char*fmt,...)
{
  char buf[MAXPDSTRING];
  va_list ap;
  va_start(ap, fmt);
  vsnprintf(buf, MAXPDSTRING-1, fmt, ap);
  va_end(ap);
  if(m_endpost && NULL!=m_objectname && NULL!=m_objectname->s_name && &s_ != m_objectname){
    ::startpost("[%s]: %s", m_objectname->s_name, buf);
  } else {
    ::startpost("%s", buf);
  }
  m_endpost=false;
}
void CPPExtern :: endpost(void)
{
  ::endpost();
  m_endpost=true;
}
void CPPExtern :: verbose(const int level, const char*fmt,...)
{
  char buf[MAXPDSTRING];
  va_list ap;
  va_start(ap, fmt);
  vsnprintf(buf, MAXPDSTRING-1, fmt, ap);
  va_end(ap);
  /* only pd>=0.39(?) supports ::verbose() */
#if defined PD_MINOR_VERSION && (PD_MAJOR_VERSION > 1 || PD_MINOR_VERSION > 38)
  if(NULL!=m_objectname && NULL!=m_objectname->s_name && &s_ != m_objectname){
    ::verbose(level, "[%s]: %s", m_objectname->s_name, buf);
  } else {
    ::verbose(level, "%s", buf);
  }
#else
    if(NULL!=m_objectname && NULL!=m_objectname->s_name && &s_ != m_objectname){
    ::post("[%s]: %s", m_objectname->s_name, buf);
  } else {
    ::post("%s", buf);
  }
#endif
}

void CPPExtern :: error(const char*fmt,...)
{
  char buf[MAXPDSTRING];
  va_list ap;
  va_start(ap, fmt);
  vsnprintf(buf, MAXPDSTRING-1, fmt, ap);
  va_end(ap);
  if(NULL!=m_objectname && NULL!=m_objectname->s_name && &s_ != m_objectname){
    char*objname=m_objectname->s_name;
    if(x_obj)
      pd_error(x_obj, "[%s]: %s", objname, buf);
    else if (m_holder)
      pd_error(m_holder, "[%s]: %s", objname, buf);
    else
      ::error("[%s]: %s", objname, buf);
  } else {
    if(x_obj)
      pd_error(x_obj, "%s", buf);
    else if (m_holder)
      pd_error(m_holder, "%s", buf);
    else
      ::error("%s", buf);
  }
}

bool CPPExtern :: checkGemVersion(int major, int minor) {
  if(!GemVersion::versionCheck(major, minor)) {
    ::error("GEM version mismatch: compiled for %d.%d but we are running %s", 
	    major, minor,
	    GemVersion::versionString());
        return false;
  }
  return true;
}
