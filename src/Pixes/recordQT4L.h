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
   
#ifndef INCLUDE_PIXES_RECORDQT4L_H_
#define INCLUDE_PIXES_RECORDQT4L_H_
   
#include "Pixes/record.h"
   
#if defined HAVE_LIBQUICKTIME && defined HAVE_LQT_ADD_VIDEO_TRACK
# include <lqt.h>
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
class GEM_EXTERN recordQT4L : public record {
 public:
  
  //////////
  // Constructor
  
  /* initialize the recordQT4Lloader
   *
   * set the default colour-space to format (like GL_RGBA)
   * if format==0, the default is set by the recordQT4Lloader
   * (for instance: the fastest colour-space)
   */
  recordQT4L(int x, int y, int width, int height);

  ////////
  // Destructor
  /* free what is apropriate */
  virtual ~recordQT4L();

#if defined HAVE_LIBQUICKTIME && defined HAVE_LQT_ADD_VIDEO_TRACK
  //////////
  // open a movie up
  /* open the recordQT4L "filename" (think better about URIs ?)
   */
  /* returns TRUE if opening was successfull, FALSE otherwise */
  virtual bool open(char *filename);
  //////////
  // close the movie file
  /* stop recording, close the file and clean up temporary things */
  virtual void close(void);


  //////////
  // compress and write the next frame
  /* this is the core-function of this class !!!!
   * when called it returns something depending on success
   * (what? the framenumber and -1 (0?) on failure?)
   */
  virtual int putFrame(imageStruct*);

  virtual bool setCodec(char*name);
  virtual bool setCodec(int  num);

  virtual int getNumCodecs();
  virtual char*getCodecName(int n);
  virtual char*getCodecDescription(int n);

 private:
  quicktime_t *m_qtfile;

  imageStruct m_image;

  lqt_codec_info_t*m_codec;
  lqt_codec_info_t**m_codecs;

  int m_track;
  int m_colormodel;
#endif
};

#endif	// for header file
