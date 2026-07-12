/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2010, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

/*-----------------------------------------------------------------

  dummy film loading backend for GEM

-----------------------------------------------------------------*/

#ifndef _INCLUDE_GEMPLUGIN__FILMTEST_FILMTEST_H_
#define _INCLUDE_GEMPLUGIN__FILMTEST_FILMTEST_H_
#include "plugins/film.h"
#include "Gem/Image.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  filmTEST

  Loads in a film

  KEYWORDS
  pix

  DESCRIPTION

  -----------------------------------------------------------------*/
namespace gem
{
namespace plugins
{
class GEM_EXPORT filmTEST : public film
{
public:

  //////////
  // Constructor
  filmTEST(void);

  //////////
  // open a movie up
  virtual bool open(const std::string&filename, const gem::Properties&);

  virtual void close(void);

  //////////
  // get the next frame
  virtual pixBlock* getFrame(void);

  //////////
  // set the next frame to read;
  virtual errCode changeImage(int imgNum, int trackNum = -1);

  virtual bool enumProperties(gem::Properties&readprops,
                              gem::Properties&writeprops);

  virtual void getProperties(gem::Properties&props);
  virtual void setProperties(gem::Properties&props);

  virtual bool isThreadable(void);

  //-----------------------------------
  pixBlock m_image;
  double m_fps;
  unsigned int m_numFrames;
};
};
};

#endif  // for header file
