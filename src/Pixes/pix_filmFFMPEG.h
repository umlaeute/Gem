/*-----------------------------------------------------------------

GEM - Graphics Environment for Multimedia

Load an digital video (like AVI, Mpeg, Quicktime) into a pix block (Linux)

Copyright (c) 1997-1999 Mark Danks. mark@danks.org
Copyright (c) Günther Geiger. geiger@epy.co.at
Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.


-----------------------------------------------------------------*/

#ifndef INCLUDE_PIX_FILMFFMPEG_H_
#define INCLUDE_PIX_FILMFFMPEG_H_

#ifdef HAVE_LIBAVCODEC
#define HAVE_FFMPEG


#include "Pixes/pix_film.h"
extern "C" {
#include "ffmpeg/avformat.h"
#include "ffmpeg/avcodec.h"
}
/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  pix_filmFFMPEG
  
  Loads in a film 
  
  KEYWORDS
  pix
  
  DESCRIPTION

  -----------------------------------------------------------------*/
class GEM_EXTERN pix_filmFFMPEG : public pix_film
{
  CPPEXTERN_HEADER(pix_filmFFMPEG, GemBase)

    public:

  //////////
  // Constructor
  pix_filmFFMPEG(t_symbol *filename);

 protected:

  //////////
  // close the movie file
  virtual void closeMess(void);
  //////////
  // open a movie up
  virtual void realOpen(char *filename);

  //////////
  // Do the rendering
  virtual void getFrame();
	
  //-----------------------------------
  // GROUP:	Movie data
  //-----------------------------------

   AVFormatContext* m_Format;
   AVFrame   m_avFrame;
   AVPacket  m_Pkt;
   int       m_PacketLen;
   unsigned char*    m_PacketPtr;
   int m_framesize;

  //-----------------------------------
  // GROUP:	Texturing
  //-----------------------------------

 protected:
	
   void  estimateFramesize(int);
  //////////
  // static member functions
  static void openMessCallback   (void *data, t_symbol *filename);
  static void changeImageCallback(void *data, t_symbol *, int argc, t_atom *argv);
  static void autoCallback(void *data, t_floatarg state);

};
#endif // HAVE_LIBAVCODEC
#endif	// for header file
