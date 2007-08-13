//////////////////////////////////////////////////////////////////////////
//
//   VideoIO-Framework for GEM/PD
//
//   The server of the FileWrite plugins.
//
//   FileWriteServer
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

#include "FileWriteServer.h"

namespace VideoIO_
{
  FileWriteServer::~FileWriteServer()
  {
    for(FileWriteVector::reverse_iterator It = file_write_plugins_.rbegin();
      It != file_write_plugins_.rend(); ++It)
        delete *It;
  }
  
  void FileWriteServer::addFileWritePlugin( auto_ptr <FileWrite> frp)
  {
    file_write_plugins_.push_back(frp.release());
  }
  
  int FileWriteServer::getPluginCount()
  {
    return file_write_plugins_.size();
  }
  
  FileWrite &FileWriteServer::getPlugin(int index)
  {
    return *file_write_plugins_.at(index);
  }
}

