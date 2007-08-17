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

#include "Base/GemMan.h"

CPPEXTERN_NEW_WITH_ONE_ARG(pix_file_write, t_symbol *, A_DEFSYM)

pix_file_write :: pix_file_write(t_symbol *filename):
    fileWriter(0), m_recording(false), m_first_time(false)
{
  // get the FileWrite plugin
  // NOTE: in future this could also go into openFile
  //       (if there are more plugins and we have to
  //        make priorities for types)
  fileWriter = &m_kernel.getFileWriteServer().getPlugin();
}

pix_file_write :: ~pix_file_write()
{
  fileWriter->stopRecording();
}

void pix_file_write :: openFile(t_symbol *filename)
{
  fileWriter->stopRecording();

  // make the right filename
  char tmp_buff[MAXPDSTRING];
  char *path=tmp_buff;
  canvas_makefilename(getCanvas(), filename->s_name, tmp_buff, MAXPDSTRING);
  if (FILE*fd=fopen(tmp_buff, "r")) fclose(fd);
  else path=filename->s_name;

  // open file
  if( !fileWriter->openFile(path) )
  {
    error("pix_file_write: could not open file %s", path);
    return;
  }
}

void pix_file_write :: render(GemState *state)
{
  if(!m_recording) return;
  
  imageStruct *im = &state->image->image;

  if( m_first_time )
  {
    // get format data from GEM
    int xsize = im->xsize;
    int ysize = im->ysize;
    int csize = im->csize;
    /// TODO handle type correct for OSX
    //int type = im->type;
    int format;

    switch(im->format)
    {
      case GL_LUMINANCE:
        format = VideoIO_::GRAY;
        break;

      case GL_YCBCR_422_GEM:
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


    /// TODO framerate von GEM muss man bekommen und auch
    ///      angeben für den fileWriter
    // schauen wie das pix_record macht !
//     float framerate = ( 0.0 != s_deltime) ?
//                         (1000. / s_deltime) : 0.0 );
//     post("-------- framerate: %f", GemMan::fps);
    fileWriter->setFramerate( 20. );

    m_first_time = false;
  }
  
  // copy data into our m_frame
  unsigned char *m_frame_data = m_frame.getFrameData();
  unsigned char *image_data = im->data;
  int i = m_frame.getXSize() * m_frame.getYSize() * m_frame.getColorSize() ;
  while(i--) *(m_frame_data++) = *(image_data++);
  
  fileWriter->pushFrame(m_frame);
}


/////////////////////////////////////////////////////////
// static member functions
//
/////////////////////////////////////////////////////////

void pix_file_write :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr, (t_method)&pix_file_write::openMessCallback,
		  gensym("open"), A_DEFSYM, A_NULL);

  class_addmethod(classPtr, (t_method)&pix_file_write::startCallback,
                  gensym("start"), A_DEFFLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_file_write::stopCallback,
                  gensym("stop"), A_DEFFLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_file_write::setCodecCallback,
		  gensym("codec"), A_DEFSYM, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_file_write::getCodecCallback,
		  gensym("getCodec"), A_NULL);
}

void pix_file_write :: openMessCallback(void *data, t_symbol*s)
{
  GetMyClass(data)->openFile(s);
}

void pix_file_write :: startCallback(void *data, t_floatarg start)
{
  GetMyClass(data)->m_recording = true;
  GetMyClass(data)->m_first_time = true;
}

void pix_file_write :: stopCallback(void *data, t_floatarg stop)
{
  GetMyClass(data)->m_recording = false;
  GetMyClass(data)->fileWriter->stopRecording();
}

void pix_file_write :: setCodecCallback(void *data, t_symbol*s)
{
  GetMyClass(data)->fileWriter->setCodec(s->s_name); 
}

void pix_file_write :: getCodecCallback(void *data)
{
  GetMyClass(data)->fileWriter->getCodec(); 
}
