////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.at
//
// Implementation file
//
//    Copyright (c) 2011-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "imageloader.h"
#include "plugins/PluginFactory.h"

#include "Gem/RTE.h"
#include "Gem/Exception.h"

#include <algorithm>

gem::plugins::imageloader :: ~imageloader(void) {}

static gem::PluginFactoryRegistrar::dummy<gem::plugins::imageloader>
fac_imageloaderdummy;

namespace gem
{
namespace plugins
{
struct imageloaderMeta : public gem::plugins::imageloader
{
public:
  static imageloaderMeta*s_instance;
  std::vector<gem::plugins::imageloader*>m_loaders;
  std::vector<std::string>m_ids;
  bool m_canThread;

  imageloaderMeta(void) : m_canThread(true)
  {
    gem::PluginFactory<gem::plugins::imageloader>::loadPlugins("image");
    std::vector<std::string>available_ids=
      gem::PluginFactory<gem::plugins::imageloader>::getIDs();

    addLoader(available_ids, "magick");
    addLoader(available_ids);

    static bool firsttime=true;
    if(firsttime && m_ids.size()>0) {
      startpost("GEM: image loading plugins:");
      for(unsigned int i=0; i<m_ids.size(); i++) {
        startpost(" %s", m_ids[i].c_str());
      }
      endpost();
    }
    firsttime=false;

    m_canThread=true;

    for(unsigned int i=0; i<m_loaders.size(); i++) {
      if(!m_loaders[i]->isThreadable()) {
        m_canThread=false;
        break;
      }
    }
  }
  bool addLoader( std::vector<std::string>available,
                  std::string ID=std::string(""))
  {
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

    for(unsigned int i=0; i<id.size(); i++) {
      std::string key=id[i];
      verbose(2, "trying to add '%s' as backend", key.c_str());
      if(std::find(m_ids.begin(), m_ids.end(), key)==m_ids.end()) {
        // not yet added, do so now!
        gem::plugins::imageloader*loader=NULL;
        try {
          loader=gem::PluginFactory<gem::plugins::imageloader>::getInstance(key);
        } catch(GemException&x) {
          loader=NULL;
          verbose(1, "cannot use image loader plugin '%s': %s", key.c_str(),
                  x.what());
        }
        if(NULL==loader) {
          continue;
        }
        m_ids.push_back(key);
        m_loaders.push_back(loader);
        count++;
        verbose(2, "added backend#%d '%s' @ %p", (int)(m_loaders.size()-1),
                key.c_str(), loader);
      }
    }
    return (count>0);
  }

  virtual ~imageloaderMeta(void)
  {
    for(unsigned int i=0; i<m_loaders.size(); i++) {
      delete m_loaders[i];
      m_loaders[i]=NULL;
    }
  }

  virtual bool load(std::string filename, imageStruct&result,
                    gem::Properties&props)
  {
    std::vector<std::string> backends;
    if(props.type("_backends")!=gem::Properties::UNSET) {
      props.get("_backends", backends);
    }
    if(!backends.empty()) {
      /* if the user requested some backends, use these */
      for(unsigned int j=0; j<backends.size(); j++) {
        std::string id=backends[j];
        for(unsigned int i=0; i<m_loaders.size(); i++) {
          if(id!=m_ids[i])
            continue;
          if(m_loaders[i]->load(filename, result, props)) {
            return true;
          }
        }
      }
    } else {
      for(unsigned int i=0; i<m_loaders.size(); i++) {
        if(m_loaders[i]->load(filename, result, props)) {
          return true;
        }
      }
    }
    return false;
  }

  virtual bool isThreadable(void)
  {
    return m_canThread;
  }

  void getProperties(gem::Properties&props) {
    std::vector<std::string> ids;
    if(props.type("_backends")!=gem::Properties::UNSET) {
      for(unsigned int i=0; i<m_ids.size(); i++) {
        ids.push_back(m_ids[i]);
      }
    }

    props.clear();

    if(!ids.empty()) {
      props.set("_backends", ids);
    }
  }
};
};
};


gem::plugins::imageloader*gem::plugins::imageloader::getInstance(void)
{
  gem::plugins::imageloader*result=new imageloaderMeta();
  return result;
}

void gem::plugins::imageloader::getProperties(gem::plugins::imageloader*loader, gem::Properties&props) {
  gem::plugins::imageloaderMeta *meta  = dynamic_cast<gem::plugins::imageloaderMeta*>(loader);
  if (meta != nullptr) {
    meta->getProperties(props);
  } else {
    pd_error(0, "Unable to get properties from generic gem::plugins::imageloader (only meta-loader is supported)");
  }
}
