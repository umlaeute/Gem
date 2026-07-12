/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2014, Dan Wilcox and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

/*-----------------------------------------------------------------

  film loading backend for GEM using the AV Framework

-----------------------------------------------------------------*/
#ifndef _INCLUDE_GEMPLUGIN__FILMAVF_FILMAVF_H_
#define _INCLUDE_GEMPLUGIN__FILMAVF_FILMAVF_H_

#include "plugins/film.h"
#include "Gem/Image.h"

// forward declare PIMPL types, depending on the compiler
#ifdef __OBJC__
@class AVFMoviePlayer;
#else
typedef struct objc_object AVFMoviePlayer;
#endif

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  filmAVF

  Loads in a film on macOS using the AVFoundation framework

  KEYWORDS
  pix

  DESCRIPTION

  -----------------------------------------------------------------*/
namespace gem
{
namespace plugins
{
class GEM_EXPORT filmAVF : public film
{

public:

  //////////
  // Constructor
  filmAVF(void);

  //////////
  // Destructor
  virtual ~filmAVF(void);

  //////////
  // open a movie up
  virtual bool open(const std::string &filename,
                    const gem::Properties &props);

  //////////
  // get the next frame
  virtual pixBlock* getFrame(void);

  //////////
  // set the next frame to read
  virtual errCode changeImage(int imgNum, int trackNum = -1);

  //////////
  // returns true if instance can be used in thread
  virtual bool isThreadable(void)
  {
    return true;
  }

  //////////
  // close the movie file
  virtual void close(void);

  //////////
  // list all properties the currently opened film supports
  virtual bool enumProperties(gem::Properties &readable,
                              gem::Properties &writeable);

  //////////
  // set properties: "auto", "colorspace"
  virtual void setProperties(gem::Properties &props);

  //////////
  // get properties: "width", "height", "frames", "fps"
  virtual void getProperties(gem::Properties &props);

protected:

  //////////
  // set the pixel format: GEM_YUV, GEM_RGBA or GEM_GRAY
  // (re)allocates when necessary, returns true if format was changed
  bool changeFormat(unsigned int format);

  // PIMPL pointer to the Obj-C object behind this class
  AVFMoviePlayer* m_moviePlayer;

  pixBlock m_image;             //< frame storage
  unsigned int m_wantedFormat;  //< requested color space
  int m_numFrames, m_numTracks; //< num frames & tracks in the stream
  int m_curFrame, m_curTrack;   //< current frame and track
  double m_fps;                 //< frame rate
  bool m_readNext;              //< should the frame be read next getFrame call?

};
};
};

#endif // header guard
