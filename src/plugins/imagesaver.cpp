////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 2011-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
  
#include "plugins/imagesaver.h"
#include "plugins/PluginFactory.h"

gem::plugins::imagesaver :: ~imagesaver(void) {}

gem::plugins::imagesaver*gem::plugins::imagesaver::getInstance(void) {
 return NULL;
}


static gem::PluginFactoryRegistrar::dummy<gem::plugins::imagesaver> fac_imagesaverdummy;
