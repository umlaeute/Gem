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
  m_name(std::string("videoInput")),
  m_ID(-1), m_wantID(-1)
{
  m_vi.setVerbose(false);
}

videoVIDS::~videoVIDS(void)
{
  close();
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
  m_vi.listDevices();
  std::vector<std::string>result=m_vi.getDeviceList();
  return result;
}

bool videoVIDS::setDevice(int ID)
{
  m_wantID=ID;
  return true;
}
bool videoVIDS::setDevice(std::string device)
{
  if(m_vi.listDevices(true)) {
    std::vector<std::string>devs=m_vi.getDeviceList();
    for(unsigned int i=0; i<devs.size(); i++) {
      if(devs[i]==device) {
	m_wantID=i;
	return true;
      }
    }
  }
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
  if(m_wantID>=0 && m_wantID<m_vi.listDevices(true))
    return true;

  return false;
}
void videoVIDS::close(void)
{
  stop();
  m_wantID=-1;
}

bool videoVIDS::start(void)
{
  if(m_wantID>=0) {
    if(m_vi.setupDevice(m_wantID)) {
      m_ID=m_wantID;
      return true;
    }
  }
  return false;
}
bool videoVIDS::stop (void)
{
  if(m_ID>=0) {
    m_vi.stopDevice(m_ID);
    m_ID=-1;
    return true;
  }
  return false;
}
bool videoVIDS::reset()
{
  if(m_ID>=0) {
    return m_vi.restartDevice(m_ID);
  }
  return false;
}

pixBlock*videoVIDS::getFrame(void)
{
  if(m_ID<0)return NULL;
  if(m_vi.isFrameNew(m_ID)){
    int w=m_vi.getWidth(m_ID);
    int h=m_vi.getHeight(m_ID);
    unsigned char*data=m_pixBlock.image.data;
    m_pixBlock.image.xsize=w;
    m_pixBlock.image.ysize=h;
    m_pixBlock.image.setCsizeByFormat(GL_RGBA);
    m_pixBlock.image.reallocate();
    m_pixBlock.newimage=(data==m_pixBlock.image.data);

    m_pixBlock.image.fromRGB(m_vi.getPixels(m_ID, true, true));
    m_pixBlock.newimage=true;
  } else {
    m_pixBlock.newimage=false;
  }
  return &m_pixBlock;
}

void videoVIDS::releaseFrame(void)
{
}

std::vector<std::string>videoVIDS::dialogs(void)
{
  std::vector<std::string>result;
  result.push_back("format");
  return result;
}
bool videoVIDS::dialog(std::vector<std::string>dlgs)
{
  bool doit=false;
  if(m_ID>=0) {
    if(dlgs.empty())
      doit=true;

    if(!doit) {
      for(unsigned int i=0; i<dlgs.size(); i++)
	if(dlgs[i]=="format")doit=true;
    }
    if(doit) {
      m_vi.showSettingsWindow(m_ID);
    }
  }
  return doit;
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
