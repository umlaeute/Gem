/*-----------------------------------------------------------------

GEM - Graphics Environment for Multimedia

Load an digital video (like AVI, Mpeg, Quicktime) into a pix block
(OS independent parent-class)

Copyright (c) 1997-1999 Mark Danks. mark@danks.org
Copyright (c) Günther Geiger. geiger@epy.co.at
Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef _INCLUDE__GEM_PIXES_PIX_FILM_H_
#define _INCLUDE__GEM_PIXES_PIX_FILM_H_
#include "Base/GemBase.h"

#ifdef HAVE_PTHREADS
# include <pthread.h>
#endif

#include "plugins/film.h"


/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  pix_film

  Loads in a movie

  KEYWORDS
  pix

  DESCRIPTION

  -----------------------------------------------------------------*/
class GEM_EXTERN pix_film : public GemBase
{
  CPPEXTERN_HEADER(pix_film, GemBase);

public:
  //////////
  // Constructor
  pix_film(t_symbol* filename);

protected:

  //////////
  // Destructor
  virtual ~pix_film();

  //////////
  // close the movie file
  virtual void closeMess(void);
  //////////
  // open a movie up (with a given format and a preferred codec)
  virtual void openMess(std::string filename, int format, std::string codec);
  virtual void openMess(std::string filename, int format,
                        unsigned int codec);

  //////////
  // prepare for texturing (on open)
  virtual void prepareTexture(void) {}

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

  //////////
  // turn on/off threaded reading
  virtual void threadMess(int);

  //////////
  // automatic frame increment
  virtual void autoMess(double state);

  //////////
  // (re)query info from loaded film
  virtual void bangMess();

  //////////
  // which backend to use (exclusively)
  std::vector<std::string>m_backends;
  virtual void backendMess(t_symbol*,int,t_atom*);
  virtual void backendMess(const std::string&);
  virtual void backendMess(int);



  //-----------------------------------
  // GROUP:     Movie data
  //-----------------------------------

  //////////
  // If a movie was loaded and what kind of Movie this is
  int           m_haveMovie;
  t_float       m_auto;
  int           m_format;

  //////////
  // frame information
  int           m_numFrames;
  t_float       m_reqFrame;
  int           m_curFrame;

  //////////
  // track information
  int           m_numTracks;
  int           m_reqTrack;
  int           m_curTrack;

  std::vector<std::string>m_ids;
  gem::plugins::film*m_handle;

  //////////
  // a outlet for information like #frames and "reached end"
  t_outlet     *m_outNumFrames;
  t_outlet     *m_outEnd;


protected:
  /* grab-thread */
#ifdef HAVE_PTHREADS
  pthread_t m_thread_id;
  pthread_mutex_t *m_mutex;
  pthread_cond_t   m_runcondition;
  static void*grabThread(void*);

  pixBlock*m_frame;

  bool m_thread_continue;
#endif
  /* do we have a thread ? */
  bool m_thread_running;

  /* does the user request reading to be threaded */
  bool m_wantThread;

  //////////
  // static member functions
  static void openMessCallback   (void *data, t_symbol*,int,t_atom*);
  static void changeImageCallback(void *data, t_symbol*, int argc,
                                  t_atom *argv);
};

#endif  // for header file
