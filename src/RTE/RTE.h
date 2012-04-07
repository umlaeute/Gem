/*-----------------------------------------------------------------
  LOG
  GEM - Graphics Environment for Multimedia

  generic access to RTE

  Copyright (c) 2012 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

  -----------------------------------------------------------------*/
#ifndef _INCLUDE__GEM_RTE_RTE_H_
#define _INCLUDE__GEM_RTE_RTE_H_


#include "Gem/ExportDef.h"
#include <string>

namespace gem {
  namespace RTE {
    class GEM_EXTERN RTE {
    private:
      class PIMPL;
      PIMPL*m_pimpl;

      RTE(void);
      virtual ~RTE(void);

    public:
      static RTE*getRuntimeEnvironment(void);

      /**
       * get the Runtime Environemnt's version
       */
      virtual const std::string getVersion(unsigned int&major, unsigned int&minor);

      /**
       * get the Runtime Environemnt's name
       */
      virtual const std::string getName(void);

      /**
       * resolve a function-name in the current RTE
       * @return pointer to the function named 'name', or NULL if it doesn't exist
       */
      virtual void*getFunction(const std::string&name);
    };
  };
};
#endif /* _INCLUDE__GEM_RTE_RTE_H_ */
