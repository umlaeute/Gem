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

#include "FileReadServer.h"
// #include "FileWriteServer.h"
// #include "StreamReadServer.h"
// #include "StreamWriteServer.h"

#include "VIOPlugin.h"
#include "FileRead.h"


using namespace std; 

namespace VideoIO_
{
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
    VIOKernel()
    { loadPlugin("dummy"); };
    
    /// destructor
    virtual ~VIOKernel()
    {};
    
    
    /// @return the file read server
    FileReadServer &getFileReadServer() {return file_read_server_;};
    /// @return the file write server
//    FileWriteServer &getWriteReadServer() {return file_write_server_;};
    /// @return the stream read server
//    StreamReadServer &getStreamReadServer() {return stream_read_server_;};
    /// @return the stream write server
//    StreamWriteServer &getStreamWriteServer() {return stream_write_server_;};
    
    /*!
    * loads a plugin
    * @param filename the filename of the plugin
    */
    void loadPlugin(const string &name);
    
    private:
      
    typedef map <string, VIOPlugin> PluginMap;
    
    /// the plugin map
    PluginMap loaded_plugins_;
    
    
    FileReadServer file_read_server_;
  //   FileWriteServer file_write_server_;
  //   StreamReadServer stream_read_server_;
  //   StreamWriteServer stream_write_server_;
  
  };
}
#endif
