#include "PluginFactory.h"
#include "Base/GemSettings.h"
#include "Base/GemFiles.h"
#include "Base/GemDylib.h"
#include "Gem/RTE.h"

#include "sstream"

using namespace gem;

class gem::BasePluginFactory::Pimpl {
  friend class BasePluginFactory;
  Pimpl(void) {
    
  }

  ~Pimpl(void) {

  }

  std::vector<std::string>p_loaded;

  std::map<std::string, void*>p_ctors;
};


gem::BasePluginFactory::BasePluginFactory(void) : m_pimpl(new Pimpl) {

}
gem::BasePluginFactory::~BasePluginFactory(void) {
  delete m_pimpl;  m_pimpl=NULL;
}

int gem::BasePluginFactory::doLoadPlugins(std::string basename, std::string path) {
  std::cerr << "load plugins '" << basename << "' in '" << path << "'" << std::endl;

  if(path.empty()){
    t_symbol*s=NULL;
    GemSettings::get("gem.path", s);
    if(NULL!=s) {
      path=s->s_name;
    }
  }

  if(!path.empty()){
    path=path+std::string("/");
  }

  std::string pattern = path+basename+std::string("*")+GemDylib::getDefaultExtension();
  std::cerr << "pattern : " << pattern << std::endl;

  std::vector<std::string>files=gem::files::getFilenameListing(pattern);
  int i=0;
  for(i=0; i<files.size(); i++) {
    GemDylib*dll=NULL;
    const std::string f=files[i];
    // check whether this file has already been loaded
    // LATER make checks more sophisticated (like checking file-handles)
    bool alreadyloaded=false;
    int j;
    for(j=0; j<m_pimpl->p_loaded.size(); j++)
      if(f == m_pimpl->p_loaded[j]) {
	alreadyloaded=true;
	std::cerr << "not reloading '"<<f<<"'"<<std::endl;
	break;
      }
    if(alreadyloaded)continue;

    std::cerr << "dylib loading file '" << f << "'!" << std::endl;
    try {
      dll=new GemDylib(f, "");
      m_pimpl->p_loaded.push_back(f);
    } catch (GemException x) {
      std::cerr << "library loading returned: " << x.what() << std::endl;
    }
  }

  return 0;
}

std::vector<std::string>gem::BasePluginFactory::get() {
  std::vector<std::string>result;
  std::map<std::string, void*>::iterator iter = m_pimpl->p_ctors.begin();
  for(; iter != m_pimpl->p_ctors.end(); ++iter) {
    if(NULL!=iter->second)
      result.push_back(iter->first);
  }
#if 0
  for(typename std::map<std::string, ctor_t*>::iterator iter = m_constructor.begin(); iter != m_constructor.end(); ++iter) {
    if(NULL!=iter->second)
      result.push_back(iter->first);
  }
#endif
  return result;
}

void*gem::BasePluginFactory::get(std::string id) {
  return m_pimpl->p_ctors[id];
}

void gem::BasePluginFactory::set(std::string id, void*ptr) {
  m_pimpl->p_ctors[id]=ptr;
}
