////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.at
//
// Implementation file
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#ifdef HAVE_LIBMPEG3
#include "filmMPEG3.h"
#include "plugins/PluginFactory.h"
#include "Gem/Properties.h"
#include "Gem/RTE.h"

using namespace gem::plugins;

REGISTER_FILMFACTORY("MPEG3", filmMPEG3);

/* take care of API changes */
#ifdef MPEG3_MAJOR
# if MPEG3_MINOR > 6
#  define FILMMPEG3_OPEN17
# endif
#endif /* MPEG3 version defines */


/////////////////////////////////////////////////////////
//
// filmMPEG3
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////

filmMPEG3 :: filmMPEG3(void) :
  m_wantedFormat(GEM_RGBA),
  m_fps(-1.0),
  m_numFrames(-1), m_numTracks(-1),
  m_curFrame(-1), m_curTrack(-1),
  m_readNext(false),
  m_newfilm(false),
  mpeg_file(NULL)
{ }

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
filmMPEG3 :: ~filmMPEG3(void)
{
  close();
}


void filmMPEG3 :: close(void)
{
  if(mpeg_file) {
    mpeg3_close(mpeg_file);
  }
  mpeg_file=NULL;
}

/////////////////////////////////////////////////////////
// really open the file ! (OS dependent)
//
/////////////////////////////////////////////////////////
bool filmMPEG3 :: open(const std::string&filename,
                       const gem::Properties&wantProps)
{
  char*cfilename=const_cast<char*>(filename.c_str());
  if (mpeg3_check_sig(cfilename)) { /* ok, this is mpeg(3) */
#ifdef FILMMPEG3_OPEN17
    // new API with more sophisticated error-feedback
    mpeg_file= mpeg3_open(cfilename, 0);
#else
    // old API
    mpeg_file= mpeg3_open(cfilename);
#endif
    if(!mpeg_file) {
      logpost(0, 3+0,
              "[GEM:filmMPEG3] this file %s does not seem to hold any video data",
              filename.c_str());
      goto unsupported;
    }
    if (!mpeg3_has_video(mpeg_file)) {
      logpost(0, 3+0,
              "[GEM:filmMPEG3] this file %s does not seem to hold any video data",
              filename.c_str());
      goto unsupported;
    }
    m_numTracks = mpeg3_total_vstreams(mpeg_file);
    if(m_curTrack>=m_numTracks || m_curTrack<0) {
      m_curTrack=0;
    }
    m_numFrames = mpeg3_video_frames(mpeg_file, m_curTrack);
    m_fps = mpeg3_frame_rate(mpeg_file, m_curTrack);

    m_image.image.xsize=mpeg3_video_width(mpeg_file, m_curTrack);
    m_image.image.ysize=mpeg3_video_height(mpeg_file, m_curTrack);
    if (!m_image.image.xsize*m_image.image.ysize) {
      goto unsupported;
    }
    double d;
    if(wantProps.get("colorspace", d)) {
      m_image.image.setFormat((int)d);
      m_wantedFormat=m_image.image.format;
    }
    m_image.image.reallocate();
    changeImage(0,-1);
    m_newfilm=true;
    return true;
  }
  goto unsupported;
unsupported:
  close();
  return false;

}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
pixBlock* filmMPEG3 :: getFrame(void)
{
  if (!m_readNext) {
    return &m_image;
  }
  m_readNext = false;

  int wantedFormat=m_wantedFormat;

  char*u=NULL,*y=NULL,*v=NULL;

  m_image.image.setFormat(wantedFormat);
  int datasize=m_image.image.xsize*m_image.image.ysize*m_image.image.csize;
  m_image.image.reallocate(datasize+4);

  if(m_wantedFormat==GEM_RGBA) {
    // the mpeg3-YUV2RGB decoder works better than ours
    unsigned char **rows = new unsigned char* [m_image.image.ysize];
    unsigned char **dummy=rows;
    int i=m_image.image.ysize;
    while(i--) {
      *dummy++=m_image.image.data+(i*m_image.image.xsize*m_image.image.csize);
    }
    if (mpeg3_read_frame(mpeg_file, rows,
                         0, 0,
                         m_image.image.xsize, m_image.image.ysize,
                         m_image.image.xsize, m_image.image.ysize,
                         MPEG3_RGBA8888,
                         0)) {
      pd_error(0, "[GEM:filmMPEG3]: could not read frame ! %d", m_curFrame);
      return 0;
    }
    // unfortunately the ALPHA is set to 0!
    i = m_image.image.xsize*m_image.image.ysize;
    unsigned char*aptr=m_image.image.data;
    while(i--) {
      aptr[chAlpha]=255;
      aptr+=4;
    }

    m_image.image.upsidedown=false;
    delete[]rows;
  } else {
    // unfortunately this is upside down.
    if(mpeg3_read_yuvframe_ptr(mpeg_file,&y,&u,&v,0)) {
      pd_error(0, "[GEM:filmMPEG3]: could not read yuv-frame ! %d", m_curFrame);
      return 0;
    }
    m_image.image.fromYV12((unsigned char*)y, (unsigned char*)u,
                           (unsigned char*)v);
    m_image.image.upsidedown=true;
  }
  if(m_newfilm) {
    m_image.newfilm=1;
  }
  m_newfilm=false;
  m_image.newimage=1;
  return &m_image;
}

film::errCode filmMPEG3 :: changeImage(int imgNum, int trackNum)
{
  m_readNext = true;
  if (imgNum  ==-1) {
    imgNum=m_curFrame;
  }
  if (m_numFrames>1 && imgNum>=m_numFrames) {
    return film::FAILURE;
  }
  if (trackNum==-1||trackNum>m_numTracks) {
    trackNum=m_curTrack;
  }

  int test;
  if ((test=mpeg3_set_frame(mpeg_file, imgNum, trackNum))) {
  }
  m_curFrame=imgNum;
  m_curTrack=trackNum;
  return film::SUCCESS;
#if 0
  m_readNext=false;
  return film::FAILURE;
#endif
}


///////////////////////////////
// Properties
bool filmMPEG3::enumProperties(gem::Properties&readable,
                               gem::Properties&writeable)
{
  readable.clear();
  writeable.clear();

  gem::any value;
  value=0.;
  readable.set("fps", value);
  readable.set("frames", value);
  readable.set("tracks", value);
  readable.set("width", value);
  readable.set("height", value);

  writeable.set("colorspace", value);

  return false;
}

void filmMPEG3::setProperties(gem::Properties&props)
{
  double d;
  if(props.get("colorspace", d)) {
    m_wantedFormat=d;
  }
}

void filmMPEG3::getProperties(gem::Properties&props)
{
  std::vector<std::string> keys=props.keys();
  for(unsigned i=0; i<keys.size(); i++) {
    gem::any value;
    double d;
    std::string key=keys[i];
    props.erase(key);
    if("fps"==key) {
      d=m_fps;
      value=d;
      props.set(key, value);
    }
    if("frames"==key) {
      d=m_numFrames;
      value=d;
      props.set(key, value);
    }
    if("tracks"==key) {
      d=m_numTracks;
      value=d;
      props.set(key, value);
    }
    if("width"==key) {
      d=m_image.image.xsize;
      value=d;
      props.set(key, value);
    }
    if("height"==key) {
      d=m_image.image.ysize;
      value=d;
      props.set(key, value);
    }
  }
}

#endif
