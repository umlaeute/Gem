/*-----------------------------------------------------------------

GEM - Graphics Environment for Multimedia

Load an digital video (like AVI, Mpeg, Quicktime) into a pix block
(OS independant parent-class)

Copyright (c) 1997-1999 Mark Danks. mark@danks.org
Copyright (c) Günther Geiger. geiger@epy.co.at
Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_PIX_FILMNEW_H_
#define INCLUDE_PIX_FILMNEW_H_

#define MAX_FILM_HANDLES 8

#define GEM_MOVIE_NONE 0
#define GEM_MOVIE_AVI  1
#define GEM_MOVIE_MPG  2
#define GEM_MOVIE_MOV  3

#include <string.h>
#include <stdio.h>

#include "Base/GemBase.h"
#include "Base/GemPixUtil.h"

#ifdef __linux__
#include "Base/config.h"
#endif /* __linux__ */
#include "Pixes/film.h"


/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  pix_filmNEW
    
  Loads in a movie
    
  KEYWORDS
  pix
    
  DESCRIPTION

  -----------------------------------------------------------------*/
class GEM_EXTERN pix_filmNEW : public GemBase
{
  CPPEXTERN_HEADER(pix_filmNEW, GemBase)
    
    public:  
  //////////
  // Constructor
  pix_filmNEW(t_symbol *filename);
  //  pix_filmNEW();

 protected:
    
  //////////
  // Destructor
  virtual ~pix_filmNEW();

  //////////
  // close the movie file
  virtual void closeMess(void);
  //////////
  // open a movie up
  virtual void openMess(t_symbol *filename, int format=0);

  //////////
  // prepare for texturing (on open)
  virtual void prepareTexture() {}

  //////////
  // Do the rendering
  virtual void render(GemState *state);

  //////////
  // Clear the dirty flag on the pixBlock
  virtual void postrender(GemState *state);

  //////////
  // Change which image to display
  virtual void changeImage(int imgNum, int trackNum);
	
  //////////
  // Change the colorspace
  // normally the cs-change will be passed through immediately.
  // however, when we want to do an openMess this will be called first and we don't
  // want to change the cs before the actual opening is done
  virtual void csMess(t_symbol*s, bool immediately=true);

  //-----------------------------------
  // GROUP:	Movie data
  //-----------------------------------

  //////////
  // If a movie was loaded and what kind of Movie this is
  int 	    	m_haveMovie;
  t_float       m_auto;
  int           m_format;

  //////////
  // frame infromation
  int           m_numFrames;
  t_float   	m_reqFrame;
  int 	    	m_curFrame;

  //////////
  // track information
  int           m_numTracks;
  int           m_track;

  //////////
  // frame data
  bool          m_film; // are we in film- or in movie-mode

  film         *m_handles[MAX_FILM_HANDLES];
  film         *m_handle;
  int           m_numHandles;

  //////////
  // a outlet for information like #frames and "reached end"
  t_outlet     *m_outNumFrames;
  t_outlet     *m_outEnd;

 protected:
	
  //////////
  // static member functions
  static void openMessCallback   (void *data, t_symbol*,int,t_atom*);
  static void changeImageCallback(void *data, t_symbol *, int argc, t_atom *argv);
  static void autoCallback       (void *data, t_floatarg state);
  static void csCallback         (void *data, t_symbol*s);
};

#endif	// for header file
