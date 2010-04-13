////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file 
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <string.h>
#include "filmQT4L.h"
using namespace gem;

#ifdef HAVE_LIBQUICKTIME
# include <colormodels.h>

REGISTER_FILMFACTORY("QT4L", filmQT4L);
#endif

/////////////////////////////////////////////////////////
//
// filmQT4L
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////

filmQT4L :: filmQT4L(int format) : film(format) {
  static bool first_time=true;
  if (first_time) {
#ifdef HAVE_LIBQUICKTIME
    post("pix_film:: quicktime support");
#endif
    first_time = false;
  }
#ifdef HAVE_LIBQUICKTIME
  m_quickfile=0;
#endif
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
filmQT4L :: ~filmQT4L()
{
  close();
}

#ifdef HAVE_LIBQUICKTIME
void filmQT4L :: close(void)
{
  if(m_quickfile)quicktime_close(m_quickfile);
  m_quickfile=0;
}

/////////////////////////////////////////////////////////
// really open the file ! (OS dependent)
//
/////////////////////////////////////////////////////////
bool filmQT4L :: open(char *filename, int format)
{
  int wantedFormat=GL_RGBA;
  switch(format){
  default:
    break;
  case GL_RGBA:
  case GL_YCBCR_422_GEM:
  case GL_LUMINANCE:
    m_wantedFormat=format;
    break;
  }

  if (quicktime_check_sig(filename)){ /* ok, this is quicktime */
    if (!(m_quickfile = quicktime_open(filename, 1, 0))){
//      post("GEM: pix_film: Unable to open file: %s", filename);
      return false;
    }
    m_curFrame = -1;
	
    // Get the number of tracks
    m_numTracks = quicktime_video_tracks(m_quickfile);
    // Get the length of the movie (on track current track)
    m_numFrames = quicktime_video_length(m_quickfile, m_curTrack) - 1;
    // Get the frame-rate
    m_fps = quicktime_frame_rate(m_quickfile, m_curTrack);
    // Get the video dimensions
    m_image.image.xsize = quicktime_video_width (m_quickfile, m_curTrack);
    m_image.image.ysize = quicktime_video_height(m_quickfile, m_curTrack);
    if (!quicktime_supported_video(m_quickfile, m_curTrack)){
      //    char *codec = quicktime_video_compressor(m_quickfile, m_curTrack);
      //    post("GEM: pix_film: unsupported CODEC '%s'!", codec);
      quicktime_close(m_quickfile);
      m_quickfile=0;
      return false;
    }
    m_image.image.setCsizeByFormat(wantedFormat);
    m_image.image.reallocate();

    m_qtimage.xsize=m_image.image.xsize;
    m_qtimage.ysize=m_image.image.ysize;
    m_qtimage.setCsizeByFormat(GL_RGB);
    m_qtimage.reallocate();

    m_newfilm = true;
   return true;
  }
  goto unsupported;
 unsupported:
  startpost("quicktime failed");
  close();
  return false;
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
pixBlock* filmQT4L :: getFrame(){
  int i=m_image.image.ysize;
  if (m_lastFrame==m_curFrame && 
      m_image.image.format==m_wantedFormat) 
  {
	  m_image.newimage=0; 
	  return &m_image;
  }

  m_image.image.setCsizeByFormat(m_wantedFormat);
  m_image.image.reallocate();

  pixBlock* pimage = 0;
  unsigned char **rows = new unsigned char*[m_image.image.ysize];
  while(i--)rows[i]=m_qtimage.data+m_qtimage.xsize*m_qtimage.csize*(m_qtimage.ysize-i-1);

  m_lastFrame=m_curFrame;

  if (quicktime_decode_video(m_quickfile, rows, m_curTrack)) {
    post("GEM: pix_film:: couldn't decode video !");
  }else {
    m_image.image.convertFrom(&m_qtimage);
    m_image.newimage=1; m_image.image.upsidedown=false;
    pimage = &m_image;
    if(m_newfilm)m_image.newfilm=1; 
	m_newfilm=false;
  }
  delete[] rows;
  return pimage;
}

int filmQT4L :: changeImage(int imgNum, int trackNum){
  if(imgNum>m_numFrames || imgNum<0)return FILM_ERROR_FAILURE;
  if  (imgNum>0)m_curFrame=imgNum;
  if(trackNum>0)m_curTrack=trackNum;

  int i=-1;
  if ((i=quicktime_set_video_position(m_quickfile, m_curFrame, m_curTrack))){  }
  return FILM_ERROR_SUCCESS;
}
#endif // QT4L
