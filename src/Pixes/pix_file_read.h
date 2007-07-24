/*-----------------------------------------------------------------

    GEM - Graphics Environment for Multimedia

    Interface to the VideoIO framework
    this object is for video file reading
    
    Copyright (c) 2007 Thomas Holzmann
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

    -----------------------------------------------------------------*/

#ifndef INCLUDE_PIX_FILEREAD_H_
#define INCLUDE_PIX_FILEREAD_H_
#include "Base/config.h"

#define MAX_FILM_HANDLES 8

#include <stdio.h>

#include "Base/GemBase.h"
#include "Base/GemPixUtil.h"
#include "../videoIO/VIOKernel.h"
#include "../videoIO/VIOException.h"

  /* some error codes */
#define FILM_ERROR_SUCCESS 0 /* no error */
#define FILM_ERROR_FAILURE 1
#define FILM_ERROR_DONTKNOW 2

/*-----------------------------------------------------------------
    
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
    virtual void csMess(t_symbol*s);
    
    /////////
    // changes the autoincrement of the video
    virtual void autoMess(t_floatarg state);

  //-----------------------------------
  // GROUP:	Movie data
  //-----------------------------------

    //////////
    // a outlet for information like #frames and "reached end"
    t_outlet     *m_outNumFrames;
    t_outlet     *m_outEnd;

    // the file reader
    VideoIO_::FileRead *fileReader;
    /// TODO kernel should be used only once
    VideoIO_::VIOKernel m_kernel;
    
    // here the frame is stored
    pixBlock m_image;
    // true if we loaded a new film
    bool m_newfilm;
 
  protected:
    //////////
    // static member functions
    static void openMessCallback   (void *data, t_symbol*,int,t_atom*);
    static void changeImageCallback(void *data, t_symbol *, int argc, t_atom *argv);
    static void autoCallback       (void *data, t_floatarg state);
    static void csCallback         (void *data, t_symbol*s);
};

#endif	// for header file
