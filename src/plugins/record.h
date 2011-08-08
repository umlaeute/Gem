/* -----------------------------------------------------------------

GEM - Graphics Environment for Multimedia

Load an digital video (like AVI, Mpeg, Quicktime) into a pix block 
(OS independant interface)

Copyright (c) 2010-2011 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_PLUGINS_RECORD_H_
#define INCLUDE_PLUGINS_RECORD_H_

#include "Gem/Image.h"
#include "Gem/Properties.h"
#include <string>


/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  record
    
  parent class for the system- and library-dependent record-loader classes
    
  KEYWORDS
  pix record movie
    
  DESCRIPTION

  -----------------------------------------------------------------*/
namespace gem { namespace plugins {
 class GEM_EXTERN record
{
public:
  //////////
  // start recording
  /* 
   * returns TRUE if opening was successfull, FALSE otherwise 
   */
  virtual bool start(const std::string filename, gem::Properties&props) = 0;

  //////////
  // stop recording
  virtual void stop (void) = 0;

  //////////
  // record a frame 
  virtual bool write(imageStruct*) = 0;

  /**
   * get a list of supported codecs (short-form names, e.g. "mjpa")
   */ 
  virtual std::vector<std::string>getCodecs(void) = 0;
  /**
   * get a human readable description of the given codec (e.g. "Motion Jpeg A")
   */
  virtual const std::string getCodecDescription(const std::string codecname) = 0;
  /**
   * set the current codec
   */
  virtual bool setCodec(const std::string name) = 0;

  /**
   * list all properties the currently selected codec supports
   * if the enumeration fails, this returns <code>false</code>
   */
  virtual bool enumProperties(gem::Properties&props) = 0;

  //////////
  // popup a dialog to set the codec interactively (interesting on os-x and w32)
  virtual bool dialog(void) = 0;


  //////////
  // returns an instance wrapping all plugins or NULL
  // if NULL is returned, you might still try your luck with manually accessing the 
  // PluginFactory
  static record*getInstance(void);
 };
}; };



/* 
 * factory code:
 * to use these macros, you have to include "plugins/PluginFactory.h"
 */

/**
 * \fn REGISTER_RECORDFACTORY(const char *id, Class recordClass)
 * registers a new class "recordClass" with the record-factory
 *
 * \param id a symbolic (const char*) ID for the given class
 * \param recordClass a class derived from "record"
 */
#define REGISTER_RECORDFACTORY(id, TYP) static gem::PluginFactoryRegistrar::registrar<TYP, gem::plugins::record> fac_record_ ## TYP (gensym(id)->s_name)


/**
 * \fn INIT_RECORDFACTORY()
 * initialize the factory
 * \note call this before any externals register themselves
 */
#define INIT_RECORDFACTORY() \
  static gem::PluginFactoryRegistrar::dummy<gem::plugins::record> fac_recorddummy



#endif	// for header file
