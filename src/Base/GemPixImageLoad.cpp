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
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute
//    Copyright (c) 2002-2003 james tittle/tigital
//
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
#include "Base/GemConfig.h"
#include "GemPixImageLoad.h"
#include "GemPixUtil.h"

#include "Gem/RTE.h"

#include "plugins/imageloader.h"
namespace gem {
  class PixImageLoader : public gem::plugins::imageloader {
  private:
    static PixImageLoader*s_instance;
    std::vector<gem::plugins::imageloader*>m_loaders;
    std::vector<std::string>m_ids;
    bool m_canThread;

    PixImageLoader(void) : m_canThread(true) {
      gem::PluginFactory<gem::plugins::imageloader>::loadPlugins("image");
      std::vector<std::string>available_ids=gem::PluginFactory<gem::plugins::imageloader>::getIDs();
      if(available_ids.size()>0) {
	startpost("Image loading support:");
	int i;
	for(i=0; i<available_ids.size(); i++) {
	  startpost(" %s", available_ids[i].c_str());
	}
	endpost();
      }

      addLoader(available_ids, "magick");
      addLoader(available_ids);

      m_canThread=true;
      int i;
      for(i=0; i<m_loaders.size(); i++) {
	if(!m_loaders[i]->isThreadable()) {
	  m_canThread=false;
	  break;
	}
      }      
    }
    bool addLoader( std::vector<std::string>available, std::string ID=std::string(""))
    {
      int i=0;
      int count=0;

      std::vector<std::string>id;
      if(!ID.empty()) {
	// if requested 'cid' is in 'available' add it to the list of 'id's
	if(std::find(available.begin(), available.end(), ID)!=available.end()) {
	  id.push_back(ID);
	} else {
	  // request for an unavailable ID
	  verbose(2, "backend '%s' unavailable", ID.c_str());
	  return false;
	}
      } else {
	// no 'ID' given: add all available IDs
	id=available;
      }

      for(i=0; i<id.size(); i++) {
	std::string key=id[i];
	verbose(2, "trying to add '%s' as backend", key.c_str());
	if(std::find(m_ids.begin(), m_ids.end(), key)==m_ids.end()) {
	  // not yet added, do so now!
	  gem::plugins::imageloader*loader=
	    gem::PluginFactory<gem::plugins::imageloader>::getInstance(key); 
	  if(NULL==loader)break;
	  m_ids.push_back(key);
	  m_loaders.push_back(loader);
	  count++;
	  verbose(2, "added backend#%d '%s' @ 0x%x", m_loaders.size()-1, key.c_str(), loader);
	}
      }
      return (count>0);
    }

  public:
    virtual ~PixImageLoader(void) {
      int i;
      for(i=0; i<m_loaders.size(); i++) {
	delete m_loaders[i];
	m_loaders[i]=NULL;
      }
    }

    virtual bool load(std::string filename, imageStruct&result, gem::Properties&props) {
      int i;
      for(i=0; i<m_loaders.size(); i++) {
	if(m_loaders[i]->load(filename, result, props))
	  return true;
      }
      return false;
    }

    static PixImageLoader*getInstance(void) {
      if(NULL==s_instance) {
	s_instance=new PixImageLoader();
      }
      return s_instance;
    }
    virtual bool isThreadable(void) {
      return m_canThread;
    }
}; };

gem::PixImageLoader*gem::PixImageLoader::s_instance=NULL;

namespace gem { namespace image { namespace load {

  bool sync(const std::string filename,
	    imageStruct&result,
	    gem::Properties&props) {
    gem::PixImageLoader*piximageloader=gem::PixImageLoader::getInstance();
    if(piximageloader) {
      if(piximageloader->load(filename, result, props)) {
	return true;
      }
    }
    return false;
  }
  
  id_t async(callback*cb,
	     void*userdata,
	     const std::string filename) {
    if(NULL==cb)
      return INVALID;

    imageStruct*result=new imageStruct;
    gem::Properties props;
    if(sync(filename, *result, props)) {
      (*cb)(userdata, IMMEDIATE, result, props);
      return IMMEDIATE;
    }
    return INVALID;
  }

  bool cancel(id_t ID) {
    return false;
  }

}; // load
}; // image
}; // gem


/***************************************************************************
 *
 * image2mem - Read in an image in various file formats
 *
 ***************************************************************************/
GEM_EXTERN imageStruct *image2mem(const char *filename)
{
  gem::Properties props;
  imageStruct *img = new imageStruct();
  if(gem::image::load::sync(filename, *img, props))
    return img;
  
  delete img;
  return NULL;
}
