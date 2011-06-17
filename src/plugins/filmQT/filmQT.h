/*-----------------------------------------------------------------

GEM - Graphics Environment for Multimedia

Load an digital video (like AVI, Mpeg, Quicktime) into a pix block (Windus/Apple)

Copyright (c) 1997-1999 Mark Danks. mark@danks.org
Copyright (c) 2002 James Tittle.  tigital@mac.com
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.


-----------------------------------------------------------------*/
#ifndef INCLUDE_FILMQT_H_
#define INCLUDE_FILMQT_H_
#include "plugins/film.h"


#if defined __APPLE__ 
# if !defined __x86_64__
// with OSX10.6, apple has removed loads of Carbon functionality (in 64bit mode)
// LATER make this a real check in configure
#  define HAVE_CARBONQUICKTIME
# elif defined HAVE_QUICKTIME
#  undef HAVE_QUICKTIME
# endif
#endif

#ifdef HAVE_QUICKTIME
# ifdef HAVE_CARBONQUICKTIME
#  include <Carbon/Carbon.h>
#  include <QuickTime/QuickTime.h>
# elif defined _WIN32
#  include <QTML.h>
#  include <Movies.h>
# else
#  undef HAVE_QUICKTIME
# endif
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
class GEM_EXPORT filmQT : public gem::plugins::film
{
 public:
  //////////
  // Constructor
  filmQT(void);
  //////////
  // Destructor
  virtual ~filmQT();

#ifdef HAVE_QUICKTIME
  //////////
  // open a movie up
  virtual bool open(const std::string filename, int format = 0);
  //////////
  // close the movie file
  virtual void close(void);

  // get the frames per seconds (or "-1" if unknown)
  virtual double getFPS();

  //////////
  // get the next frame
  virtual pixBlock* getFrame();

  //////////
  // set the next frame to read;
  virtual int changeImage(int imgNum, int trackNum = -1);

 protected:
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
#endif // HAVE_QT
};};};

#endif	// for header file
