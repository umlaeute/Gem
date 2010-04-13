/*-----------------------------------------------------------------

GEM - Graphics Environment for Multimedia

Load a digital video (quicktime4linux) for linux

Copyright (c) 1997-1999 Mark Danks. mark@danks.org
Copyright (c) Günther Geiger. geiger@epy.co.at
Copyright (c) 2001-2003 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.


-----------------------------------------------------------------*/
#ifndef INCLUDE_FILMGMERLIN_H_
#define INCLUDE_FILMGMERLIN_H_
#include "plugins/film.h"
#include <stdio.h>

#ifdef HAVE_LIBGMERLIN_AVDEC
# define HAVE_GMERLIN
#endif

#ifdef HAVE_GMERLIN
# ifdef __cplusplus
extern "C" {
# endif
# include <gmerlin/avdec.h>
# ifdef __cplusplus
}
# endif
#endif // GMERLIN

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  filmGMERLIN
  
  Loads in a film
  
  KEYWORDS
  pix
  
  DESCRIPTION

  -----------------------------------------------------------------*/
namespace gem { class GEM_EXTERN filmGMERLIN : public film {
 public:

  //////////
  // Constructor
  filmGMERLIN(int format=0);

  //////////
  // Destructor
  virtual ~filmGMERLIN();

#ifdef HAVE_GMERLIN
  //////////
  // open a movie up
  virtual bool open(char *filename, int format = 0);
  //////////
  // close the movie file
  virtual void close(void);

  //////////
  // get the next frame
  virtual pixBlock* getFrame();

  //////////
  // set the next frame to read;
  virtual int changeImage(int imgNum, int trackNum = -1);

  //-----------------------------------
  // GROUP:	Movie data
  //-----------------------------------
 protected:
  bgav_t*   	 m_file;
  bgav_options_t * m_opt;
  bool           m_seekable; /* the track can be seeked */
  gavl_video_format_t*m_gformat,*m_finalformat;
  int m_track, m_stream;
  gavl_video_frame_t*m_gframe,*m_finalframe;
  gavl_video_converter_s*m_gconverter;

  int m_fps_num, m_fps_denum;


  static void log_callback(void *data, bgav_log_level_t level, const char *log_domain, const char *message);
  virtual void log(bgav_log_level_t level, const char *log_domain, const char *message);


#endif
  int m_lastFrame;

 private:
  // whether we need to convert to use it in Gem
  bool m_doConvert;
};};

#endif	// for header file
