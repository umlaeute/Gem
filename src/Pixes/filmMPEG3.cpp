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
#include "Pixes/filmMPEG3.h"

/////////////////////////////////////////////////////////
//
// filmMPEG3
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////

filmMPEG3 :: filmMPEG3(int format) : film(format) {
  static bool first_time=true;
  if (first_time) {
#ifdef HAVE_LIBMPEG3
    post("pix_film:: libmpeg3 support");
#endif
    first_time = false;
  }

#ifdef HAVE_LIBMPEG3
  mpeg_file=0;
#endif
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
filmMPEG3 :: ~filmMPEG3()
{
  close();
}
#ifdef HAVE_LIBMPEG3
void filmMPEG3 :: close(void)
{
  if(mpeg_file)mpeg3_close(mpeg_file);
  mpeg_file=NULL;
}

/////////////////////////////////////////////////////////
// really open the file ! (OS dependent)
//
/////////////////////////////////////////////////////////
bool filmMPEG3 :: open(char *filename, int format)
{
  if (mpeg3_check_sig(filename)){/* ok, this is mpeg(3) */
    mpeg_file= mpeg3_open(filename);
    if (!mpeg3_has_video(mpeg_file)){
      post("GEM: pix_film: this file %s does not seem to hold any video data", filename);
      goto unsupported;
    }
    m_numTracks = mpeg3_total_vstreams(mpeg_file);
    m_numFrames = mpeg3_video_frames(mpeg_file, m_curTrack);

    m_image.image.xsize=mpeg3_video_width(mpeg_file, 0);
    m_image.image.ysize=mpeg3_video_height(mpeg_file, 0);
    m_image.image.csize=4;

    switch (format){
    default: format=GL_RGBA;
    case GL_RGBA:    case GL_BGRA:   m_wantedFormat=format;m_image.image.csize=4; break; 
    case GL_RGB:     case GL_BGR:    m_wantedFormat=format;m_image.image.csize=3; break; 
    case GL_YCBCR_422_GEM:        m_wantedFormat=format;m_image.image.csize=2; break;
    }
    if (!m_image.image.xsize*m_image.image.ysize*m_image.image.csize)goto unsupported;

    m_image.image.format=m_wantedFormat;
    m_image.image.reallocate();
    changeImage(0,-1);
    post("MPEG3 opened");
    return true; 
  }
  goto unsupported;
 unsupported:
  post("MPEG3: unsupported!");
  close();
  return false;

}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
pixBlock* filmMPEG3 :: getFrame(){
  if (!m_readNext){
    return &m_image;
  }
  m_readNext = false;

  int i;
  int mpegFormat=0;
  int wantedFormat=m_wantedFormat;
  switch(wantedFormat){
  case GL_BGRA:
    mpegFormat=MPEG3_BGRA8888;
    m_image.image.csize=4;
    break;
  case GL_BGR: mpegFormat= MPEG3_BGR888; m_image.image.csize=3; break;
  case GL_RGB: mpegFormat= MPEG3_RGB888; m_image.image.csize=3; break;
    //  case GL_YCBCR_422_GEM: mpegFormat = MPEG3_YUV422P; m_image.image.csize=2; break;
  default: wantedFormat=GL_RGBA;
  case GL_RGBA:
    mpegFormat=MPEG3_RGBA8888; m_image.image.csize=4;
  }
  m_image.image.format=wantedFormat;
  int datasize=m_image.image.xsize*m_image.image.ysize*m_image.image.csize+4;
  m_image.image.reallocate(datasize);

  i=m_image.image.ysize;
  //  unsigned char *rows[m_image.image.ysize];
  unsigned char **rows = new unsigned char* [m_image.image.ysize];

  unsigned char **dummy=rows;
  while(i--)*dummy++=(unsigned char *)(m_image.image.data+i*m_image.image.xsize*m_image.image.csize);
  if (mpeg3_read_frame(mpeg_file, rows,
		       0, 0, 
		       m_image.image.xsize, m_image.image.ysize, 
		       m_image.image.xsize, m_image.image.ysize,
		       mpegFormat, 0)) {
    post("GEM: pix_film:: could not read frame ! %d", m_curFrame);
    return 0;
  }
  m_image.newimage=1;
  return &m_image;
}

int filmMPEG3 :: changeImage(int imgNum, int trackNum){
  m_readNext = true;
  if (imgNum  ==-1)  imgNum=m_curFrame;
  if (m_numFrames>1 && imgNum>=m_numFrames)return FILM_ERROR_FAILURE;
  if (trackNum==-1||trackNum>m_numTracks)trackNum=m_curTrack;
  int test;
  if ((test=mpeg3_set_frame(mpeg_file, imgNum, trackNum))) {
  }
    m_curFrame=imgNum;
    m_curTrack=trackNum;
    return FILM_ERROR_SUCCESS;
  m_readNext=false;
  return FILM_ERROR_FAILURE;
}
#endif
