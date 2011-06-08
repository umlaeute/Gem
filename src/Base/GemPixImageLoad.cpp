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

    PixImageLoader(void) {
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

    virtual bool load(std::string filename, imageStruct&result) {
      int i;
      for(i=0; i<m_loaders.size(); i++) {
	if(m_loaders[i]->load(filename, result))
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
}; };

gem::PixImageLoader*gem::PixImageLoader::s_instance=NULL;

/***************************************************************************
 *
 * image2mem - Read in an image in various file formats
 *
 ***************************************************************************/
GEM_EXTERN imageStruct *image2mem(const char *filename)
{
   imageStruct *image_block = NULL;
   gem::PixImageLoader*piximageloader=gem::PixImageLoader::getInstance();
   if(piximageloader) {
     std::string fname=filename;
     image_block=new imageStruct();
     if(piximageloader->load(filename, *image_block)) {
       return image_block;
     }
   }
   return NULL;
}
