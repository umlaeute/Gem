/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2011, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

/*-----------------------------------------------------------------

  image loading backend using QuickTime
  image writing backend using QuickTime

-----------------------------------------------------------------*/

#ifndef _INCLUDE_GEMPLUGIN__IMAGEQT_IMAGEQT_H_
#define _INCLUDE_GEMPLUGIN__IMAGEQT_IMAGEQT_H_
#include "plugins/imageloader.h"
#include "plugins/imagesaver.h"
#include <stdio.h>

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  imageQT

  Loads in a picture

  KEYWORDS
  pix

  DESCRIPTION

  -----------------------------------------------------------------*/
namespace gem
{
namespace plugins
{
class GEM_EXPORT imageQT : public gem::plugins::imageloader,
  public gem::plugins::imagesaver
{
public:

  //////////
  // Constructor
  imageQT(void);
  virtual ~imageQT(void);

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
  // QuickTime cannot be used from threaded contexts
  virtual bool isThreadable(void)
  {
    return false;
  }
};
};
};

#endif  // for header file
