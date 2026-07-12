/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2003, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

/*-----------------------------------------------------------------

  film loading backend for GEM using libMPEG1

-----------------------------------------------------------------*/

#ifndef _INCLUDE_GEMPLUGIN__FILMMPEG1_FILMMPEG1_H_
#define _INCLUDE_GEMPLUGIN__FILMMPEG1_FILMMPEG1_H_
#include "plugins/film.h"
#include "Gem/Image.h"
#include <stdio.h>

#include <mpeg.h>

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  filmMPEG1

  Loads in a film

  KEYWORDS
  pix

  DESCRIPTION

  -----------------------------------------------------------------*/
namespace gem
{
namespace plugins
{
class GEM_EXPORT filmMPEG1 : public film
{
public:

  //////////
  // Constructor
  filmMPEG1(void);

  //////////
  // Destructor
  virtual ~filmMPEG1(void);

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
  int m_curFrame;
  pixBlock m_image; // output image
  bool m_newfilm;

  FILE         *m_streamfile;
  ImageDesc     m_streamVid;
  bool          m_reachedEnd;
  unsigned char*m_data;
  int           m_length;
};
};
};

#endif  // for header file
