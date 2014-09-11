////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 2010-2012 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "plugins/video.h"
#include "plugins/PluginFactory.h"

#include <algorithm>

gem::plugins::video :: ~video(void) {}

static gem::PluginFactoryRegistrar::dummy<gem::plugins::video> fac_videodummy;

#include "Gem/Exception.h"
#include "Gem/RTE.h"

namespace gem { namespace plugins {
  class videoMeta : public gem::plugins::video {
  private:
    static videoMeta*s_instance;
    std::vector<gem::plugins::video*>m_allHandles, // all available handles
      m_selectedHandles; // handles with the currently selected codec
    gem::plugins::video*m_handle; // currently opened handle (or NULL)
    std::vector<std::string>m_ids; // list of handle names
    std::string m_codec; // currently selected codec

    struct codechandle {
      codechandle(gem::plugins::video*h, const std::string c):handle(h), codec(c) {}

      gem::plugins::video*handle;
      std::string codec;
    };
    std::map<std::string, std::vector<codechandle> >m_codechandle;
    std::vector<std::string>m_codecs;

    void addCodecHandle(gem::plugins::video*handle, const std::string codec) {
      /* FIXME: we should generate a unique codec-ID, e.g. "<handlename>:<codec>" */
      m_codechandle[codec].push_back(codechandle(handle, codec));
      m_codecs.push_back(codec);
    }
    void clearCodecHandle(void) {
      m_codecs.clear();
      m_codechandle.clear();
    }
    bool addPlugin( std::vector<std::string>available, std::string ID=std::string("")) {
      int count=0;

      std::vector<std::string>id;
      if(!ID.empty()) {
        // if requested 'cid' is in 'available' add it to the list of 'id's
        if(std::find(available.begin(), available.end(), ID)!=available.end()) {
          id.push_back(ID);
        } else {
          // request for an unavailable ID
          verbose(2, "Gem::video: backend '%s' unavailable", ID.c_str());
          return false;
        }
      } else {
        // no 'ID' given: add all available IDs
        id=available;
      }

      unsigned int i=0;
      for(i=0; i<id.size(); i++) {
        std::string key=id[i];
        verbose(2, "Gem::video: trying to add '%s' as backend", key.c_str());
        if(std::find(m_ids.begin(), m_ids.end(), key)==m_ids.end()) {
          // not yet added, do so now!
          gem::plugins::video*handle=NULL;
          try {
            handle=gem::PluginFactory<gem::plugins::video>::getInstance(key);
          } catch(GemException&x) {
            handle=NULL;
            verbose(1, "Gem::video: cannot use video plugin '%s': %s", key.c_str(), x.what());
          }
          if(NULL==handle)continue;
          m_ids.push_back(key);
          m_allHandles.push_back(handle);
          count++;
          verbose(2, "Gem::video: added backend#%d '%s'", (int)(m_allHandles.size()-1), key.c_str());
        }
      }
      return (count>0);
    }

    // set to TRUE if we can use the current handle in another thread
    bool m_canThread;

  public:
    videoMeta(void) :
      m_handle(NULL),
      m_canThread(false)
    {
      // compat
      gem::PluginFactory<gem::plugins::video>::loadPlugins("video");
      std::vector<std::string>ids=gem::PluginFactory<gem::plugins::video>::getIDs();

      // LATER: also add w32 & osx default backends
      addPlugin(ids, "v4l2");
      addPlugin(ids, "v4l");
      addPlugin(ids, "dv4l");
      addPlugin(ids);

      for(unsigned int i=0; i<m_allHandles.size(); i++) {
        if(!m_allHandles[i]->isThreadable()) {
          m_canThread=false;
          break;
        }
      }
    }

