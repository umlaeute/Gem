////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file 
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) G�nther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f�r::uml�ute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
#include "Pixes/filmAVIPLAY.h"

#ifdef HAVE_LIBAVIPLAY
# include <unistd.h>
#endif

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

  m_rawdata=NULL;
  m_rawlength=0;
#endif
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
filmAVIPLAY :: ~filmAVIPLAY()
{
  close();
  if(m_rawdata)delete[]m_rawdata;
}
#ifdef HAVE_LIBAVIPLAY
void filmAVIPLAY :: close(void)
{
  if (m_avistream)(*m_avistream).StopStreaming();
}

/////////////////////////////////////////////////////////
// open the file
//
/////////////////////////////////////////////////////////
bool filmAVIPLAY :: open(char *filename, int format)
{
  if (format>0)m_wantedFormat=format;
  // how do we close the avifile ??? automagically ?
  if (!(m_avifile = CreateIAviReadFile(filename)))goto unsupported;
  while(!(*m_avifile).IsOpened())usleep(500);
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
    //m_image.image.csize = (*m_avistream).GetFrameSize()/(m_image.image.xsize*m_image.image.ysize);
    int format = (*m_avistream).GetVideoFormat();
  }
  m_image.image.setCsizeByFormat(m_wantedFormat);
  if (!(m_image.image.xsize*m_image.image.ysize*m_image.image.csize))goto unsupported;
  m_readNext=true;
  m_newfilm = true;
  return true;
  goto unsupported;
 unsupported:
  startpost("aviplay failed");
  close();
  return false;
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
pixBlock* filmAVIPLAY :: getFrame(){
  if (!m_avistream)return 0;
  if (!m_readNext)return &m_image;
  if(m_aviimage)(*m_aviimage).Release();
  /* for MPEGs ReadFrame() will return 0 only when errors occur
   * other formats return 0 all the time (and -1 on file end)
   */
  m_aviimage = (*m_avistream).GetFrame(true); // this might crash sometimes...
  if (m_aviimage){
    int format = (*m_aviimage).Format();
    int i = (*m_aviimage).Pixels();

    m_rawdata=(*m_aviimage).Data();
    m_image.image.setCsizeByFormat(m_wantedFormat);
    switch(format){
    case IMG_FMT_RGB24: m_image.image.fromRGB (m_rawdata); break;
    case IMG_FMT_RGB32: m_image.image.fromRGBA(m_rawdata); break;
    case IMG_FMT_BGR24: m_image.image.fromBGR (m_rawdata); break;
    case IMG_FMT_BGR32: m_image.image.fromBGRA(m_rawdata); break;
    case IMG_FMT_Y800 : 
    case IMG_FMT_Y8   : m_image.image.fromGray(m_rawdata); break;
    case IMG_FMT_UYVY : m_image.image.fromUYVY(m_rawdata); break;
    case IMG_FMT_YUY2 : m_image.image.fromYUY2(m_rawdata); break;
    case IMG_FMT_YVYU : m_image.image.fromYVYU(m_rawdata); break;
    case IMG_FMT_YV12 : m_image.image.fromYV12(m_rawdata); break;
    case IMG_FMT_BGR16: // it seems like this was RGB16
    default:
      m_image.image.fromRGB16(m_rawdata); break;
    }
    m_image.newimage=1;
    if (m_newfilm)m_image.newfilm=1;  m_newfilm=false;
    m_image.image.upsidedown=true;
    m_readNext=false;
    return &m_image;
  }
  return 0;
}

int filmAVIPLAY :: changeImage(int imgNum, int trackNum){
  if (!m_avistream)return FILM_ERROR_FAILURE;
  m_avistream->Seek(imgNum);
  m_readNext=true;
  return FILM_ERROR_SUCCESS;
}
#endif // AVIPLAY
