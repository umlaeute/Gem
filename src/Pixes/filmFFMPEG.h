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

#ifndef INCLUDE_FILMFFMPEG_H_
#define INCLUDE_FILMFFMPEG_H_
  
#include "Pixes/film.h"

#if defined(HAVE_LIBAVFORMAT) & defined(HAVE_LIBAVCODEC)
#define HAVE_LIBFFMPEG
#else
#undef HAVE_LIBFFMPEG
#endif

#ifdef HAVE_LIBFFMPEG
extern "C" {
#include "ffmpeg/avformat.h"
#include "ffmpeg/avcodec.h"
}
#endif

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  filmFFMPEG
    
  film-loader class for FFMPEG
    
  KEYWORDS
  pix film movie
    
  DESCRIPTION

  -----------------------------------------------------------------*/
class GEM_EXTERN filmFFMPEG : public film {
 public:
  
  //////////
  // Constructor
  filmFFMPEG(int format=0);

  //////////
  // Destructor
  virtual ~filmFFMPEG();

  //////////
  // open a movie up
  virtual bool open(char *filename, int format=0);
  //////////
  // close the movie file
  virtual void close(void);

  //////////
  // get the next frame
  virtual pixBlock* getFrame();

  //////////
  // set the next frame to read;
  virtual int changeImage(int imgNum, int trackNum=-1);

 protected:
  bool m_readNext;
#ifdef HAVE_LIBFFMPEG
   AVFormatContext* m_Format;
#if LIBAVCODEC_VERSION_INT == 0x000406
   /* jmz: i don't know which version of ffmpeg Günter uses.
    * i don't know either, whether it is more recent or older than mine
    * at least with 0.4.6 avcodec_decode_video wants AVFrame instead of AVPicture
    */
   AVFrame   m_avFrame;
#endif // CODEC_VERSION 0.4.6
   AVPicture m_Picture;
   AVPacket  m_Pkt;
   int       m_PacketLen;
   UINT8*    m_PacketPtr;
#endif //FFMPEG

};

#endif	// for header file
