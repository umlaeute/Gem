//////////////////////////////////////////////////////////////////////////
//
//   VideoIO-Framework for GEM/PD
//
//   The server of the FileRead plugins.
//
//   PluginServer
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

#ifndef PLUGIN_SERVER_H_
#define PLUGIN_SERVER_H_

using namespace std;  

#include <vector>

namespace VideoIO_
{
  /*!
  * \class PluginServer
  *
  * This is the server of the plugins.
  */
  template<class PluginType> class PluginServer
  {
   public:
    
    /// constructor
    PluginServer(){};
    
    /// destructor
    virtual ~PluginServer();

    /*!
    * adds a plugin
    */
    void addPlugin(auto_ptr<PluginType> plug)
    { plugins_.push_back( plug.release() ); }

    /*!
    * @param index the index of the plugin
    * @return the plugin with the choosen index
    */
    PluginType &getPlugin(int index=0);
    
    /// @return the Nr. of plugins
    int getPluginCount()
    { return plugins_.size(); }

   protected:

    /// the map which holds all the file read plugins
    vector<PluginType *> plugins_;
  };


  // Implementations

  template<class PluginType>
  PluginServer<PluginType>::~PluginServer()
  {
    for(int i=0; i<plugins_.size(); ++i)
    {
      delete plugins_[i];
    }
  }

  template<class PluginType>
  PluginType &PluginServer<PluginType>::getPlugin(int index)
  {
    /// TODO bessere priorities der einzelnen plugins machen
    /// d.h. nicht in einem vector speichern, sondern irgendwo
    /// anders und eine Liste für welchen mimetype welches
    /// plugin am besten genommen werden soll !

    return *plugins_.at(index);
  }

}

#endif
