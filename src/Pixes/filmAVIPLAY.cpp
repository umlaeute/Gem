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
#include "Pixes/filmAVIPLAY.h"

/////////////////////////////////////////////////////////
//
// filmAVIPLAY
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////

filmAVIPLAY :: filmAVIPLAY(int format) : film(format) {
  static bool first_time=true;
  if (first_time) {
#ifdef HAVE_LIBAVIPLAY
    post("pix_film:: libaviplay support");
#endif
    first_time = false;
  }
#ifdef HAVE_LIBAVIPLAY
  m_avifile=0;
  m_avistream=0;
  m_aviimage=0;
#endif
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
filmAVIPLAY :: ~filmAVIPLAY()
{
  close();
}

void filmAVIPLAY :: close(void)
{
#ifdef HAVE_LIBAVIPLAY
  if (m_avistream)(*m_avistream).StopStreaming();
#endif
}

/////////////////////////////////////////////////////////
// open the file
//
/////////////////////////////////////////////////////////
bool filmAVIPLAY :: open(char *filename, int format)
{
  if (format>0)m_wantedFormat=format;
#ifdef HAVE_LIBAVIPLAY
  // how do we close the avifile ??? automagically ?
  if (!(m_avifile = CreateIAviReadFile(filename)))goto unsupported;
  while(!(*m_avifile).IsOpened());
  if (!(*m_avifile).IsValid())goto unsupported;
  m_numTracks = (*m_avifile).VideoStreamCount();
  if (m_numTracks<1)return false;
  if (m_curTrack>=m_numTracks)m_curTrack = 0;   
  try {
    m_avistream=(*m_avifile).GetStream(m_curTrack, avm::IStream::StreamType(1));
  } catch (const char* string) {
    m_avistream = 0;
  }
  if (!m_avistream)goto unsupported;
  if ((*m_avistream).StartStreaming()==-1)goto unsupported;
  
  m_numFrames = (*m_avistream).GetLength();
  m_curFrame = -1;
  if (1){
    StreamInfo *m_info = (*m_avistream).GetStreamInfo();
    m_image.image.xsize = (*m_info).GetVideoWidth();
    m_image.image.ysize = (*m_info).GetVideoHeight();
    m_image.image.csize = (*m_avistream).GetFrameSize()/(m_image.image.xsize*m_image.image.ysize);
    int format = (*m_avistream).GetVideoFormat();
    post("opened format = %x", format);
  }
  if (!(m_image.image.xsize*m_image.image.ysize*m_image.image.csize))goto unsupported;
  m_readNext=true;
  post("AVIPLAY opened");
  return true;
#endif
  goto unsupported;
 unsupported:
  post("AVIPLAY: unsupported!");
  close();
  return false;
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
pixBlock* filmAVIPLAY :: getFrame(){
#ifdef HAVE_LIBAVIPLAY
  if (!m_avistream)return 0;
  if (!m_readNext)return &m_image;
  if(m_aviimage)(*m_aviimage).Release();
  m_aviimage = (*m_avistream).GetFrame(true);
  if (m_aviimage){
    int format = (*m_aviimage).Format();
    //post("AVI-format %d", format);
    switch (format){
    case IMG_FMT_RGB24: format = GL_RGB; break;
    case IMG_FMT_RGB32: format = GL_RGBA; break;
    case IMG_FMT_BGR24: format = GL_BGR; break;
    case IMG_FMT_BGR32: format = GL_BGRA; break;
    case IMG_FMT_Y422:
    case IMG_FMT_UYVY:
    case IMG_FMT_CPLA:
    case IMG_FMT_UYNV:
    case 0x32315659: // this is NOT YUV422...
      format = GL_YCBCR_422_GEM;
      break;
    default:
      post("format conversion...");
      CImage buf(m_image.image.data, m_image.image.xsize, m_image.image.ysize);
      post("...");
      buf.Convert(m_aviimage);
      post("0x%X to format 0x%X", (*m_aviimage).Format(), buf.Format());
      format = GL_RGB;
      //post("unknown format 0x%X", format);
      //format = 0;
      m_aviimage = &buf;
    }
    m_image.image.format= format;
    m_image.image.data = (*m_aviimage).Data();
    m_image.newimage=1;
    m_readNext=false;
    return &m_image;
  }
#endif
  return 0;
}

int filmAVIPLAY :: changeImage(int imgNum, int trackNum){
#ifdef HAVE_LIBAVIPLAY
  if (!m_avistream)return FILM_ERROR_FAILURE;
  m_avistream->Seek(imgNum);
  m_readNext=true;
  return FILM_ERROR_SUCCESS;
#endif
  return FILM_ERROR_FAILURE;
}
