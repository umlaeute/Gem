/* -----------------------------------------------------------------

GEM - Graphics Environment for Multimedia

Load an digital video (like AVI, Mpeg, Quicktime) into a pix block 
(OS independant parent-class)

Copyright (c) 1997-1999 Mark Danks. mark@danks.org
Copyright (c) Günther Geiger. geiger@epy.co.at
Copyright (c) 2001-2003 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/
   
#ifndef INCLUDE_PLUGINS_RECORDQT4L_H_
#define INCLUDE_PLUGINS_RECORDQT4L_H_
   
#include "plugins/record.h"
   
#if defined HAVE_LIBQUICKTIME 
#define GEM_USE_RECORDQT4L

/* don't add relative paths to the quicktime-headers here!
 * they should be found by configure!
 * if not, then rather give the full path by hand in Make.config
 * or the command line!
 * using things like "lqt/lqt.h" will break compilation on other 
 * systems!
 */
# include <lqt.h>
# include <colormodels.h>
#endif
 
/*---------------------------------------------------------------
 -------------------------------------------------------------------
  CLASS
  recordQT4L
    
  class for recording video-streams into a qt4l-movie
    
  KEYWORDS
  pix record movie
  
  DESCRIPTION
  
  -----------------------------------------------------------------*/
namespace gem { class GEM_EXPORT recordQT4L : public record {
 public:
  
  //////////
  // Constructor
  
  /* initialize the recordQT4Lloader
   *
   * set the default colour-space to format (like GL_RGBA)
   * if format==0, the default is set by the recordQT4Lloader
   * (for instance: the fastest colour-space)
   */
  recordQT4L();

  ////////
  // Destructor
  /* free what is apropriate */
  virtual ~recordQT4L();

#if defined GEM_USE_RECORDQT4L

  //////////
  // close the movie file
  // stop recording, close the file and clean up temporary things
  virtual void close(void);

  //////////
  // open a movie up
  // open the recordQT4L "filename" (think better about URIs ?)
  // returns TRUE if opening was successfull, FALSE otherwise 
  virtual bool open(const std::string filename);


  
  //////////
  // initialize the encoder
  // dummyImage provides meta-information (e.g. size) that must not change during the encoding cycle
  // (if it does, abort the recording session)
  // framedur is the duration of one frame in [ms]
  //   
  // returns TRUE if init was successfull, FALSE otherwise 
  virtual bool init(const imageStruct* dummyImage, const int framedur);


  //////////
  // compress and write the next frame
  /* this is the core-function of this class !!!!
   * when called it returns something depending on success
   * (what? the framenumber and -1 (0?) on failure?)
   */
  virtual bool putFrame(imageStruct*);

  virtual bool setCodec(const std::string name);


  /**
   * get a list of supported codecs (short-form names, e.g. "mjpa")
   */ 
  virtual std::vector<std::string>getCodecs(void);

 private:
  quicktime_t *m_qtfile;

  imageStruct m_image;

  /* the selected codec */
  bool setCodec(lqt_codec_info_t**codec, int num);
  lqt_codec_info_t*m_codec;
  lqt_codec_info_t**m_codecs;
  std::string m_codecname;


  /* a buffer for the quicktime encoder */
  unsigned char ** m_qtbuffer;

  /* in which colormodel do we have to present the data to lqt? */
  int m_colormodel;

  /* frame dimensions (on change we have to stop writing */
  int m_width, m_height;

  /* re-initialize the recorder */
  bool m_restart;
#endif /* QT */
}; };

#endif	// for header file
