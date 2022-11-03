////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.at
//
// Implementation file
//
//    Copyright (c) 2022 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "LICENSE.txt"
//
/////////////////////////////////////////////////////////
#include "videoPIPEWIRE.h"
#include "plugins/PluginFactory.h"
using namespace gem::plugins;

REGISTER_VIDEOFACTORY("pipewire", videoPIPEWIRE);


videoPIPEWIRE::videoPIPEWIRE(void) :
  m_name(std::string("test")),
  m_open(false),
  m_type(0)
{
  m_pixBlock.image.xsize = 64;
  m_pixBlock.image.ysize = 64;
  m_pixBlock.image.setCsizeByFormat(GEM_RGBA);
  m_pixBlock.image.reallocate();
}

videoPIPEWIRE::~videoPIPEWIRE(void)
{
}

bool videoPIPEWIRE::open(gem::Properties&props)
{
  setProperties(props);
  return (m_open);
}


pixBlock*videoPIPEWIRE::getFrame(void)
{
  m_pixBlock.image.setCsizeByFormat(GEM_RGBA);
  m_pixBlock.image.reallocate();
  const unsigned int count = m_pixBlock.image.xsize * m_pixBlock.image.ysize;
  unsigned int i=0;
  unsigned char*data=m_pixBlock.image.data;


  m_pixBlock.newimage = true;

  return &m_pixBlock;
}

std::vector<std::string>videoPIPEWIRE::enumerate(void)
{
  std::vector<std::string>result;
  result.push_back("pipewire");
  return result;
}

bool videoPIPEWIRE::setDevice(int ID)
{
  m_open=(0==ID);
  return m_open;
}
bool videoPIPEWIRE::setDevice(const std::string&device)
{
  m_open=("pipewire"==device);
  return m_open;
}
bool videoPIPEWIRE::enumProperties(gem::Properties&readable,
                               gem::Properties&writeable)
{
  readable.clear();
  writeable.clear();


  writeable.set("width", 64);
  readable.set("width", 64);
  writeable.set("height", 64);
  readable.set("height", 64);

  //writeable.set("type", std::string("noise"));
  return true;
}
void videoPIPEWIRE::setProperties(gem::Properties&props)
{
  m_props=props;

  double d;
  if(props.get("width", d)) {
    if(d>0) {
      m_pixBlock.image.xsize = d;
    }
  }
  if(props.get("height", d)) {
    if(d>0) {
      m_pixBlock.image.ysize = d;
    }
  }
}
void videoPIPEWIRE::getProperties(gem::Properties&props)
{
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

std::vector<std::string>videoPIPEWIRE::dialogs(void)
{
  std::vector<std::string>result;
  return result;
}
bool videoPIPEWIRE::provides(const std::string&name)
{
  return (name==m_name);
}
std::vector<std::string>videoPIPEWIRE::provides(void)
{
  std::vector<std::string>result;
  result.push_back(m_name);
  return result;
}
const std::string videoPIPEWIRE::getName(void)
{
  return m_name;
}
