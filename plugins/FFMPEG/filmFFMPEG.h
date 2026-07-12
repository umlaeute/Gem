/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2023, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

/*-----------------------------------------------------------------

  film loading backend for GEM using FFMPEG

-----------------------------------------------------------------*/
#ifndef _INCLUDE_GEMPLUGIN__FILMFFMPEG_FILMFFMPEG_H_
#define _INCLUDE_GEMPLUGIN__FILMFFMPEG_FILMFFMPEG_H_
#include "plugins/film.h"
#include "Gem/Image.h"

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
}


/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  filmFFMPEG

  Loads in a film

  KEYWORDS
  pix

  DESCRIPTION

  -----------------------------------------------------------------*/
namespace gem
{
namespace plugins
{
class GEM_EXPORT filmFFMPEG : public film
{
public:

  //////////
  // Constructor
  filmFFMPEG(void);

  //////////
  // Destructor
  virtual ~filmFFMPEG(void);

  //////////
  // open a movie up
  virtual bool open(const std::string&filename, const gem::Properties&);
  //////////
  // close the movie file
  virtual void close(void);

  //////////
  // get the next frame
  virtual pixBlock* getFrame(void);

  //////////
  // set the next frame to read;
  virtual errCode changeImage(int imgNum, int trackNum = -1);

  virtual bool isThreadable(void);

  // Property handling
  virtual bool enumProperties(gem::Properties&readable,
                              gem::Properties&writeable);
  virtual void setProperties(gem::Properties&props);
  virtual void getProperties(gem::Properties&props);

  //-----------------------------------
  // GROUP:     Movie data
  //-----------------------------------
protected:
  pixBlock m_image; // output image
  int m_numFrames, m_numTracks; // number of frames in video
  int m_track, m_stream;
  double m_fps;

  // the user can wish for some things
  unsigned int  m_wantedFormat;
  std::string m_wantedCodec;

  // whether we need to convert the image before using it in Gem
  bool m_resetConverter;

  AVFormatContext *m_avformat;
  AVCodecContext *m_avdecoder;
  AVStream*m_avstream;
  AVFrame*m_avframe;
  AVPacket*m_avpacket;
  struct SwsContext *m_avconverter;

  struct {
    int width;
    int height;
    int srcformat;
    int dstformat;
  } m_convertinfo;

  /* helpers to get the frame */
  int decodePacket(void);
  int convertFrame(void);
  void initConverter(const int width, const int height, const int format);
};
};
};

#endif  // for header file
