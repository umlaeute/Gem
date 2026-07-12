/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2019, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

/*-----------------------------------------------------------------

  image loading backend using Use Sean T. Barret's single-file header-lib
  image writing backend using Use Sean T. Barret's single-file header-lib

-----------------------------------------------------------------*/

#ifndef _INCLUDE_GEMPLUGIN__IMAGESTB_IMAGESTB_H_
#define _INCLUDE_GEMPLUGIN__IMAGESTB_IMAGESTB_H_
#include "plugins/imageloader.h"
#include "plugins/imagesaver.h"
#include <stdio.h>

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  imageSTB

  Loads in a picture

  KEYWORDS
  pix

  DESCRIPTION

  -----------------------------------------------------------------*/
namespace gem
{
namespace plugins
{
class GEM_EXPORT imageSTB : public gem::plugins::imageloader,
  public gem::plugins::imagesaver
{
public:

  //////////
  // Constructor
  imageSTB(void);
  virtual ~imageSTB(void);

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
