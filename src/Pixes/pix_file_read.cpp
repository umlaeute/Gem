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

#include "Base/GemMan.h"

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
    m_newfilm(false), m_already_banged(true), fileReader(0)
{
  // create audio outlets
  m_outAudio[0]=outlet_new(this->x_obj, &s_signal);
  m_outAudio[1]=outlet_new(this->x_obj, &s_signal);

  // create outlet for frame data and bang at the end
  m_outNumFrames = outlet_new(this->x_obj, 0);
  m_outEnd       = outlet_new(this->x_obj, 0);

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

  outlet_free(m_outAudio[0]);
  outlet_free(m_outAudio[1]);
  outlet_free(m_outNumFrames);
  outlet_free(m_outEnd);
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

  // make the right filename
  char tmp_buff[MAXPDSTRING];
  char *path=tmp_buff;
  canvas_makefilename(getCanvas(), filename->s_name, tmp_buff, MAXPDSTRING);
  if (FILE*fd=fopen(tmp_buff, "r")) fclose(fd);
  else path=filename->s_name;

  // get GEM framerate
  fileReader->setHostFramerate( GemMan::getFramerate() );

  // open file
  if(!(fileReader->openFile(path)))
  {
    error("pix_file_read: could not open file %s", path);
    return;
  }
}

/////////////////////////////////////////////////////////
// DSP Message
//
/////////////////////////////////////////////////////////
void pix_file_read :: dspMess(void *data, t_signal** sp)
{
  dsp_add(perform, 4, data, sp[0]->s_vec, sp[1]->s_vec, sp[0]->s_n);
}

/////////////////////////////////////////////////////////
// DSP Loop
//
/////////////////////////////////////////////////////////
t_int* pix_file_read :: perform(t_int* w)
{
  pix_file_read *x = GetMyClass((void*)w[1]);

  // stereo only for now
  t_float* left = (t_float*)(w[2]);
  t_float* right = (t_float*)(w[3]);
  int N = (t_int)(w[4]);

  x->fileReader->getAudioBlock(left, right, N);

  return (w+5);
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
    // output end of video bang in playing mode
    // and stop video
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
  
  t_atom ap[3];
  SETFLOAT(ap, fileReader->getDuration() );
  SETFLOAT(ap+1, fileReader->getWidth() );
  SETFLOAT(ap+2, fileReader->getHeight() );

  post("loaded file with %f msec (%dx%d) at %f fps", 
      fileReader->getDuration(), 
      fileReader->getWidth(), 
      fileReader->getHeight(), (float)fileReader->getFPS());
  outlet_list(m_outNumFrames, 0, 3, ap);
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
  class_addmethod(classPtr, (t_method)&pix_file_read::speedCallback,
                  gensym("speed"), A_DEFFLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_file_read::csCallback,
                  gensym("forceColorspace"), A_DEFSYM, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_file_read::atrackCallback,
                  gensym("audioTrack"), A_DEFFLOAT, A_NULL);
    class_addmethod(classPtr, (t_method)&pix_file_read::vtrackCallback,
                  gensym("videoTrack"), A_DEFFLOAT, A_NULL);

  class_addmethod(classPtr, (t_method)&pix_file_read::dspMessCallback,
		  gensym("dsp"), A_NULL);
}

void pix_file_read :: openMessCallback(void *data, t_symbol*s)
{
  GetMyClass(data)->openFile(s);
}

void pix_file_read :: startCallback(void *data, t_floatarg start)
{
  GetMyClass(data)->fileReader->startVideo();
  GetMyClass(data)->m_already_banged=false;
}

void pix_file_read :: stopCallback(void *data, t_floatarg stop)
{
  GetMyClass(data)->fileReader->stopVideo();
  GetMyClass(data)->m_already_banged=true;
}

void pix_file_read :: seekCallback(void *data, t_floatarg seek)
{
  GetMyClass(data)->fileReader->setPosition( seek );
}

void pix_file_read :: speedCallback(void *data, t_floatarg speed)
{
  GetMyClass(data)->fileReader->setSpeed(speed);
}

void pix_file_read :: csCallback(void *data, t_symbol *s)
{
  GetMyClass(data)->forceColorspace(s);
}

void pix_file_read :: atrackCallback(void *data, t_floatarg track)
{
  GetMyClass(data)->fileReader->setAudioTrack((int)track);
}

void pix_file_read :: vtrackCallback(void *data, t_floatarg track)
{
  GetMyClass(data)->fileReader->setVideoTrack((int)track);
}

void pix_file_read :: dspMessCallback(void *data, t_signal **sp)
{
  GetMyClass(data)->dspMess(data, sp);
}
