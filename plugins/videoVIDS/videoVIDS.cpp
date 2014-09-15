////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 2014 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <stdio.h>
#ifdef _MSC_VER
# define snprintf _snprintf
#endif

#include "videoVIDS.h"
#include "plugins/PluginFactory.h"

#include "Gem/Exception.h"
#include "Gem/RTE.h"

#define MAXVIDSSTRING 1024

using namespace gem::plugins;

REGISTER_VIDEOFACTORY("VIDS", videoVIDS);

videoVIDS::videoVIDS(void) :
  m_name(std::string("videoInput"))
{
}

videoVIDS::~videoVIDS(void)
{
}
const std::string videoVIDS::getName(void)
{
  return m_name;
}
bool videoVIDS::provides(const std::string name)
{
  return (name==m_name);
}
std::vector<std::string>videoVIDS::provides(void)
{
  std::vector<std::string>result;
  result.push_back(m_name);
  return result;
}

std::vector<std::string>videoVIDS::enumerate(void)
{
  std::vector<std::string>result;
  return result;
}

bool videoVIDS::setDevice(int ID)
{
  return false;
}
bool videoVIDS::setDevice(std::string device)
{
  return false;
}
bool videoVIDS::enumProperties(gem::Properties&readable,
                               gem::Properties&writeable)
{
  readable.clear();
  writeable.clear();

  writeable.set("width",  m_pixBlock.image.xsize);
  readable.set("width",  m_pixBlock.image.xsize);
  writeable.set("height",  m_pixBlock.image.ysize);
  readable.set("height",  m_pixBlock.image.ysize);
  return false;
}
void videoVIDS::setProperties(gem::Properties&props)
{
  int width=-1;
  int height=-1;

  m_props=props;

  double d;
  if(props.get("width", d)) {
    if(d>0) {
      width = d;
    }
  }
  if(props.get("height", d)) {
    if(d>0) {
      height=d;
    }
  }
}

void videoVIDS::getProperties(gem::Properties&props)
{
  std::vector<std::string>keys=props.keys();
  double d;
  int i;

  for(i=0; i<keys.size(); i++) {
    if("width"==keys[i]) {
      props.set(keys[i], m_pixBlock.image.xsize);
    }
    if("height"==keys[i]) {
      props.set(keys[i], m_pixBlock.image.ysize);
    }
  }
}

bool videoVIDS::open(gem::Properties&props)
{
  return false;
}
void videoVIDS::close(void)
{
}

bool videoVIDS::start(void)
{
  return false;
}
bool videoVIDS::stop (void)
{
  return false;
}
bool videoVIDS::reset()
{
  return false;
}

pixBlock*videoVIDS::getFrame(void)
{
  return 0;
}

void videoVIDS::releaseFrame(void)
{
}

std::vector<std::string>videoVIDS::dialogs(void)
{
  std::vector<std::string>result;
  return result;
}
bool videoVIDS::dialog(std::vector<std::string>dlgs)
{
  return false;
}

bool videoVIDS::isThreadable()
{
  return false;
}
bool videoVIDS::grabAsynchronous(bool)
{
  return false;
}
bool videoVIDS::setColor(int)
{
  return false;
}
