//////////////////////////////////////////////////////////////////////////
//
//   VideoIO-Framework for GEM/PD
//
//   
//
//   VIOPlugin
//   implementation file
//
//   copyright            : (C) 2007 by Thomas Holzmann
//   email                : holzi1@gmx.at
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 3 of the License, or
//   (at your option) any later version.
//
///////////////////////////////////////////////////////////////////////////

#include "VIOPlugin.h"


namespace VideoIO_
{
  VIOPlugin::VIOPlugin(const string &filename):
      dll_ref_count_(0),
      pfn_get_engine_version_(0),
      pfn_register_plugin_(0),
      handle_(0)
  {
    post("before: %d, handle: %d", pfn_register_plugin_, handle_);
    
    handle_ = dlopen("videoIO/plugins/FRDummy.so", RTLD_LAZY);
    
        
    if(handle_ == NULL)
    {
      post("ERROR im lib laden:");
      error("Error: open/load error of dynamic.so failed: %sn", dlerror());
    }
    
    pfn_register_plugin_ = reinterpret_cast<fnRegisterPlugin *>(
                           dlsym(handle_, "registerPlugin") );
    
    post("after: %d, handle: %d", pfn_register_plugin_, handle_);
    
    // Initialize a new DLL reference counter
    dll_ref_count_ = new size_t(1);
  }
  
  VIOPlugin::VIOPlugin(const VIOPlugin &other) :
      dll_ref_count_(other.dll_ref_count_),
      pfn_get_engine_version_(other.pfn_get_engine_version_),
      pfn_register_plugin_(other.pfn_register_plugin_),
      handle_(other.handle_)
  {
    post("In the copy constructor");
    ++*dll_ref_count_;
    post("end of copy constructor");
  }
  
  VIOPlugin::~VIOPlugin()
  {
    if(!--*dll_ref_count_)
    {
      delete dll_ref_count_;
      /// TODO die dyn library unter linux wieder freigeben
    }
    
    
  // Only unload the DLL if there are no more references to it
  //if(!--*m_pDLLRefCount) {
  //  delete m_pDLLRefCount;
  //  ::FreeLibrary(m_hDLL);
  //}
  }
  
  int VIOPlugin::getEngineVersion()
  {
    post("get engine version"); /// TODO machen
  }
  
  void VIOPlugin::registerPlugin(VIOKernel &K)
  {
    post("at VIOPlugin::registerPlugin");
    //pfn_register_plugin_(K);
    post("a");
    
    post("schauma mal: %d, handle: %d", pfn_register_plugin_, handle_);
    
    pfn_register_plugin_ = reinterpret_cast<fnRegisterPlugin *>(
                           dlsym(handle_, "registerPlugin") );
    post("b");
    pfn_register_plugin_(K);
    
    post("after pfn_register_plugin_");
  }
  
}
