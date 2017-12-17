/*-----------------------------------------------------------------

GEM - Graphics Environment for Multimedia

Load an digital video (like AVI, Mpeg, Quicktime) into a pix block (Windus/Apple)

Copyright (c) 1997-1999 Mark Danks. mark@danks.org
Copyright (c) 2002 James Tittle.  tigital@mac.com
Copyright (c) 2011-2014 IOhannes m zmölnig. zmoelnig@iem.at
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.


-----------------------------------------------------------------*/
#ifndef _INCLUDE_GEMPLUGIN__FILMQT_FILMQT_H_
#define _INCLUDE_GEMPLUGIN__FILMQT_FILMQT_H_
#include "plugins/film.h"
#include "Gem/Image.h"

#if defined __APPLE__
# include <Carbon/Carbon.h>
# include <QuickTime/QuickTime.h>
#elif defined _WIN32

# if defined __MINGW32__
/* hack to avoid the use of microsofts non-standard extension (u)i64 instead of (U)LL */
#  include <ConditionalMacros.h>
#  undef TARGET_OS_WIN32
#  include <Math64.h>
#  define TARGET_OS_WIN32 1
# endif /* MINGW */

# include <QTML.h>
# include <Movies.h>
#endif /* OS */

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  filmQT

  Loads in a film

  KEYWORDS
  pix

  DESCRIPTION

  -----------------------------------------------------------------*/
namespace gem
{
namespace plugins
{
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
  virtual bool isThreadable(void)
  {
    return false;
  }

  // Property handling
  virtual bool enumProperties(gem::Properties&readable,
                              gem::Properties&writeable);
  virtual void setProperties(gem::Properties&props);
  virtual void getProperties(gem::Properties&props);

protected:
  GLenum m_wantedFormat; // the requested pixel format (in GL)
  double m_fps;  // the frame-rate
  int m_numFrames, m_numTracks; // number of frames in video
  int m_curFrame, m_curTrack;
  pixBlock m_image; // output image
  bool m_readNext;
  float                 m_auto;

  //-----------------------------------
  // GROUP:     Movie data
  //-----------------------------------
  Movie                 m_movie;
  GWorldPtr             m_srcGWorld;
  TimeValue             m_movieTime;
  Track                 m_movieTrack;
  TimeValue             m_timeScale;
  TimeValue             m_frameDuration;

  // managed to initialize our Quicktime-Decoder
  bool                  m_bInit;
};
};
};

#endif  // for header file
