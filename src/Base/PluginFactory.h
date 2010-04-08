#ifndef INCLUDE_PLUGINFACTORY_H_
#define INCLUDE_PLUGINFACTORY_H_

#include "Base/GemExportDef.h"


#include <map>
#include <vector>

#include <typeinfo>
#include <iostream>

namespace gem {

  class GEM_EXTERN BasePluginFactory {
  public:
    int doLoadPlugins(std::string basename, std::string path);
  protected:
    BasePluginFactory();
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

    /**
     * get a list of all IDs currently registered with this factory
     */
    static std::vector<IdClass>getIDs(void);

    /**
     * load more plugins
     */
    static int loadPlugins(std::string basename, std::string path=std::string(""));

  private:
    typedef std::map<IdClass, ctor_t*> ctormap_t;
    ctormap_t m_constructor;

    static PluginFactory<Class, IdClass>*s_factory;
    static PluginFactory<Class, IdClass>*getPluginFactory();

    void doRegisterClass(IdClass id, ctor_t*c);
    Class*doGetInstance(IdClass id);
    std::vector<IdClass>doGetIDs(void);
  };





  namespace PluginFactoryRegistrar {
    /**
     * creates a new ChildClass and returns it as a (pointer to) an instance of BaseClass
     */
    template<class ChildClass, class BaseClass>
      static BaseClass* allocator(void);

    /**
     * registers a ChildClass with a certain ID in the BaseClass factory
     *
     * example:
     *  static gem::PluginFactoryRegistrar<Child, Base, std::string > basefac_childreg("childID"); // register Child as 'childID'
     *  Base*instance=gem::PluginFactory<Base>::getInstance("childID"); // returns an instance of Child
     */
    template<class ChildClass, class BaseClass, class IdClass>
      struct registrar {
        registrar(IdClass ID);
      };
  };

/* include the actual implementation */
#include "PluginFactoryTimple.h"


}; // namespace gem


#endif /* INCLUDE_PLUGINFACTORY_H */
