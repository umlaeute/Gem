/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, James Tittle II and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

/* -----------------------------------------------------------------

   Darwin film loading backend for GEM

-----------------------------------------------------------------*/

#ifndef _INCLUDE_GEMPLUGIN__FILMDARWIN_FILMDARWIN_H_
#define _INCLUDE_GEMPLUGIN__FILMDARWIN_FILMDARWIN_H_

#if defined __APPLE__ && !defined __x86_64__
// with OSX10.6, apple has removed loads of Carbon functionality (in 64bit mode)
// LATER make this a real check in configure
# define HAVE_CARBONQUICKTIME
#endif


#ifdef HAVE_CARBONQUICKTIME
#include "plugins/film.h"
#include "Gem/Image.h"

# include <Carbon/Carbon.h>
# include <QuickTime/QuickTime.h>

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  filmDarwin

  film-loader class for MacOS-X (Darwin)

  KEYWORDS
  pix film movie

  DESCRIPTION

  -----------------------------------------------------------------*/
namespace gem
{
namespace plugins
{
class GEM_EXPORT filmDarwin : public film
{
public:

  //////////
  // Constructor
  filmDarwin(void);

  //////////
  // Destructor
  virtual ~filmDarwin(void);

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
  virtual errCode changeImage(int imgNum, int trackNum=-1);

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
  unsigned int  m_wantedFormat; // format requested by the user
  bool m_auto;  // automatic progression
  int m_numFrames, m_numTracks; // number of frames in video
  int m_curFrame, m_lastFrame;
  pixBlock m_image; // output image


  Movie                 m_movie;
  GWorldPtr             m_srcGWorld;
  TimeValue             m_movieTime;
  Track                 m_movieTrack;
  Media                 m_movieMedia;
  TimeValue             m_timeScale;
  double                m_durationf;
  double                m_fps;

};
};
};

#endif //HAVE_CARBONQUICKTIME
#endif  // for header file
