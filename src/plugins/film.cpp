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

#include "film.h"
#include "plugins/PluginFactory.h"
#include "Gem/RTE.h"
#include "Gem/Exception.h"
#include "Gem/Properties.h"
#include "imageloader.h"

#include <algorithm>

gem::plugins::film :: ~film(void) {}
/* initialize the film factory */
static gem::PluginFactoryRegistrar::dummy<gem::plugins::film> fac_filmdummy;

namespace gem { namespace plugins {
  class filmIMAGE : public gem::plugins::film {
    gem::plugins::imageloader*m_handle;
    pixBlock m_image;

  public:
    filmIMAGE(void) :
      m_handle(gem::plugins::imageloader::getInstance())
    {
      if(!m_handle)
        throw(GemException("no 'image' backends"));
    }

    virtual ~filmIMAGE(void) {
      delete m_handle;
    }
    virtual bool open(const std::string name, const gem::Properties&requestprops) {
      gem::Properties props=requestprops;
      m_image.newfilm=true;
      return m_handle->load(name, m_image.image, props);
    }
    virtual void close(void) {
      return;
    }
    virtual errCode changeImage(int imgNum, int trackNum=-1){
      if(imgNum!=0)return FAILURE;
      return SUCCESS;
    }
    virtual pixBlock* getFrame(void) {
      m_image.newimage=true;
      return &m_image;
    }
    virtual bool isThreadable(void) { return false; }

    virtual bool enumProperties(gem::Properties&readable,
                                gem::Properties&writeable) {
      writeable.clear();
      readable.clear();
      return false;
    }
    virtual void setProperties(gem::Properties&props) {
    }
    virtual void getProperties(gem::Properties&props) {
      std::vector<std::string>keys=props.keys();
      unsigned int i=0;
      for(i=0; i<keys.size(); i++) {
        std::string key=keys[i];
        props.erase(key);
        if("frames"==key) {double d=1; props.set(key, d); post("frames...");}
        if("width"==key)  {double d=m_image.image.xsize; props.set(key, d); }
        if("height"==key) {double d=m_image.image.ysize; props.set(key, d); }
      }
    }

  };


  class filmMeta : public gem::plugins::film {
  private:
    std::vector<gem::plugins::film*>m_handles; // all available handles
    gem::plugins::film*m_handle; // currently opened handle (or NULL)
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
          gem::plugins::film*handle=NULL;
          try {
            handle=gem::PluginFactory<gem::plugins::film>::getInstance(key);
          } catch(GemException&x) {
            handle=NULL;
            verbose(1, "cannot use film plugin '%s': %s", key.c_str(), x.what());
          }
          if(NULL==handle)continue;
          m_ids.push_back(key);
          m_handles.push_back(handle);
          count++;
          verbose(2, "added backend#%d '%s'", (int)(m_handles.size()-1), key.c_str());
        }
      }
      return (count>0);
    }

  public:
    filmMeta(void) :
      m_handle(NULL),
      m_canThread(true)
    {
      gem::PluginFactory<gem::plugins::film>::loadPlugins("film");
      std::vector<std::string>ids=gem::PluginFactory<gem::plugins::film>::getIDs();

      if(!addPlugin(ids, "DirectShow"))
        addPlugin(ids, "AVI");

      addPlugin(ids, "gmerlin");
      addPlugin(ids, "QuickTime");
      addPlugin(ids, "quicktime4linux");
      addPlugin(ids, "MPEG3");
      addPlugin(ids, "aviplay");
      addPlugin(ids, "MPEG1");
      addPlugin(ids);

      unsigned int i;
      for(i=0; i<m_handles.size(); i++) {
        if(!m_handles[i]->isThreadable()) {
          m_canThread=false;
          break;
        }
      }
      try {
        gem::plugins::film*filmImage=new filmIMAGE();
        if(NULL!=filmImage) {
          m_handles.push_back(filmImage);
          m_ids.push_back("image");
        }
      } catch (GemException&) {

      }
    }

    virtual ~filmMeta(void) {
      unsigned int i;
      for(i=0; i<m_handles.size(); i++) {
        delete m_handles[i];
        m_handles[i]=NULL;
      }
    }

    virtual bool open(const std::string name, const gem::Properties&requestprops) {
      if(m_handle)close();

      std::vector<std::string> backends;
      if(requestprops.type("backends")!=gem::Properties::UNSET) {
        requestprops.get("backends", backends);
      }
      //      requestprops.erase("backends");

      bool tried=false;
      if(!backends.empty()) {
        unsigned int i, j;
        for(j=0; !m_handle && j<backends.size(); j++) {
          std::string id=backends[j];
          for(i=0; i<m_handles.size(); i++) {
	    /* coverity[assign_where_compare_meant] we set 'tried' to true if we have found at least one matching backend */
            if(id==m_ids[i] && (tried=true) && m_handles[i]->open(name, requestprops)) {
              m_handle=m_handles[i];
              break;
            }
          }
        }
      }
      if(!tried) {
        if(!backends.empty() && !m_handles.empty()) {
          verbose(2, "no available backend selected, fall back to valid ones");
        }
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

    virtual errCode changeImage(int imgNum, int trackNum=-1){
      if(m_handle)
        return m_handle->changeImage(imgNum, trackNum);

      return FAILURE;
    }

    virtual pixBlock* getFrame(void) {
      if(m_handle)
        return m_handle->getFrame();
      return NULL;
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
      //      post("enumProperties stub");
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
      std::vector<std::string> ids;
      if(props.type("backends")!=gem::Properties::UNSET) {
        unsigned int i;
        for(i=0; i<m_ids.size(); i++) {
          ids.push_back(m_ids[i]);
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


gem::plugins::film*gem::plugins::film::getInstance(void) {
  gem::plugins::film*result=new filmMeta();
  return result;
}