    virtual ~videoMeta(void) {
      // compat
      unsigned int i;
      for(i=0; i<m_allHandles.size(); i++) {
        delete m_allHandles[i];
        m_allHandles[i]=NULL;
      }
    }
    virtual std::vector<std::string>enumerate(void){ 
      // almost compat
      // LATER: the consumer should know which device corresponds to which backend/driver
      /* maybe something like
       *   "devicename:backend" with ':' being some special character unlikely to be found in devicenames
       */
      std::vector<std::string>result;
      for(unsigned int i=0; i<m_allHandles.size(); i++) {
			  std::vector<std::string>res=m_allHandles[i]->enumerate();
			  for(unsigned int j=0; j<res.size(); j++) {
				  result.push_back(res[j]);
			  }
		  }
		  return result;
    }
	  virtual bool setDevice(int ID){
      // compat
		  bool result=false;
		  for(unsigned int i=0; i<m_allHandles.size(); i++) {
			  if(m_allHandles[i]->setDevice(ID))
				  result=true;
		  }
		  return result;
	  }
	  virtual bool	    	setDevice(const std::string ID) {
      // compat
		  bool result=false;
		  for(unsigned int i=0; i<m_allHandles.size(); i++) {
			  if(m_allHandles[i]->setDevice(ID))
				  result=true;
		  }
		  return result;
	  }
	  virtual bool open(gem::Properties&props) {
      // LATER: driver-selection
      /* e.g. add a reserved "_backend" property and use that to select the correct driver
       * available backends could be queried with "_backends"
       */
		  if(m_handle)close();
		  for(unsigned int i=0; i<m_allHandles.size(); i++) {
			  if(m_allHandles[i]->open(props)) {
				  m_handle=m_allHandles[i];
				  return true;
			  }
		  }
		  return false;
	  }
	  virtual bool start(void) {
      // OK
		  if(m_handle)
			  return m_handle->start();
		  return false;
	  }
	  virtual pixBlock *getFrame(void) {
      // OK
		  if(m_handle)return m_handle->getFrame();
		  return NULL;
	  }
    virtual void releaseFrame(void) {
      // OK
		  if(m_handle)m_handle->releaseFrame();
	  }
	  virtual bool stop(void) {
      // OK
		  if(m_handle)
			  return m_handle->stop();
		  return false;
	  }
	  virtual void close(void) {
      // OK
		  if(m_handle)
			  m_handle->close();
		  m_handle=NULL;
	  }
	  virtual bool reset(void) {
      // OK
		  if(m_handle)
			  return m_handle->reset();

	  bool result=false;
		for(unsigned int i=0; i<m_allHandles.size(); i++) {
			if(m_allHandles[i]->reset())
				result=true;
		}
		return result;
	  }
	  virtual bool enumProperties(gem::Properties&readable, gem::Properties&writeable) {
      // OK
		  if(m_handle)
			  return m_handle->enumProperties(readable, writeable);
		  return false;
	  }
    virtual void setProperties(gem::Properties&props) {
      // OK
		  if(m_handle)
			  m_handle->setProperties(props);
	  }
    virtual void getProperties(gem::Properties&props) {
      // OK
		  if(m_handle)
			  m_handle->getProperties(props);
	  }
    virtual bool dialog(std::vector<std::string>names) {
      // OK
      /* LATER: what if no device is currently opened?
       *  e.g. when we want to set the device with dialog?
       *  think: if there's only one backend, use that
       *  try all available backends, until one returns true
       *  filter backends based on "_backend" selection?
       */
		  if(m_handle)
			  return m_handle->dialog(names);
		  return false;
	  }
    virtual std::vector<std::string>dialogs(void) {
      // OK
		  if(m_handle)
			  return m_handle->dialogs();

		  std::vector<std::string>result;
      for(unsigned int i=0; i<m_allHandles.size(); i++) {
			  std::vector<std::string>res=m_allHandles[i]->dialogs();
			  for(unsigned int j=0; j<res.size(); j++) {
				  result.push_back(res[j]);
			  }
		  }
		  return result;
	  }
	  virtual bool isThreadable(void) {
      // OK
      // LATER properly set m_canThread in ctor
		  if(m_handle)
			  return m_handle->isThreadable();
		  return m_canThread;
	  }
	  virtual bool grabAsynchronous(bool state) {
      // OK
      // LATER get rid of that entirely?
		  if(m_handle)
			  return m_handle->grabAsynchronous(state);
		  return false;
	  }
	  virtual bool setColor(int color){
      // OK
      // LATER get rid of that!
      // think about the return value...
      bool result=true;
      for(unsigned int i=0; i<m_allHandles.size(); i++) {
        if(!m_allHandles[i]->setColor(color)) {
          result=false;
        }
		  }
		  return result;
	  }
	  virtual bool provides(const std::string ID){
      // OK
		  for(unsigned int i=0; i<m_allHandles.size(); i++) {
			  if(m_allHandles[i]->provides(ID))
				  return true;
		  }
		  return false;
	  }
	  virtual std::vector<std::string>provides(void){
      // OK
      // LATER: remove dupes
		  std::vector<std::string>result;
		  for(unsigned int i=0; i<m_allHandles.size(); i++) {
			  std::vector<std::string>res=m_allHandles[i]->provides();
			  for(unsigned int j=0; j<res.size(); j++) {
				  result.push_back(res[i]);
			  }
		  }
		  return result;
	  }
	  virtual const std::string getName(void) {
      // OK
      // LATER: special handling of empty names in host(!)
      return std::string("");
    }
  };
};
};

gem::plugins::video*gem::plugins::video::getInstance(void) {
  video*result=new videoMeta();
	return result;
}
