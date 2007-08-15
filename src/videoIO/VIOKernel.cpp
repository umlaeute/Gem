//////////////////////////////////////////////////////////////////////////
//
//   VideoIO-Framework for GEM/PD
//
//   The kernel of the plugin loader.
//
//   VIOKernel
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

#include "VIOKernel.h"
#include <fstream>

using namespace std;

namespace VideoIO_
{
  // init static variables
  PluginMap VIOKernel::loaded_plugins_;
  PluginServer<FileRead> VIOKernel::file_read_server_;
  PluginServer<FileWrite> VIOKernel::file_write_server_;
  PluginServer<DeviceRead> VIOKernel::device_read_server_;
  PathList VIOKernel::search_path_;
  bool VIOKernel::first_time_ = true;


  VIOKernel::VIOKernel()
  {
    if(first_time_)
    {
      // add standard relative path
      addSearchPath(".");
      /// TODO naechster pfad jetzt nur zum Testen, dann wieder wegtun,
      /// weil sie e eher im gleichen pfad wie gem sein sollen ?
      addSearchPath("videoIO/plugins");

      // add standard system path
      /// TODO schauen ob man diese Standard Systempfade
      /// ueberhaupt hinzufügen soll
      /// TODO unter Windows/OSX sind die natürlich auch anders
      addSearchPath("/usr/lib/videoIO");
      addSearchPath("/usr/local/lib/videoIO");

      // load plugins
      loadPlugins();
    }
    else first_time_ = false;
  }

  void VIOKernel::loadPlugins()
  {
    // try to load all possible plugins
    registerPlugin("FileReadGst");
    registerPlugin("FileWriteGst");
    registerPlugin("DeviceReadGst");
  }

  void VIOKernel::addSearchPath(const string &path)
  {
    search_path_.insert(path);
  }

  void VIOKernel::registerPlugin(const string &name)
  {
    PathList::iterator iter;
    string filename;
    fstream tmp;
    bool file_exists;

    for( iter = search_path_.begin(); iter != search_path_.end(); iter++ )
    {
      filename = *iter + "/" + name + PLUGIN_FILE_EXTENSION;

      // check if file exists
      tmp.open(filename.c_str());
      file_exists = tmp.is_open();
      tmp.close();

      // DEBUG
//       post("testfile: %s - exists: %d", filename.c_str(), file_exists);

      if(!file_exists) continue;

      // now try to open plugin
      if( loaded_plugins_.find(name) == loaded_plugins_.end() )
      {
        // exception are the only way to get a error message
        // from the VIOPlugin Constructor
        try
        {
          loaded_plugins_.insert(
              make_pair( name, VIOPlugin(filename) )
             ).first->second.registerPlugin(*this);
        }
        catch(const char*)
        {
          error("could not load %s", filename.c_str() );
        }
      }
    } // for loop

  }

}
