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

using namespace std;  /// nachschauen wie das geschrieben wird


/*!
 * \class VIOKernel
 * 
 * The VIOKernel has the different plugin server and the
 * different plugins.
 */
class VIOKernel
{
  /// constructor
  VIOKernel();
  
  /// destructor
  virtual ~VIOKernel();
  
  
  /// @return the file read server
  FileReadServer &getFileReadServer();
  /// @return the file write server
  FileWriteServer &getWriteReadServer();
  /// @return the stream read server
  StreamReadServer &getStreamReadServer();
  /// @return the stream write server
  StreamWriteServer &getStreamWriteServer();
  
  /*!
   * loads a plugin
   * @param filename the filename of the plugin
   */
  void loadPlugin(string filename); 
  
  typedef map <string, Plugin> PluginMap;
  
  /// the plugin map
  PluginMap loaded_plugins_;
  
  
  FileReadServer file_read_server_;
  FileWriteServer file_write_server_;
  StreamReadServer stream_read_server_;
  StreamWriteServer stream_write_server_;

}

#endif