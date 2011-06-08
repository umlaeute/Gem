////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file 
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <string.h>
#include "imageSGI.h"

#include "Gem/RTE.h"

using namespace gem::plugins;

REGISTER_IMAGEFACTORY("SGI", imageSGI);


/////////////////////////////////////////////////////////
//
// imageSGI
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
imageSGI :: imageSGI() 
{
  post("imageSGI");
}
imageSGI :: ~imageSGI()
{
  post("~imageSGI");
}

/////////////////////////////////////////////////////////
// really open the file ! (OS dependent)
//
/////////////////////////////////////////////////////////
bool imageSGI :: load(std::string filename, imageStruct&result)
{
  return false;
}
bool imageSGI::save(std::string filename, const imageStruct&constimage) {
  return false;
}
#endif
