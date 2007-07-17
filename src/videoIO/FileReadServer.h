//////////////////////////////////////////////////////////////////////////
//
//   VideoIO-Framework for GEM/PD
//
//   The server of the FileRead plugins.
//
//   FileReadServer
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

#ifndef FILE_READ_SERVER_H_
#define FILE_READ_SERVER_H_
    
#include "FileRead.h"

using namespace std;  

#include <vector>

namespace VideoIO_
{
  /*!
  * \class FileReadServer
  * 
  * This is the server of the file read plugins.
  */
  class FileReadServer
  {
    public:
    
    /// constructor
    FileReadServer(){};
    
    /// destructor
    virtual ~FileReadServer();
    
    /*!
    * adds a file read plugin
    
    */
    void addFileReadPlugin( auto_ptr <FileRead> frp); 
    
    /// @return the plugin counter
    int getPluginCount();
    
    /*!
    * @param index the index of the plugin
    * @return the plugin with the choosen index
    */
    FileRead &getPlugin(int index=0);
    
    typedef vector <FileRead *> FileReadVector;
    
    /// the vector which holds all the file read plugins
    FileReadVector file_read_plugins_;
  
  };
}
#endif
