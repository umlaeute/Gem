////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.at
//
// Implementation file
//
//    Copyright (c) 1997-2000 Mark Danks.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    Copyright (c) 2002 James Tittle & Chris Clepper
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
// a wrapper for calling Pd's sys_register_loader()
//
/////////////////////////////////////////////////////////
#ifdef _MSC_VER
# pragma warning( disable: 4091)
#endif /* _MSC_VER */

#include "GemDylib.h"
#include "CPPExtern.h"

#include <string>
using std::string;


#include <stdio.h>
#include <unistd.h>


#if defined __linux__ || defined __APPLE__
# define DL_OPEN
#endif

#ifdef DL_OPEN
# include <dlfcn.h>
#endif

#if defined _WIN32
# include <io.h>
# include <windows.h>
#elif defined __APPLE__
//# include <mach-o/dyld.h> 
#endif


class GemDylibHandle {
public:
#ifdef DL_OPEN
  void *
#elif defined _WIN32
  HINSTANCE 
#endif
    handle;

  GemDylibHandle(void) : handle(NULL) {;}
};


GemDylib::GemDylib(const CPPExtern*obj, const char*filename, const char*extension) throw (GemException) : m_handle(0) {
  m_handle=open(obj, filename, extension);
  if(NULL==m_handle) {
    std::string err="unable to load '";
    err+=filename;
    if(extension) {
      err+=".";
      err+=extension;
    }
    err+="'";
    throw GemException(err);
  }
}

GemDylib::GemDylib(const char*filename, const char*extension) throw (GemException) : m_handle(0) {
  m_handle=open(0,   filename, extension);
  if(NULL==m_handle) {
    std::string err="unable to load '";
    err+=filename;
    if(extension) {
      err+=".";
      err+=extension;
    }
    err+="'";
    throw GemException(err);
  }
}


GemDylib::~GemDylib(void) {
#ifdef DL_OPEN
  dlclose(m_handle->handle);
#elif defined _WIN32
  FreeLibrary(m_handle->handle);
#endif
  m_handle->handle=NULL;
  delete m_handle;
}

void *GemDylib::proc(const char*procname) {
  //  if(NULL==procname)return NULL;
#ifdef DL_OPEN
  dlerror();
  return dlsym(m_handle->handle, procname);
#elif defined _WIN32
  return reinterpret_cast<void*>(GetProcAddress(m_handle->handle, procname));
#endif

  return NULL;
}

typedef void (*t_method)(void);

bool GemDylib::run(const char*procname) {
  t_method runproc=reinterpret_cast<t_method>(proc(procname));
  if(runproc) {
    runproc();
    return true;
  }

  return false;
}


GemDylibHandle* GemDylib::open(const CPPExtern*obj, const char*filename, const char*extension) {
  GemDylibHandle*handle=new GemDylibHandle();
  char buf[MAXPDSTRING];
  char*bufptr;

  string fullname = "";

  const t_canvas*canvas=(obj)?(canvas=const_cast<CPPExtern*>(obj)->getCanvas()):0;

  const char*ext=extension;
#ifdef DL_OPEN
  if(0==ext)
    ext=".so";
#elif defined _WIN32
  if(0==ext)
    ext=".dll";
#endif
  int fd=0;
  if ((fd=canvas_open(const_cast<t_canvas*>(canvas), filename, ext, buf, &bufptr, MAXPDSTRING, 1))>=0){
    close(fd);
    fullname=buf;
    fullname+="/";
    fullname+=bufptr;
  } else {
      if(canvas) {
        canvas_makefilename(const_cast<t_canvas*>(canvas), const_cast<char*>(filename), buf, MAXPDSTRING);
        fullname=buf;
      } else {
        string error="couldn't find '";
        error+=filename;
        error+="'.'";
        error+=ext;
        error+="'";
        throw(GemException(error));
      }
  }
  sys_bashfilename(fullname.c_str(), buf);
  
#ifdef DL_OPEN
  handle->handle=dlopen(fullname.c_str(), RTLD_NOW);
#elif defined _WIN32
  handle->handle=LoadLibrary(buf);
#endif

  if(handle->handle) {
    return handle;
  }
  
  delete handle;
  return 0;
}


bool GemDylib::LoadLib(const char*basefilename, const char*extension, const char*procname) {
  
  return false;
}

#if 0


extern "C" {
  typedef void (*loader_registrar_t)(gem_loader_t loader);
}
static loader_registrar_t pd_register_loader = NULL;

static int find_pd_loader(void) {
  if(pd_register_loader)return 1;

#ifdef DL_OPEN
  pd_register_loader=(loader_registrar_t)dlsym(RTLD_DEFAULT, "sys_register_loader");
#elif defined _WIN32
  /* no idea whether this actually works... */
  pd_register_loader = (loader_registrar_t)GetProcAddress( GetModuleHandle("pd.dll"), "sys_register_loader");  
#else
  // no loader for older Pd's....
#endif

  return(NULL!=pd_register_loader);
}

void gem_register_loader(gem_loader_t loader) {
  if(find_pd_loader()) {
    pd_register_loader(loader);
  }
}
#endif
