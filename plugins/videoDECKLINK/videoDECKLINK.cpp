////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.at
//
// Implementation file
//
//    Copyright (c) 2015 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "videoDECKLINK.h"
#include "plugins/PluginFactory.h"

using namespace gem::plugins;

REGISTER_VIDEOFACTORY("decklink", videoDECKLINK);

videoDECKLINK::videoDECKLINK(void)
  : m_name(std::string("decklink"))
{
  m_pixBlock.image.xsize = 64;
  m_pixBlock.image.ysize = 64;
  m_pixBlock.image.setCsizeByFormat(GL_RGBA);
  m_pixBlock.image.reallocate();
}

videoDECKLINK::~videoDECKLINK(void) {
  close();
}

void videoDECKLINK::close(void) {
}


bool videoDECKLINK::open(gem::Properties&props) {
  if(m_devname.empty())return false;
  setProperties(props);
  return false;
}

pixBlock*videoDECKLINK::getFrame(void) {
  return 0;
  return &m_pixBlock;
}

std::vector<std::string>videoDECKLINK::enumerate(void) {
  std::vector<std::string>result;
  //result.push_back("decklink");
  return result;
}

bool videoDECKLINK::setDevice(int ID) {
  m_devname.clear();
  return false;
}
bool videoDECKLINK::setDevice(std::string device) {
  m_devname.clear();
  const std::string prefix="decklink://";
  if (!device.compare(0, prefix.size(), prefix)) {
    m_devname=device.substr(prefix.size());
    return true;
  }
  return false;
}
bool videoDECKLINK::enumProperties(gem::Properties&readable,
			       gem::Properties&writeable) {
  std::string dummy_s;
  int dummy_i=0;
  readable.clear();
  writeable.clear();

  readable.set("width", m_pixBlock.image.xsize);
  readable.set("height", m_pixBlock.image.ysize);

  //writeable.set("mouse.mask", dummy_i);

  return true;
}
void videoDECKLINK::setProperties(gem::Properties&props) {
  m_props=props;

  double num;
  std::string s;

}
void videoDECKLINK::getProperties(gem::Properties&props) {
  std::vector<std::string>keys=props.keys();
  unsigned int i;
  for(i=0; i<keys.size(); i++) {
    if("width"==keys[i]) {
      props.set(keys[i], m_pixBlock.image.xsize);
    }
    if("height"==keys[i]) {
      props.set(keys[i], m_pixBlock.image.ysize);
    }
  }
}

std::vector<std::string>videoDECKLINK::dialogs(void) {
  std::vector<std::string>result;
  return result;
}
bool videoDECKLINK::provides(const std::string name) {
  return (name==m_name);
}
std::vector<std::string>videoDECKLINK::provides(void) {
  std::vector<std::string>result;
  result.push_back(m_name);
  return result;
}
const std::string videoDECKLINK::getName(void) {
  return m_name;
}

