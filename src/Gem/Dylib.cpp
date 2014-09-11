////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.at
//
// Implementation file
//
//    Copyright (c) 1997-2000 Mark Danks.
//    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    Copyright (c) 2002 James Tittle & Chris Clepper
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
// a wrapper for calling Pd's sys_register_loader()
//
/////////////////////////////////////////////////////////
#ifdef _MSC_VER
# pragma warning( disable: 4091)
# define snprintf _snprintf
#endif /* _MSC_VER */

#include "Dylib.h"
#include "Files.h"
#include "Base/CPPExtern.h"

#include <string>
#include <stdio.h>

#if defined __linux__ || defined __APPLE__ || defined __FreeBSD_kernel__
#include <unistd.h>
# define DL_OPEN
#endif

#ifdef DL_OPEN
# include <dlfcn.h>
#endif

#if defined _WIN32
# include <io.h>
# include <windows.h>
#endif

#include <iostream>

class GemDylibHandle {
public:
  std::string fullname;
#ifdef DL_OPEN
  void * dlhandle;
#endif
#ifdef _WIN32
  HINSTANCE w32handle;
#endif
  int dummy2;


  GemDylibHandle(void) :
    fullname(std::string()),
#ifdef DL_OPEN
    dlhandle(NULL),
#endif
#ifdef _WIN32
    w32handle(NULL),
#endif
    dummy2(0)
  {;}

  ~GemDylibHandle(void) {
    close();
  }

  static std::string getFullfilename(const t_canvas*canvas, const char*filename, const char*ext) {
    std::string fullname_;

    char buf[MAXPDSTRING];
    char*bufptr;
    int fd=0;
    if ((fd=canvas_open(const_cast<t_canvas*>(canvas), filename, ext, buf, &bufptr, MAXPDSTRING, 1))>=0){
      gem::files::close(fd);
      fullname_=buf;
      fullname_+="/";
      fullname_+=bufptr;
    } else {
      if(canvas) {
        canvas_makefilename(const_cast<t_canvas*>(canvas), const_cast<char*>(filename), buf, MAXPDSTRING);
        fullname_=buf;
      } else {
        return std::string("");
      }
    }
    return fullname_;
  }

  static GemDylibHandle*open(const std::string filename) {
    GemDylibHandle*handle=new GemDylibHandle();

    char buf[MAXPDSTRING];
    if(filename.empty()) {
      throw(GemException(std::string("No DyLib name given!")));
    }

    sys_bashfilename(filename.c_str(), buf);

#ifdef DL_OPEN
    handle->dlhandle=dlopen(filename.c_str(), RTLD_NOW);
    if(handle->dlhandle) {
      handle->fullname=filename;
      return handle;
    }
#endif
#ifdef _WIN32
    UINT errorboxflags=SetErrorMode(SEM_FAILCRITICALERRORS);
	SetLastError(0);
    handle->w32handle=LoadLibrary(buf);
	DWORD errorNumber = GetLastError();
    errorboxflags=SetErrorMode(errorboxflags);
    if(handle->w32handle) {
      handle->fullname=filename;
      return handle;
    }
#endif

    delete handle;
    handle=NULL;

    std::string errormsg;
#ifdef DL_OPEN
    errormsg=dlerror();
    if(!errormsg.empty()) {
      std::string error="dlerror '";
      error+=errormsg;
      error+="'";
      throw(GemException(error));
    }
#endif
#ifdef _WIN32
    LPVOID lpErrorMessage=NULL;
	if(errorNumber) {
		FormatMessage(
                  FORMAT_MESSAGE_ALLOCATE_BUFFER |
                  FORMAT_MESSAGE_FROM_SYSTEM,
                  NULL,
                  errorNumber,
                  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                  (LPTSTR) &lpErrorMessage,
                  0, NULL );
	}
	std::cerr << "GemDylib failed: #"<<errorNumber<<": ";
	if(lpErrorMessage) {
		std::cerr<<(const char*)lpErrorMessage;
	}else {
		std::cerr<<"(unknown)";
	}
	std::cerr<<std::endl;
    std::string error = "DLLerror(";
    char errbuf[10];
    snprintf(errbuf, 10, "0x%x", errorNumber);
    errbuf[10-1]=0;
    error+=errbuf;
	error+=")";
	if(lpErrorMessage) {
		error+=(const char*)lpErrorMessage;
	}
	std::cerr << "GemDylib throwing: "<< error << std::endl;
	throw(GemException(std::string(error)));
#endif

    return NULL;
  }

