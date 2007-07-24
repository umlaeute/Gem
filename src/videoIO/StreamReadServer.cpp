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

#include "StreamReadServer.h"

namespace VideoIO_
{
  StreamReadServer::~StreamReadServer()
  {
    for(StreamReadVector::reverse_iterator It = stream_read_plugins_.rbegin();
      It != stream_read_plugins_.rend(); ++It)
        delete *It;
  }
  
  void StreamReadServer::addStreamReadPlugin( auto_ptr <StreamRead> frp)
  {
    stream_read_plugins_.push_back(frp.release());
  }
  
  int StreamReadServer::getPluginCount()
  {
    return stream_read_plugins_.size();
  }
  
  StreamRead &StreamReadServer::getPlugin(int index)
  {
    return *stream_read_plugins_.at(index);
  }
}

