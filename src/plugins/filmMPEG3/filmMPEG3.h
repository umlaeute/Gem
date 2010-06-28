/*-----------------------------------------------------------------

GEM - Graphics Environment for Multimedia

Load an digital video (like AVI, Mpeg, Quicktime) into a pix block (Linux)

Copyright (c) 1997-1999 Mark Danks. mark@danks.org
Copyright (c) Günther Geiger. geiger@epy.co.at
Copyright (c) 2001-2003 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.


-----------------------------------------------------------------*/

#ifndef INCLUDE_FILMMPEG3_H_
#define INCLUDE_FILMMPEG3_H_
#include "plugins/film.h"
#include <stdio.h>
#ifdef HAVE_LIBMPEG3
#include <libmpeg3.h>
#endif // MPEG3

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  filmMPEG3
  
  Loads in a film
  
  KEYWORDS
  pix
  
  DESCRIPTION

  -----------------------------------------------------------------*/
namespace gem { class GEM_EXPORT filmMPEG3 : public film {
 public:

  //////////
  // Constructor
  filmMPEG3(int format=0);

  //////////
  // Destructor
  virtual ~filmMPEG3();

#ifdef HAVE_LIBMPEG3
  //////////
  // open a movie up
  virtual bool open(char *filename, int format = 0);
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

  mpeg3_t      *mpeg_file;
#endif
};};

#endif	// for header file
