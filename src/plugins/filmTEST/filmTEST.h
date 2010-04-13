/*-----------------------------------------------------------------

GEM - Graphics Environment for Multimedia

Load an digital video (like AVI, Mpeg, Quicktime) into a pix block (Linux)

Copyright (c) 1997-1999 Mark Danks. mark@danks.org
Copyright (c) Günther Geiger. geiger@epy.co.at
Copyright (c) 2001-2003 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.


-----------------------------------------------------------------*/

#ifndef INCLUDE_FILMTEST_H_
#define INCLUDE_FILMTEST_H_
#include "plugins/film.h"
#include <stdio.h>

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  filmTEST
  
  Loads in a film
  
  KEYWORDS
  pix
  
  DESCRIPTION

  -----------------------------------------------------------------*/
namespace gem { class GEM_EXTERN filmTEST : public film {
 public:

  //////////
  // Constructor
  filmTEST(int format=0);

  //////////
  // open a movie up
  virtual bool open(char *filename, int format = 0);

  //////////
  // get the next frame
#ifdef TEST_GETFRAME
  virtual pixBlock* getFrame();
#endif

  //////////
  // set the next frame to read;
  virtual int changeImage(int imgNum, int trackNum = -1);

  //-----------------------------------
  unsigned char*m_data;
  int           m_length;
};};

#endif	// for header file
