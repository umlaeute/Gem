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
#include <string.h>
#include "Pixes/filmQT4L.h"

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

void filmQT4L :: close(void)
{
  //  post("qt4l: closing");
#ifdef HAVE_LIBQUICKTIME
  if(m_quickfile)quicktime_close(m_quickfile);
  m_quickfile=0;
#endif
}

/////////////////////////////////////////////////////////
// really open the file ! (OS dependent)
//
/////////////////////////////////////////////////////////
bool filmQT4L :: open(char *filename, int format)
{
  if (format>0)m_wantedFormat=format;
  int wantedFormat= (m_wantedFormat)?m_wantedFormat:GL_RGBA;
#ifdef HAVE_LIBQUICKTIME
  if (quicktime_check_sig(filename)){ /* ok, this is quicktime */
    if (!(m_quickfile = quicktime_open(filename, 1, 0))){
      post("GEM: pix_film: Unable to open file: %s", filename);
      return false;
    }
    m_curFrame = -1;
	
    // Get the number of tracks
    m_numTracks = quicktime_video_tracks(m_quickfile);
    // Get the length of the movie (on track 0)
    m_numFrames = quicktime_video_length(m_quickfile, 0) - 1;
    // Get the video dimensions
    m_image.image.xsize = quicktime_video_width (m_quickfile, 0);
    m_image.image.ysize = quicktime_video_height(m_quickfile, 0);
    char *codec = quicktime_video_compressor(m_quickfile, 0);
    if (!quicktime_supported_video(m_quickfile, 0)){
      post("GEM: pix_film: unsupported CODEC '%s'!", codec);
      quicktime_close(m_quickfile);
      return false;
    } 
    switch (wantedFormat){
    case GL_BGR:
      if (quicktime_reads_cmodel(m_quickfile, BC_BGR888, 0)){
	m_image.image.csize=3; m_qtformat=BC_BGR888;
      } else goto label_rgb;
      break;
      break;
    default:
    case GL_RGBA:
      //      post("rgba");
      if (quicktime_reads_cmodel(m_quickfile, BC_RGBA8888, 0)){
	m_image.image.csize=4;m_qtformat=BC_RGBA8888;
      } else goto label_rgb;
      break;
    case GL_YCBCR_422_GEM: // this crashes ?
      if (quicktime_reads_cmodel(m_quickfile, BC_YUV422P, 0)){
	m_image.image.csize=2;m_qtformat=BC_YUV422P;
      } else goto label_rgb;
      break;
    label_rgb:
   case GL_RGB:
     //     post("rgb");
      if (quicktime_reads_cmodel(m_quickfile, BC_RGB888, 0)){
	m_image.image.csize=3;m_qtformat=0;
	wantedFormat=GL_RGB;
      }
    }
    m_image.image.format=wantedFormat;
    m_image.image.reallocate();
    return true;
  }
#endif
  goto unsupported;
 unsupported:
  post("quicktime4linux: unsupported!");
  close();
  return false;
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
pixBlock* filmQT4L :: getFrame(){
  int i=m_image.image.ysize;
  if (m_lastFrame==m_curFrame){m_image.newimage=0; return &m_image;}
  pixBlock* pimage = 0;
#ifdef HAVE_LIBQUICKTIME
  unsigned char **rows = new unsigned char*[m_image.image.ysize];
  m_lastFrame=m_curFrame;
  while(i--)rows[i]=m_image.image.data
	      +m_image.image.xsize*m_image.image.csize*(m_image.image.ysize-i-1);
  if (m_qtformat){
    if (quicktime_decode_scaled(m_quickfile, 
				0, 0, m_image.image.xsize, m_image.image.ysize, m_image.image.xsize, m_image.image.ysize,
				m_qtformat,
				&m_image.image.data, 0)) {
      post("GEM: pix_film:: couldn't decode video !");
    } else {m_image.newimage=1;pimage = &m_image;}
  } else {
    if (quicktime_decode_video(m_quickfile, rows, m_curTrack)) {
      post("GEM: pix_film:: couldn't decode video !");
    }else {m_image.newimage=1;pimage = &m_image;}
  }
  delete[] rows;
#endif
  return pimage;
}

int filmQT4L :: changeImage(int imgNum, int trackNum){
  if  (imgNum>0)m_curFrame=imgNum;
  if(trackNum>0)m_curTrack=trackNum;

#ifdef HAVE_LIBQUICKTIME
  if (quicktime_set_video_position(m_quickfile, m_curFrame, m_curTrack)){}
  return FILM_ERROR_SUCCESS;
#endif
  return FILM_ERROR_DONTKNOW;
}
