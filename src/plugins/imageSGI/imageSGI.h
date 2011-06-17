/*-----------------------------------------------------------------

GEM - Graphics Environment for Multimedia

Load an SGI picture

Copyright (c) 2011 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.


-----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_PLUGINS_IMAGESGI_H_
#define INCLUDE_GEM_PLUGINS_IMAGESGI_H_
#include "plugins/imageloader.h"
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
namespace gem { namespace plugins { 
class GEM_EXPORT imageSGI : public gem::plugins::imageloader {
 public:

  //////////
  // Constructor
  imageSGI(void);
  virtual ~imageSGI(void);

  //////////
  // read an image
  virtual bool load(std::string filename, imageStruct&result, gem::Properties&props);
  //////////
  // write an image
  virtual bool save(std::string filename, const imageStruct&result);

};
};};

#endif	// for header file
