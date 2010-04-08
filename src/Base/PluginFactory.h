#ifndef INCLUDE_PLUGINFACTORY_H_
#define INCLUDE_PLUGINFACTORY_H_

#include "Base/GemExportDef.h"


#include <map>
#include <vector>

#include <typeinfo>
#include <iostream>

class GEM_EXTERN BasePluginFactory {
 protected:
  BasePluginFactory(void);
  virtual ~BasePluginFactory(void);

 private:
  class Pimpl;
  Pimpl*m_pimpl;

  typedef int id_t;
};

template<class Class, class IdClass>
class GEM_EXTERN PluginFactory : public BasePluginFactory {
 public:
  typedef Class*(ctor_t)(void);

 private:
  typedef std::vector<ctor_t*> ctors_t;
  typedef std::map<IdClass, ctors_t*> ctormap_t;
  ctormap_t m_constructor;

  static PluginFactory<Class, IdClass>*s_factory;
  static PluginFactory<Class, IdClass>*getPluginFactory(void) {
    if(NULL==s_factory) {
      s_factory=new PluginFactory<Class, IdClass>;
    }
    return s_factory;
  }

  void doRegisterClass(IdClass id, ctor_t*c) {
    typename std::map<IdClass, ctors_t*>::iterator it = m_constructor.find(id);
    ctors_t*ctors=NULL;
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

  Class*doGetInstance(IdClass id) {
    std::cout << "getInstance("<<id<<")" << std::endl;
    ctors_t*ctors=NULL;
    if(ctors=m_constructor[id]) {
      // iterate through all the ctors until on works
      typename std::vector<ctor_t*>::iterator it;
      int i=0;
      for (it = ctors->begin(); it != ctors->end(); ++it) {
	ctor_t*ctor=*it;
	Class*c=NULL;
	std::cout << "trying ctor#"<<i<<" for class:"<<id<<std::endl;
	i++;
	if(ctor)return ctor;
      }
    }

    std::cout << " (no valid ctor)" << std::endl;
    return NULL;
  }

 public:

  static void registerClass(IdClass id, ctor_t*c) {
    PluginFactory<Class, IdClass>*fac=getPluginFactory();
    if(NULL==fac) {
      std::cout << "unable to get a factory!" << std::endl;
    }
    fac->doRegisterClass(id, c);
  }

  static Class*getInstance(IdClass id) {
    PluginFactory<Class, IdClass>*fac=getPluginFactory();
    if(NULL==fac) {
      return NULL;
    }
    return(fac->doGetInstance(id));
  }

};

template<class Class, class IdClass>
  PluginFactory<Class, IdClass>* PluginFactory<Class, IdClass>::s_factory=NULL;






namespace PluginFactoryRegistrar {
  template<class ChildClass, class BaseClass>
    static BaseClass* allocator() {
    ChildClass* res0 = new ChildClass();
    BaseClass* res1 = dynamic_cast<BaseClass*>(res0);
    if(NULL==res1) {
      delete res0;
    }
    return res1;
  }

  template<class ChildClass, class BaseClass, class IdClass=int>
    class registrar {
  public:
    registrar(IdClass id) {
      PluginFactory<BaseClass, IdClass>::registerClass(id, allocator<ChildClass, BaseClass>);
    }
  };
};
#endif /* INCLUDE_PLUGINFACTORY_H */
