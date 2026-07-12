/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2011, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

/*-----------------------------------------------------------------

  image loader backend using ImageMagick
  image writer backend using ImageMagick

-----------------------------------------------------------------*/

#ifndef _INCLUDE_GEMPLUGIN__IMAGEMAGICK_IMAGEMAGICK_H_
#define _INCLUDE_GEMPLUGIN__IMAGEMAGICK_IMAGEMAGICK_H_
#include "plugins/imageloader.h"
#include "plugins/imagesaver.h"
#include <stdio.h>

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  imageMAGICK

  Loads in a picture

  KEYWORDS
  pix

  DESCRIPTION

  -----------------------------------------------------------------*/
namespace gem
{
namespace plugins
{
class GEM_EXPORT imageMAGICK : public gem::plugins::imageloader,
  public gem::plugins::imagesaver
{
public:

  //////////
  // Constructor
  imageMAGICK(void);
  virtual ~imageMAGICK(void);

  //////////
  // read an image
  virtual bool load(std::string filename, imageStruct&result,
                    gem::Properties&props);
  //////////
  // write an image
  virtual bool          save(const imageStruct&img,
                             const std::string&filename, const std::string&mimetype,
                             const gem::Properties&props);
  //////////
  // estimate, how well we could save this image
  virtual float estimateSave(const imageStruct&img,
                             const std::string&filename, const std::string&mimetype,
                             const gem::Properties&props);

  ////////
  // get writing capabilities of this backend (informative)
  virtual void getWriteCapabilities(std::vector<std::string>&mimetypes,
                                    gem::Properties&props);

  ////////
  // can be used from threaded contexts
  virtual bool isThreadable(void)
  {
    return true;
  }


private:
  std::vector<std::string>m_mimetypes;

};
};
};

#endif  // for header file
