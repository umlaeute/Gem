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
  
#include "plugins/imagesaver.h"
//#include "Gem/RTE.h"

using namespace gem::plugins;

/////////////////////////////////////////////////////////
//
// imagesaver
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
imagesaver :: imagesaver(bool threadable) : m_threadable(m_threadable) {
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
imagesaver :: ~imagesaver()
{
}

float imagesaver ::estimateSave( const imageStruct&img, const std::string&filename, const std::string&mimetype, const gem::Properties&props) {
  /* the default is rather bad */
  return 0.;
}
void imagesaver ::getWriteCapabilities(std::vector<std::string>&mimetypes, gem::Properties&props) {
  mimetypes.clear();
  props.clear();
}


INIT_IMAGESAVERFACTORY();
