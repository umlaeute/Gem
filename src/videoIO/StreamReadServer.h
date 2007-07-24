//////////////////////////////////////////////////////////////////////////
//
//   VideoIO-Framework for GEM/PD
//
//   The server of the StreamRead plugins.
//
//   StreamReadServer
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

#ifndef STREAM_READ_SERVER_H_
#define STREAM_READ_SERVER_H_
    
#include "StreamRead.h"

using namespace std;  

#include <vector>

namespace VideoIO_
{
  /*!
  * \class StreamReadServer
  * 
  * This is the server of the file read plugins.
  */
  class StreamReadServer
  {
    public:
    
    /// constructor
    StreamReadServer(){};
    
    /// destructor
    virtual ~StreamReadServer();
    
    /*!
    * adds a file read plugin
    
    */
    void addStreamReadPlugin( auto_ptr <StreamRead> frp); 
    
    /// @return the plugin counter
    int getPluginCount();
    
    /*!
    * @param index the index of the plugin
    * @return the plugin with the choosen index
    */
    StreamRead &getPlugin(int index=0);
    
    typedef vector <StreamRead *> StreamReadVector;
    
    /// the vector which holds all the file read plugins
    StreamReadVector stream_read_plugins_;
  
  };
}
#endif
