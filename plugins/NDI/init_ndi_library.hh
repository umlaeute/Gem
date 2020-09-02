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
#else
# include <stdlib.h>
# include <dlfcn.h>
#endif

namespace {
const NDIlib_v4* init_ndi_library(const char*prefix)
  {
#ifdef _WIN32
    // We check whether the NDI run-time is installed
    const char* p_ndi_runtime_v4 = getenv(NDILIB_REDIST_FOLDER);
    if (!p_ndi_runtime_v4)
    {       // The NDI run-time is not yet installed. Let the user know and take them to the download URL.
      error("[GEM:%s] Please install the NewTek NDI Runtimes to use this plugin.", prefix);
      if (std::string("") != NDILIB_REDIST_URL)
        error("               get it from %s", NDILIB_REDIST_URL);
      return 0;
    }

    // We now load the DLL as it is installed
    std::string ndi_path = p_ndi_runtime_v4;
    ndi_path += "\\" NDILIB_LIBRARY_NAME;

    // Try to load the library
    HMODULE hNDILib = LoadLibraryA(ndi_path.c_str());

    // The main NDI entry point for dynamic loading if we got the librari
    const NDIlib_v4* (*NDIlib_v4_load)(void) = NULL;
    if (hNDILib)
      *((FARPROC*)&NDIlib_v4_load) = GetProcAddress(hNDILib, "NDIlib_v4_load");

    // If we failed to load the library then we tell people to re-install it
    if (!NDIlib_v4_load)
    {       // Unload the DLL if we loaded it
      if (hNDILib)
        FreeLibrary(hNDILib);

      // The NDI run-time is not installed correctly. Let the user know and take them to the download URL.
      error("[GEM:%s] Please re-install the NewTek NDI Runtimes to use this plugin.", prefix);
      error("               need to find the library '%s'", NDILIB_LIBRARY_NAME);
      if (std::string("") != NDILIB_REDIST_URL)
        error("               get it from %s", NDILIB_REDIST_URL);
      return 0;
    }
#else
    std::string ndi_path;

    const char* p_NDI_runtime_folder = getenv(NDILIB_REDIST_FOLDER);
    if (p_NDI_runtime_folder)
    {
      ndi_path = p_NDI_runtime_folder;
      ndi_path += NDILIB_LIBRARY_NAME;
    }
    else ndi_path = NDILIB_LIBRARY_NAME;

    // Try to load the library
    void *hNDILib = dlopen(ndi_path.c_str(), RTLD_LOCAL | RTLD_LAZY);

    // The main NDI entry point for dynamic loading if we got the library
    const NDIlib_v4* (*NDIlib_v4_load)(void) = NULL;
    if (hNDILib)
      *((void**)&NDIlib_v4_load) = dlsym(hNDILib, "NDIlib_v4_load");
    // If we failed to load the library then we tell people to re-install it
    if (!NDIlib_v4_load)
    {       // Unload the library if we loaded it
      if (hNDILib)
        dlclose(hNDILib);

      error("[GEM:%s] Please (re)install the NewTek NDI Runtimes to use this plugin.", prefix);
      error("               need to find the library '%s'", NDILIB_LIBRARY_NAME);
      if (std::string("") != NDILIB_REDIST_URL)
        error("               get it from %s", NDILIB_REDIST_URL);
      return 0;
    }
#endif

    // Lets get all of the DLL entry points
    return NDIlib_v4_load();
  }
};
#endif /* _INCLUDE_GEMPLUGIN__INITNDILIBRARY_INITNDILIBRARY_H_ */
