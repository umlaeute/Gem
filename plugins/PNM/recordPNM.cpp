////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.at
//
// Implementation file
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    Copyright (c) 2022 Claude Heiland-Allen. claude@mathr.co.uk
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
// Future plans
//
//    - support 16bit PNM formats (PPM, PGM)
//    - support 1bit PNM format (PBM)
//    - support plain (ascii decimal) formats
//
/////////////////////////////////////////////////////////
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "recordPNM.h"
#include "Gem/RTE.h"
#include "Gem/Manager.h"

#include "plugins/PluginFactory.h"


using namespace gem::plugins;

#include <stdio.h>

REGISTER_RECORDFACTORY("PNM", recordPNM);

/////////////////////////////////////////////////////////
//
// recordPNM
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
recordPNM :: recordPNM(void)
  : m_file(NULL)
  , m_channels(3)
  , m_image()
{
}

////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
recordPNM :: ~recordPNM(void)
{
  stop();
}

void recordPNM :: stop(void)
{
  if(m_file) {
    fclose(m_file);
    m_file = nullptr;
  }
}

/////////////////////////////////////////////////////////
// open a file !
//
/////////////////////////////////////////////////////////
/* guess the file-format by inspecting the extension */
static int guess_channels(const std::string&filename)
{
  const char * extension = strrchr(filename.c_str(), '.');
  unsigned int i=0;

  if(!extension) {
    verbose(0,
            "[GEM:recordPNM] no extension given: encoding will be PPM");
    return 3;
  }

  extension++;

  if(!strcasecmp(extension, "pgm")) {
    return 1;
  }
  if(!strcasecmp(extension, "ppm")) {
    return 3;
  }

  verbose(0,
          "[GEM:recordPNM] unknown extension: encoding will be PPM");
  return 3;
}


bool recordPNM :: start(const std::string&filename, gem::Properties&props)
{
  stop();

  m_channels = guess_channels(filename);

  m_file = fopen(filename.c_str(), "wb");
  if(m_file==NULL) {
    pd_error(0, "[GEM:recordPNM] starting to record to %s failed",
             filename.c_str());
    return false;
  }

  return (true);
}


/////////////////////////////////////////////////////////
// initialize the encoder
//
/////////////////////////////////////////////////////////
bool recordPNM :: init(const imageStruct*img, double fps)
{
  if(!m_file || !img || fps < 0.) {
    return false;
  }
  return true;
}

/////////////////////////////////////////////////////////
// do the actual encoding and writing to file
//
/////////////////////////////////////////////////////////
bool recordPNM :: write(imageStruct*img)
{
  if(!m_file || !img) {
    return false;
  }

  switch(m_channels) {
  case 1:
    m_image.convertFrom(img, GEM_GRAY);
    break;
  case 3:
    m_image.convertFrom(img, GEM_RGB);
    break;
  default:
    pd_error(0, "[GEM:recordPNM] unsupported channels %d...", m_channels);
    return false;
  }

  if (m_image.csize != m_channels) {
    pd_error(0, "[GEM:recordPNM] unsupported data layout %d != %d...",
             m_image.csize, m_channels);
    return false;
  }

  if(fprintf(m_file, "P%d\n%d %d\n255\n",
             m_channels == 1 ? 5 : 6, m_image.xsize, m_image.ysize) < 0) {
    pd_error(0, "[GEM:recordPNM] error writing image header...");
    return false;
  }

  size_t stride = m_image.xsize * m_image.csize;
  if (m_image.upsidedown) {
    if(fwrite(m_image.data, m_image.ysize * stride, 1, m_file) != 1) {
      pd_error(0, "[GEM:recordPNM] error writing image data...");
      return false;
    }
  } else {
    for (int y = m_image.ysize - 1; y >= 0; --y) {
      if(fwrite(m_image.data + y * stride, stride, 1, m_file) != 1) {
        pd_error(0, "[GEM:recordPNM] error writing image data row...");
        return false;
      }
    }
  }
  return true;
}


/////////////////////////////////////////////////////////
// codecs
//
/////////////////////////////////////////////////////////

bool recordPNM :: setCodec(const std::string &name)
{
  if(name == "ppm") {
    m_channels = 3;
    return true;
  } else if(name == "pgm") {
    m_channels = 1;
    return true;
  } else {
    pd_error(0, "[GEM:recordPNM] unknown codec...");
    return false;
  }
}

std::vector<std::string> recordPNM :: getCodecs(void)
{
  std::vector<std::string> codecs;
  codecs.push_back("ppm");
  codecs.push_back("pgm");
  return codecs;
}

const std::string recordPNM :: getCodecDescription(const std::string&codecname)
{
  if(codecname == "ppm") {
    return "Portable Pixmap (PPM)";
  } else if(codecname == "pgm") {
    return "Portable Greymap (PGM)";
  } else {
    pd_error(0, "[GEM:recordPNM] cannot describe unknown codec...");
    return "(unknown codec)";
  }
}

/////////////////////////////////////////////////////////
// properties
//
/////////////////////////////////////////////////////////

bool recordPNM :: enumProperties(gem::Properties&props)
{
  props.clear();
  return true;
}
