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

#include "RTE/RTE.h"

#ifdef _WIN32
# include <io.h>
#else
# include <unistd.h>
#endif

#ifdef _MSC_VER  /* This is only for Microsoft's compiler, not cygwin, e.g. */
# define snprintf _snprintf
# define vsnprintf _vsnprintf
# define close _close
#endif

#include <stdio.h>
#include <stdarg.h>


void *Obj_header::operator new(size_t, void *location, void *)
{
  return(location);
}

t_object * CPPExtern::m_holder=NULL;
const char* CPPExtern::m_holdname=NULL;

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
  if(m_holdname) {
    m_objectname=gensym(m_holdname);
  } else {
    m_objectname=gensym("unknown Gem object");
  }
}
CPPExtern :: CPPExtern(const CPPExtern&org) :
  x_obj(org.x_obj),
  m_objectname(org.m_objectname),
  m_canvas(org.m_canvas),
  m_endpost(true)
{
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
CPPExtern :: ~CPPExtern()
{ }


void CPPExtern :: post(const char*fmt,...) const
{
  char buf[MAXPDSTRING];
  va_list ap;
  va_start(ap, fmt);
  vsnprintf(buf, MAXPDSTRING-1, fmt, ap);
  va_end(ap);
  if(m_endpost && NULL!=m_objectname && NULL!=m_objectname->s_name
      && &s_ != m_objectname) {
    ::post("[%s]: %s", m_objectname->s_name, buf);
  } else {
    ::post("%s", buf);
  }
  m_endpost=true;
}
void CPPExtern :: startpost(const char*fmt,...) const
{
  char buf[MAXPDSTRING];
  va_list ap;
  va_start(ap, fmt);
  vsnprintf(buf, MAXPDSTRING-1, fmt, ap);
  va_end(ap);
  if(m_endpost && NULL!=m_objectname && NULL!=m_objectname->s_name
      && &s_ != m_objectname) {
    ::startpost("[%s]: %s", m_objectname->s_name, buf);
  } else {
    ::startpost("%s", buf);
  }
  m_endpost=false;
}
void CPPExtern :: endpost(void) const
{
  ::endpost();
  m_endpost=true;
}
typedef void (*verbose_t)(int level, const char *fmt, ...);

void CPPExtern :: verbose(const int level, const char*fmt,...) const
{
  char buf[MAXPDSTRING];
  va_list ap;
  va_start(ap, fmt);
  vsnprintf(buf, MAXPDSTRING-1, fmt, ap);
  va_end(ap);
  static verbose_t rte_verbose=NULL;
  static bool rte_verbose_checked=false;
  if(false==rte_verbose_checked) {
    gem::RTE::RTE*rte=gem::RTE::RTE::getRuntimeEnvironment();
    if(rte) {
      rte_verbose=(verbose_t)rte->getFunction("verbose");
    }
  }
  rte_verbose_checked=true;

  /* only pd>=0.39(?) supports ::verbose() */
  if(rte_verbose) {
    if(NULL!=m_objectname && NULL!=m_objectname->s_name
        && &s_ != m_objectname) {
      rte_verbose(level, "[%s]: %s", m_objectname->s_name, buf);
    } else {
      rte_verbose(level, "%s", buf);
    }
  } else {
    if(NULL!=m_objectname && NULL!=m_objectname->s_name
        && &s_ != m_objectname) {
      ::post("[%s]: %s", m_objectname->s_name, buf);
    } else {
      ::post("%s", buf);
    }
  }
}

void CPPExtern :: error(const char*fmt,...) const
{
  char buf[MAXPDSTRING];
  va_list ap;
  va_start(ap, fmt);
  vsnprintf(buf, MAXPDSTRING-1, fmt, ap);
  va_end(ap);
  if(NULL!=m_objectname && NULL!=m_objectname->s_name
      && &s_ != m_objectname) {
    const char*objname=m_objectname->s_name;
    if(x_obj) {
      pd_error(x_obj, "[%s]: %s", objname, buf);
    } else if (m_holder) {
      pd_error(m_holder, "[%s]: %s", objname, buf);
    } else {
      pd_error(0, "[%s]: %s", objname, buf);
    }
  } else {
    if(x_obj) {
      pd_error(x_obj, "%s", buf);
    } else if (m_holder) {
      pd_error(m_holder, "%s", buf);
    } else {
      pd_error(0, "%s", buf);
    }
  }
}

typedef int (*close_t)(int fd);

std::string CPPExtern::findFile(const std::string&f,
                                const std::string&e) const
{
  std::string result;
  gem::RTE::RTE*rte=gem::RTE::RTE::getRuntimeEnvironment();
  if(rte) {
    result=rte->findFile(f, e, getCanvas());
  }
  return result;
}

std::string CPPExtern::findFile(const std::string&file) const
{
  return findFile(file, "");

}
bool CPPExtern :: checkGemVersion(const int major, const int minor)
{
  if(!GemVersion::versionCheck(major, minor)) {
    pd_error(0, "GEM version mismatch: compiled for %d.%d but we are running %s",
             major, minor,
             GemVersion::versionString());
    return false;
  }
  return true;
}


CPPExtern&CPPExtern::operator=(const CPPExtern&org)
{
  x_obj=org.x_obj;
  m_objectname=org.m_objectname;
  m_canvas=org.m_canvas;
  m_endpost=true;
  return *this;
}

void CPPExtern::beforeDeletion(void)
{
  //post("CPPExtern to be deleted");
}


/////////////////////////////////
// CPPExtern_proxy
//
// helper class for creating and initializing CPPExtern
/////////////////////////////////
#include "Gem/Exception.h"

struct gem::CPPExtern_proxy::PIMPL {
  Obj_header*obj;
  int argc, realargc;
  t_atom*argv;

  void forwardmsg(int argc, t_atom*argv) {
    t_pd*x = (t_pd*)obj;
    if(x && argc>0) {
      if (A_SYMBOL == argv->a_type)
        pd_typedmess(x, argv->a_w.w_symbol, argc - 1, argv + 1);
      else
        pd_typedmess(x, gensym("list"), argc, argv);
    }
  }
};

gem::CPPExtern_proxy::CPPExtern_proxy(
        t_class*cls, const char*name,
        t_symbol*s, int realargc, t_atom*argv,
        size_t numtypes, const int*types,
        bool wantInitmsgs)
  : pimpl(new PIMPL())
{
  int argc = realargc;
  if(!name && s)
    name=s->s_name;
  CPPExtern::m_holder = 0;
  CPPExtern::m_holdname = name;

  /* if we want init-messages, check if we have a semi-colon
   * (that marks the beginning of the init-messages),
   * and remember it's position (so we know how many
   * usable objectargs there are */
  if(wantInitmsgs && argc) {
    t_symbol*semi = gensym(";");
    for(int i = 0; i < realargc; i++) {
      if ((A_SYMBOL == argv[i].a_type) && (semi == argv[i].a_w.w_symbol)) {
        argc = i;
        break;
      }
    }
  }
  if(!gem::RTE::Atom::checkSignature(argc, argv, numtypes, types, name)) {
    throw(GemException());
  }
  Obj_header*obj = new (pd_new(cls), (void *)NULL) Obj_header;
  if(!obj) {
    throw(GemException("unknown class"));
  }

  CPPExtern::m_holder = &obj->pd_obj;

  pimpl->obj = obj;
  pimpl->realargc = realargc;
  pimpl->argc = argc;
  pimpl->argv = argv;
}


gem::CPPExtern_proxy::~CPPExtern_proxy()
{
  delete pimpl;
  CPPExtern::m_holder = 0;
  CPPExtern::m_holdname = 0;
}
void gem::CPPExtern_proxy::setObject(CPPExtern*obj)
{
  pimpl->obj->data = obj;
}
int gem::CPPExtern_proxy::getNumArgs()
{
  return pimpl->argc;
}
Obj_header*gem::CPPExtern_proxy::initialize()
{
  if(pimpl->realargc > pimpl->argc) {
    int argc = pimpl->realargc - pimpl->argc - 1;
    t_atom *argv = pimpl->argv + pimpl->argc + 1;
    t_symbol*semi = gensym(";");
    int i, start = 0;
    for(i = start; i < argc; i++) {
      if ((A_SYMBOL != argv[i].a_type) || (semi != argv[i].a_w.w_symbol))
        continue;
      pimpl->forwardmsg(i - start, argv + start);
      start = i = i + 1;
    }
    if (i > argc)
      i = argc;
    if (i > start)
      pimpl->forwardmsg(i - start, argv + start);
  }
  return pimpl->obj;
}
