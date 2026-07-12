/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2006, Chris Clepper and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

/* -----------------------------------------------------------------

   film loading backend for GEM using DirectShow

   -----------------------------------------------------------------*/

#ifndef _INCLUDE_GEMPLUGIN__FILMDS_FILMDS_H_
#define _INCLUDE_GEMPLUGIN__FILMDS_FILMDS_H_
#include "plugins/film.h"
#include "Gem/Image.h"

#include <dshow.h>

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  filmDS

  film-loader class for AVI(windoze)

  KEYWORDS
  pix film movie

  DESCRIPTION

  -----------------------------------------------------------------*/
namespace gem
{
namespace plugins
{
class GEM_EXPORT filmDS : public film
{
public:

  //////////
  // Constructor
  filmDS(void);

  //////////
  // Destructor
  virtual ~filmDS(void);

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

  // can be used within a threaded context
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
  class DirectShowVideo;
  DirectShowVideo * player;
};
};
};

#endif  // for header file
