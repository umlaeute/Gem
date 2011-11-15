/*-----------------------------------------------------------------

GEM - Graphics Environment for Multimedia

Load an digital video (like AVI, Mpeg, Quicktime) into a pix block (Windus/Apple)

Copyright (c) 1997-1999 Mark Danks. mark@danks.org
Copyright (c) 2002 James Tittle.  tigital@mac.com
Copyright (c) 2011 IOhannes m zmölnig. zmoelnig@iem.at
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.


-----------------------------------------------------------------*/
#ifndef _INCLUDE_GEMPLUGIN__FILMQT_FILMQT_H_
#define _INCLUDE_GEMPLUGIN__FILMQT_FILMQT_H_
#include "plugins/film.h"
#include "Gem/Image.h"

#ifdef HAVE_CARBONQUICKTIME
# include <Carbon/Carbon.h>
# include <QuickTime/QuickTime.h>
#else defined _WIN32
# include <QTML.h>
# include <Movies.h>
#endif

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  filmQT

  Loads in a film

  KEYWORDS
  pix

  DESCRIPTION

  -----------------------------------------------------------------*/
namespace gem { namespace plugins {
class GEM_EXPORT filmQT : public film
{
 public:
  //////////
  // Constructor
  filmQT(void);
  //////////
  // Destructor
  virtual ~filmQT(void);

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
  virtual errCode changeImage(int imgNum, int trackNum = -1);

  // cannot be used within a threaded context
  virtual bool isThreadable(void) { return false; }

  // Property handling
  virtual bool enumProperties(gem::Properties&readable,gem::Properties&writeable);
  virtual void setProperties(gem::Properties&props);
  virtual void getProperties(gem::Properties&props);

 protected:
  double m_fps;  // the frame-rate
  int m_numFrames, m_numTracks; // number of frames in video
  int m_curFrame, m_curTrack;
  pixBlock m_image; // output image
  bool m_readNext;

  //-----------------------------------
  // GROUP:	Movie data
  //-----------------------------------
  Movie			m_movie;
  GWorldPtr		m_srcGWorld;
  TimeValue		m_movieTime;
  Track			m_movieTrack;
  TimeValue		m_timeScale;
  TimeValue		duration;

  // managed to initialize our Quicktime-Decoder
  bool			m_bInit;
};};};

#endif	// for header file
