/* -----------------------------------------------------------------

GEM - Graphics Environment for Multimedia

Load an digital video (like AVI, Mpeg, Quicktime) into a pix block 
(OS independant parent-class)

Copyright (c) 2010-2011 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_PLUGINS_RECORD_H_
#define INCLUDE_PLUGINS_RECORD_H_

#include "Gem/Image.h"
#include "Gem/Properties.h"
#include "plugins/PluginFactory.h"

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
 protected:

  //////////
  // compress and write the next frame
  /* this is the core-function of this class !!!!
   * when called it returns something depending on success
   * (what? the framenumber and -1 (0?) on failure?)
   */
  virtual bool putFrame(imageStruct*)=0;

  //////////
  // open a movie up
  /* open the record "filename" (think better about URIs ?)
   */
  /* returns TRUE if opening was successfull, FALSE otherwise */
  virtual bool open(const std::string filename);
  //////////
  // close the movie file
  /* stop recording, close the file and clean up temporary things */
  virtual void close(void);

public:


  //////////
  // popup a dialog to set the codec interactively (interesting on os-x and w32)
  virtual bool dialog();

  /**
   * get a list of supported codecs (short-form names, e.g. "mjpa")
   */ 
  virtual std::vector<std::string>getCodecs(void);
  /**
   * get a human readable description of the given codec (e.g. "Motion Jpeg A")
   */
  virtual const std::string getCodecDescription(const std::string codecname);
  /**
   * set the current codec
   */
  virtual bool setCodec(const std::string name);

  /**
   * list all properties the currently selected codec supports
   * if the enumeration fails, this returns <code>false</code>
   */
  virtual bool enumProperties(gem::Properties&props);


 public:
  
  //////////
  // Constructor
  
  /* initialize the recordloader
   */
  record(void);

  ////////
  // Destructor
  /* free what is apropriate */
  virtual ~record();

  //////////
  // start/stop recording
  /* these are the handles for pix_record to open/close 
   * returns TRUE if opening was successfull, FALSE otherwise */
  bool start(const std::string filename, gem::Properties&props);
  void stop (void);
  //////////
  // record a frame (wrapper around putFrame()
  bool write(imageStruct*);

 protected:
  // map codec-names to codec-descriptions
  std::map<std::string, std::string>m_codecdescriptions;
  // write properties
  gem::Properties m_props;

 private:
  class PIMPL;
  PIMPL*m_pimpl;
 };
}; };


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
