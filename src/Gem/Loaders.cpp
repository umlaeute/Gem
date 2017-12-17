////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.at
//
// Implementation file
//
//    Copyright (c) 2001-2016 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
// a wrapper for calling Pd's sys_register_loader()
//
/////////////////////////////////////////////////////////
#ifdef _MSC_VER
# pragma warning( disable: 4091)
#endif /* _MSC_VER */


#include "Loaders.h"
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

#include <vector>

static std::vector<gem_loader_t>loaders_path;
static std::vector<gem_loader_t>loaders_nopath;
static std::vector<gem_loader_t>loaders;

extern "C" {
  typedef void (*loader_registrar_t)(gem_loader_t loader);
}

static int gem_loader(const t_canvas *canvas, const char *classname,
                      const char* path)
{
  if(path) {
    for (std::vector<gem_loader_t>::iterator it = loaders_path.begin() ;
         it != loaders_path.end(); ++it)
      if((*it)(canvas, classname, path)) {
        return true;
      }
  } else {
    for (std::vector<gem_loader_t>::iterator it = loaders_nopath.begin() ;
         it != loaders_nopath.end(); ++it)
      if((*it)(canvas, classname, path)) {
        return true;
      }
  }
  return false;
}
static int gem_loader_legacy(const t_canvas *canvas, const char *classname)
{
  for (std::vector<gem_loader_t>::iterator it = loaders.begin() ;
       it != loaders.end(); ++it)
    if((*it)(canvas, classname, 0)) {
      return true;
    }
  return false;
}

static loader_registrar_t rte_register_loader = NULL;

static bool check_rte_loader(void)
{
  if(rte_register_loader) {
    return true;
  }
  gem::RTE::RTE*rte=gem::RTE::RTE::getRuntimeEnvironment();
  if(rte) {
    rte_register_loader=(loader_registrar_t)
                        rte->getFunction("sys_register_loader");
  }
  if(rte_register_loader) {
    unsigned int major=0, minor=0;
    rte->getVersion(major, minor);
    if (major>0 || minor >=47) {
      rte_register_loader(gem_loader);
    } else {
      rte_register_loader((gem_loader_t)gem_loader_legacy);
    }
  }
  return(NULL!=rte_register_loader);
}

void gem_register_loader(gem_loader_t loader)
{
  if(check_rte_loader()) {
    loaders_path.push_back(loader);
    loaders.push_back(loader);
  }
}
void gem_register_loader_nopath(gem_loader_t loader)
{
  if(check_rte_loader()) {
    loaders_nopath.push_back(loader);
    loaders.push_back(loader);
  }
}
