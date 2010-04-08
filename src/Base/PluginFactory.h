#ifndef INCLUDE_PLUGINFACTORY_H_
#define INCLUDE_PLUGINFACTORY_H_

#include "Base/GemExportDef.h"


#include <map>
#include <vector>

#include <typeinfo>
#include <iostream>

namespace gem {

  class GEM_EXTERN BasePluginFactory {
  protected:
    BasePluginFactory(void);
    virtual ~BasePluginFactory(void);

  private:
    class Pimpl;
    Pimpl*m_pimpl;
  };

  template<class Class, class IdClass>
    class GEM_EXTERN PluginFactory : public BasePluginFactory {
  public:

    /** 
     * constructor function type (without arguments)
     */
    typedef Class*(ctor_t)(void);

    /**
     * register a a constructor associated with a given ID
     */
    static void registerClass(IdClass id, ctor_t*c);
    /**
     * get an instance of class constructed by the constructor associated with the given ID
     */
    static Class*getInstance(IdClass id);


  private:
    typedef std::map<IdClass, ctor_t*> ctormap_t;
    ctormap_t m_constructor;

    static PluginFactory<Class, IdClass>*s_factory;
    static PluginFactory<Class, IdClass>*getPluginFactory(void);

    void doRegisterClass(IdClass id, ctor_t*c);
    Class*doGetInstance(IdClass id);
  };





  namespace PluginFactoryRegistrar {
    template<class ChildClass, class BaseClass>
      static BaseClass* allocator(void);

    template<class ChildClass, class BaseClass, class IdClass>
      struct registrar {
        registrar(IdClass id);
      };
  };

/* include the actual implementation */
#include "PluginFactoryTimple.h"


}; // namespace gem


#endif /* INCLUDE_PLUGINFACTORY_H */
