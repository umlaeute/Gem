//////////////////////////////////////////////////////////////////////////
//
//   VideoIO-Framework for GEM/PD
//
//   The kernel of the plugin loader.
//
//   VIOKernel
//   header file
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
    
    
#ifndef VIOKERNEL_H_
#define VIOKERNEL_H_
    
#include <string>
#include <map>
#include <set>

#include "FileReadServer.h"
#include "FileWriteServer.h"
// #include "StreamReadServer.h"
// #include "StreamWriteServer.h"

#include "VIOPlugin.h"

using namespace std;

namespace VideoIO_
{
  /// file extension used by the plugins
  /// TODO auf windows anders machen (mac gleich ?)
  const string PLUGIN_FILE_EXTENSION = ".so";

  typedef map<string, VIOPlugin> PluginMap;
  typedef set<string> PathList;

  /*!
  * \class VIOKernel
  * 
  * The VIOKernel has the different plugin server and the
  * different plugins.
  */    
  class VIOKernel
  {

   public:

    /// constructor
    VIOKernel();

    /// destructor
    virtual ~VIOKernel()
    {};

    /// searches for plugins and loads them
    /// here all theoretical available plugins are listed
    void loadPlugins();

    /*!
     * adds a search path, where loadPlugin() will search
     * @param path a path which will be added to the search list
     */
    static void addSearchPath(const string &path);
    
    /// @return the file read server
    static FileReadServer &getFileReadServer() {return file_read_server_;};
    /// @return the file write server
    static FileWriteServer &getFileWriteServer() {return file_write_server_;};
    /// @return the stream read server
//    static StreamReadServer &getStreamReadServer() {return stream_read_server_;};
    /// @return the stream write server
//    static StreamWriteServer &getStreamWriteServer() {return stream_write_server_;};

   protected:

    /*!
    * tries to register a plugin if it is found in the
    * search path
    * @param name the name of the plugin (e.g. FileReadGst)
    */
    void registerPlugin(const string &name);
    
   private:

    /// the plugin map
    static PluginMap loaded_plugins_;
    
    // the plugin servers
    static FileReadServer file_read_server_;
    static FileWriteServer file_write_server_;
  //   static StreamReadServer stream_read_server_;
  //   static StreamWriteServer stream_write_server_;
  
    /// path list where to search for plugins
    static PathList search_path_;

    // helper
    static bool first_time_;
  };
}

#endif
