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
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "plugins/record.h"
#include "plugins/PluginFactory.h"

#include "Gem/RTE.h"
#include "Gem/Exception.h"

#include <algorithm>

gem::plugins::record :: ~record(void) {}

static gem::PluginFactoryRegistrar::dummy<gem::plugins::record>
fac_recorddummy;


namespace gem
{
namespace plugins
{
class recordMeta : public gem::plugins::record
{
private:
  static recordMeta*s_instance;
  std::vector<gem::plugins::record*>m_handles, // all available handles
      m_selectedHandles; // handles with the currently selected codec
  gem::plugins::record*m_handle; // currently opened handle (or NULL)
  std::vector<std::string>m_ids; // list of handle names
  std::string m_codec; // currently selected codec

  struct codechandle {
    codechandle(gem::plugins::record*h, const std::string&c):handle(h),
      codec(c) {}

    gem::plugins::record*handle;
    std::string codec;
  };
  std::map<std::string, std::vector<codechandle> >m_codechandle;
  std::vector<std::string>m_codecs;

  void addCodecHandle(gem::plugins::record*handle, const std::string&codec)
  {
#ifdef __GNUC__
# warning better handling of duplicate codecs
#endif
    /* FIXME: we should generate a unique codec-ID, e.g. "<handlename>:<codec>" */
    m_codechandle[codec].push_back(codechandle(handle, codec));
    m_codecs.push_back(codec);
  }
  void clearCodecHandle(void)
  {
    m_codecs.clear();
    m_codechandle.clear();
  }

  // set to TRUE if we can use the current handle in another thread
  bool m_canThread;

public:
  recordMeta(void) :
    m_handle(NULL),
    m_canThread(false)
  {
    gem::PluginFactory<gem::plugins::record>::loadPlugins("record");
    std::vector<std::string>ids=
      gem::PluginFactory<gem::plugins::record>::getIDs();

    addPlugin(ids, "QT");
    addPlugin(ids, "QT4L");
    addPlugin(ids);

#ifdef __GNUC__
# warning isThreadable
#endif
#if 0
    for(unsigned int i=0; i<m_handles.size(); i++) {
      //if(!m_handles[i]->isThreadable()) {
      if(1) {
        m_canThread=false;
        break;
      }
    }
#endif

    static bool firsttime=true;
    if(firsttime && m_ids.size()>0) {
      startpost("GEM: video record plugins:");
      for(unsigned int i=0; i<m_ids.size(); i++) {
        startpost(" %s", m_ids[i].c_str());
      }
      endpost();
    }
    firsttime=false;
  }
  bool addPlugin( std::vector<std::string>available,
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
        logpost(0, 3+2, "backend '%s' unavailable", ID.c_str());
        return false;
      }
    } else {
      // no 'ID' given: add all available IDs
      id=available;
    }

    for(unsigned int i=0; i<id.size(); i++) {
      std::string key=id[i];
      logpost(0, 3+2, "trying to add '%s' as backend", key.c_str());
      if(std::find(m_ids.begin(), m_ids.end(), key)==m_ids.end()) {
        // not yet added, do so now!
        gem::plugins::record*handle=NULL;
        try {
          handle=gem::PluginFactory<gem::plugins::record>::getInstance(key);
        } catch(GemException&x) {
          handle=NULL;
          logpost(0, 3+1, "cannot use record plugin '%s': %s", key.c_str(), x.what());
        }
        if(NULL==handle) {
          continue;
        }
        m_ids.push_back(key);
        m_handles.push_back(handle);
        count++;
        logpost(0, 3+2, "added backend#%d '%s'", (int)(m_handles.size()-1),
                key.c_str());
      }
    }
    return (count>0);
  }

public:
  virtual ~recordMeta(void)
  {
    for(unsigned int i=0; i<m_handles.size(); i++) {
      delete m_handles[i];
      m_handles[i]=NULL;
    }
  }

  virtual std::vector<std::string>getCodecs(void)
  {
    clearCodecHandle();
    for(unsigned int i=0; i<m_handles.size(); i++) {
      std::vector<std::string>c=m_handles[i]->getCodecs();
      for(unsigned int j=0; j<c.size(); j++) {
        addCodecHandle(m_handles[i], c[j]);
      }
    }
    return m_codecs;
  }
  virtual const std::string getCodecDescription(const std::string&codecname)
  {
    std::string descr;
    std::vector<codechandle>handles=m_codechandle[codecname];
    if(handles.size()>0) {
      return handles[0].handle->getCodecDescription(codecname);
    }

    return descr;
  }

