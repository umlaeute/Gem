////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// Implementation file
//
// Copyright (c) 2007, Thomas Holzmann
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "pix_file_read.h"
#include <ctype.h>

#include <stdio.h>

CPPEXTERN_NEW_WITH_ONE_ARG(pix_file_read, t_symbol *, A_DEFSYM)

/////////////////////////////////////////////////////////
//
// pix_file_read
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_file_read :: pix_file_read(t_symbol *filename) :
    m_newfilm(false), m_already_banged(false), fileReader(0)
{
  // create outlet for frame data and bang at the end
  m_outNumFrames = outlet_new(this->x_obj, 0);
  m_outEnd       = outlet_new(this->x_obj, 0);

  /// TODO bang am ende des videos noch nicht implementiert
  /// schauen wie man das im gstreamer am besten macht, am
  /// einfachsten einfach wenn nach einem start kein neues frame
  /// in der render methode mehr kommt, dann ein bang ausgeben

  // get the FileRead plugin
  // NOTE: in future this could also go into openFile
  //       (if there are more plugins and we have to
  //        make priorities for types)
  fileReader = &m_kernel.getFileReadServer().getPlugin();
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_file_read :: ~pix_file_read()
{
  // Clean up the movie
  closeFile();
}

/////////////////////////////////////////////////////////
// closeFile
//
/////////////////////////////////////////////////////////
void pix_file_read :: closeFile(void)
{
  fileReader->closeFile();
}

/////////////////////////////////////////////////////////
// openFile
//
/////////////////////////////////////////////////////////
void pix_file_read :: openFile(t_symbol *filename)
{
  closeFile();
  m_already_banged = false;
  
  // make the right filename
  char tmp_buff[MAXPDSTRING];
  char *path=tmp_buff;
  canvas_makefilename(getCanvas(), filename->s_name, tmp_buff, MAXPDSTRING);
  if (FILE*fd=fopen(tmp_buff, "r")) fclose(fd);
  else path=filename->s_name;

  // open file
  if(!(fileReader->openFile(path)))
  {
    error("pix_file_read: could not open file %s", path);
    return;
  }
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void pix_file_read :: render(GemState *state)
{
  // get pointer to the frame data
  unsigned char *vioframe_ptr = fileReader->getFrameData();
  
  if( vioframe_ptr == NULL)
  {
    if(!m_already_banged)
    {
      outlet_bang(m_outEnd);
      m_already_banged = true;
    }
    return;
  }

  // check if image size changed
  if( m_image.image.xsize != fileReader->getWidth() ||
      m_image.image.ysize != fileReader->getHeight() ||
      m_image.image.csize != fileReader->getColorSize() )
    reallocate_m_image();
  
  // read frame data into m_image
  unsigned char *image_ptr = m_image.image.data;
  int i = m_image.image.xsize * m_image.image.ysize * m_image.image.csize ;
  while(i--)
  {
    *(image_ptr++) = *(vioframe_ptr++);
  }
  
  // set flag if we have a new film
  if(m_newfilm)
  {
    m_image.newfilm = true;
    m_newfilm = false;
  }
  m_image.newimage = true;

  m_image.newimage = true;
  state->image = &m_image;
}

/////////////////////////////////////////////////////////
// postrender
//
/////////////////////////////////////////////////////////
void pix_file_read :: postrender(GemState *state)
{
  if (state && state->image)
    state->image->newimage = 0;
}

/////////////////////////////////////////////////////////
// forceColorspace
//
/////////////////////////////////////////////////////////
void pix_file_read :: forceColorspace(t_symbol *cs)
{
  char c =*cs->s_name;
  switch (c)
  {
    case 'g': case 'G':
      fileReader->forceColorspace(VideoIO_::GRAY);
      break;
    case 'y': case 'Y':
      fileReader->forceColorspace(VideoIO_::YUV422);
      break;
    case 'r': case 'R': 
      if(gensym("RGB")==cs||gensym("rgb")==cs)
        fileReader->forceColorspace(VideoIO_::RGB);
      else
        fileReader->forceColorspace(VideoIO_::RGBA);
      break;
    default:
      error("colorspace must be 'RGBA', 'YUV' or 'Gray'");
  }
}

/////////////////////////////////////////////////////////
// reallocate_m_image
//
/////////////////////////////////////////////////////////
void pix_file_read :: reallocate_m_image()
{
  // allocate memory for m_image
  m_image.image.xsize = fileReader->getWidth();
  m_image.image.ysize = fileReader->getHeight();
  m_image.image.type = GL_UNSIGNED_BYTE; /// TODO under OSX ?
  m_image.image.csize = fileReader->getColorSize();

  switch( fileReader->getColorspace() )
  {
    case VideoIO_::GRAY:
      m_image.image.format = GL_LUMINANCE;
      break;
    case VideoIO_::YUV422:
      m_image.image.format = GL_YCBCR_422_GEM;
      break;
    case VideoIO_::RGB:
      m_image.image.format = GL_RGB;
      break;
    case VideoIO_::RGBA:
      m_image.image.format = GL_RGBA;
      break;
    default:
      error("error in reallocate_m_image");
  }

  m_image.image.reallocate();
  m_newfilm = true;
  
  t_atom ap[4];
  SETFLOAT(ap, fileReader->getNrOfFrames() );
  SETFLOAT(ap+1, fileReader->getWidth() );
  SETFLOAT(ap+2, fileReader->getHeight() );
  SETFLOAT(ap+3, (float)fileReader->getFPS() );

  post("pix_file_read: loaded file with %d frames (%dx%d) at %f fps", 
      fileReader->getNrOfFrames(), 
      fileReader->getWidth(), 
      fileReader->getHeight(), (float)fileReader->getFPS());
  outlet_list(m_outNumFrames, 0, 4, ap);
}


/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_file_read :: obj_setupCallback(t_class *classPtr)
{
  class_addcreator((t_newmethod)_classpix_file_read, gensym("pix_file_read"), A_DEFSYM, A_NULL);

  class_addmethod(classPtr, (t_method)&pix_file_read::openMessCallback,
                  gensym("open"), A_DEFSYM, A_NULL);

  class_addmethod(classPtr, (t_method)&pix_file_read::startCallback,
                  gensym("start"), A_DEFFLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_file_read::stopCallback,
                  gensym("stop"), A_DEFFLOAT, A_NULL);

  class_addmethod(classPtr, (t_method)&pix_file_read::seekCallback,
                  gensym("seek"), A_DEFFLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_file_read::seekCallback,
                  gensym("getFrame"), A_DEFFLOAT, A_NULL);

  class_addmethod(classPtr, (t_method)&pix_file_read::csCallback,
                  gensym("forceColorspace"), A_DEFSYM, A_NULL);
}

void pix_file_read :: openMessCallback(void *data, t_symbol*s)
{
  GetMyClass(data)->openFile(s);
}

void pix_file_read :: startCallback(void *data, t_floatarg start)
{
  GetMyClass(data)->fileReader->startVideo();
}

void pix_file_read :: stopCallback(void *data, t_floatarg stop)
{
  GetMyClass(data)->fileReader->stopVideo();
}

void pix_file_read :: seekCallback(void *data, t_floatarg seek)
{
  GetMyClass(data)->fileReader->setPosition( (int)seek );
}

void pix_file_read :: csCallback(void *data, t_symbol *s)
{
  GetMyClass(data)->forceColorspace(s);
}
