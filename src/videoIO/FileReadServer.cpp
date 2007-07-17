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

#include "FileReadServer.h"

namespace VideoIO_
{
  FileReadServer::~FileReadServer()
  {
    for(FileReadVector::reverse_iterator It = file_read_plugins_.rbegin();
      It != file_read_plugins_.rend(); ++It)
        delete *It;
  }
  
  void FileReadServer::addFileReadPlugin( auto_ptr <FileRead> frp)
  {
    file_read_plugins_.push_back(frp.release());
  }
  
  int FileReadServer::getPluginCount()
  {
    return file_read_plugins_.size();
  }
  
  FileRead &FileReadServer::getPlugin(int index)
  {
    return *file_read_plugins_.at(index);
  }
}

