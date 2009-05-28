/*-----------------------------------------------------------------
  LOG
  GEM - Graphics Environment for Multimedia

  data specific to a rendering context

  Copyright (c) 2009 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

  -----------------------------------------------------------------*/

#ifndef INCLUDE_GEMCONTEXTDATA_H_
#define INCLUDE_GEMCONTEXTDATA_H_

#include "Base/config.h"
#include "Base/GemExportDef.h"
#include <vector>

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  GemContextData
    
  rendering context specific data
  this is heavily inspired by VrJuggler

  DESCRIPTION

  several things in openGL like display-lists are context dependent
  if we have multiple contexts, such values most be generated for each context
  GemContextData provides a generic (templated) datatype for this

  LATER (SOONER) think about splitting the render() into a context-specific section that 
  set's up display-lists,... and a draw() function that just calls the pre-generated values
    
  -----------------------------------------------------------------*/

class GEM_EXTERN GemContextDataBase {
 protected:
  int getCurContext(void);
};


template<class T = int>
  class GEM_EXTERN GemContextData : GemContextDataBase
  {
    public:
   
    //////////
    // Constructor
    GemContextData(void) {;}

    ~GemContextData() {
      m_ContextDataVector.clear();
    }
    	
    /**
     * Returns reference to user data for the current context.
     *
     * @pre We are in a draw process.
     * @note Should only be called from the draw function.
     *        Results are un-defined for other functions.
     */
    T& operator*()
    {
      return (*getPtrToCur());
    }

    /**
     * Returns reference to user data for the current context.
     *
     * @pre We are in a draw process.
     * @note Should only be called from the draw function.
     *       Results are un-defined for other functions.
     */
    T* operator->()
    {
      return getPtrToCur();
    }

    private:
    /* Makes sure that the vector is at least requiredSize large */
    void checkSize(unsigned int requiredSize)
    {
      if(requiredSize > m_ContextDataVector.size())
        {
          m_ContextDataVector.reserve(requiredSize);          // Resize smartly
          while(m_ContextDataVector.size() < requiredSize)    // Add any new items needed
            {
              m_ContextDataVector.push_back(new T());
            }
        }
    }

    std::vector<T*>  m_ContextDataVector;

    /**
     * Returns a pointer to the correct data element in the current context.
     *
     * @pre We are in the draw function.
     * @post Synchronized.
     * @note ASSERT: Same context is rendered by same thread each time.
     */
    T* getPtrToCur(void)
    {
      // Get current context
      int context_id = getCurContext();
      // Cache ref for better performance
      checkSize(context_id+1);     // Make sure we are large enough (+1 since we have index)

      return m_ContextDataVector[context_id];
    }
  };



#endif	// for header file
