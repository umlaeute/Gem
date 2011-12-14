
/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

	- template implementation for PluginFactory

    Copyright (c) 2010-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef _INCLUDE__GEM_PLUGINS_PLUGINFACTORY_H_
# error you must not include PluginFactory Implementation directly! include PluginFactory.h instead
#endif

#if 0 /* set to 1 to enable some debugging printout */
# define GEM_PLUGFAC_DEBUG 1
#else
# define GEM_PLUGFAC_DEBUG 0
#endif

/* on W32 (regardless of whether we use MinGW or M$VC) we must _not_
 * define the template-implementation in the plugins, if we want to
 * use the same implementation in both host and plugin
 * (if we do, the host and the plugin will see different factories!)
 *
 * on linux/gcc we have to provide the implementation
 * (on osx/gcc as well, it seems)
 */
#if defined _WIN32 && !defined GEM_INTERNAL
# define OMIT_PLUGINFACTORY_TEMPLATE_IMPLEMENATION
#endif

#ifndef OMIT_PLUGINFACTORY_TEMPLATE_IMPLEMENATION

/* Implementation of templated PluginFactory */
/* actually this should be done in a cpp file rather than a header-file,
 * but since virtually no compiler can handle this it is done here...
 */


/* ********************************************************************* */
/* Implementation of PluginFactory<Class>                                */

template<class Class>
  PluginFactory<Class>* PluginFactory<Class>::s_factory=NULL;

template<class Class>
  PluginFactory<Class>* PluginFactory<Class>::getPluginFactory(void) {
  if(NULL==s_factory) {
    s_factory=new PluginFactory<Class>;
  }
#if GEM_PLUGFAC_DEBUG
	std::cerr << "factory @ " << (void*)s_factory << " --> " << typeid(s_factory).name() << std::endl;
#endif /* GEM_PLUGFAC_DEBUG */
  return s_factory;
}

template<class Class>
  void  PluginFactory<Class>::doRegisterClass(std::string id, ctor_t*c) {
  set(id, (void*)c);
}

template<class Class>
  Class*PluginFactory<Class>::doGetInstance(std::string id) {
  ctor_t*ctor=(ctor_t*)get(id);
  if(ctor)
    return ctor();
  else
    return NULL;
}

template<class Class>
void PluginFactory<Class>::registerClass(std::string id, ctor_t*c) {
  PluginFactory<Class>*fac=getPluginFactory();
  if(NULL==fac) {
    std::cerr << "unable to get a factory!" << std::endl;
  }
#if GEM_PLUGFAC_DEBUG
  std::cerr << "register " << typeid(Class).name() << " @ factory: " << (void*)fac << std::endl;
#endif /* GEM_PLUGFAC_DEBUG */
  fac->doRegisterClass(id, c);
}

template<class Class>
Class*PluginFactory<Class>::getInstance(std::string id) {
  PluginFactory<Class>*fac=getPluginFactory();
#if GEM_PLUGFAC_DEBUG
  std::cerr << "getting " << typeid(Class).name() << " instance '" << id << "' from factory: " << (void*)fac << std::endl;
#endif /* GEM_PLUGFAC_DEBUG */
  if(NULL==fac) {
    return NULL;
  }
  return(fac->doGetInstance(id));
}

template<class Class>
  int PluginFactory<Class>::loadPlugins(std::string basename, std::string path) {
  PluginFactory<Class>*fac=getPluginFactory();
#if GEM_PLUGFAC_DEBUG
  std::cerr << "loading " << typeid(Class).name() << " plugins from factory: " << (void*)fac << std::endl;
#endif /* GEM_PLUGFAC_DEBUG */
  if(NULL==fac) {
    return 0;
  }
  return fac->doLoadPlugins(basename, path);
}

template<class Class>
  std::vector<std::string>PluginFactory<Class>::doGetIDs() {
  return get();
}

template<class Class>
  std::vector<std::string>PluginFactory<Class>::getIDs() {
  std::vector<std::string>result;
  PluginFactory<Class>*fac=getPluginFactory();
  if(fac) {
    return fac->doGetIDs();
  }
  return result;
}

#endif /*  !OMIT_PLUGINFACTORY_TEMPLATE_IMPLEMENATION */

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

  template<class ChildClass, class BaseClass>
    registrar<ChildClass, BaseClass> :: registrar(std::string id) {
    PluginFactory<BaseClass>::registerClass(id, allocator<ChildClass, BaseClass>);
  }
  template<class BaseClass>
    dummy<BaseClass> :: dummy() {
    std::string id; // default ID
    PluginFactory<BaseClass>::registerClass(id, NULL);
  }

};