  static GemDylibHandle*open(const CPPExtern*obj, const std::string filename, const std::string extension) {
    //const t_canvas*canvas=(obj)?(const_cast<CPPExtern*>(obj)->getCanvas()):0;
    const char*ext=extension.c_str();

    //std::string fullname=getFullfilename(canvas, filename.c_str(), ext);
    std::string fullname=gem::files::getFullpath(filename+ext, obj);
    if(fullname.empty()) {
      //fullname=getFullfilename(canvas, filename.c_str(), GemDylibHandle::defaultExtension.c_str());
      fullname=gem::files::getFullpath(filename+GemDylibHandle::defaultExtension, obj);
    }

    if(fullname.empty()) {
      std::string error="couldn't find '";
      error+=filename;
      error+="'.'";
      error+=ext;
      error+="'";
      throw(GemException(error));
    }

    return open(fullname);
  }

  static const std::string defaultExtension;

  void close(void) {
#ifdef DL_OPEN
    if(dlhandle)
      dlclose(dlhandle);
    dlhandle=NULL;
#endif
#ifdef _WIN32
    if(w32handle)
      FreeLibrary(w32handle);
    w32handle=NULL;
#endif
  }

};

const std::string GemDylibHandle::defaultExtension =
#ifdef _WIN32
                               std::string(".dll")
#elif defined DL_OPEN
                               std::string(".so")
#else
                               std::string("")
#endif
  ;




GemDylib::GemDylib(const CPPExtern*obj, const std::string filename, const std::string extension)
  throw (GemException) :
  m_handle(0) {
    m_handle=GemDylibHandle::open(obj, filename, extension);
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
  m_handle=GemDylibHandle::open(0,   filename, extension);
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

GemDylib::GemDylib(const GemDylib&org) : m_handle(NULL) {
  std::string filename=org.m_handle->fullname;
  m_handle=GemDylibHandle::open(filename);
  if(NULL==m_handle) {
    std::string err="unable to open '";
    err+=filename;
    err+="'";
    throw GemException(err);
  }
}

GemDylib::~GemDylib(void) {
  if(m_handle)
    delete m_handle;
  m_handle=NULL;
}

GemDylib& GemDylib::operator=(const GemDylib&org) {
  if(m_handle)
    delete m_handle;
  m_handle=NULL;
  if(org.m_handle) {
    m_handle=GemDylibHandle::open(org.m_handle->fullname);
  }
  return *this;
}


GemDylib::function_t GemDylib::proc(const std::string procname) {
  function_t result=NULL;
  //  if(NULL==procname)return NULL;
#ifdef DL_OPEN
  dlerror();
  if(m_handle->dlhandle)
    result=(function_t)(dlsym(m_handle->dlhandle, procname.c_str()));
  if(NULL!=result)return result;
#endif
#ifdef _WIN32
  if(m_handle->w32handle)
    result=(function_t)(GetProcAddress(m_handle->w32handle, procname.c_str()));
  if(NULL!=result)return result;
#endif

  return result;
}

bool GemDylib::run(const std::string procname) {
  function_t runproc=proc(procname);
  if(runproc) {
    (runproc)();
    return true;
  }
  return false;
}

bool GemDylib::LoadLib(const std::string basefilename, const std::string extension, const std::string procname) {
  try {
    GemDylib*dylib=new GemDylib(basefilename, extension);
    if(NULL!=dylib) {
      dylib->run(procname);
      return true;
    }
  } catch (GemException&x) {
    std::cerr << "GemDylib::LoadLib: "<<x.what()<<std::endl;
  }

  return false;
}


const std::string GemDylib::getDefaultExtension(void) {
  return GemDylibHandle::defaultExtension;
}
