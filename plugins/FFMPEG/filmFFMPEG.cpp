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

#define GEMFFMPEG_GREY AV_PIX_FMT_GRAY8
#define GEMFFMPEG_YUV AV_PIX_FMT_UYVY422
#define GEMFFMPEG_RGBA AV_PIX_FMT_RGBA
#define GEMFFMPEG_RGB AV_PIX_FMT_RGB24

REGISTER_FILMFACTORY("ffmpeg", filmFFMPEG);


namespace {
  /* the default prefix aligns with '[GEM:filmFFMPEG] ' */
  static void show_error(int errcode, const char*prefix="                 ") {
    char errbuf[MAXPDSTRING];
    logpost(0, 3+0, "%s%s", prefix, av_make_error_string(errbuf, sizeof(errbuf), errcode));
  }
};

/////////////////////////////////////////////////////////
//
// filmFFMPEG
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
filmFFMPEG :: filmFFMPEG(void)
  : m_numFrames(-1), m_numTracks(-1)
  , m_track(0), m_stream(0)
  , m_fps(0.)
  , m_wantedFormat(0)
  , m_wantedCodec("")
  , m_resetConverter(false)
  , m_avformat(0)
  , m_avdecoder(0)
  , m_avstream(0)
  , m_avframe(0)
  , m_avpacket(0)
  , m_avconverter(0)
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
  sws_freeContext(m_avconverter);

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
  close();

  const char*filename = sfilename.c_str();
  int ret;

  if ((ret = avformat_open_input(&m_avformat, filename, NULL, NULL)) < 0) {
    return false;
  }
  m_avformat->seek2any = 1;
  if ((ret = avformat_find_stream_info(m_avformat, NULL)) < 0) {
    logpost(0, 3+0, "[GEM:filmFFMPEG] Unable to find stream information in %s", filename);
    show_error(ret);
    close();
    return false;
  }

  /* create a codec */
  AVStream *st;
  const AVCodec *dec = 0;
  ret = av_find_best_stream(m_avformat, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
  int stream_index = ret;
  if(ret < 0) {
    logpost(0, 3+0, "[GEM:filmFFMPEG] Could not find video stream in %s", filename);
    show_error(ret);
    close();
    return false;
  }
  st = m_avformat->streams[stream_index];

  if(!dec) {
    dec = avcodec_find_decoder_by_name(m_wantedCodec.c_str());
  }
#if 1
  if (!dec && AV_CODEC_ID_VP9 == st->codecpar->codec_id) {
    dec = avcodec_find_decoder_by_name("libvpx-vp9");
  }
#endif
  if(!dec)
    dec = avcodec_find_decoder(st->codecpar->codec_id);
  if(!dec) {
    logpost(0, 3+0, "[GEM:filmFFMPEG] Failed to find video codec for %s", filename);
    close();
    return false;
  }
  m_avdecoder = avcodec_alloc_context3(dec);
  if(!m_avdecoder) {
    logpost(0, 3+0, "[GEM:filmFFMPEG] Failed to allocate the video codec context");
    close();
    return false;
  }

  /* Copy codec parameters from input stream to output codec context */
  if ((ret = avcodec_parameters_to_context(m_avdecoder, st->codecpar)) < 0) {
    logpost(0, 3+0, "[GEM:filmFFMPEG] Failed to copy video codec parameters to decoder context");
    show_error(ret);
    close();
    return false;
  }
  /* Init the decoders */
  if ((ret = avcodec_open2(m_avdecoder, dec, NULL)) < 0) {
    logpost(0, 3+0, "[GEM:filmFFMPEG] Failed to open codec");
    show_error(ret);
    close();
    return false;
  }

  m_avstream = st;
  m_stream = stream_index;
  m_fps = av_q2d(m_avstream->avg_frame_rate);
  m_numFrames = m_avstream->nb_frames;
  m_image.image.xsize = m_avdecoder->width;
  m_image.image.ysize = m_avdecoder->height;
  m_image.image.setFormat(GEM_RGBA);
  m_image.image.reallocate();
  m_image.newfilm = true;

  return true;
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void filmFFMPEG :: initConverter(const int width, const int height, const int format) {
  /* check if we need a new decoder object */
  if(width != m_convertinfo.width ||
     height != m_convertinfo.height ||
     format != m_convertinfo.srcformat ||
     m_resetConverter
     ) {
    /* things have changed, reset the converter */
    AVPixelFormat dstformats[] = {
      GEMFFMPEG_YUV,
      GEMFFMPEG_RGBA,
      //GEMFFMPEG_GREY, /* disabled for now, as ffmpeg always prefers this... */
      AV_PIX_FMT_NONE
    };
    switch(m_wantedFormat) {
    default: break;
    case GEM_GRAY:
      dstformats[0] = GEMFFMPEG_GREY;
      dstformats[1] = AV_PIX_FMT_NONE;
      break;
    case GEM_YUV:
      dstformats[0] = GEMFFMPEG_YUV;
      dstformats[1] = AV_PIX_FMT_NONE;
      break;
    case GEM_RGBA:
      dstformats[0] = GEMFFMPEG_RGBA;
      dstformats[1] = AV_PIX_FMT_NONE;
      break;
    case GEM_RGB:
      dstformats[0] = GEMFFMPEG_RGB;
      dstformats[1] = AV_PIX_FMT_NONE;
      break;
    }
    int loss;
    int has_alpha = 1;
    AVPixelFormat srcformat = (AVPixelFormat)format;
    /* get the best destination format when converting
       https://ffmpeg.org/doxygen/trunk/group__lavc__misc__pixfmt.html
    */
    AVPixelFormat dstformat = avcodec_find_best_pix_fmt_of_list(
        dstformats, srcformat,
        has_alpha, &loss);

    m_convertinfo.width = width;
    m_convertinfo.height = height;
    m_convertinfo.srcformat = srcformat;
    m_convertinfo.dstformat = dstformat;

    sws_freeContext(m_avconverter);
    m_avconverter = sws_getContext(
        width, height, srcformat,
        width, height, dstformat,
        SWS_FAST_BILINEAR, NULL, NULL, NULL);
    m_resetConverter = (0 == m_avconverter);
  }

  /* finally adjust our output image */
  int gformat;
  switch(m_convertinfo.dstformat) {
  case GEMFFMPEG_GREY:
    gformat = GEM_GRAY;
    break;
  case GEMFFMPEG_YUV:
    gformat = GEM_YUV;
    break;
  case GEMFFMPEG_RGB:
    gformat = GEM_RGB;
    break;
  case GEMFFMPEG_RGBA:
  default:
    gformat = GEM_RGBA;
    break;
  }
  if(width != m_image.image.xsize ||
     height != m_image.image.ysize ||
     gformat != m_image.image.format
     ) {
    m_image.image.xsize = width;
    m_image.image.ysize = height;
    m_image.image.setFormat(gformat);
    m_image.image.reallocate();
    m_image.newfilm = true;
  }
}

int filmFFMPEG :: convertFrame(void)
{
  /* use libswscale for colorspace conversion
     https://ffmpeg.org/doxygen/trunk/group__libsws.html
     https://ffmpeg.org/doxygen/trunk/scaling_video_8c-example.html#a1
  */

  /* (re)create the colorspace converter */
  initConverter(m_avdecoder->width, m_avdecoder->height, m_avdecoder->pix_fmt);
  if(!m_avconverter)
    return -1;
  /* dst_linesize:
     GREY   : linesize={w*1, 0,...}, data={%p, NULL,...}
     YUYV422: linesize={w*2, 0,...}, data={%p, NULL,...}
     RGBA   : linesize={w*4, 0,...}, data={%p, NULL,...}
  */
  int dst_linesize = m_image.image.csize * m_image.image.xsize;
  uint8_t*dst_data = (uint8_t*)m_image.image.data;

  sws_scale(m_avconverter,
            (const uint8_t **)(m_avframe->data), m_avframe->linesize, 0, m_avframe->height,
            &dst_data, &dst_linesize);
  m_image.newimage = true;
  return 0;
}

/* decodes a single packet */
int filmFFMPEG :: decodePacket(void)
{
  // submit the packet to the decoder
  int ret = avcodec_send_packet(m_avdecoder, m_avpacket);
  if (ret < 0) {
    logpost(0, 3+0, "[GEM:filmFFMPEG] Error submitting packet for decoding (%d)", ret);
    show_error(ret);
    return ret;
  }

  // get all the frame from the decoder
  /* with video decoding, one packet always has a single frame */
  ret = avcodec_receive_frame(m_avdecoder, m_avframe);
  if (ret < 0) {
    // those two return values are special and mean there is no output
    // frame available, but there were no errors during decoding
    if (ret == AVERROR_EOF || ret == AVERROR(EAGAIN))
      return 0;

    logpost(0, 3+0, "[GEM:filmFFMPEG] Error during decoding (%d)", ret);
    show_error(ret);
    return ret;
  }

  // write the frame data to output file
  if (m_avdecoder->codec->type == AVMEDIA_TYPE_VIDEO) {
#if 0
    if(1) {
      enum AVPixelFormat pix_fmt = (AVPixelFormat)m_avframe->format;
      logpost(0, 3+0, "[GEM:filmFFMPEG] decoded VIDEO for %lu/%lu: %dx%d@%s!"
              , (unsigned long)m_avframe->pts, (unsigned long)m_avframe->pkt_dts
              , m_avframe->width, m_avframe->height, av_get_pix_fmt_name(pix_fmt)
              );
    }
#endif
    ret = convertFrame();
  } else {
    logpost(0, 3+0, "[GEM:filmFFMPEG] ouch. unexpected type %s", av_get_media_type_string(m_avdecoder->codec->type));
  }

  av_frame_unref(m_avframe);
  if (ret < 0)
    return ret;

  return 0;
}
pixBlock* filmFFMPEG :: getFrame(void)
{
  if(!m_avdecoder || !m_avformat) {
    return NULL;
  }

  if (av_read_frame(m_avformat, m_avpacket) >= 0) {
    int ret = -1;
    if (m_avpacket->stream_index == m_stream) {
      ret = decodePacket();
      av_packet_unref(m_avpacket);
    } else {
      av_packet_unref(m_avpacket);
    }
    if (ret >= 0) {

    } else {
      // ouch
    }
  }
  return &m_image;
}


/////////////////////////////////////////////////////////
// changeFrame
//
/////////////////////////////////////////////////////////
film::errCode filmFFMPEG :: changeImage(int imgNum, int trackNum)
{
  if(!m_avformat) {
    return film::FAILURE;
  }
  if(!m_numFrames) {
    return film::DONTKNOW;
  }
  if(trackNum<0) {
    /* just automatically proceed to the next frame: this might speed up things for linear decoding */
    return film::SUCCESS;
  }

  int frameNum = imgNum;
  if (m_avstream &&
      (m_avstream->r_frame_rate.den && m_avstream->r_frame_rate.num) &&
      (m_avstream->time_base.den && m_avstream->time_base.num)) {
    frameNum = (int)((long int)imgNum *
                     (m_avstream->time_base.den * m_avstream->r_frame_rate.den) /
                     (m_avstream->time_base.num * m_avstream->r_frame_rate.num));
#if 0
    post("frame_rate=%d/%d", m_avstream->r_frame_rate.num, m_avstream->r_frame_rate.den);
    post("time_base=%d/%d", m_avstream->time_base.num, m_avstream->time_base.den);
    post("frame: %d => %d", imgNum, frameNum);
#endif
  }

  /* setting the minimum-frame to '0' will prevent an error if we reach the end.
   * also, it seems that some movies don't correctly play back...
   */
  int ret = avformat_seek_file(m_avformat
    , m_stream
    , frameNum, frameNum, frameNum
    , AVSEEK_FLAG_FRAME | AVSEEK_FLAG_ANY | AVSEEK_FLAG_BACKWARD
    );

  if (ret < 0) {
    //show_error(ret);
    return film::FAILURE;
  }

  if(imgNum>=m_numFrames || imgNum<0) {
    return film::DONTKNOW;
  }

  return film::SUCCESS;
}
///////////////////////////////
// Properties
bool filmFFMPEG::enumProperties(gem::Properties&readable,
                                 gem::Properties&writeable)
{
  std::string dummy_s = "auto";
  int dummy_i = 0;
  double dummy_f = 0.;

  readable.clear();
  writeable.clear();

  readable.set("fps", dummy_f);
  readable.set("frames", dummy_i);
  readable.set("width", dummy_i);
  readable.set("height", dummy_i);
  readable.set("codec", dummy_s);

  writeable.set("colorspace", dummy_i);
  writeable.set("codec", dummy_s);

  return false;
}

void filmFFMPEG::setProperties(gem::Properties&props)
{
  std::vector<std::string>keys=props.keys();
  for(int i=0; i<keys.size(); i++) {
    double d;
    std::string s;
    const std::string key =keys[i];
    if(props.get("colorspace", d)) {
      m_wantedFormat = d;
      m_resetConverter = true;
      continue;
    }
    if(props.get("codec", s)) {
      m_wantedCodec = s;
      continue;
    }
  }
}

void filmFFMPEG::getProperties(gem::Properties&props)
{
  std::vector<std::string> keys=props.keys();
  gem::any value;
  double d;
  for(unsigned int i=0; i<keys.size(); i++) {
    std::string key=keys[i];
    props.erase(key);
    if("fps"==key) {
      d=m_fps;
      value=d;
      props.set(key, value);
      continue;
    }
    if("frames"==key && m_numFrames>=0) {
      d=m_numFrames;
      value=d;
      props.set(key, value);
      continue;
    }
    if("tracks"==key && m_numTracks>=0) {
      d=m_numTracks;
      value=d;
      props.set(key, value);
      continue;
    }
    if("width"==key) {
      d=m_image.image.xsize;
      value=d;
      props.set(key, value);
      continue;
    }
    if("height"==key) {
      d=m_image.image.ysize;
      value=d;
      props.set(key, value);
      continue;
    }
    if("codec"==key) {
      const AVCodecDescriptor*desc=m_avdecoder?avcodec_descriptor_get(m_avdecoder->codec_id):0;
      if(desc) {
        std::string s = desc->name;
        props.set(key, s);
      }
      continue;
    }
  }
}
