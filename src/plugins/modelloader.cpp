////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 2001-2012 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "modelloader.h"
#include "plugins/PluginFactory.h"
#include "Gem/RTE.h"
#include "Gem/Exception.h"

#include "Gem/Properties.h"
#include <string>


namespace gem { namespace plugins {

  modelloader :: ~modelloader(void) {}
  /* initialize the modelloader factory */
  static gem::PluginFactoryRegistrar::dummy<modelloader> fac_modelloaderdummy;



class modelloaderMeta : public gem::plugins::modelloader {
private:
  std::vector<gem::plugins::modelloader*>m_handles; // all available handles
  gem::plugins::modelloader*m_handle; // currently opened handle (or NULL)
  std::vector<std::string>m_ids; // list of handle names

  // set to TRUE if we can use the current handle in another thread
  bool m_canThread;

  bool addPlugin( std::vector<std::string>available, std::string ID=std::string("")) {
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

    unsigned int i=0;
    for(i=0; i<id.size(); i++) {
      std::string key=id[i];
      verbose(2, "trying to add '%s' as backend", key.c_str());
      if(std::find(m_ids.begin(), m_ids.end(), key)==m_ids.end()) {
        // not yet added, do so now!
        gem::plugins::modelloader*handle=NULL;
        try {
          handle=gem::PluginFactory<gem::plugins::modelloader>::getInstance(key);
        } catch(GemException&x) {
          handle=NULL;
          verbose(1, "cannot use modelloader plugin '%s': %s", key.c_str(), x.what());
        }
        if(NULL==handle)continue;
        m_ids.push_back(key);
        m_handles.push_back(handle);
        count++;
        verbose(2, "added backend#%d '%s'", m_handles.size()-1, key.c_str());
      }
    }
    return (count>0);
  }

public:
  modelloaderMeta(void) :
    m_handle(NULL),
    m_canThread(true)
  {
    static bool firsttime=true;
    if(firsttime)
      gem::PluginFactory<gem::plugins::modelloader>::loadPlugins("model");
    firsttime=false;
    std::vector<std::string>ids=gem::PluginFactory<gem::plugins::modelloader>::getIDs();

    //addPlugin(ids, "MPEG1");
    addPlugin(ids);

    unsigned int i;
    for(i=0; i<m_handles.size(); i++) {
      if(!m_handles[i]->isThreadable()) {
        m_canThread=false;
        break;
      }
    }
  }

  virtual ~modelloaderMeta(void) {
    unsigned int i;
    for(i=0; i<m_handles.size(); i++) {
      delete m_handles[i];
      m_handles[i]=NULL;
    }
  }

  virtual bool open(const std::string&name, const gem::Properties&requestprops) {
    if(m_handle)close();

    std::string ids;
    if(requestprops.type("backends")!=gem::Properties::UNSET) {
      requestprops.get("backends", ids);
    }
    //      requestprops.erase("backends");

    if(!ids.empty()) {
      // LATER: allow multiple IDs to be passed via 'backend'
      unsigned int i=0;
      for(i=0; i<m_handles.size(); i++) {
        if(ids==m_ids[i] && m_handles[i]->open(name, requestprops)) {
          m_handle=m_handles[i];
        }
      }
    }
    if(!m_handle) {
      unsigned int i=0;
      for(i=0; i<m_handles.size(); i++) {
        if(m_handles[i] && m_handles[i]->open(name, requestprops)) {
          m_handle=m_handles[i];
          break;
        } else {

        }
      }
    }
    return (NULL!=m_handle);
  }

  virtual bool render(void) {
    if(m_handle)
      return m_handle->render();
    return false;
  }
  virtual bool compile(void) {
    if(m_handle)
      return m_handle->compile();
    return false;
  }

  virtual void close(void)  {
    if(m_handle)
      m_handle->close();
    m_handle=NULL;
  }

  virtual bool isThreadable(void) {
    if(m_handle)
      return m_handle->isThreadable();

    return m_canThread;
  }

  virtual bool enumProperties(gem::Properties&readable,
                              gem::Properties&writeable) {
    // LATER: shouldn't we merge properties of all handles?
#ifdef __GNUC__
# warning enumProperties stub
#endif

    readable.clear();
    writeable.clear();

    if(m_handle)
      return m_handle->enumProperties(readable, writeable);

    return false;
  }

  virtual void setProperties(gem::Properties&props) {
    if(m_handle)
      m_handle->setProperties(props);
  }
  virtual void getProperties(gem::Properties&props) {
    std::string ids;
    if(props.type("backends")!=gem::Properties::UNSET) {
      unsigned int i;
      for(i=0; i<m_ids.size(); i++) {
        if(!ids.empty())
          ids+=" ";
        ids=ids+m_ids[i];
      }
    }
    props.erase("backends");

    if(m_handle)
      m_handle->getProperties(props);
    else
      props.clear();

    if(!ids.empty()) {
      props.set("backends", ids);
    }
  }
};
};
};


gem::plugins::modelloader*gem::plugins::modelloader::getInstance(void) {
  gem::plugins::modelloader*result=new modelloaderMeta();
  return result;
}

