//////////////////////////////////////////////////////////////////////////
//
//   VideoIO-Framework for GEM/PD
//
//   Writes a digital video (like AVI, Mpeg, Quicktime) to the harddisc.
//
//   FileWrite
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

#include "pix_file_write.h"

CPPEXTERN_NEW_WITH_GIMME(pix_file_write)

pix_file_write :: pix_file_write(int argc, t_atom *argv):
    m_filewriter(NULL), m_recording(false), m_first_time(false)
{
  /// TODO filenamen als init argument lesen
}

pix_file_write :: ~pix_file_write()
{

}

void pix_file_write :: render(GemState *state)
{
  if(!m_filewriter) return;
  if(!m_recording) return;
  
  imageStruct *im = &state->image->image;

  if( m_first_time )
  {
    // get format data from GEM
    int xsize = im->xsize;
    int ysize = im->ysize;
    int csize = im->csize;
    //int type = im->type; /// TODO type noch richtig handeln
    int format;

    switch(im->format)
    {
      case GL_LUMINANCE:
        format = VideoIO_::GRAY;
        break;

      case GL_YCBCR_422_GEM:
      /// TODO fuer apple gibts da noch ein anderes define
      ///      siehe GemPixUtils.h
        format = VideoIO_::YUV422;
        break;
        
      case GL_RGB:
        format = VideoIO_::RGB;
        break;
    
      case GL_RGBA:
      default:
        format = VideoIO_::RGBA;
        break;
    }

    post("-------------------------------------");
    post("writing to video file:");
    post("xsize: %d", xsize);
    post("ysize: %d", ysize);
    post("csize: %d", csize);
    post("format: %d", format);

    // allocate m_frame
    m_frame.allocate(xsize, ysize, format);

    /// TODO Methoden im Filewriter machen fuer
    ///      compression properties usw.

    // init recording data
    m_filewriter->setSize(xsize,ysize);
    m_filewriter->setColorspace(format);
    m_filewriter->initRecording();

    m_first_time = false;
  }
  
  // copy data into our m_frame
  unsigned char *m_frame_data = m_frame.getFrameData();
  unsigned char *image_data = im->data;
  int i = m_frame.getXSize() * m_frame.getYSize() * m_frame.getColorSize() ;
  while(i--)
  {
    *(m_frame_data++) = *(image_data++);
  }
  
  m_filewriter->pushFrame(m_frame);
}

void pix_file_write :: openFile(t_symbol *filename)
{
  char tmp_buff[MAXPDSTRING];
  char *path=tmp_buff;
  // we first try to find the file-to-open with canvas_makefilename
  // if this fails, we just pass the given filename (could be a stream)
  canvas_makefilename(getCanvas(), filename->s_name, tmp_buff, MAXPDSTRING);
  
  if (FILE*fd=fopen(tmp_buff, "r")) fclose(fd);
    else path=filename->s_name;
  
  // get plugin and open file
  m_filewriter = &m_kernel.getFileWriteServer().getPlugin();
  
  if( !m_filewriter->openFile(path) )
  {
    error("GEM: pix_file_write: file cannot be created");
    return;
  }
}


/////////////////////////////////////////////////////////
// static member functions
//
/////////////////////////////////////////////////////////

void pix_file_write :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr, (t_method)&pix_file_write::fileMessCallback,
		  gensym("open"), A_GIMME, A_NULL);
  
  class_addmethod(classPtr, (t_method)&pix_file_write::recordMessCallback,
		  gensym("record"), A_FLOAT, A_NULL);
}

void pix_file_write :: fileMessCallback(void *data, t_symbol *s, int argc, t_atom *argv)
{
  if (argv[0].a_type != A_SYMBOL) goto illegal_openmess;

  GetMyClass(data)->openFile(atom_getsymbol(argv));
  return;
 
illegal_openmess:
     GetMyClass(data)->error("open <filename>");
 return;
}

void pix_file_write :: recordMessCallback(void *data, t_floatarg on)
{
  if( (int)on )
  {
    GetMyClass(data)->m_recording = true;
    GetMyClass(data)->m_first_time = true;
  }
  else
  {
    GetMyClass(data)->m_recording = false;
    GetMyClass(data)->m_filewriter->stopRecording();
  }
}
