/* -----------------------------------------------------------------

GEM - Graphics Environment for Multimedia

Load an digital video (like AVI, Mpeg, Quicktime) into a pix block 
(OS independant parent-class)

Copyright (c) 1997-1999 Mark Danks. mark@danks.org
Copyright (c) Günther Geiger. geiger@epy.co.at
Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/
   
#ifndef _INCLUDE_GEMPLUGIN__RECORDV4L2_RECORDV4L2_H_
#define _INCLUDE_GEMPLUGIN__RECORDV4L2_RECORDV4L2_H_
   
#include "plugins/record.h"
   
#if defined HAVE_LINUX_VIDEODEV2_H
# define HAVE_VIDEO4LINUX2
# include <linux/videodev2.h>
#endif

#include <string>
#include <map>
 
/*---------------------------------------------------------------
 -------------------------------------------------------------------
  CLASS
  recordV4L2
    
  class for outputting video using a v4l2 loopback device
    
  KEYWORDS
  pix record movie
  
  DESCRIPTION
  
  -----------------------------------------------------------------*/
namespace gem { namespace plugins {
 class GEM_EXPORT recordV4L2 : public record {
 public:
  
  //////////
  // Constructor
  
  recordV4L2(void);

  ////////
  // Destructor
  /* free what is apropriate */
  virtual ~recordV4L2(void);

#if defined HAVE_VIDEO4LINUX2

  //////////
  // close the movie file
  // stop recording, close the file and clean up temporary things
  virtual void stop(void);

  //////////
  // open a movie up
  // open the recordV4L2 "filename" (or URI)
  // returns TRUE if opening was successfull, FALSE otherwise 
  virtual bool start(const std::string filename, gem::Properties&);

  
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
   * when called it returns TRUE on success
   * if FALSE is returned, recording must be stopped
   */
  virtual bool write(imageStruct*);

  virtual bool setCodec(const std::string name);

  /**
   * get a list of supported codecs (short-form names, e.g. "mjpa")
   */ 
  virtual std::vector<std::string>getCodecs(void);
  virtual const std::string getCodecDescription(const std::string);
  virtual bool enumProperties(gem::Properties&);

  virtual bool dialog(void) {return false;}


 private:
  std::map<std::string, std::string>m_codecdescriptions;
  int m_fd;
  imageStruct m_image;
  bool m_init;
  int m_palette;

#endif /* V4L2 */
 };
};};

#endif	// for header file
