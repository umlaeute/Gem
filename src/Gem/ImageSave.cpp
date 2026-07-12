////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// Implementation file
//
// SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
// SPDX-License-Identifier: GPL-2.0-or-later
//
////////////////////////////////////////////////////////

#include "ImageIO.h"
#include "Gem/RTE.h"
#include "Gem/Files.h"

#include "plugins/imagesaver.h"
#include "plugins/PluginFactory.h"

namespace gem
{
namespace PixImageSaver
{
static gem::plugins::imagesaver*s_instance=NULL;
static gem::plugins::imagesaver*getInstance(void)
{
  if(NULL==s_instance) {
    s_instance=gem::plugins::imagesaver::getInstance();
  }
  return s_instance;
}
};
};


/***************************************************************************
 *
 * mem2image - Save an image to a file
 *
 ***************************************************************************/
GEM_EXTERN int mem2image(imageStruct* image, const char *filename,
                         const int type)
{
  gem::plugins::imagesaver*piximagesaver=gem::PixImageSaver::getInstance();
  if(piximagesaver) {
    std::string mimetype;
    gem::Properties props;
    if(type>0) {
      props.set("quality", (float)type);
    }
    if(piximagesaver->save(*image, filename, mimetype, props)) {
      return (1);
    }
  }
  pd_error(0, "GEM: Unable to save image to '%s'", filename);
  return (0);
}
