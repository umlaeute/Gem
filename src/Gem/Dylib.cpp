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

#include "Dylib.h"
#include "Base/CPPExtern.h"

#include <string>
#include <stdio.h>

#if defined __linux__ || defined __APPLE__
#include <unistd.h>
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

#include <iostream>

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


GemDylib::GemDylib(const CPPExtern*obj, const std::string filename, const std::string extension) 
  throw (GemException) : 
  m_handle(0) {
    m_handle=open(obj, filename, extension);
    if(NULL==m_handle) {
      std::string err="unable to open '";
      err+=filename;
      if(!extension.empty()) {
	err+=".";
	err+=extension;
      }
      err+="'";
      throw GemException(err);
    }
  }

GemDylib::GemDylib(const std::string filename, const std::string extension) throw (GemException) : m_handle(0) {
  m_handle=open(0,   filename, extension);
  if(NULL==m_handle) {
    std::string err="unable to open '";
    err+=filename;
    if(!extension.empty()) {
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

void *GemDylib::proc(const std::string procname) {
  //  if(NULL==procname)return NULL;
#ifdef DL_OPEN
  dlerror();
  return dlsym(m_handle->handle, procname.c_str());
#elif defined _WIN32
  return reinterpret_cast<void*>(GetProcAddress(m_handle->handle, procname.c_str()));
#endif

  return NULL;
}

typedef void (*t_method)(void);

bool GemDylib::run(const std::string procname) {
  t_method runproc=reinterpret_cast<t_method>(proc(procname));
  if(runproc) {
    runproc();
    return true;
  }
  return false;
}

static std::string defaultExtension = 
#ifdef DL_OPEN
  std::string(".so")
#elif defined _WIN32
  std::string(".dll")
#else
  std::string("")
#endif
  ;

static std::string getFullfilename(const t_canvas*canvas, const char*filename, const char*ext) {
  std::string fullname;

  char buf[MAXPDSTRING];
  char*bufptr;
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
      return std::string("");
    }
  }
  return fullname;
}

GemDylibHandle* GemDylib::open(const CPPExtern*obj, const std::string filename, const std::string extension) {
  GemDylibHandle*handle=new GemDylibHandle();
  char buf[MAXPDSTRING];

  std::string fullname = "";

  const t_canvas*canvas=(obj)?(canvas=const_cast<CPPExtern*>(obj)->getCanvas()):0;
  const char*ext=extension.c_str();

  fullname=getFullfilename(canvas, filename.c_str(), ext);
  if(fullname.empty()) {
    fullname=getFullfilename(canvas, filename.c_str(), defaultExtension.c_str());
  }

  if(fullname.empty()) {
    std::string error="couldn't find '";
    error+=filename;
    error+="'.'";
    error+=ext;
    error+="'";
    throw(GemException(error));
  }

  sys_bashfilename(fullname.c_str(), buf);
  
#ifdef DL_OPEN
  handle->handle=dlopen(fullname.c_str(), RTLD_NOW);
#elif defined _WIN32
  UINT errorboxflags=SetErrorMode(SEM_FAILCRITICALERRORS);
  handle->handle=LoadLibrary(buf);
  errorboxflags=SetErrorMode(errorboxflags);
#endif

  if(handle->handle) {
    return handle;
  } else {
    delete handle;
    handle=NULL;
#ifdef DL_OPEN
    std::string error="dlerror '";
    error+=dlerror();
    error+="'";
    throw(GemException(error));
#elif defined _WIN32
    DWORD errorNumber = GetLastError();
    LPVOID lpErrorMessage;
    FormatMessage(
                            FORMAT_MESSAGE_ALLOCATE_BUFFER | 
                            FORMAT_MESSAGE_FROM_SYSTEM,
                            NULL,
                            errorNumber,
                            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                            (LPTSTR) &lpErrorMessage,
                            0, NULL );
    std::cerr << "GemDylib: "<<errorNumber<<std::endl;
    std::string error = "DLLerror: ";
    error+=(unsigned int)errorNumber;
    throw(GemException(error));
#endif
  }
  
  delete handle;
  return 0;
}


bool GemDylib::LoadLib(const std::string basefilename, const std::string extension, const std::string procname) {
  
  return false;
}


const std::string GemDylib::getDefaultExtension(void) {
  return defaultExtension;
}
