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

#include "filmFFMPEG.h"
#include "plugins/PluginFactory.h"
#include "Gem/RTE.h"
#include "Gem/Properties.h"
#include "Gem/Exception.h"

using namespace gem::plugins;

REGISTER_FILMFACTORY("ffmpeg", filmFFMPEG);

/////////////////////////////////////////////////////////
//
// filmFFMPEG
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////

filmFFMPEG :: filmFFMPEG(void)
  :  m_numFrames(-1), m_numTracks(-1)
  , m_track(0), m_stream(0)
  , m_fps(0.)
  , m_doConvert(false)
  , m_avformat(0)
  , m_avdecoder(0)
  , m_avstream(0)
  , m_avframe(0)
  , m_avpacket(0)
{
  m_avframe = av_frame_alloc();
  m_avpacket = av_packet_alloc();
  if(!m_avframe || !m_avpacket) {
    av_packet_free(&m_avpacket);
    av_frame_free(&m_avframe);
    throw(GemException("unable to allocate FFMPEG frame resp. packet"));
  }
}

////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
filmFFMPEG :: ~filmFFMPEG(void)
{
  close();
  av_packet_free(&m_avpacket);
  av_frame_free(&m_avframe);
}

bool filmFFMPEG :: isThreadable(void)
{
  if(m_numFrames<0) {
    return false;
  }
  return true;
}

void filmFFMPEG :: close(void)
{
  /* LATER: free frame buffers */
  avcodec_free_context(&m_avdecoder);
  avformat_close_input(&m_avformat);
}

/////////////////////////////////////////////////////////
// really open the file !
//
/////////////////////////////////////////////////////////
bool filmFFMPEG :: open(const std::string&sfilename,
                         const gem::Properties&wantProps)
{
  const char*filename = sfilename.c_str();
  close();
  if (avformat_open_input(&m_avformat, filename, NULL, NULL) < 0) {
    return false;
  }
  if (avformat_find_stream_info(m_avformat, NULL) < 0) {
    verbose(0, "[GEM:filmFFMPEG] Unable to find stream information in %s", filename);
    close();
    return false;
  }
  /* create a codec */
  #if 0
  if (open_codec_context(&video_stream_idx, &video_dec_ctx, fmt_ctx, AVMEDIA_TYPE_VIDEO) >= 0);
  #endif

  AVStream *st;
  const AVCodec *dec = 0;
  int ret = av_find_best_stream(m_avformat, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
  int stream_index = ret;
  if(ret < 0) {
    verbose(0, "[GEM:filmFFMPEG] Could not find video stream in %s", filename);
    close();
    return false;
  }
  st = m_avformat->streams[stream_index];
  dec = avcodec_find_decoder(st->codecpar->codec_id);
  if(!dec) {
    verbose(0, "[GEM:filmFFMPEG] Failed to find video codec for %s", filename);
    close();
    return false;
  }
  m_avdecoder = avcodec_alloc_context3(dec);
  if(!m_avdecoder) {
    verbose(0, "[GEM:filmFFMPEG] Failed to allocate the video codec context");
    close();
    return false;
  }
  /* Copy codec parameters from input stream to output codec context */
  if ((ret = avcodec_parameters_to_context(m_avdecoder, st->codecpar)) < 0) {
    verbose(0, "[GEM:filmFFMPEG] Failed to copy video codec parameters to decoder context");
    close();
    return false;
  }
  /* Init the decoders */
  if ((ret = avcodec_open2(m_avdecoder, dec, NULL)) < 0) {
    verbose(0, "[GEM:filmFFMPEG] Failed to open codec");
    close();
    return false;
  }

  return true;
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
pixBlock* filmFFMPEG :: getFrame(void)
{
  if(!m_avdecoder) {
    return NULL;
  }
  return &m_image;
}


/////////////////////////////////////////////////////////
// changeFrame
//
/////////////////////////////////////////////////////////
film::errCode filmFFMPEG :: changeImage(int imgNum, int trackNum)
{
  if(trackNum<0) {
    /* just automatically proceed to the next frame: this might speed up things for linear decoding */
    return film::SUCCESS;
  }

  if(!m_avdecoder) {
    return film::FAILURE;
  }

  if(imgNum>=m_numFrames || imgNum<0) {
    return film::FAILURE;
  }

#if 0
  /* TODO: implement seeking */
  return film::SUCCESS;
#endif

  return film::FAILURE;
}
///////////////////////////////
// Properties
bool filmFFMPEG::enumProperties(gem::Properties&readable,
                                 gem::Properties&writeable)
{
  readable.clear();
  writeable.clear();

  gem::any value;
  value=0.;
  readable.set("fps", value);
  readable.set("frames", value);
  readable.set("width", value);
  readable.set("height", value);

  return false;
}

void filmFFMPEG::setProperties(gem::Properties&props)
{
}

void filmFFMPEG::getProperties(gem::Properties&props)
{
  std::vector<std::string> keys=props.keys();
  gem::any value;
  double d;
  unsigned int i=0;
  for(i=0; i<keys.size(); i++) {
    std::string key=keys[i];
    props.erase(key);
    if("fps"==key) {
      d=m_fps;
      value=d;
      props.set(key, value);
    }
    if("frames"==key && m_numFrames>=0) {
      d=m_numFrames;
      value=d;
      props.set(key, value);
    }
    if("tracks"==key && m_numTracks>=0) {
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
