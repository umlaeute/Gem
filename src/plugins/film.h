/* -----------------------------------------------------------------

GEM - Graphics Environment for Multimedia

Load an digital video (like AVI, Mpeg, Quicktime) into a pix block 
(OS independant parent-class)

Copyright (c) 1997-1999 Mark Danks. mark@danks.org
Copyright (c) Günther Geiger. geiger@epy.co.at
Copyright (c) 2001-2003 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_FILM_H_
#define INCLUDE_FILM_H_

#include "Gem/ExportDef.h"
#include "Gem/GemGL.h"
#include <string>

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  film
    
  parent class for the system- and library-dependent film-loader classes
    
  KEYWORDS
  pix film movie
    
  DESCRIPTION

  -----------------------------------------------------------------*/
class pixBlock;

namespace gem { namespace plugins {
class GEM_EXTERN film
{
 public:

  //////////
  // returns an instance wrapping all plugins or NULL
  // if NULL is returned, you might still try your luck with manually accessing the 
  // PluginFactory
  static film*getInstance(void);

  /////////
  // dtor must be virtual
  virtual ~film(void);

  //////////
  // open a movie up
  /* open the film "filename" (think better about URIs ?)
   * try to open the film in the colourspace requested by "format"
   * discussion: should the colourspace be only a hint or should we force it
   * (evt. by converting the actual cs by hand to the desired one)
   * more discussion: i guess the cs should really be forced somehow by [pix_film]
   * now i don't know, whether the cs-conversion should be done by [pix_film] itself or
   * rather by the film*-classes. 
   * but i guess film* makes more sense, because then, [pix_film] doesn't have to know
   * anything about the internal cs of the decoder
   */
  /* returns TRUE if loading was successfull, FALSE otherwise */
  virtual bool open(const std::string, int format=0) = 0;

  /* some error codes */
  enum errCode { SUCCESS = 0,
		 FAILURE = 1,
		 DONTKNOW= 2 };

  //////////
  // Change which image to display
  /* this is the second core function of this class:
   * most decoding-libraries can set the frame-number on a random-access basis.
   * some cannot, then this might do nothing
   * you could also switch between various tracks of a file (if the format supports it)
   * specifying trackNum as -1 means "same track as before"
   */
  virtual errCode changeImage(int imgNum, int trackNum=-1) = 0;


  //////////
  // get the next frame
  /* this is the core-function of this class !!!!
   * when called it returns the current frame in the *pixBlock structure
   * dev: you can use "m_image" for this (and "return &m_image;")
   * if the image cannot be read, returns 0
   * dev: you probably want to set the whole meta-information
   *      (xsize,ysize,csize,format) over again
   *      if you are smart and the colour-space is fine, just point 
   * if this is a "new" frame (e.g. freshly decoded),
   * pixblock.newimage should be set to 1
   */
  virtual pixBlock* getFrame(void) = 0;

  //////////
  // close the movie file
  /* close the file and clean up temporary things */
  virtual void close(void) = 0;


  //////////
  // do we have a film loaded ?
  /* returns TRUE if it is possible to read frames without any more open()
   */
  virtual bool haveFilm(void) = 0;

  ////////
  // returns true if instance can be used in thread
  virtual bool isThreadable(void) = 0;

  //////////
  // set the wanted color-space
  /* could be used for switching the colourspace on the fly 
   * normally the colour-space of a film could be set when opening a movie
   */
  virtual void requestColor(GLenum format) = 0;
  //////////
  // get the actual color-space
  /* what colour-space is in use ?
   * returns 0 for none
   */    
  virtual int getColor(void) = 0;

  //////////
  // get the number of frames
  /* the number of frames can depend on the track
   * so this will return the framenum of the current track
   */
  virtual int getFrameNum(void) = 0;

  // get the frames per seconds (or "-1" if unknown)
  virtual double getFPS(void) = 0;

  // get xsize of the frame
  virtual int getWidth(void) = 0;
  // get ysize of the frame
  virtual int getHeight(void) = 0;

  virtual void setAuto(double) = 0;
};

};}; // namespace gem::plugins


/**
 * \fn REGISTER_FILMFACTORY(const char *id, Class filmClass)
 * registers a new class "filmClass" with the film-factory
 *
 * \param id a symbolic (const char*) ID for the given class
 * \param filmClass a class derived from "film"
 */
#define REGISTER_FILMFACTORY(id, TYP) static gem::PluginFactoryRegistrar::registrar<TYP, gem::plugins::film> fac_film_ ## TYP (gensym(id)->s_name)


/**
 * \fn INIT_FILMFACTORY()
 * initialized the factory
 * \note call this before any externals register themselves
 */
#define INIT_FILMFACTORY() \
  static gem::PluginFactoryRegistrar::dummy<gem::plugins::film> fac_filmdummy


#endif	// for header file
