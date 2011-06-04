/* -----------------------------------------------------------------

GEM - Graphics Environment for Multimedia

Load an image and return the frame(OS independant parent-class)

Copyright (c) 2011 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_IMAGELOADER_H_
#define INCLUDE_IMAGELOADER_H_

#include "Base/GemPixUtil.h"

#include <string>
#include "plugins/PluginFactory.h"

#include "Base/Properties.h"


   /*-----------------------------------------------------------------
     -------------------------------------------------------------------
     CLASS
     imageloader
    
     parent class for the system- and library-dependent imageloader-loader classes
    
     KEYWORDS
     pix load an image
    
     DESCRIPTION

     -----------------------------------------------------------------*/
   namespace gem { namespace plugins {
  class GEM_EXTERN imageloader
  {
  public:
  
    //////////
    // Constructor
  
    /* initialize the imageloaderer
     */
    imageloader(void);

    ////////
    // Destructor
    /* free what is apropriate */
    virtual ~imageloader();


    /* read a image
     */
    /* returns TRUE if loading was successfull, FALSE otherwise */
    virtual bool load(std::string filename, imageStruct&result) = 0;


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

};}; // namespace gem


/**
 * \fn REGISTER_IMAGELOADERFACTORY(const char *id, Class imageloaderClass)
 * registers a new class "imageloaderClass" with the imageloader-factory
 *
 * \param id a symbolic (const char*) ID for the given class
 * \param imageloaderClass a class derived from "imageloader"
 */
#if 1
#define REGISTER_IMAGELOADERFACTORY(id, TYP) static gem::PluginFactoryRegistrar::registrar<TYP, gem::plugins::imageloader> fac_imageloader_ ## TYP (gensym(id)->s_name)
#else
#define REGISTER_IMAGELOADERFACTORY(id, TYP) static gem::PluginFactoryRegistrar::registrar<TYP, gem::plugins::imageloader> fac_imageloader_ ## TYP (id)
#endif


/**
 * \fn INIT_IMAGELOADERFACTORY()
 * initialized the factory
 * \note call this before any externals register themselves
 */
#define INIT_IMAGELOADERFACTORY()                                         \
  static gem::PluginFactoryRegistrar::dummy<gem::plugins::imageloader> fac_imageloaderdummy


#endif	// for header file
