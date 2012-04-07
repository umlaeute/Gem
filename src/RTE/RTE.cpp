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

#include "RTE/RTE.h"
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

class RTE::PIMPL {
  PIMPL(void) {}
};

RTE :: RTE (void) :
  m_pimpl(NULL)
{  }

RTE :: ~RTE (void)
{
  if(m_pimpl)
    delete(m_pimpl);
  m_pimpl=NULL;
}

const std::string RTE :: getName(void) {
  return std::string("Pd");
}

const std::string RTE :: getVersion(unsigned int&major, unsigned int&minor) {
  return std::string("???");
}

void*RTE :: getFunction(const std::string&name) {
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
RTE* RTE::getRuntimeEnvironment(void) {
  if(s_rte==NULL)
    s_rte=new RTE();

  return s_rte;
}

