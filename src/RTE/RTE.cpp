////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.at
//
// Implementation file
//
//    Copyright (c) 2012 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
// a wrapper for accessing the RTE's arrays
// currently only numeric arrays
//
/////////////////////////////////////////////////////////
#include "Gem/GemConfig.h"


#include "RTE/RTE.h"
#include "m_pd.h"
#if defined HAVE_S_STUFF_H
extern "C" {
# include "s_stuff.h"
}
#else
# warning s_stuff.h missing
#endif


#include <sstream>

#if defined __linux__ || defined __APPLE__
# define DL_OPEN
#endif

#ifdef DL_OPEN
# include <dlfcn.h>
#endif

#if defined _WIN32
# include <io.h>
# include <windows.h>
#endif

using namespace gem::RTE;

class RTE::PIMPL
{
  PIMPL(void) {}
};

RTE :: RTE (void) :
  m_pimpl(NULL)
{  }

RTE :: ~RTE (void)
{
  if(m_pimpl) {
    delete(m_pimpl);
  }
  m_pimpl=NULL;
}

const std::string RTE :: getName(void)
{
  return std::string("Pd");
}

typedef void (*rte_getversion_t)(int*major,int*minor,int*bugfix);
const std::string RTE :: getVersion(unsigned int&major, unsigned int&minor)
{
  static rte_getversion_t rte_getversion=NULL;
  if(NULL==rte_getversion) {
    rte_getversion=(rte_getversion_t)this->getFunction("sys_getversion");
  }

  if(rte_getversion) {
    int imajor, iminor, ibugfix;
    rte_getversion(&imajor, &iminor, &ibugfix);
    major=(unsigned int)imajor;
    minor=(unsigned int)iminor;

    std::stringstream sstm;
    sstm << "" << imajor << "."<<iminor<<"-"<<ibugfix;
    return sstm.str();
  }

  /* empty string: unknown */
  return std::string("");
}

void*RTE :: getFunction(const std::string&name) const
{
#ifdef DL_OPEN
  return (void*)dlsym(RTLD_DEFAULT, name.c_str());
#elif defined _WIN32
  /* no idea whether this actually works... */
  return (void*)GetProcAddress( GetModuleHandle("pd.dll"), name.c_str());
#else

  // no loader for older Pd's....
#endif
  return NULL;
}

static  RTE*s_rte=NULL;
RTE* RTE::getRuntimeEnvironment(void)
{
  if(s_rte==NULL) {
    s_rte=new RTE();
  }

  return s_rte;
}

typedef int (*close_t)(int fd);
#ifndef _WIN32
#include <unistd.h>
#endif
std::string RTE::findFile(const std::string&f, const std::string&e,
                          const void* ctx) const
{
  char buf[MAXPDSTRING], buf2[MAXPDSTRING];
  char*bufptr=0;
  std::string result="";
  int fd=-1;

  t_canvas*canvas=static_cast<t_canvas*>(const_cast<void*>(ctx));
  char*filename=const_cast<char*>(f.c_str());
  char*ext=const_cast<char*>(e.c_str());
  const char nullstring[]="\0";
  const char*cnvdir=nullstring;

  if(canvas) {
    cnvdir=canvas_getdir(canvas)->s_name;
  }
  if ((fd=open_via_path(cnvdir, filename, ext,
                        buf2, &bufptr, MAXPDSTRING, 1))>=0) {
    static close_t rte_close=NULL;
    if(NULL==rte_close) {
      rte_close=(close_t)this->getFunction("sys_close");
      if(NULL==rte_close) {
        rte_close=close;
      }
    }
    rte_close(fd);
    result=buf2;
    if(buf2 != bufptr) {
      result+="/";
      result+=bufptr;
    }
  } else {
    if(canvas) {
      canvas_makefilename(canvas, filename, buf, MAXPDSTRING);
      result=buf;
    } else {
      result=f+e;
    }
  }
  return result;
}


bool RTE::addSearchPath(const std::string&path, void* ctx)
{
  static bool didit=false;
  static t_namelist *rte_searchpath = 0;
  static bool modern = true;
  if(ctx) {
    return false;
  }

  if(!didit) {
    unsigned int major = 0, minor = 0;
    rte_searchpath=(t_namelist*)this->getFunction("sys_searchpath");
    this->getVersion(major, minor);
    modern = ((major>0) || (minor>47));
  }
  if(modern) {
    t_atom ap[2];
    const char *inptr = path.c_str();
    char encoded[MAXPDSTRING];
    char*outptr = encoded;
    *outptr++='+';
    while(inptr && ((outptr+2) < (encoded+MAXPDSTRING))) {
      *outptr++ = *inptr++;
      if ('+'==inptr[-1]) {
        *outptr++='+';
      }
    }
    *outptr=0;
    SETSYMBOL(ap+0, gensym(encoded));
    SETFLOAT (ap+1, 0.f);
    pd_typedmess(gensym("pd")->s_thing, gensym("add-to-path"), 2, ap);
  } else {
    if(!rte_searchpath) {
      return false;
    }
    rte_searchpath = namelist_append(rte_searchpath, path.c_str(), 0);
  }
  return true;
}
