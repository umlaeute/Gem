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
  
#include "plugins/image.h"
//#include "Gem/RTE.h"

using namespace gem::plugins;

/////////////////////////////////////////////////////////
//
// image
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
image :: image() {
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
image :: ~image()
{
}

bool image :: enumProperties(gem::Properties&readable,
			     gem::Properties&writeable) 
{
  readable.clear();
  writeable.clear();
  return false;
}

void image :: setProperties(gem::Properties&props) {
  // nada
  m_properties=props;
#if 0
  std::vector<std::string> keys=props.keys();
  int i=0;
  for(i=0; i<keys.size(); i++) {
    enum gem::Properties::PropertyType typ=props.type(keys[i]);
    std::cerr  << "key["<<keys[i]<<"]: "<<typ<<" :: ";
    switch(typ) {
    case (gem::Properties::NONE):
      props.erase(keys[i]);
      break;
    case (gem::Properties::DOUBLE):
      std::cerr << gem::any_cast<double>(props.get(keys[i]));
      break;
    case (gem::Properties::STRING):
      std::cerr << "'" << gem::any_cast<std::string>(props.get(keys[i])) << "'";
      break;
    default:
      std::cerr << "<unkown:" << props.get(keys[i]).get_type().name() << ">";
      break;
    }
  }
  std::cerr << std::endl;
#endif
}

void image :: getProperties(gem::Properties&props) {
  // nada
  std::vector<std::string>keys=props.keys();
  int i=0;
  for(i=0; i<keys.size(); i++) {
    gem::any unset;
    props.set(keys[i], unset);
  }
}
