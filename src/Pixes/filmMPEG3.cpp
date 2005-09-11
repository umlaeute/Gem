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
    if(m_curTrack>=m_numTracks || m_curTrack<0)
      m_curTrack=0;
    m_numFrames = mpeg3_video_frames(mpeg_file, m_curTrack);
    m_fps = mpeg3_frame_rate(mpeg_file, m_curTrack);

    m_image.image.xsize=mpeg3_video_width(mpeg_file, m_curTrack);
    m_image.image.ysize=mpeg3_video_height(mpeg_file, m_curTrack);
    if (!m_image.image.xsize*m_image.image.ysize)goto unsupported;
    m_wantedFormat=format;
    m_image.image.setCsizeByFormat(format);
    m_image.image.reallocate();
    changeImage(0,-1);
    m_newfilm=true;
    return true; 
  }
  goto unsupported;
 unsupported:
  startpost("MPEG3 failed");
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
  int wantedFormat=m_wantedFormat;

  char*u=NULL,*y=NULL,*v=NULL;

  m_image.image.setCsizeByFormat(wantedFormat);
  int datasize=m_image.image.xsize*m_image.image.ysize*m_image.image.csize;
  m_image.image.reallocate(datasize+4);

  if(m_wantedFormat==GL_RGBA){
    // the mpeg3-YUV2RGB decoder works better than ours
    unsigned char **rows = new unsigned char* [m_image.image.ysize];
    unsigned char **dummy=rows;
    i=m_image.image.ysize;
    while(i--)*dummy++=m_image.image.data+(i*m_image.image.xsize*m_image.image.csize);
    if (mpeg3_read_frame(mpeg_file, rows,
			 0, 0, 
			 m_image.image.xsize, m_image.image.ysize, 
			 m_image.image.xsize, m_image.image.ysize,
			 MPEG3_RGBA8888,
			 0)) {
      post("GEM: pix_film:: could not read frame ! %d", m_curFrame);
      return 0;
    }
    // unfortunately the ALPHA is set to 0!
    i = m_image.image.xsize*m_image.image.ysize;
    unsigned char*aptr=m_image.image.data;
    while(i--){
      aptr[chAlpha]=255;
      aptr+=4;
    }

    m_image.image.upsidedown=false;
    delete[]rows;
  } else {
    // unfortunately this is upside down.
    if(mpeg3_read_yuvframe_ptr(mpeg_file,&y,&u,&v,0)){
      post("GEM: pix_film:: could not read yuv-frame ! %d", m_curFrame);
      return 0;
    }
    m_image.image.fromYV12((unsigned char*)y, (unsigned char*)u, (unsigned char*)v);
    m_image.image.upsidedown=true;
  }
  if(m_newfilm)m_image.newfilm=1;  m_newfilm=false;
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