  // this is mainly a filter:
  // after setCodec() is called, m_selectedHandles will only hold handles
  // that can handle the given codec
  // calling this method will eventually call the setCodec() method of all handles
  // specifying an empty codec, will select all handles
  virtual bool setCodec(const std::string&codecname)
  {
    m_selectedHandles.clear();
    if(codecname.empty()) {
      checkSelectedHandles();
      return true;
    }
    std::vector<codechandle>handles=m_codechandle[codecname];

    if(handles.size()==0) {
      return false;
    }

    for(unsigned int i=0; i<handles.size(); i++) {
      gem::plugins::record*handle=handles[i].handle;
      std::string codec=handles[i].codec;
      if(handle->setCodec(codec)) {
        m_codec=codecname;
        m_selectedHandles.push_back(handle);
      }
    }
    return (m_selectedHandles.size()>0);
  }

  bool checkSelectedHandles(void)
  {
    if(m_selectedHandles.size()==0 && m_codec.empty()) {
      m_selectedHandles=m_handles;
    }
    return (m_selectedHandles.size()>0);
  }

  //
  virtual bool dialog(void)
  {
    if(m_handle) {
      return m_handle->dialog();
    }
    checkSelectedHandles();

    gem::plugins::record*handle=NULL;
    for(unsigned int i=0; i<m_selectedHandles.size(); i++) {
      handle=m_selectedHandles[i];
      if(handle->dialog()) {
        break;
      }
      handle=NULL;
    }
    if(handle) {
      m_selectedHandles.clear();
      m_selectedHandles.push_back(handle);
      return true;
    }
    return false;
  }

  virtual bool enumProperties(gem::Properties&props)
  {
#ifdef __GNUC__
# warning enumerateProperties
#endif
    // shouldn't this return a merge of the properties of all selectedHandles?
    if(!checkSelectedHandles()) {
      return false;
    }

    for(unsigned int i=0; i<m_selectedHandles.size(); i++) {
      props.clear();
      if(m_selectedHandles[i]->enumProperties(props)) {
        return true;
      }
    }

    return false;
  }

  virtual void getProperties(gem::Properties&props)
  {
    std::vector<std::string> ids;
    if(props.type("_backends")!=gem::Properties::UNSET) {
      for(unsigned int i=0; i<m_ids.size(); i++) {
        ids.push_back(m_ids[i]);
      }
    }
    props.erase("_backends");

    if(m_handle) {
      m_handle->getProperties(props);
    } else {
      props.clear();
    }

    if(!ids.empty()) {
      props.set("_backends", ids);
    }
  }


  //////////
  // start recording
  //
  // try all of the currently selected handles (selection through codec)
  // until one of them succeeds
  virtual bool start(const std::string&filename, gem::Properties&props)
  {
    stop();

    if(!checkSelectedHandles()) {
      return false; // no selected codec available
    }

    /* if the user requested some backends, prioritize these */
    std::vector<std::string> backends;
    if(props.type("_backends")!=gem::Properties::UNSET) {
      props.get("_backends", backends);
    }

    bool tried=false;
    if(!backends.empty()) {
      for(unsigned int j=0; !m_handle && j<backends.size(); j++) {
        std::string id=backends[j];
        for(unsigned int i=0; i<m_selectedHandles.size(); i++) {
          /* coverity[assign_where_compare_meant] we set 'tried' to true if we have found at least one matching backend */
          if(id==m_ids[i] && (tried=true)
             && m_selectedHandles[i]->start(filename, props)) {
            m_handle=m_selectedHandles[i];
            break;
          }
        }
      }
    }
    if(!tried) {
      if(!backends.empty() && !m_selectedHandles.empty()) {
        logpost(0, 3+2, "no available backend selected, fall back to valid ones");
      }
      for(unsigned int i=0; i<m_selectedHandles.size(); i++) {
        if(m_selectedHandles[i]->start(filename, props)) {
          m_handle=m_selectedHandles[i];
          break;
        }
      }
    }
    return (NULL != m_handle);
  }

  //////////
  // record a frame
  //
  // write a frame with the current handle (if any)
  virtual bool write(imageStruct*img)
  {
    if(!m_handle) {
      return false;
    }
    if(!img) {
      return true;
    }
    bool result=m_handle->write(img);
    if(!result) {
      stop();
    }

    return result;
  }

  //////////
  // stop recording
  //
  // stop recording with the current handle
  // and "release" current handle
  virtual void stop (void)
  {
    if(m_handle) {
      m_handle->stop();
    }
    m_handle=NULL;
  }

  virtual bool isThreadable(void)
  {
    return m_canThread;
  }
};
};
};

gem::plugins::record*gem::plugins::record::getInstance(void)
{
  gem::plugins::record*result=new recordMeta();
  return result;
}
