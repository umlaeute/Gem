/*-----------------------------------------------------------------

GEM - Graphics Environment for Multimedia

Load an digital video (like AVI, Mpeg, Quicktime) into a texture

Copyright (c) 1997-1999 Mark Danks. mark@danks.org
Copyright (c) 2002 James Tittle & Chris Clepper
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_pix_movieDarwin_H_
#define INCLUDE_pix_movieDarwin_H_

#ifdef _WINDOWS
#include "Pixes/pix_filmNT.h"
#elif __linux__
#include "Pixes/pix_filmLinux.h"
#elif __APPLE__
#include "pix_filmDarwinYUV.h"
#else
#error Define pix_film for this OS
#endif

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  pix_movie
    
  Loads in a movie
    
  KEYWORDS
  pix
    
  DESCRIPTION

  -----------------------------------------------------------------*/

class GEM_EXTERN pix_movieDarwin : public GemBase
{

    CPPEXTERN_HEADER(pix_movieDarwin, GemBase)

   
    public:
  
  //////////
  // Constructor
  pix_movieDarwin(t_symbol *filename);

 protected:
    
  //////////
  // Destructor
  virtual ~pix_movieDarwin();

  //////////
  // Do the rendering
  virtual void texFrame(GemState *state, int doit);
    	
  //////////
  virtual void setUpTextureState();


  //////////
  // create and delete buffers
  virtual void createBuffer();

  virtual void deleteBuffer();

  //////////
  // prepare for texturing (on open)
  virtual void prepareTexture();

   //////////
  // Clear the dirty flag on the pixBlock
  virtual void postrender(GemState *state);
  
  virtual void render(GemState *state);

  //////////
  virtual void startRendering();

  //////////
  // Delete texture object
  virtual void stopRendering();

  virtual void closeMess();

  //////////
  // open a movie up
  virtual void openMess(t_symbol *filename, int format);

  //////////
  // open a movie up
  virtual void realOpen(char *filename);

  //////////
  // Do the rendering
  virtual void getFrame();
  
  virtual void MovRate(float rate);
  
  virtual void changeImage(int imgNum, int trackNum);
  
  //////////
  // load film into RAM
  virtual void LoadRam();


  //-----------------------------------
  // GROUP:	Texture data
  //-----------------------------------

  //////////
  // The texture coordinates
  TexCoord    	m_coords[4];
  //////////
  // The size of the texture (so we can use sub image)
  int		m_dataSize[3];

  GLuint	m_textureObj;	
  float		m_xRatio;
  float		m_yRatio;
  
   //////////
  // the current file
  t_symbol *x_filename;
	
  //////////
  // a outlet for information like #frames and "reached end"
  t_outlet     *m_outNumFrames;
  t_outlet     *m_outEnd;
        
  //////////
  // frame data
  unsigned char*m_frame;  /* this points to the main texture (might be black) */
  unsigned char*m_data;   /* this points always to the real data */
  
  
  //////////
  // If a movie was loaded and what kind of Movie this is
  int 	    	m_haveMovie;

  int           m_auto;

  //////////
  // frame infromation
  int           m_numFrames;
  int 	    	m_reqFrame;
  int 	    	m_curFrame;

  //////////
  // track information
  int           m_numTracks;
  int           m_track;

  
  pixBlock    	m_pixBlock;
  imageStruct   m_imageStruct;
  

  int		m_xsize;
  int		m_ysize;
  int       	m_csize;


  bool      	m_film; // are we in film- or in movie-mode
  int		m_newFilm;
  int 		newImage;
  int		m_colorspace;
  int		m_format;
  int		m_rectangle;
  
    //-----------------------------------
  // GROUP:	Movie data
  //-----------------------------------
   
  GWorldPtr		m_srcGWorld;
  Rect			m_srcRect;
  TimeValue		m_movieTime;
  Track			m_movieTrack;
  TimeValue		m_timeScale;
  TimeValue		duration;
  float			durationf;
  long			movieDur, movieScale;
  int			m_hiquality;
  int			m_play;
  float			m_rate;
  Fixed			playRate;
  TimeValue 		prevTime;
  TimeValue		curTime;

private:
  Movie			m_movie;

 protected:
	
  //////////
  // static member functions
  static void openMessCallback   (void *data, t_symbol *filename);
  static void changeImageCallback(void *data, t_symbol *, int argc, t_atom *argv);
  static void autoCallback       (void *data, t_floatarg state);
  static void rateCallback       (void *data, t_floatarg state);
  static void ramCallback(void *data);
  static void hiqualityCallback(void *data, t_floatarg state);
  static void rectangleCallback(void *data, t_floatarg state);
};

#endif	// for header file
