////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 2011 IOhannes m zmoelnig. forum::für::umläute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
  
#include "imageloader.h"
#include "plugins/PluginFactory.h"

gem::plugins::imageloader :: ~imageloader(void) {}

gem::plugins::imageloader*gem::plugins::imageloader::getInstance(void) {
 return NULL;
}

INIT_IMAGELOADERFACTORY();
