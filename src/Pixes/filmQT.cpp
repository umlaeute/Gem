////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// tigital@mac.com
//
// Implementation file 
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) 2002 James Tittle & Chris Clepper
//    Copyright (c) 2003 IOhannes m zmoelnig. forum::für::umläute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
#include "filmQT.h"

#ifdef HAVE_QUICKTIME
# ifdef __APPLE__
#  include "OpenGL/glext.h"
# else
#  include "TextUtils.h"
# endif
#endif

/////////////////////////////////////////////////////////
//
// filmQT
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////

filmQT :: filmQT(int format) : film(format)
			     , m_bInit(false){
  static bool first_time=true;
  if (first_time) {
#ifdef HAVE_QUICKTIME
    post("pix_film:: quicktime support");
#endif
    first_time = false;
  }
#ifdef HAVE_QUICKTIME
#ifdef _WINDOWS
  OSErr		err = noErr;

  // Initialize QuickTime Media Layer
  err = InitializeQTML(0);
  if (err){
    error("filmQT: Could not initialize quicktime: error %d\n", err);
    return;
  }
	
  // Initialize QuickTime
  EnterMovies();
  if (err) {
    error("filmQT: Could not initialize quicktime: error %d\n", err);
    return;
  }	
#endif // WINDOWS

  m_pixBlock.image.setCsizeByFormat(GL_RGBA);
#ifdef __APPLE__
  m_pixBlock.image.type = GL_UNSIGNED_INT_8_8_8_8_REV;
#else
  m_pixBlock.image.type = GL_UNSIGNED_BYTE;
#endif // APPLE

  m_bInit = true;
#endif // HAVE_QUICKTIME
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
filmQT :: ~filmQT()
{
    close();
    /* i'd rather have "#ifdef QTML" (jmz) */
#ifdef HAVE_QUICKTIME
# ifndef __APPLE__
    // Deinitialize QuickTime Media Layer
    ExitMovies();
    // Deinitialize QuickTime Media Layer
    TerminateQTML();
# endif // APPLE
#endif // HAVE_QT
}

#ifdef HAVE_QUICKTIME
void filmQT :: close(void)
{
  ::DisposeMovie(m_movie);
  ::DisposeGWorld(m_srcGWorld);
  m_srcGWorld = NULL;
  m_haveMovie = GEM_MOVIE_NONE;
}

bool filmQT :: open(char*filename, int format) {
  if (!m_bInit){
    error("filmQT: object not correctly initialized\n");
    return false;
  }
  if (format>0)m_wantedFormat=format;
  int wantedFormat= (m_wantedFormat)?m_wantedFormat:GL_RGBA;
  // Clean up any open files:  closeMess();

  FSSpec	theFSSpec;
  OSErr		err = noErr;
  FSRef		ref;
  Rect		m_srcRect;
  long		m_rowBytes;

  {  
    Str255	pstrFilename;
    CopyCStringToPascal(filename, pstrFilename);                          // Convert to Pascal string
    err = FSMakeFSSpec (0, 0L, pstrFilename, &theFSSpec);  // Make specification record
    if (err) {
      error("GEM: pix_film: Unable to find file: %s", filename);
      return false;
    }
    m_haveMovie = GEM_MOVIE_MOV;
  }
    
  short	refnum = 0;
  err = ::OpenMovieFile(&theFSSpec, &refnum, fsRdPerm);
  if (err) {
    error("GEM: pix_movie: Couldn't open the movie file: %#s (%d)", theFSSpec.name, err);
    if (refnum) ::CloseMovieFile(refnum);
    return false;
  }

  ::NewMovieFromFile(&m_movie, refnum, NULL, NULL, newMovieActive, NULL);
  if (refnum) ::CloseMovieFile(refnum);

  m_reqFrame = 0;
  m_curFrame = -1;
  m_numTracks = (int)GetMovieTrackCount(m_movie);
  post("GEM: filmQT:  m_numTracks = %d",m_numTracks);

  // Get the length of the movie
  long	movieDur, movieScale;
  movieDur = (long)GetMovieDuration(m_movie);
  movieScale = (long)GetMovieTimeScale(m_movie);
  
  post("Movie duration = %d timescale = %d timebase = %d",movieDur,
       movieScale,
       (long)GetMovieTimeBase(m_movie));
                                            
  OSType	whichMediaType = VisualMediaCharacteristic;
  short		flags = nextTimeMediaSample + nextTimeEdgeOK;
  
  GetMovieNextInterestingTime( m_movie, flags, (TimeValue)1, &whichMediaType, 0, 
			       (Fixed)1<<16, NULL, &duration);
  m_numFrames = movieDur/duration;

  // Get the bounds for the movie
  ::GetMovieBox(m_movie, &m_srcRect);
#ifdef __APPLE__
  OffsetRect(&m_srcRect,  -m_srcRect.left,  -m_srcRect.top);
#else
  MacOffsetRect(&m_srcRect,  -m_srcRect.left,  -m_srcRect.top);
#endif
  SetMovieBox(m_movie, &m_srcRect);	
  m_image.image.xsize = m_srcRect.right - m_srcRect.left;
  m_image.image.ysize = m_srcRect.bottom - m_srcRect.top;
  post("rect rt:%d lt:%d", m_srcRect.right, m_srcRect.left);
  post("rect top:%d bottom:%d", m_srcRect.top, m_srcRect.bottom);
  post("movie size x:%d y:%d", m_xsize, m_ysize);

#ifdef __APPLE__
  m_pixBlock.image.type = GL_UNSIGNED_INT_8_8_8_8_REV;
#else
  m_pixBlock.image.type = GL_UNSIGNED_BYTE;
#endif

  createBuffer();
  prepareTexture();
  m_rowBytes = m_xsize * 4;
  SetMoviePlayHints(m_movie, hintsHighQuality, hintsHighQuality);
  err = QTNewGWorldFromPtr(	&m_srcGWorld, 
#ifdef __APPLE__
				k32ARGBPixelFormat, // gives noErr
#else
				k32BGRAPixelFormat,
#endif
				&m_srcRect, 
				NULL, 
				NULL, 
				0, 
				m_pixBlock.image.data, 
				m_rowBytes);
  if (err) {
    error("GEM: filmQT: Couldn't make QTNewGWorldFromPtr %d", err);
    m_haveMovie = 0;
    return false;
  }
  m_movieTime = 0;
  // *** set the graphics world for displaying the movie ***
  ::SetMovieGWorld(m_movie, m_srcGWorld, GetGWorldDevice(m_srcGWorld));
  ::MoviesTask(m_movie, 0);	// *** this does the actual drawing into the GWorld ***

  return true;
}

/////////////////////////////////////////////////////////
// getFrame
//
/////////////////////////////////////////////////////////
pixBlock* filmQT :: getFrame()
{
  CGrafPtr	savedPort;
  GDHandle     	savedDevice;
  Rect		m_srcRect;
  PixMapHandle	m_pixMap;
  Ptr			m_baseAddr;
    
  ::GetGWorld(&savedPort, &savedDevice);
  ::SetGWorld(m_srcGWorld, NULL);
  ::GetMovieBox(m_movie, &m_srcRect);
    
  m_pixMap = ::GetGWorldPixMap(m_srcGWorld);
  m_baseAddr = ::GetPixBaseAddr(m_pixMap);
 
  int num;

  // get the next frame of the source movie
  short 	flags = nextTimeStep;
  OSType	whichMediaType = VisualMediaCharacteristic;

  if (m_reqFrame > m_curFrame) {
    num = m_reqFrame - m_curFrame;
  } else {
    num = m_reqFrame;
    if (!m_auto) m_movieTime = 0;
  }
    
  //check for last frame to loop the clip
  if (m_curFrame >= m_numFrames){
    m_curFrame = 0;
    m_movieTime = 0;
  }
    
  //check for -1
  if (m_movieTime < 0) m_movieTime = 0;
    
  // if this is the first frame, include the frame we are currently on
  if (m_curFrame == 0) flags |= nextTimeEdgeOK;

  if (m_auto) {
    ::GetMovieNextInterestingTime(m_movie,
				  flags,
				  1,
				  &whichMediaType,
				  m_movieTime,
				  0,
				  &m_movieTime,
				  // NULL);
				  &duration);
    flags = 0;
    flags = nextTimeStep;
  }else{
    m_movieTime = m_reqFrame * duration;
  }

  // set the time for the frame and give time to the movie toolbox	
  SetMovieTimeValue(m_movie, m_movieTime); 
  MoviesTask(m_movie, 0);	// *** this does the actual drawing into the GWorld ***
  
  m_image.image.setCsizeByFormat(m_wantedFormat);
  m_image.image.reallocate();
  m_image.image.fromBGRA((unsigned char *)m_baseAddr);
  m_image.image.upsidedown=true;
}

#ifdef LOADRAM
//////////
// load film into RAM
void filmQT :: LoadRam(){
  TimeValue	length;
  OSErr err;
  if (m_haveMovie){      
    m_movieTime = 0;
    length = GetMovieDuration(m_movie);
    err =LoadMovieIntoRam(m_movie,m_movieTime,length,keepInRam);
    if (err) {
      post("pix_film: LoadMovieIntoRam failed miserably");
    }
  }else{
    post("pix_film: no movie to load into RAM!");
  }
}
#endif // LoadRAM
#endif // HAVE_QT
