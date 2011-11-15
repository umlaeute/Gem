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

#ifndef _INCLUDE_GEMPLUGIN__FILMAVI_FILMAVI_H_
#define _INCLUDE_GEMPLUGIN__FILMAVI_FILMAVI_H_

#include "plugins/film.h"
#include "Gem/Image.h"
#include <vfw.h>

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  filmAVI

  film-loader class for AVI(windoze)

  KEYWORDS
  pix film movie

  DESCRIPTION

  -----------------------------------------------------------------*/
namespace gem { namespace plugins {
class GEM_EXPORT filmAVI : public film {
 public:

  //////////
  // Constructor
  filmAVI(void);

  //////////
  // Destructor
  virtual ~filmAVI(void);

  //////////
  // open a movie up
  virtual bool open(const std::string filename, const gem::Properties&);
  //////////
  // close the movie file
  virtual void close(void);

  //////////
  // get the next frame
  virtual pixBlock* getFrame(void);

  //////////
  // set the next frame to read;
  virtual errCode changeImage(int imgNum, int trackNum=-1);

  // can be used within a threaded context
  virtual bool isThreadable(void) { return true; }

  // Property handling
  virtual bool enumProperties(gem::Properties&readable,gem::Properties&writeable);
  virtual void setProperties(gem::Properties&props);
  virtual void getProperties(gem::Properties&props);

 protected:
  GLenum  m_wantedFormat; // format requested by the user
  double m_fps;  // the frame-rate
  int m_numFrames; // number of frames in video
  int m_curFrame;
  pixBlock m_image; // output image

  // the raw buffer for decoding...
  int			m_nRawBuffSize;
  unsigned char*	m_RawBuffer;
  GLint   m_format;
  int 	    	m_reqFrame;

  unsigned char*  m_frame;  /* this points to a buffer for decompression */

  BITMAPINFOHEADER* m_pbmihRaw;
  BITMAPINFOHEADER* m_pbmihDst;

  HIC m_hic;

  PGETFRAME	m_getFrame;		// the frame information
  PAVISTREAM	m_streamVid;		// the stream itself
};};};

#endif	// for header file
