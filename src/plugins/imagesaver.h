/* -----------------------------------------------------------------

GEM - Graphics Environment for Multimedia

Load an image and return the frame(OS independant parent-class)

Copyright (c) 2011 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_IMAGESAVER_H_
#define INCLUDE_IMAGESAVER_H_

#include "Base/GemPixUtil.h"

#include <string>
#include "plugins/PluginFactory.h"

#include "Base/Properties.h"


   /*-----------------------------------------------------------------
     -------------------------------------------------------------------
     CLASS
     imagesaver
    
     parent class for the system- and library-dependent imagesaver-loader classes
    
     KEYWORDS
     pix load an image
    
     DESCRIPTION

     -----------------------------------------------------------------*/
   namespace gem {
  class GEM_EXTERN imagesaver
  {
  public:
  
    //////////
    // Constructor
  
    /* initialize the imagesaverer
     */
    imagesaver(void);

    ////////
    // Destructor
    /* free what is apropriate */
    virtual ~imagesaver();


    /* read a image
     */
    /* returns TRUE if loading was successfull, FALSE otherwise */
    virtual bool save(std::string filename, imageStruct&result) = 0;


    /**
     * list all properties this backend supports
     * after calling, "readable" will hold a list of all properties that can be read
     * and "writeable" will hold a list of all properties that can be set
     * if the enumeration fails, this returns <code>false</code>
     */
    virtual bool enumProperties(gem::Properties&readable,
                                gem::Properties&writeable);

    /**
     * set a number of properties (as defined by "props")
     * the "props" may hold properties not supported by the currently opened device,
     *  which is legal; in this case the superfluous properties are simply ignored
     * this function MAY modify the props; 
     */
    virtual void setProperties(gem::Properties&props);

    /**
     * get the current value of the given properties from the device
     * if props holds properties that can not be read from the device, they are set to UNSET 
     */
    virtual void getProperties(gem::Properties&props);

  protected:
    /* used to store the "set" properties */
    gem::Properties m_properties;
    

  };

}; // namespace gem


/**
 * \fn REGISTER_IMAGESAVERFACTORY(const char *id, Class imagesaverClass)
 * registers a new class "imagesaverClass" with the imagesaver-factory
 *
 * \param id a symbolic (const char*) ID for the given class
 * \param imagesaverClass a class derived from "imagesaver"
 */
#define REGISTER_IMAGESAVERFACTORY(id, TYP) static gem::PluginFactoryRegistrar::registrar<TYP, gem::imagesaver> fac_imagesaver_ ## TYP (gensym(id)->s_name)


/**
 * \fn INIT_IMAGESAVERFACTORY()
 * initialized the factory
 * \note call this before any externals register themselves
 */
#define INIT_IMAGESAVERFACTORY()                                         \
  static gem::PluginFactoryRegistrar::dummy<gem::imagesaver> fac_imagesaverdummy


#endif	// for header file
