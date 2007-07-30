//////////////////////////////////////////////////////////////////////////
//
//   VideoIO-Framework for GEM/PD
//
//   The plugin for GStreamer.
//
//   FileReadGst
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

#include "FileReadGst.h"


bool FileReadGst::is_initialized_ = false;

FileReadGst::FileReadGst() 
{
 
}

bool FileReadGst::openFile(string filename)
{
    guint major, minor, micro, nano;
  string nano_str;
  

  if(!is_initialized_)
  {
    if(gst_init_check (NULL, NULL, NULL))  ///TODO funktioniert nicht richtig
      is_initialized_ = true;
    else
    {
      post("Gstreamer could not be initialised");
      return false;
    }
  }
  
  gst_version (&major, &minor, &micro, &nano);
  
  if (nano == 1)
    nano_str = "(CVS)";
  else if (nano == 2)
    nano_str = "(Prerelease)";
  else
    nano_str = "";
  
  post ("This program is linked against GStreamer %d.%d.%d %s",
          major, minor, micro, nano_str.c_str());
  
  
  
  play_ = gst_element_factory_make ("playbin", "play_");
  if(!play_)
    post("One element could not be loaded.");
  
  g_object_set (G_OBJECT(play_), "uri", filename.c_str(), NULL);
  
  
  GstFormat fmt = GST_FORMAT_DEFAULT;
  gint64 length;
  gst_element_query_duration (play_, &fmt, &length);
  nr_of_frames_ = length;
  
  
  /* start playback */
  
  gst_element_set_state (GST_ELEMENT (play_), GST_STATE_PLAYING);
  

  
  
}


  
/// Tells us to register our functionality to an engine kernel
void registerPlugin(VIOKernel &K)
{
  post("hurra - wir sind in der Library da !!!!");
  
  K.getFileReadServer().addFileReadPlugin(
    auto_ptr<FileRead>(new FileReadGst()));
  
  post("und schon wieder draußen");
}

