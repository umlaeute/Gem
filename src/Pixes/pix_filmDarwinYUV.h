/*-----------------------------------------------------------------

GEM - Graphics Environment for Multimedia

Load an digital video (like AVI, Mpeg, Quicktime) into a pix block (Linux)

Copyright (c) 1997-1999 Mark Danks. mark@danks.org
Copyright (c) 2002 James Tittle & Chris Clepper
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.


-----------------------------------------------------------------*/
#ifndef INCLUDE_PIX_FILMDARWINYUV_H_
#define INCLUDE_PIX_FILMDARWINYUV_H_

#include "pix_filmYUV.h"

#include <Carbon/carbon.h>
#include <QuickTime/quicktime.h>

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  pix_filmdarwinYUV
  
  Loads in a film
  
  KEYWORDS
  pix
  
  DESCRIPTION

  -----------------------------------------------------------------*/
class GEM_EXTERN pix_filmDarwinYUV : public pix_filmYUV
{
  CPPEXTERN_HEADER(pix_filmDarwinYUV, GemBase)

    public:

  //////////
  // Constructor
  pix_filmDarwinYUV(t_symbol *filename);

 protected:

  //////////
  // Destructor
  virtual ~pix_filmDarwinYUV();

  //////////
  // close the movie file
  virtual void closeMess(void);
  //////////
  // open a movie up
  virtual void realOpen(char *filename);

  //////////
  // Do the rendering
  virtual void getFrame();
  
  //////////
  // load film into RAM
  virtual void LoadRam();
	
  //-----------------------------------
  // GROUP:	Movie data
  //-----------------------------------
  Movie			m_movie;
  GWorldPtr		m_srcGWorld;
  TimeValue		m_movieTime;

  Track			m_movieTrack;
  TimeValue		m_timeScale;
  TimeValue		duration;
 float			durationf;

  //-----------------------------------
  // GROUP:	Texturing
  //-----------------------------------

 protected:
	
  //////////
  // static member functions
  static void openMessCallback   (void *data, t_symbol *filename);
  static void changeImageCallback(void *data, t_symbol *, int argc, t_atom *argv);
  static void autoCallback(void *data, t_floatarg state);
  static void ramCallback(void *data);

};

#endif	// for header file
