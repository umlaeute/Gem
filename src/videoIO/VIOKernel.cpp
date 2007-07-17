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

using namespace std;

namespace VideoIO_
{
  void VIOKernel::loadPlugin(const string &name)
  {
    post("in the kernel");
    if( loaded_plugins_.find(name) == loaded_plugins_.end() )
    {
      loaded_plugins_.insert(make_pair(name, VIOPlugin(name))
                            ).first->second.registerPlugin(*this);
    }
  }
}
