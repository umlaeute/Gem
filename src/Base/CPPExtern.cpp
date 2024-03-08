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

t_object * CPPExtern::s_holder=NULL;
const char* CPPExtern::s_holdname=NULL;

namespace {
  static t_class*s_holdclass = NULL;
};

struct CPPExtern::PIMPL {
  t_symbol*objectname;
  t_canvas*canvas;
  t_class*cls;
  mutable bool endpost; /* internal state for startpost/post/endpost */
  PIMPL(const char*name)
    : objectname(name?gensym(name):gensym("unknown Gem object"))
    , canvas(canvas_getcurrent())
    , cls(s_holdclass)
    , endpost(true)
  {  }
  PIMPL(PIMPL*p)
    : objectname(p->objectname)
    , canvas(p->canvas)
    , cls(p->cls)
    , endpost(true)
  {  }

};


/////////////////////////////////////////////////////////
//
// CPPExtern
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
CPPExtern :: CPPExtern()
  : x_obj(s_holder)
  , pimpl(new PIMPL(s_holdname))
{
  t_symbol*asym = gensym("#A");
  /* bashily unbind #A -- this would create garbage if #A were
     multiply bound but we believe in this context it's at most
     bound to whichever textobj or array was created most recently */
  asym->s_thing = 0;
  /* and now bind #A to us to receive following messages in the
     saved file or copy buffer */
  pd_bind(&x_obj->ob_pd, asym);
}
CPPExtern :: CPPExtern(const CPPExtern&org)
  : x_obj(org.x_obj)
  , pimpl(new PIMPL(org.pimpl))
{
  t_symbol*asym = gensym("#A");
  /* bashily unbind #A -- this would create garbage if #A were
     multiply bound but we believe in this context it's at most
     bound to whichever textobj or array was created most recently */
  asym->s_thing = 0;
  /* and now bind #A to us to receive following messages in the
     saved file or copy buffer */
  pd_bind(&x_obj->ob_pd, asym);
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
CPPExtern :: ~CPPExtern()
{

  if(pimpl->cls) {
    /* just in case we're still bound to #A from loading... */
    t_pd*x;
    while ((x = pd_findbyclass(gensym("#A"), pimpl->cls))) {
      pd_unbind(x, gensym("#A"));
    }
  }
  delete pimpl;
  pimpl=0;
}


void CPPExtern :: post(const char*fmt,...) const
{
  char buf[MAXPDSTRING];
  va_list ap;
  va_start(ap, fmt);
  vsnprintf(buf, MAXPDSTRING-1, fmt, ap);
  va_end(ap);
  if(pimpl->endpost && NULL!=pimpl->objectname && NULL!=pimpl->objectname->s_name
      && &s_ != pimpl->objectname) {
    ::post("[%s]: %s", pimpl->objectname->s_name, buf);
  } else {
    ::post("%s", buf);
  }
  pimpl->endpost=true;
}
void CPPExtern :: startpost(const char*fmt,...) const
{
  char buf[MAXPDSTRING];
  va_list ap;
  va_start(ap, fmt);
  vsnprintf(buf, MAXPDSTRING-1, fmt, ap);
  va_end(ap);
  if(pimpl->endpost && NULL!=pimpl->objectname && NULL!=pimpl->objectname->s_name
      && &s_ != pimpl->objectname) {
    ::startpost("[%s]: %s", pimpl->objectname->s_name, buf);
  } else {
    ::startpost("%s", buf);
  }
  pimpl->endpost=false;
}
void CPPExtern :: endpost(void) const
{
  ::endpost();
  pimpl->endpost=true;
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
    if(NULL!=pimpl->objectname && NULL!=pimpl->objectname->s_name
        && &s_ != pimpl->objectname) {
      rte_verbose(level, "[%s]: %s", pimpl->objectname->s_name, buf);
    } else {
      rte_verbose(level, "%s", buf);
    }
  } else {
    if(NULL!=pimpl->objectname && NULL!=pimpl->objectname->s_name
        && &s_ != pimpl->objectname) {
      ::post("[%s]: %s", pimpl->objectname->s_name, buf);
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
  if(NULL!=pimpl->objectname && NULL!=pimpl->objectname->s_name
      && &s_ != pimpl->objectname) {
    const char*objname=pimpl->objectname->s_name;
    if(x_obj) {
      pd_error(x_obj, "[%s]: %s", objname, buf);
    } else if (s_holder) {
      pd_error(s_holder, "[%s]: %s", objname, buf);
    } else {
      pd_error(0, "[%s]: %s", objname, buf);
    }
  } else {
    if(x_obj) {
      pd_error(x_obj, "%s", buf);
    } else if (s_holder) {
      pd_error(s_holder, "%s", buf);
    } else {
      pd_error(0, "%s", buf);
    }
  }
}

typedef int (*close_t)(int fd);

const t_canvas* CPPExtern::getCanvas(void) const {
  return pimpl->canvas;
}


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
  pimpl->objectname=org.pimpl->objectname;
  pimpl->canvas=org.pimpl->canvas;
  pimpl->cls = org.pimpl->cls;
  pimpl->endpost = true;
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
  CPPExtern::s_holder = 0;
  CPPExtern::s_holdname = name;

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

  s_holdclass = cls;
  CPPExtern::s_holder = &obj->pd_obj;

  pimpl->obj = obj;
  pimpl->realargc = realargc;
  pimpl->argc = argc;
  pimpl->argv = argv;
}


gem::CPPExtern_proxy::~CPPExtern_proxy()
{
  delete pimpl;
  CPPExtern::s_holder = 0;
  CPPExtern::s_holdname = 0;
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
