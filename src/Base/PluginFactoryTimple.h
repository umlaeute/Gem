
/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

	- template implementation for PluginFactory

    Copyright (c) 2010 IOhannes m zmoelnig. forum::f-bür::umläute. IEM. zmoelnig@iem.kug.ac.at-A
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_PLUGINFACTORY_H_
# error you must not include PluginFactory Implementation directly! include PluginFactory.h instead
#endif

/* Implementation of templated PluginFactory */
/* actually this should be done in a cpp file rather than a header-file, 
 * but since virtually no compiler can handle this it is done here...
 */


/* ********************************************************************* */
/* Implementation of PluginFactory<Class>                                */

template<class Class, class IdClass>
  PluginFactory<Class, IdClass>* PluginFactory<Class, IdClass>::s_factory=NULL;

template<class Class, class IdClass>
  PluginFactory<Class, IdClass>* PluginFactory<Class, IdClass>::getPluginFactory(void) {
  if(NULL==s_factory) {
    s_factory=new PluginFactory<Class, IdClass>;
  }
  return s_factory;
}

template<class Class, class IdClass>
  void  PluginFactory<Class, IdClass>::doRegisterClass(IdClass id, PluginFactory<Class, IdClass>::ctor_t*c) {
  typename std::map<IdClass, PluginFactory<Class, IdClass>::ctor_t*>::iterator it = m_constructor.find(id);
  PluginFactory<Class, IdClass>::ctor_t*ctor=NULL;
  if(it != m_constructor.end()) {
    // we already have an entry for this id!
    if(it->second == c)return;
    if(NULL != it->second) {
      std::cerr << "refusing to register constructor " << (void*)c << " again for id " << id << std::endl;
      return;
    }
  }
  m_constructor[id]=c;
}

template<class Class, class IdClass>
  Class*PluginFactory<Class, IdClass>::doGetInstance(IdClass id) {
  PluginFactory<Class, IdClass>::ctor_t*ctor=m_constructor[id];
  //if(NULL==ctor)std::cerr << " (no valid ctor for '" << id << "')" << std::endl;
  if(ctor)
    return ctor();
  else
    return NULL;
}

template<class Class, class IdClass>
void PluginFactory<Class, IdClass>::registerClass(IdClass id, PluginFactory<Class, IdClass>::ctor_t*c) {
  PluginFactory<Class, IdClass>*fac=getPluginFactory();
  if(NULL==fac) {
    std::cerr << "unable to get a factory!" << std::endl;
  }
  fac->doRegisterClass(id, c);
}

template<class Class, class IdClass>
Class*PluginFactory<Class, IdClass>::getInstance(IdClass id) {
  PluginFactory<Class, IdClass>*fac=getPluginFactory();
  if(NULL==fac) {
    return NULL;
  }
  return(fac->doGetInstance(id));
}

template<class Class, class IdClass>
  int PluginFactory<Class, IdClass>::loadPlugins(std::string basename, std::string path) {
  PluginFactory<Class, IdClass>*fac=getPluginFactory();
  if(NULL==fac) {
    return 0;
  }
  return fac->doLoadPlugins(basename, path);
}

template<class Class, class IdClass>
  std::vector<IdClass>PluginFactory<Class, IdClass>::doGetIDs() {
  std::vector<IdClass>result;
  for(typename std::map<IdClass, PluginFactory<Class, IdClass>::ctor_t*>::iterator iter = m_constructor.begin(); iter != m_constructor.end(); ++iter) {
    if(NULL!=iter->second)
      result.push_back(iter->first);
  }
  return result;
}

template<class Class, class IdClass>
  std::vector<IdClass>PluginFactory<Class, IdClass>::getIDs() {
  std::vector<IdClass>result;
  PluginFactory<Class, IdClass>*fac=getPluginFactory();
  if(fac) {
    return fac->doGetIDs();
  }
  return result;
}

/* ********************************************************************* */
/* Implementation of PluginFactoryRegistrar<ChildClass, BaseClass>       */

namespace PluginFactoryRegistrar {
  template<class ChildClass, class BaseClass>
    BaseClass* allocator() {
    ChildClass* res0 = new ChildClass();
    BaseClass* res1 = dynamic_cast<BaseClass*>(res0);
    if(NULL==res1) {
      delete res0;
    }
    return res1;
  }

  template<class ChildClass, class BaseClass, class IdClass>
    registrar<ChildClass, BaseClass, IdClass> :: registrar(IdClass id) {
    PluginFactory<BaseClass, IdClass>::registerClass(id, allocator<ChildClass, BaseClass>);
  }
};

