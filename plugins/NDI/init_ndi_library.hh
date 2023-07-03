/*-----------------------------------------------------------------

    GEM - Graphics Environment for Multimedia

    Load an video into a pix block

    Copyright (c) 2020 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef _INCLUDE_GEMPLUGIN__INITNDILIBRARY_INITNDILIBRARY_H_
#define _INCLUDE_GEMPLUGIN__INITNDILIBRARY_INITNDILIBRARY_H_

#include "Processing.NDI.Lib.h"

#include "Gem/RTE.h"
#include <string>

#ifdef _WIN32
# include <windows.h>
# define NDI_PATH_SEPARATOR "\\"
#else
# include <stdlib.h>
# include <dlfcn.h>
# define NDI_PATH_SEPARATOR "/"
#endif

typedef NDIlib_v4 NDIlib_gem;

namespace {
const NDIlib_gem* init_ndi_library(const char*prefix)
  {
    static bool s_firsttime = true;
    bool firsttime = s_firsttime;
    s_firsttime = false;

    // The main NDI entry point for dynamic loading if we got the librari
    const NDIlib_gem* (*NDIlib__load)(void) = NULL;

    const char* p_NDI_runtime_folder = getenv(NDILIB_REDIST_FOLDER);
    std::string ndi_path = p_NDI_runtime_folder ? p_NDI_runtime_folder : "";
    if (!ndi_path.empty())
    {
      ndi_path += NDI_PATH_SEPARATOR;
    }
    ndi_path += NDILIB_LIBRARY_NAME;

#ifdef _WIN32
    // Try to load the library
    HMODULE hNDILib = LoadLibraryA(ndi_path.c_str());

    if (hNDILib)
      *((FARPROC*)&NDIlib__load) = GetProcAddress(hNDILib, "NDIlib_v4_load");

    // If we failed to load the library then we tell people to re-install it
    if (!NDIlib__load)
    {       // Unload the DLL if we loaded it
      if (hNDILib)
        FreeLibrary(hNDILib);

      // The NDI run-time is not installed correctly. Let the user know and take them to the download URL.
      goto notfound;
      return 0;
    }
#else
    // Try to load the library
    void *hNDILib = dlopen(ndi_path.c_str(), RTLD_LOCAL | RTLD_LAZY);

    if (hNDILib)
      *((void**)&NDIlib__load) = dlsym(hNDILib, "NDIlib_v4_load");
    // If we failed to load the library then we tell people to re-install it
    if (!NDIlib__load)
    {       // Unload the library if we loaded it
      if (hNDILib)
        dlclose(hNDILib);
      goto notfound;
    }
#endif
    if(firsttime) {
      verbose(1, "[GEM::%s] loading NewTek NDI Runtime from '%s'", prefix, ndi_path.c_str());
    }
    // Lets get all of the DLL entry points
    return NDIlib__load();
  notfound:
    if(firsttime) {
      pd_error(0, "[GEM:%s] Please (re)install the NewTek NDI Runtimes to use this plugin.", prefix);
      pd_error(0, "        need to find the library '%s'", NDILIB_LIBRARY_NAME);
      if (std::string("") != NDILIB_REDIST_URL)
        pd_error(0, "        get the NewTek Runtimes from %s", NDILIB_REDIST_URL);
      pd_error(0, "        use the '%s' environment variable to set the path to the library.", NDILIB_REDIST_FOLDER);
      if(p_NDI_runtime_folder)
        pd_error(0, "        (currently set to '%s').", p_NDI_runtime_folder);
    }
    return 0;
  }
};
#endif /* _INCLUDE_GEMPLUGIN__INITNDILIBRARY_INITNDILIBRARY_H_ */
