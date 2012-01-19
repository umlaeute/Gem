////////////////////////////////////////////////////////
//
// videoOptiTrack - Graphics Environment for Multimedia
//
// daniel@bogusfront.org
// zmoelnig@iem.at
//
// Implementation file
//
//    Copyright (c) 2003 Daniel Heckenberg.
//    Copyright (c) 2010-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif
#ifdef HAVE_LIBOPTITRACK

#include "videoOptiTrack.h"
using namespace gem::plugins;
#include "Gem/RTE.h"
#include "Gem/Exception.h"

#include "plugins/PluginFactory.h"

REGISTER_VIDEOFACTORY("OptiTrack", videoOptiTrack);


using namespace CameraLibrary;

namespace {
 static const std::string s_name = std::string("OptiTrack");
 static unsigned int s_refCount=0;
}


videoOptiTrack::videoOptiTrack(void) :
	m_camera(NULL),
	m_frame(NULL)
{
	CameraManager::X();
	if(s_refCount==0) {
		if(!CameraManager::X().WaitForInitialization()) {
			throw(GemException("couldn't initialize OptiTrack"));
		}
	}
	s_refCount++;

	if(!CameraManager::X().AreCamerasInitialized()) {
	  post("deferring camera init!");
	} else {
	  post("cameras initialized");
	}

	post("m_pixBlock@%p", &m_pixBlock);
	post("image@%p", &m_pixBlock.image);
	post("width@%p", &m_pixBlock.image.xsize);
return;
	m_pixBlock.image.xsize = 320;
	m_pixBlock.image.ysize = 240;
	m_pixBlock.image.setCsizeByFormat(GL_RGBA);
	m_pixBlock.image.reallocate();
}

videoOptiTrack::~videoOptiTrack(void) {
}
void videoOptiTrack::close(void) {
	stop();
	if(m_camera)
		m_camera->Release();
	m_camera=NULL;

	s_refCount--;
	if(s_refCount==0) {
      CameraManager::X().Shutdown();
	}
}

bool videoOptiTrack::open(gem::Properties&props) {
	m_camera = CameraManager::X().GetCamera();
	if(!m_camera) {
      return false;
	}	
	m_pixBlock.image.xsize = m_camera->Width();	
	m_pixBlock.image.ysize = m_camera->Height();	
	setProperties(props);
	return true;
}

pixBlock*videoOptiTrack::getFrame(void) {
  m_frame = m_camera->GetFrame();
  if(!m_frame)return NULL;

  m_pixBlock.image.reallocate();
  m_frame->Rasterize(m_pixBlock.image.xsize, m_pixBlock.image.ysize,
	                 m_pixBlock.image.xsize, m_pixBlock.image.csize*8,
					 m_pixBlock.image.data);

  return &m_pixBlock;
}

void videoOptiTrack::releaseFrame(void) {
	if(m_frame)
		m_frame->Release();
	m_frame=NULL;
}

std::vector<std::string>videoOptiTrack::enumerate(void) {
  std::vector<std::string>result;
  if(CameraManager::X().AreCamerasInitialized()) {
	  post("got cameras!");

  } else {
	  post("re-trying initialization....");
	  CameraManager::X().WaitForInitialization();
  }

  //  result.push_back("vlc");
  return result;
}

bool videoOptiTrack::setDevice(int ID) {
  m_devname.clear();
  return false;
}
bool videoOptiTrack::setDevice(std::string device) {
  m_devname=device;
  return true;
}
bool videoOptiTrack::enumProperties(gem::Properties&readable,
			       gem::Properties&writeable) {
  readable.clear();
  writeable.clear();

  writeable.set("width",  m_pixBlock.image.xsize);  readable.set("width",  m_pixBlock.image.xsize);
  writeable.set("height",  m_pixBlock.image.ysize); readable.set("height",  m_pixBlock.image.ysize);
  return false;
}
void videoOptiTrack::setProperties(gem::Properties&props) {
  int width=-1;
  int height=-1;

  m_props=props;

  double d;
  if(props.get("width", d)) {
	  if(d>0) {
		  width = d;
		  m_pixBlock.image.xsize=width;
	  }
  }
  if(props.get("height", d)) {
	  if(d>0) {
		  height=d;
		  m_pixBlock.image.ysize=height;
	  }
  }
}


void videoOptiTrack::getProperties(gem::Properties&props) {
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

std::vector<std::string>videoOptiTrack::dialogs(void) {
  std::vector<std::string>result;
  return result;
}
bool videoOptiTrack::provides(const std::string name) {
  return (name==s_name);
}
std::vector<std::string>videoOptiTrack::provides(void) {
  std::vector<std::string>result;
  result.push_back(s_name);
  return result;
}
const std::string videoOptiTrack::getName(void) {
  return s_name;
}


bool videoOptiTrack::start(void) {
	if(m_camera)
		m_camera->Start();

  return (NULL!=m_camera);
}
bool videoOptiTrack::stop (void) {
	bool status=false;
	if(m_camera) {
		status=m_camera->IsCameraRunning();
		m_camera->Stop();
	}
  return status;
}
#endif /* HAVE_LIBOPTITRACK */
