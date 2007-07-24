//////////////////////////////////////////////////////////////////////////
//
//   VideoIO-Framework for GEM/PD
//
//   The Quicktime 4 Linux file read plugin
//
//   FileReadQT4L
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

#include "FileReadQT4L.h"


FileReadQT4L::FileReadQT4L()
{
  qt_file_ = 0;
}

FileReadQT4L::~FileReadQT4L()
{
  closeFile();
}



bool FileReadQT4L::openFile(string filename)
      throw (VIOException)
{
  if(!(quicktime_check_sig((char *)filename.c_str())))/// TODO cast korrekt von const char* auf char* ???
    return false;
  
  if(!(qt_file_ = quicktime_open((char *)filename.c_str(), 1, 0)))
    return false;
  
  //throw VideoIO_::VIOException(string("File cannot be opened"));
  
  nr_of_tracks_ = quicktime_video_tracks(qt_file_);
 
  nr_of_frames_ = (int) quicktime_video_length(qt_file_, cur_track_);
  
  frame_.allocate(quicktime_video_width(qt_file_, cur_track_), 
                  quicktime_video_height(qt_file_, cur_track_), RGBA); ///TODO at the moment only RGBA
  
  has_video_file_ = true;

  
  return true; 
}



void FileReadQT4L::closeFile()
{
  post("in closeFile");
   quicktime_close(qt_file_);
}

bool FileReadQT4L::setPosition(int frame, int track)
{
  if(frame > nr_of_frames_ || frame < 0)
    return false;
  
  if( quicktime_set_video_position(qt_file_, cur_frame_, cur_track_))
  {
    cur_frame_ = frame;
    if (nr_of_tracks_ > 0) 
      cur_track_ = track;
    return true;
  }
  
  else
    return false;
  
}


unsigned char *FileReadQT4L::getFrameData()
{
  //int quicktime_supported_video(quicktime_t *file, int track);
  /// TODO should i also use this??
  
  int i = frame_.getYSize();
  
  unsigned char **rows = new unsigned char*[frame_.getYSize()];
  
  
  
  while(i--)
   rows[i] = frame_.getFrameData() + frame_.getXSize()*frame_.getColorSize()*(frame_.getYSize()-i-1);
  post("a");
//   for(int i=0; i<frame_.getYSize(); i++)
//     rows[i] = frame_.getFrameData() + frame_.getXSize()*frame_.getColorSize()*i;
  post("b");
  quicktime_set_cmodel(qt_file_, BC_RGBA8888); ///sets return type in RGBA
  post("c");
  quicktime_decode_video(qt_file_, rows, cur_track_);  /// TODO what does return value mean
  post("d");
  
  delete[] rows;
  return frame_.getFrameData();
  
}





/// Tells us to register our functionality to an engine kernel
void registerPlugin(VIOKernel &K)
{
  post("hurra - wir sind in der Library da !!!!");
  
  K.getFileReadServer().addFileReadPlugin(
    auto_ptr<FileRead>(new FileReadQT4L()));
  
  post("und schon wieder draußen");
}


