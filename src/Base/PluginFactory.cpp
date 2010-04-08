#include "PluginFactory.h"

class BasePluginFactory::Pimpl {
  friend class BasePluginFactory;
  Pimpl(void) {
    
  }

  ~Pimpl(void) {

  }

  //  std::map<int, std::vector<std::vector<int> > >p_types;
};


BasePluginFactory::BasePluginFactory(void) : m_pimpl(new Pimpl) {

}
BasePluginFactory::~BasePluginFactory(void) {
  delete m_pimpl;  m_pimpl=NULL;
}



