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
#ifndef _WINDOWS
#include "Base/config.h"
#endif
/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  film
    
  parent class for the system- and library-dependent film-loader classes
    
  KEYWORDS
  pix film movie
    
  DESCRIPTION

  -----------------------------------------------------------------*/
class GEM_EXTERN film
{
 public:
  
  //////////
  // Constructor
  film(int format=0);
  ~film();

  //////////
  // set the wanted color-space
  virtual void requestColor(int){}
  //////////
  // get the actual color-space
  virtual int getColor() {return 0;}

  //////////
  // open a movie up
  virtual bool open(char *filename, int format=0);
  //////////
  // close the movie file
  virtual void close(void);

  //////////
  // do we have a film loaded ?
  //
  virtual bool haveFilm();

  //////////
  // get the next frame
  virtual pixBlock* getFrame();

  //////////
  // get the number of frames
  virtual int getFrameNum(){return m_numFrames;}
  // get xsize
  virtual int getWidth() {return m_image.image.xsize;}
  // get ysize
  virtual int getHeight() {return m_image.image.ysize;}
		
  //////////
  // Change which image to display
#define FILM_ERROR_SUCCESS 0
#define FILM_ERROR_FAILURE 1
#define FILM_ERROR_DONTKNOW 2
  virtual int changeImage(int imgNum, int trackNum=-1);

 protected:
  pixBlock m_image;
  int  m_wantedFormat;
  int  m_numFrames, m_numTracks;
  int  m_curFrame, m_curTrack;
  bool m_readNext;
};

#endif	// for header file
