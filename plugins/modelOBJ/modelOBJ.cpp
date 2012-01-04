////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 2001-2012 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "modelOBJ.h"
#include "plugins/PluginFactory.h"
#include "Gem/RTE.h"
#include "Gem/Exception.h"

#include "Gem/Properties.h"
#include <string>

using namespace gem::plugins;

REGISTER_MODELLOADERFACTORY("OBJ", modelOBJ);

modelOBJ :: modelOBJ(void) {
}

modelOBJ ::~modelOBJ(void) {
}

bool modelOBJ :: open(const std::string&name, const gem::Properties&requestprops) {
  return false;
}

void modelOBJ :: render(void) {
}
void modelOBJ :: close(void)  {
}

bool modelOBJ :: enumProperties(gem::Properties&readable,
                                gem::Properties&writeable) {
  readable.clear();
  writeable.clear();
  return false;
}

void modelOBJ :: setProperties(gem::Properties&props) {
}
void modelOBJ :: getProperties(gem::Properties&props) {
}
