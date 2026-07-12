/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2011, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_PLUGINS_IMAGELOADER_H_
#define _INCLUDE__GEM_PLUGINS_IMAGELOADER_H_

#include "Gem/Image.h"
#include "Gem/Properties.h"

#include <string>

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  imageloader

  interface for the system- and library-dependent imageloader classes

  KEYWORDS
  pix load an image

  DESCRIPTION

  -----------------------------------------------------------------*/
namespace gem
{
namespace plugins
{
class GEM_EXTERN imageloader
{
public:

  //////////
  // returns an instance wrapping all plugins or NULL
  // if NULL is returned, you might still try your luck with manually accessing the
  // PluginFactory
  static imageloader*getInstance(void);

  ////////
  // dtor must be virtual
  virtual ~imageloader(void);

  /* read a image
   *
   * props can be filled by the loader with additional information on the image
   * e.g. EXIF tags,...
   */
  /* returns TRUE if loading was successful, FALSE otherwise */
  virtual bool load(std::string filename,
                    imageStruct&result,
                    gem::Properties&props) = 0;

  /* returns TRUE if this object can be used from within a thread */
  virtual bool isThreadable(void) = 0;

  /**
   * get the current value of the given properties from the loader
   * not really used, except for controlling the meta-loader
   */
  static void getProperties(imageloader*loader, gem::Properties&props);
};

};
}; // namespace gem


/**
 * \fn REGISTER_IMAGELOADERFACTORY(const char *id, Class imageloaderClass)
 * registers a new class "imageloaderClass" with the imageloader-factory
 *
 * \param id a symbolic (const char*) ID for the given class
 * \param imageloaderClass a class derived from "imageloader"
 */
#define REGISTER_IMAGELOADERFACTORY(id, TYP) static gem::PluginFactoryRegistrar::registrar<TYP, gem::plugins::imageloader> fac_imageloader_ ## TYP (id)

#endif  // for header file
