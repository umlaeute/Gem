////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file 
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "filmMPEG1.h"
#include "plugins/PluginFactory.h"
#include "Gem/Properties.h"

using namespace gem::plugins;


#ifdef HAVE_LIBMPEG
REGISTER_FILMFACTORY("MPEG1", filmMPEG1);

/////////////////////////////////////////////////////////
//
// filmMPEG1
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////

filmMPEG1 :: filmMPEG1(void) : 
  filmBase(),
  m_wantedFormat(GL_RGBA),
  m_fps(-1.0),
  m_curFrame(-1),
  m_newfilm(false),
  m_streamfile(NULL),
  m_reachedEnd(false),
  m_data(NULL), 
  m_length(0)
{}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
filmMPEG1 :: ~filmMPEG1()
{
  close();
  if(m_data)delete[]m_data;
}

void filmMPEG1 :: close(void)
{
  if (m_streamfile)fclose(m_streamfile);
  m_streamfile=0;
}

/////////////////////////////////////////////////////////
// really open the file ! (OS dependent)
//
/////////////////////////////////////////////////////////
bool filmMPEG1 :: open(const std::string filename, int format)
{
  if (format>0)m_wantedFormat=format;
  if (!(m_streamfile = fopen (filename.c_str(), "rb")))return false;
  int wantedFormat= (m_wantedFormat)?m_wantedFormat:GL_RGBA;
  switch (wantedFormat){
  case GL_LUMINANCE:
      SetMPEGOption (MPEG_DITHER, GRAY_DITHER);
      break;
  default:
    wantedFormat=GL_RGBA;
  case GL_YCBCR_422_GEM:
  case GL_RGBA:
    SetMPEGOption (MPEG_DITHER, FULL_COLOR_DITHER);
  }
  if (OpenMPEG (m_streamfile, &m_streamVid)) { /* let's hope it's MPEG */
    m_curFrame = 0;
    
    // Unfortunately there is no way to get the length of an MPEG-Stream
    m_fps = (double)m_streamVid.PictureRate; // ??
    
    m_image.image.xsize  = m_streamVid.Width;
    m_image.image.ysize  = m_streamVid.Height;
    if (!(m_image.image.xsize*m_image.image.ysize))goto unsupported;

    m_image.image.setCsizeByFormat(wantedFormat);
    m_image.image.reallocate();

    int length=m_image.image.xsize*m_image.image.ysize;
    length*=((m_image.image.format==GL_LUMINANCE)?1:4)+4;
    if(m_length<length){
      if (m_data)delete[]m_data;
      m_length=length;
      m_data=new unsigned char[m_length];
    }
    m_reachedEnd=false;
    m_newfilm = true;
    return true;
  }
  goto unsupported;
 unsupported:
  close();
  return false;
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
pixBlock* filmMPEG1 :: getFrame(){
  if (m_reachedEnd){
    m_curFrame=-1;
    return 0;
  }
  if (!m_readNext){
    return &m_image;
  }
  m_image.image.upsidedown=true;

  m_readNext = false;
  int length=m_image.image.xsize*m_image.image.ysize;
  length*=((m_image.image.format==GL_LUMINANCE)?1:4)+4;
  if(m_length<length){
    if (m_data)delete[]m_data;
    m_length=length;
    m_data=new unsigned char[m_length];
  }
  if (m_reachedEnd=!GetMPEGFrame ((char*)(m_data))){
    if(m_image.image.format==GL_YCBCR_422_GEM){
      m_image.image.fromRGBA(m_data);
    }  else  m_image.image.data=m_data;
    m_curFrame=-1;
    return &m_image;// was 0; but then we have one non-textured frame in auto-mode
  } else {
    if(m_image.image.format==GL_YCBCR_422_GEM){
      m_image.image.fromRGBA(m_data);
    }  else  m_image.image.data=m_data;
    if(m_newfilm)m_image.newfilm=1;  m_newfilm=false;
    m_image.newimage=1;
    return &m_image;
  }
  return 0;
}

film::errCode filmMPEG1 :: changeImage(int imgNum, int trackNum){
  if (m_reachedEnd&&imgNum>0)return film::FAILURE;

  m_readNext = true;
  if (imgNum==0){
    if (!RewindMPEG(m_streamfile, &m_streamVid))return film::FAILURE;
    m_curFrame=0;
    m_reachedEnd=false;
  }
  return film::DONTKNOW;
}

///////////////////////////////
// Properties
bool filmMPEG1::enumProperties(gem::Properties&readable,
			      gem::Properties&writeable) {
  readable.clear();
  writeable.clear();

  gem::any value;
  value=0.;
  readable.set("fps", value);
  readable.set("width", value);
  readable.set("height", value);

  writeable.set("colorspace", value);

  return false;
}

void filmMPEG1::setProperties(gem::Properties&props) {
  double d;
  if(props.get("colorspace", d)) {
    m_wantedFormat=(GLenum)d;
  }
}

void filmMPEG1::getProperties(gem::Properties&props) {
  std::vector<std::string> keys=props.keys();
  gem::any value;
  double d;
  unsigned int i=0;
  for(i=0; i<keys.size(); i++) {
    std::string key=keys[i];
    props.erase(key);
    if("fps"==key) {
      d=m_fps;
      value=d; props.set(key, value);
    }
    if("width"==key) {
      d=m_image.image.xsize;
      value=d; props.set(key, value);
    }
    if("height"==key) {
      d=m_image.image.ysize;
      value=d; props.set(key, value);
    }
  }
}




#endif
