////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// Implementation file
//
// SPDX-FileCopyrightText: © 2010, IOhannes m zmölnig and the GEM contributors
// SPDX-License-Identifier: GPL-2.0-or-later
//
////////////////////////////////////////////////////////
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <string.h>
#include "filmTEST.h"
#include "plugins/PluginFactory.h"
#include "Gem/RTE.h"
#include "Gem/Properties.h"

using namespace gem::plugins;

REGISTER_FILMFACTORY("test", filmTEST);

/////////////////////////////////////////////////////////
//
// filmTEST
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////

filmTEST :: filmTEST(void)
  : m_fps(20)
  , m_numFrames(100)
{
  m_image.image.setFormat(GEM_RGBA);
  m_image.image.xsize=320;
  m_image.image.ysize=240;
  m_image.image.reallocate();
}

/////////////////////////////////////////////////////////
// really open the file ! (OS dependent)
//
/////////////////////////////////////////////////////////
bool filmTEST :: open(const std::string&filename,
                      const gem::Properties&wantProps)
{
  changeImage(0,0);

  return true;
}

void filmTEST::close(void) {}
bool filmTEST::isThreadable(void)
{
  return true;
}

void filmTEST::setProperties(gem::Properties&props)
{
}

void filmTEST::getProperties(gem::Properties&props)
{
  std::vector<std::string> keys=props.keys();
  unsigned int i=0;
  for(i=0; i<keys.size(); i++) {
    std::string key=keys[i];
    props.erase(key);
#define SETPROP(k, v) } else if(k == key) { double d=(double)v; props.set(key, d)
    if(""==key) {
      SETPROP("fps", m_fps);
      SETPROP("frames", m_numFrames);
      SETPROP("width", m_image.image.xsize);
      SETPROP("height", m_image.image.ysize);
    }
  }
}


/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
pixBlock* filmTEST :: getFrame()
{
  return &m_image;
}

film::errCode filmTEST :: changeImage(int imgNum, int trackNum)
{
  unsigned char*data=m_image.image.data;
  unsigned int size=
    m_image.image.xsize*m_image.image.ysize*m_image.image.csize;

  unsigned char value=(unsigned char)imgNum;

  while(size-->0) {
    *data++=value;
    value++;
  }

  m_image.newimage=true;

  return film::SUCCESS;
}


bool filmTEST::enumProperties(gem::Properties&readprops,
                              gem::Properties&writeprops)
{
  readprops.clear();
  writeprops.clear();

  double d=0;

  readprops.set("width", d);
  readprops.set("height", d);
  readprops.set("fps", d);
  readprops.set("frames", d);

  return true;
}
