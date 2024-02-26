////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.at
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

#include <algorithm>

namespace gem
{
namespace plugins
{

modelloader :: modelloader(void)
//: m_refresh(false)
{}
modelloader :: ~modelloader(void) {}
/* initialize the modelloader factory */
static gem::PluginFactoryRegistrar::dummy<modelloader>
fac_modelloaderdummy;



class modelloaderMeta : public gem::plugins::modelloader
{
private:
  std::vector<gem::plugins::modelloader*>m_handles; // all available handles
  gem::plugins::modelloader*m_handle; // currently opened handle (or NULL)
  std::vector<std::string>m_ids; // list of handle names

  // set to TRUE if we can use the current handle in another thread
  bool m_canThread;

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
        gem::plugins::modelloader*handle=NULL;
        try {
          handle=gem::PluginFactory<gem::plugins::modelloader>::getInstance(key);
        } catch(GemException&x) {
          handle=NULL;
          verbose(1, "cannot use modelloader plugin '%s': %s", key.c_str(),
                  x.what());
        }
        if(NULL==handle) {
          continue;
        }
        m_ids.push_back(key);
        m_handles.push_back(handle);
        count++;
        verbose(2, "added backend#%d '%s'", (int)(m_handles.size()-1),
                key.c_str());
      }
    }
    return (count>0);
  }

public:
  modelloaderMeta(void) :
    m_handle(NULL),
    m_canThread(true)
  {
    gem::PluginFactory<gem::plugins::modelloader>::loadPlugins("model");
    std::vector<std::string>ids=
      gem::PluginFactory<gem::plugins::modelloader>::getIDs();

    //addPlugin(ids, "MPEG1");
    addPlugin(ids);

    for(unsigned int i=0; i<m_handles.size(); i++) {
      if(!m_handles[i]->isThreadable()) {
        m_canThread=false;
        break;
      }
    }

    static bool firsttime=true;
    if(firsttime && ids.size()>0) {
      startpost("GEM: model loading plugins:");
      for(unsigned int i=0; i<ids.size(); i++) {
        startpost(" %s", ids[i].c_str());
      }
      endpost();
    }
    firsttime=false;
  }

  virtual ~modelloaderMeta(void)
  {
    for(unsigned int i=0; i<m_handles.size(); i++) {
      delete m_handles[i];
      m_handles[i]=NULL;
    }
  }

  virtual bool open(const std::string&name,
                    const gem::Properties&requestprops)
  {
    if(m_handle) {
      close();
    }

    std::vector<std::string> backends;
    if(requestprops.type("_backends")!=gem::Properties::UNSET) {
      requestprops.get("_backends", backends);
    }

    bool tried=false;
    if(!backends.empty()) {
      for(unsigned int j=0; !m_handle && j<backends.size(); j++) {
        std::string id=backends[j];

        for(unsigned int i=0; i<m_handles.size(); i++) {
          /* coverity[assign_where_compare_meant] we set 'tried' to true if we have found at least one matching backend */
          if(id==m_ids[i]&& (tried=true) && m_handles[i]->open(name, requestprops)) {
            m_handle=m_handles[i];
          }
        }
      }
    }
    if(!m_handle && !tried) {
      if(!backends.empty() && !m_handles.empty()) {
        verbose(2, "no available loader selected, falling back to valid ones");
      }
      for(unsigned int i=0; i<m_handles.size(); i++) {
        if(m_handles[i] && m_handles[i]->open(name, requestprops)) {
          m_handle=m_handles[i];
          break;
        } else {

        }
      }
    }
    return (NULL!=m_handle);
  }

  virtual struct mesh*getMesh(size_t meshNum) {
    if (m_handle) {
      return m_handle->getMesh(meshNum);
    }
    return nullptr;
  }
  virtual size_t getNumMeshes(void) {
    if (m_handle) {
      return m_handle->getNumMeshes();
    }
    return 0;
  }
  virtual bool updateMeshes(void) {
    if (m_handle) {
      return m_handle->updateMeshes();
    }
    return false;
  }

  virtual void close(void)
  {
    if(m_handle) {
      m_handle->close();
    }
    m_handle=NULL;
  }

  virtual bool isThreadable(void)
  {
    if(m_handle) {
      return m_handle->isThreadable();
    }

    return m_canThread;
  }

  virtual bool enumProperties(gem::Properties&readable,
                              gem::Properties&writeable)
  {
    // LATER: shouldn't we merge properties of all handles?
#ifdef __GNUC__
# warning enumProperties stub
#endif

    readable.clear();
    writeable.clear();

    if(m_handle) {
      return m_handle->enumProperties(readable, writeable);
    }

    return false;
  }

  virtual void setProperties(gem::Properties&props)
  {
    if(m_handle) {
      m_handle->setProperties(props);
    }
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
};
};
};


gem::plugins::modelloader*gem::plugins::modelloader::getInstance(void)
{
  gem::plugins::modelloader*result=new modelloaderMeta();
  return result;
}

namespace {
  GLfloat*color2gl(const gem::plugins::modelloader::color&c, GLfloat buf[4]) {
    buf[0] = c.r;
    buf[1] = c.g;
    buf[2] = c.b;
    buf[3] = c.a;
    return buf;
  }
}

void gem::plugins::modelutils::render_material(const gem::plugins::modelloader::material&m)
{
  GLfloat glcol[4];
#define COLOR2FLOAT4(col) color2gl(col, glcol)
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, COLOR2FLOAT4(m.diffuse));
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, COLOR2FLOAT4(m.specular));
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, COLOR2FLOAT4(m.ambient));
  glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, COLOR2FLOAT4(m.emissive));
  glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, m.shininess);
#if 0
  post("============== material ================");
  post("diffuse  = %f/%f/%f/%f", m.diffuse.r, m.diffuse.g, m.diffuse.b, m.diffuse.a);
  post("specular = %f/%f/%f/%f", m.specular.r, m.specular.g, m.specular.b, m.specular.a);
  post("ambient  = %f/%f/%f/%f", m.ambient.r, m.ambient.g, m.ambient.b, m.ambient.a);
  post("emissive = %f/%f/%f/%f", m.emissive.r, m.emissive.g, m.emissive.b, m.emissive.a);
#endif
}
