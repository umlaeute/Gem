/* -----------------------------------------------------------------

GEM - Graphics Environment for Multimedia

Load an asset (like .obj oder .dxf)
(OS independent parent-class)

Copyright (c) 2001-2012 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef _INCLUDE__GEM_PLUGINS_MODELLOADER_H_
#define _INCLUDE__GEM_PLUGINS_MODELLOADER_H_

#include "Gem/ExportDef.h"
#include "Gem/VertexBuffer.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  modelloader

  parent class for the system- and library-dependent model-loader classes

  KEYWORDS
  asset model

  DESCRIPTION

  -----------------------------------------------------------------*/
#include <string>
#include <vector>
namespace gem {
  class Properties;
}
namespace gem { namespace plugins {
class GEM_EXTERN modelloader
{
 public:
  //////////
  // returns an instance wrapping all plugins or NULL
  // if NULL is returned, you might still try your luck with manually accessing the
  // PluginFactory
  static modelloader*getInstance(void);

  modelloader(void);
  /////////
  // dtor must be virtual
  virtual ~modelloader(void);

  //////////
  // open a movie up
  /* open the modelloader "filename" (think better about URIs ?)
   *
   * try to open the modelloader with the requested properties
   *
   * about properties:
   *  requestprops: are properties that can change the behaviour of how the
   *                asset is opened; examples are "rescale" (e.g. true);
   *                the backend need not implement any of the properties
   *
   *  resultprops: give feedback about the opened asset
   *               if the asset could not be opened, the content is undefined
   *               if the asset was successfully opened, the specified properties should be set to their values
   *         if a property can not be determined (e.g. variable fps), it should be set unset
   *
   */
  /* returns TRUE if loading was successful, FALSE otherwise */
  virtual bool open(const std::string&,
                    const gem::Properties&requestprops) = 0;
  //////////
  // close the asset file
  /* close the asset and clean up temporary things */
  virtual void close(void) = 0;

  ////////
  // returns true if instance can be used in thread
  virtual bool isThreadable(void) = 0;

  /**
   * list all properties the currently opened asset supports
   * if no asset is opened, this returns generic backend properties
   * which can be different from media specific properties
   * after calling, "readable" will hold a list of all properties that can be read
   * and "writeable" will hold a list of all properties that can be set
   * if the enumeration fails, this returns <code>false</code>
   */

  virtual bool enumProperties(gem::Properties&readable,
                              gem::Properties&writeable) = 0;

  /**
   * set a number of properties (as defined by "props")
   * the "props" may hold properties not supported by the currently opened media,
   *  which is legal; in this case the superfluous properties are simply ignored
   * this function MAY modify the props;
   * namely one-shot properties should be removed from the props
   *
   * examples: "smooth" 0.5
   *           "group"  3
   */
  virtual void setProperties(gem::Properties&props) = 0;

  /**
   * get the current value of the given properties from the media
   * if props holds properties that can not be read for the media, they are set to UNSET
   *
   * examples: "/textures" (number of textures)
   *           "/animations" (number of animations)
     *           "/camera/1/pos" (position of camera #1)
   */
  virtual void getProperties(gem::Properties&props) = 0;

  /**
   * data array (e.g. vertices)
   * TODO: rename to ArrayData (or similar)
   */
  class VBOarray {
    public:
    std::vector<std::vector<float> >* data;
    VertexBuffer::Type type;
  };

  /**
   * get a named vector (e.g. to pass it to VBO)
   * TODO: return VBOarray
   */
  virtual std::vector<std::vector<float> > getVector(std::string vectorName) = 0;

  /**
   * get all vectors
   * TODO: rename to getVectors()
   */
  virtual std::vector<VBOarray> getVBOarray() = 0;

  /* returns TRUE if the array data has changed (and the VBO needs refresh) */
  virtual bool needRefresh() = 0;
  /* signal the loader that we have updated our local copy of the data (clear the needRefresh() flag)
     TODO: shouldn't this be automatically called when getVBOarray() got called? */
   */
  virtual void unsetRefresh() = 0;

};

};}; // namespace gem::plugins


/**
 * \fn REGISTER_MODELLOADERFACTORY(const char *id, Class modelloaderClass)
 * registers a new class "modelloaderClass" with the modelloader-factory
 *
 * \param id a symbolic (const char*) ID for the given class
 * \param modelloaderClass a class derived from "modelloader"
 */
#define REGISTER_MODELLOADERFACTORY(id, TYP) static gem::PluginFactoryRegistrar::registrar<TYP, gem::plugins::modelloader> fac_modelloader_ ## TYP (id)

#endif	// for header file
