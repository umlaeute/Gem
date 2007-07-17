/*-----------------------------------------------------------------

    GEM - Graphics Environment for Multimedia

    Load an digital video (like AVI, Mpeg, Quicktime) into a pix block
    (OS independant parent-class)

    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyright (c) Gnther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::fr::uml�te. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

    -----------------------------------------------------------------*/

#ifndef INCLUDE_PIX_FILEREAD_H_
#define INCLUDE_PIX_FILEREAD_H_
#include "Base/config.h"

    /*
#ifndef VIDEO_IO
# define NO_AUTO_REGISTER_CLASS
#else
# define DO_AUTO_REGISTER_CLASS
#endif
*/
#define MAX_FILM_HANDLES 8

#include <string.h>
#include <stdio.h>

#include "Base/GemBase.h"
#include "Base/GemPixUtil.h"
#include "../videoIO/VIOKernel.h"

#ifdef HAVE_PTHREADS
# include <pthread.h>
#endif

  /* some error codes */
#define FILM_ERROR_SUCCESS 0 /* no error */
#define FILM_ERROR_FAILURE 1
#define FILM_ERROR_DONTKNOW 2

/*-----------------------------------------------------------------
    -------------------------------------------------------------------
    CLASS
    pix_file_read
    
    Loads in a movie
    
    KEYWORDS
    pix
    
    DESCRIPTION

    -----------------------------------------------------------------*/
    class GEM_EXTERN pix_file_read : public GemBase
{
  CPPEXTERN_HEADER(pix_file_read, GemBase)
    
  public:  
    //////////
  // Constructor
    pix_file_read(t_symbol *filename);


  protected:
    
    //////////
  // Destructor
    virtual ~pix_file_read();

    //////////
  // close the movie file
    virtual void closeMess(void);
    //////////
  // open a movie up (with a given format and a preferred codec)
    virtual void openMess(t_symbol *filename, int format=0, int codec=-1);

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

    //////////
  // turn on/off threaded reading
    virtual void threadMess(int);

  //-----------------------------------
  // GROUP:	Movie data
  //-----------------------------------

    //////////
  // If a movie was loaded and what kind of Movie this is
//  int 	    	m_haveMovie;
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
    int           m_reqTrack;
    int           m_curTrack;



    //////////
  // a outlet for information like #frames and "reached end"
    t_outlet     *m_outNumFrames;
    t_outlet     *m_outEnd;



    VideoIO_::FileRead *fileReader;
    VideoIO_::VIOKernel m_kernel;
 
  protected:
    /* grab-thread */
#ifdef HAVE_PTHREADS
    pthread_t m_thread_id;
    pthread_mutex_t *m_mutex;
    static void *grabThread(void*);

    pixBlock *m_frame;

    int m_thread_continue;
#endif
    /* do we have a thread ? */
    int m_thread_running;

    /* does the user request reading to be threaded */
    int m_wantThread;
	
    //////////
  // static member functions
    static void openMessCallback   (void *data, t_symbol*,int,t_atom*);
    static void changeImageCallback(void *data, t_symbol *, int argc, t_atom *argv);
    static void autoCallback       (void *data, t_floatarg state);
    static void csCallback         (void *data, t_symbol*s);
    static void threadCallback     (void *data, t_floatarg state);
};

#endif	// for header file
