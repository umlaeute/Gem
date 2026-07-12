/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

/*-----------------------------------------------------------------

  image loading backend for GEM with SGI images
  image writing backend for GEM with SGI images

-----------------------------------------------------------------*/

#ifndef _INCLUDE_GEMPLUGIN__IMAGESGI_IMAGESGI_H_
#define _INCLUDE_GEMPLUGIN__IMAGESGI_IMAGESGI_H_
#include "plugins/imageloader.h"
#include "plugins/imagesaver.h"
#include <stdio.h>

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  imageSGI

  Loads in a picture

  KEYWORDS
  pix

  DESCRIPTION

  -----------------------------------------------------------------*/
namespace gem
{
namespace plugins
{
class GEM_EXPORT imageSGI : public gem::plugins::imageloader,
  public gem::plugins::imagesaver
{
public:

  //////////
  // Constructor
  imageSGI(void);
  virtual ~imageSGI(void);

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


  // this is always threadable
  virtual bool isThreadable(void)
  {
    return true;
  }
  virtual void getWriteCapabilities(std::vector<std::string>&mimetypes,
                                    gem::Properties&props);


};
};
};

#endif  // for header file
