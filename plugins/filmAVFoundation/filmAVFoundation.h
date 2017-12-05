/*-----------------------------------------------------------------

GEM - Graphics Environment for Multimedia

Load an digital video (like AVI, Mpeg, Quicktime) into a pix block (Apple)

Copyright (c) 2014 Dan Wilcox. danomatika@gmail.com

Ported from the OpenFrameworks (openframeworks.cc) QTKit video implementation
by James George <james@jamesgeorge.org>, 2013-2014.

For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/
#ifndef _INCLUDE_GEMPLUGIN__FILMQTKIT_FILMQTKIT_H_
#define _INCLUDE_GEMPLUGIN__FILMQTKIT_FILMQTKIT_H_
#include "plugins/film.h"
#include "Gem/Image.h"

#ifdef __OBJC__
 #import "AVFMoviePlayer.h"
#endif

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  filmAVFoundation

  Loads in a film

  KEYWORDS
  pix

  DESCRIPTION

  -----------------------------------------------------------------*/
namespace gem { namespace plugins {
class GEM_EXPORT filmAVFoundation : public film
{
 public:
  //////////
  // Constructor
  filmAVFoundation(void);
  //////////
  // Destructor
  virtual ~filmAVFoundation(void);

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

  // cannot be used within a threaded context
  virtual bool isThreadable(void) { return false; }

  // Property handling
  virtual bool enumProperties(gem::Properties&readable,gem::Properties&writeable);
  virtual void setProperties(gem::Properties&props);
  virtual void getProperties(gem::Properties&props);

 protected:
  GLenum m_wantedFormat; // the requested pixel format (in GL)
  double m_fps; // the frame-rate
  int m_numFrames;//, m_numTracks; // number of frames in video
  int m_curFrame;//, m_curTrack;
  pixBlock m_image; // output image
  bool m_readNext;
  float m_auto;

  //-----------------------------------
  // GROUP: Movie data
  //-----------------------------------
  // Movie       m_movie;
  // GWorldPtr   m_srcGWorld;
  // TimeValue   m_movieTime;
  // Track       m_movieTrack;
  // TimeValue   m_timeScale;
  // TimeValue   m_frameDuration;
  double m_movieTime;
  double m_frameDuration;

  // managed to initialize our Quicktime-Decoder
  bool m_bInit;

  // This #ifdef is so you can include this .h file in .cpp files
  // and avoid ugly casts in the .mm file
  #ifdef __OBJC__
    AVFMoviePlayer* m_moviePlayer;
  #else
    void* m_moviePlayer;
  #endif

};};};

#endif  // for header file
