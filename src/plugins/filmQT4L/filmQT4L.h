/*-----------------------------------------------------------------

GEM - Graphics Environment for Multimedia

Load a digital video (quicktime4linux) for linux

Copyright (c) 1997-1999 Mark Danks. mark@danks.org
Copyright (c) Günther Geiger. geiger@epy.co.at
Copyright (c) 2001-2003 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.


-----------------------------------------------------------------*/

#ifndef INCLUDE_FILMQT4L_H_
#define INCLUDE_FILMQT4L_H_
#include "plugins/film.h"
#include <stdio.h>

#ifdef HAVE_LIBQUICKTIME
#include <quicktime.h>
#include <colormodels.h>
#endif // QT

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  filmQT4L1
  
  Loads in a film
  
  KEYWORDS
  pix
  
  DESCRIPTION

  -----------------------------------------------------------------*/
namespace gem { namespace plugins {
class GEM_EXPORT filmQT4L : public gem::plugins::film {
 public:

  //////////
  // Constructor
  filmQT4L(int format=0);

  //////////
  // Destructor
  virtual ~filmQT4L();

#ifdef HAVE_LIBQUICKTIME
  //////////
  // open a movie up
  virtual bool open(const std::string filename, int format = 0);
  //////////
  // close the movie file
  virtual void close(void);

  //////////
  // get the next frame
  virtual pixBlock* getFrame();

  //////////
  // set the next frame to read;
  virtual int changeImage(int imgNum, int trackNum = -1);

  //-----------------------------------
  // GROUP:	Movie data
  //-----------------------------------
 protected:

  quicktime_t  *m_quickfile;
  int           m_qtformat;

  imageStruct   m_qtimage;
#endif
  int m_lastFrame;

};};};

#endif	// for header file
