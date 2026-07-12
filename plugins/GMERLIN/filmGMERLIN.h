/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2008, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

/*-----------------------------------------------------------------

  film loading library using libgmerlin-avdec

-----------------------------------------------------------------*/
#ifndef _INCLUDE_GEMPLUGIN__FILMGMERLIN_FILMGMERLIN_H_
#define _INCLUDE_GEMPLUGIN__FILMGMERLIN_FILMGMERLIN_H_
#include "plugins/film.h"
#include "Gem/Image.h"
#include <stdio.h>

# ifdef __cplusplus
extern "C" {
# endif
# include <gmerlin/avdec.h>

# include <gmerlin/bgav_version.h>
# if BGAV_BUILD >= BGAV_MAKE_BUILD(1,0,2) && !HAVE_BGAV_SEEK_TO_VIDEO_FRAME
#  define USE_FRAMETABLE 1
# else
#  define USE_FRAMETABLE 0
# endif

# ifdef HAVE_GAVL_LOG_H
#  include <gavl/log.h>
# else
#  define gavl_log_level_t bgav_log_level_t
# endif /* HAVE_GAVL_LOG_H */


# ifdef __cplusplus
}
# endif

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  filmGMERLIN

  Loads in a film

  KEYWORDS
  pix

  DESCRIPTION

  -----------------------------------------------------------------*/
namespace gem
{
namespace plugins
{
class GEM_EXPORT filmGMERLIN : public film
{
public:

  //////////
  // Constructor
  filmGMERLIN(void);

  //////////
  // Destructor
  virtual ~filmGMERLIN(void);

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

  bgav_t*        m_file;
  bgav_options_t * m_opt;
  int m_track, m_stream;
  gavl_video_frame_t*m_gframe,*m_finalframe;
  gavl_video_converter_t*m_gconverter;

  double m_fps;  // the frame-rate
  int m_fps_num, m_fps_denum;

  int64_t m_next_timestamp;
#ifdef USE_FRAMETABLE
  gavl_frame_table_t *m_frametable;
#endif

  // whether we need to convert the image before using it in Gem
  bool m_doConvert;


  virtual void log(gavl_log_level_t level, const char *log_domain,
                   const char *message);
};
};
};

#endif  // for header file
