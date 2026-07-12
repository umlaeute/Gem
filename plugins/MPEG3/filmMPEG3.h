/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2003, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

/*-----------------------------------------------------------------

  film loading backend for GEM using libMPEG3

-----------------------------------------------------------------*/

#ifndef _INCLUDE_GEMPLUGIN__FILMMPEG3_FILMMPEG3_H_
#define _INCLUDE_GEMPLUGIN__FILMMPEG3_FILMMPEG3_H_
#include "plugins/film.h"
#include "Gem/Image.h"
#include <stdio.h>
#ifdef HAVE_LIBMPEG3
#include <libmpeg3.h>
#endif // MPEG3

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  filmMPEG3

  Loads in a film

  KEYWORDS
  pix

  DESCRIPTION

  -----------------------------------------------------------------*/
namespace gem
{
namespace plugins
{
class GEM_EXPORT filmMPEG3 : public film
{
public:

  //////////
  // Constructor
  filmMPEG3(void);

  //////////
  // Destructor
  virtual ~filmMPEG3(void);

#ifdef HAVE_LIBMPEG3
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

  //-----------------------------------
  // GROUP:     Movie data
  //-----------------------------------
protected:
  unsigned int  m_wantedFormat; // format requested by the user
  double m_fps;  // the frame-rate
  int m_numFrames, m_numTracks; // number of frames in video
  int m_curFrame, m_curTrack;
  pixBlock m_image; // output image
  bool m_readNext;
  bool m_newfilm;

  mpeg3_t      *mpeg_file;
#endif
};
};
};

#endif  // for header file
