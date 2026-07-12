/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2003, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

/*-----------------------------------------------------------------

  film loading backend for GEM using QT4L

-----------------------------------------------------------------*/

#ifndef _INCLUDE_GEMPLUGIN__FILMQT4L_FILMQT4L_H_
#define _INCLUDE_GEMPLUGIN__FILMQT4L_FILMQT4L_H_
#include "plugins/film.h"
#include "Gem/Image.h"
#include <stdio.h>

#include <lqt.h>
#include <colormodels.h>

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  filmQT4L1

  Loads in a film

  KEYWORDS
  pix

  DESCRIPTION

  -----------------------------------------------------------------*/
namespace gem
{
namespace plugins
{
class GEM_EXPORT filmQT4L : public film
{
public:

  //////////
  // Constructor
  filmQT4L(void);

  //////////
  // Destructor
  virtual ~filmQT4L(void);

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

  // can be used within a threaded context
  virtual bool isThreadable(void)
  {
    return true;
  }

  // Property handling
  virtual bool enumProperties(gem::Properties&readable,
                              gem::Properties&writeable);
  virtual void setProperties(gem::Properties&props);
  virtual void getProperties(gem::Properties&props);

  //-----------------------------------
  // GROUP:     Movie data
  //-----------------------------------
protected:
  unsigned int  m_wantedFormat; // format requested by the user
  double m_fps;  // the frame-rate
  int m_numFrames, m_numTracks; // number of frames in video
  int m_curFrame, m_curTrack;
  pixBlock m_image; // output image
  bool m_newfilm;

  quicktime_t  *m_quickfile;
  int           m_qtformat;

  imageStruct   m_qtimage;
  int m_lastFrame;

};
};
};

#endif  // for header file
