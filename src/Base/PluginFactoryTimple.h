
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
  typename std::map<IdClass, PluginFactory<Class, IdClass>::ctors_t*>::iterator it = m_constructor.find(id);
  PluginFactory<Class, IdClass>::ctors_t*ctors=NULL;
  if(it != m_constructor.end()) {
    // we already have an entry for this id!
    ctors=it->second;
  } 
  
  if(NULL==ctors) {
    // no ctors in the map-entry (or no entry at all)
    ctors=new std::vector<ctor_t*>;
    m_constructor[id]=ctors;
  }
  
  // find out whether the given ctor is in the ctors-list
  typename std::vector<ctor_t*>::iterator vit=std::find(ctors->begin(), ctors->end(), c);
  if(vit != ctors->end()) {
    // found: this ctor has already been registered
    std::cout << "refusing to register constructor " << (void*)c << " again for id " << id << std::endl;
  } else {
    // not found: save this ctor
    ctors->push_back(c);
  }
}

template<class Class, class IdClass>
  Class*PluginFactory<Class, IdClass>::doGetInstance(IdClass id) {
  std::cout << "getInstance("<<id<<")" << std::endl;
  PluginFactory<Class, IdClass>::ctors_t*ctors=NULL;
  if(ctors=m_constructor[id]) {
    // iterate through all the ctors until on works
    typename std::vector<PluginFactory<Class, IdClass>::ctor_t*>::iterator it;
    int i=0;
    for (it = ctors->begin(); it != ctors->end(); ++it) {
      PluginFactory<Class, IdClass>::ctor_t*ctor=*it;
      Class*c=NULL;
      std::cout << "trying ctor#"<<i<<" for class:"<<id<<std::endl;
      i++;
      if(ctor)return ctor;
    }
  }
  
  std::cout << " (no valid ctor)" << std::endl;
  return NULL;
}

template<class Class, class IdClass>
void PluginFactory<Class, IdClass>::registerClass(IdClass id, PluginFactory<Class, IdClass>::ctor_t*c) {
  PluginFactory<Class, IdClass>*fac=getPluginFactory();
  if(NULL==fac) {
    std::cout << "unable to get a factory!" << std::endl;
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
