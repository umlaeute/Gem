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

#include "Gem/ExportDef.h"
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
  static const int INVALID_CONTEXT;
  virtual int getCurContext(void);
};


template<class GemContextDataType = int>
  class GEM_EXTERN GemContextData : GemContextDataBase
  {
    private:
    public:
   
    //////////
    // Constructor
    GemContextData(void) : m_haveDefaultValue(false) {;}

    GemContextData(GemContextDataType v) : m_haveDefaultValue(true), m_defaultValue(v) {;}

    virtual ~GemContextData() {
      m_ContextDataVector.clear();
    }
    	
    /**
     * returns the context-specific value
     *
     * @usage GemContextData<GLenum>m_fun; m_fun=GL_FUNC_ADD;
     *
     * @pre We are in a draw process.
     * @note Should only be called from the draw function.
     *        Results are un-defined for other functions.
     */
    virtual operator GemContextDataType()
    {
      return (*getPtrToCur());
    }

    /**
     * assigns a value to the correct context
     *
     * @pre We are in a draw process.
     * @note Should only be called from the draw function.
     *       Results are un-defined for other functions.
     */
    virtual GemContextDataType&operator = (GemContextDataType value)
    {
      /* simplistic approach to handle out-of-context assignments:
       *  assign the value to all context instances
       */
      if(INVALID_CONTEXT==getCurContext()) {
        doSetAll(value);
      }

      return (*getPtrToCur()=value);
    }

    private:
    bool m_haveDefaultValue;
    GemContextDataType m_defaultValue;
    std::vector<GemContextDataType*>  m_ContextDataVector;


    /* Makes sure that the vector is at least requiredSize large */
    void checkSize(unsigned int requiredSize)
    {
      if(requiredSize > m_ContextDataVector.size())
        {
          m_ContextDataVector.reserve(requiredSize);          // Resize smartly
          while(m_ContextDataVector.size() < requiredSize)    // Add any new items needed
            {
                if(m_haveDefaultValue) {
                    m_ContextDataVector.push_back(new GemContextDataType(m_defaultValue));
                } else {
                    m_ContextDataVector.push_back(new GemContextDataType);
                }
            }
        }
    }

    /**
     * Returns a pointer to the correct data element in the current context.
     *
     * @pre We are in the draw function.
     * @post Synchronized.
     * @note ASSERT: Same context is rendered by same thread each time.
     */
    GemContextDataType* getPtrToCur(void)
    {
      // Get current context
      int context_id = getCurContext();
      // Cache ref for better performance
      checkSize(context_id+1);     // Make sure we are large enough (+1 since we have index)

      return m_ContextDataVector[context_id];
    }

    void doSetAll(GemContextDataType v)
    {
      unsigned int i=0;
      for(i=0; i< m_ContextDataVector.size(); i++) {
        *m_ContextDataVector[i]=v;
      }
    }
  };



#endif	// for header file
