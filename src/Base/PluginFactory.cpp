#include "PluginFactory.h"
#include "GemSettings.h"
#include "GemFiles.h"
#include "GemDylib.h"
#include "Gem/RTE.h"

class gem::BasePluginFactory::Pimpl {
  friend class BasePluginFactory;
  Pimpl(void) {
    
  }

  ~Pimpl(void) {

  }

  //  std::map<int, std::vector<std::vector<int> > >p_types;
};


gem::BasePluginFactory::BasePluginFactory(void) : m_pimpl(new Pimpl) {

}
gem::BasePluginFactory::~BasePluginFactory(void) {
  delete m_pimpl;  m_pimpl=NULL;
}

int gem::BasePluginFactory::doLoadPlugins(std::string basename, std::string path) {
  std::cerr << "load plugins " << basename << " in " << path << std::endl;

  if(path.empty()){
    t_symbol*s=NULL;
    //    GemSettings::get("gem.path", &s);
    if(NULL!=s) {
      path=s->s_name;
    }
  }

  std::string pattern = path+basename+std::string("*.*");
  std::cerr << "pattern : " << pattern << std::endl;

  std::vector<std::string>files=gem::files::getFilenameListing(pattern);
  int i=0;
  for(i=0; i<files.size(); i++) {
    GemDylib*dll=NULL;
    std::string f=files[i];
    std::cerr << "dylib loading file " << f << "!" << std::endl;
    try {
      dll=new GemDylib(f.c_str(), "");
    } catch (GemException x) {
      std::cerr << "library loading returned: " << x.what() << std::endl;
    }
  }

  return 0;
}



