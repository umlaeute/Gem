/*-----------------------------------------------------------------

GEM - Graphics Environment for Multimedia

Load an digital video (like AVI, Mpeg, Quicktime) into a pix block (Linux)

Copyright (c) 1997-1999 Mark Danks. mark@danks.org
Copyright (c) Günther Geiger. geiger@epy.co.at
Copyright (c) 2001-2003 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.


-----------------------------------------------------------------*/

#ifndef INCLUDE_FILMMPEG1_H_
#define INCLUDE_FILMMPEG1_H_

#include "Pixes/film.h"
#include <stdio.h>

#ifdef HAVE_LIBMPEG
#include <mpeg.h>
#endif // MPEG

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  filmMPEG1
  
  Loads in a film
  
  KEYWORDS
  pix
  
  DESCRIPTION

  -----------------------------------------------------------------*/
class GEM_EXTERN filmMPEG1 : public film {
 public:

  //////////
  // Constructor
  filmMPEG1(int format=0);

  //////////
  // Destructor
  virtual ~filmMPEG1();

#ifdef HAVE_LIBMPEG
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
  // get the number of frames
  virtual int getFrameNum() {return -1;}

  //////////
  // set the next frame to read;
  virtual int changeImage(int imgNum, int trackNum = -1);

  //-----------------------------------
  // GROUP:	Movie data
  //-----------------------------------
 protected:

  FILE         *m_streamfile;
  ImageDesc     m_streamVid;
  bool          m_reachedEnd;
#endif
  unsigned char*m_data;
  int           m_length;
};

#endif	// for header file
