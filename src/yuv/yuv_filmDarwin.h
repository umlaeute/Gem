/*-----------------------------------------------------------------

GEM - Graphics Environment for Multimedia

Load an digital video (like AVI, Mpeg, Quicktime) into a pix block (Linux)

Copyright (c) 1997-1999 Mark Danks. mark@danks.org
Copyright (c) 2002 James Tittle & Chris Clepper
tigital@mac.com & cclep@artic.edu

For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.


-----------------------------------------------------------------*/
#ifndef INCLUDE_YUV_FILMDARWIN_H_
#define INCLUDE_YUV_FILMDARWIN_H_

#include "yuv_film.h"

#include <Carbon/carbon.h>
#include <QuickTime/quicktime.h>

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  yuv_filmDarwin
  
  Loads in a film
  
  KEYWORDS
  pix
  
  DESCRIPTION

  -----------------------------------------------------------------*/
class GEM_EXTERN yuv_filmDarwin : public yuv_film
{
  CPPEXTERN_HEADER(yuv_filmDarwin, GemBase)

    public:

  //////////
  // Constructor
  yuv_filmDarwin(t_symbol *filename);

 protected:

  //////////
  // Destructor
  virtual ~yuv_filmDarwin();

  //////////
  // close the movie file
  virtual void closeMess(void);
  //////////
  // open a movie up
  virtual void realOpen(char *filename);

  //////////
  // Do the rendering
  virtual void getFrame();
	
  //-----------------------------------
  // GROUP:	Movie data
  //-----------------------------------
  Movie			m_movie;
  GWorldPtr		m_srcGWorld;
  TimeValue		m_movieTime;
  //-----------------------------------
  // GROUP:	Texturing
  //-----------------------------------

 protected:
	
  //////////
  // static member functions
  static void openMessCallback   (void *data, t_symbol *filename);
  static void changeImageCallback(void *data, t_symbol *, int argc, t_atom *argv);
  static void autoCallback(void *data, t_floatarg state);

};

#endif	// for header file
