//////////////////////////////////////////////////////////////////////////
//
//   VideoIO-Framework for GEM/PD
//
//   The server of the FileWrite plugins.
//
//   FileWriteServer
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

#ifndef FILE_WRITE_SERVER_H_
#define FILE_WRITE_SERVER_H_
    
#include "FileWrite.h"

using namespace std;

#include <vector>

namespace VideoIO_
{
  /*!
  * \class FileWriteServer
  * 
  * This is the server of the file write plugins.
  */
  class FileWriteServer
  {
    public:
    
    /// constructor
    FileWriteServer(){};
    
    /// destructor
    virtual ~FileWriteServer();
    
    /// adds a file write plugin
    void addFileWritePlugin( auto_ptr <FileWrite> frp); 
    
    /// @return the plugin counter
    int getPluginCount();
    
    /*!
    * @param index the index of the plugin
    * @return the plugin with the choosen index
    */
    FileWrite &getPlugin(int index=0);
    
    typedef vector <FileWrite *> FileWriteVector;
    
    /// the vector which holds all the file write plugins
    FileWriteVector file_write_plugins_;
  
  };
}
#endif
