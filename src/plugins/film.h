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

#include "Base/GemBase.h"
#include "Base/GemPixUtil.h"

#include <string>
#include "plugins/PluginFactory.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  film
    
  parent class for the system- and library-dependent film-loader classes
    
  KEYWORDS
  pix film movie
    
  DESCRIPTION

  -----------------------------------------------------------------*/
namespace gem { namespace plugins {
class GEM_EXTERN film
{
 public:
  
  //////////
  // Constructor
  
  /* initialize the filmloader
   *
   * set the default colour-space to format (like GL_RGBA)
   * if format==0, the default is set by the filmloader
   * (for instance: the fastest colour-space)
   */
  film(GLenum format=0);

  ////////
  // Destructor
  /* free what is apropriate */
  virtual ~film();

  //////////
  // set the wanted color-space
  /* could be used for switching the colourspace on the fly 
   * normally the colour-space of a film could be set when opening a movie
   */
  virtual void requestColor(GLenum format){m_wantedFormat=format;}
  //////////
  // get the actual color-space
  /* what colour-space is in use ?
   * returns 0 for non
   */    
  virtual int getColor() {return 0;}

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
  //////////
  // close the movie fil
  /* close the file and clean up temporary things */
  virtual void close(void);

  //////////
  // do we have a film loaded ?
  /* returns TRUE if it is possible to read frames without any more open()
   */
  virtual bool haveFilm();

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
  virtual pixBlock* getFrame();

  //////////
  // get the number of frames
  /* the number of frames can depend on the track
   * so this will return the framenum of the current track
   */
  virtual int getFrameNum(){return m_numFrames;}

  // get the frames per seconds (or "-1" if unknown)
  virtual double getFPS();

  // get xsize of the frame
  virtual int getWidth() {return m_image.image.xsize;}
  // get ysize of the frame
  virtual int getHeight() {return m_image.image.ysize;}


  virtual void setAuto(t_float);

  /* some error codes */
#define FILM_ERROR_SUCCESS 0 /* no error */
#define FILM_ERROR_FAILURE 1
#define FILM_ERROR_DONTKNOW 2

  //////////
  // Change which image to display
  /* this is the second core function of this class:
   * most decoding-libraries can set the frame-number on a random-access basis.
   * some cannot, then this might do nothing
   * you could also switch between various tracks of a file (if the format supports it)
   * specifying trackNum as -1 means "same track as before"
   */
  virtual int changeImage(int imgNum, int trackNum=-1);

 protected:
  /* i guess every child-class might need (some of the) following variables  */

  /* here the frame is stored
   */
  pixBlock m_image;
  /* this is the colour-space the user requested (like GL_RGBA)
   */
  GLenum  m_wantedFormat;

  /* probably a good idea to know how many frames/tracks there are in this movie
   * the number of frames might vary between tracks, so this refers to the current track
   */
  int  m_numFrames, m_numTracks;
  /* most often we will also want to know what the current frame/track is...
   */
  int  m_curFrame, m_curTrack;

  /* if the (frame,track) is the same as the last time, 
   * we probably don't want to decode this frame again.
   * if so m_readNext should be FALSE
   */
  bool m_readNext;

  // auto increment
  t_float m_auto;

  //////////////////////
  // the frame-rate
  double m_fps;

  bool m_newfilm;
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
