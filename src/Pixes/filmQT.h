/*-----------------------------------------------------------------

GEM - Graphics Environment for Multimedia

Load an digital video (like AVI, Mpeg, Quicktime) into a pix block (Linux)

Copyright (c) 1997-1999 Mark Danks. mark@danks.org
Copyright (c) 2002 James Tittle.  tigital@mac.com
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.


-----------------------------------------------------------------*/
#ifndef INCLUDE_FILMQT_H_
#define INCLUDE_FILMQT_H_

#include "Pixes/film.h"


#ifdef __APPLE__
# include <Carbon/carbon.h>
# include <QuickTime/quicktime.h>
# ifndef HAVE_QUICKTIME
#  define HAVE_QUICKTIME
# endif
#elif defined _WINDOWS && defined (HAVE_QUICKTIME)
# include <QTML.h>
# include <Movies.h>
#endif

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  filmQT
  
  Loads in a film
  
  KEYWORDS
  pix
  
  DESCRIPTION

  -----------------------------------------------------------------*/
class GEM_EXTERN filmQT : public film
{
 public:
  //////////
  // Constructor
  filmQT(int format=0);
  //////////
  // Destructor
  virtual ~filmQT();

#ifdef HAVE_QUICKTIME
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

 protected:
  //-----------------------------------
  // GROUP:	Movie data
  //-----------------------------------
  Movie			m_movie; 
  GWorldPtr		m_srcGWorld;
  TimeValue		m_movieTime;
  Track			m_movieTrack;
  TimeValue		m_timeScale;
  TimeValue		duration;
#endif // HAVE_QT

  bool			m_bInit;

  t_float       m_auto;
  int           m_reqFrame;
};

#endif	// for header file
