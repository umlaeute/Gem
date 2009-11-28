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
#include "Base/GemConfig.h"
#include "filmQT.h"

#ifdef HAVE_QUICKTIME
# ifdef __APPLE__
#  define FILMQT_DEFAULT_PIXELFORMAT k32ARGBPixelFormat

static bool filmQT_initQT(void) { return true; }
static bool filmQT_deinitQT(void) { return true; }

# else /* !__APPLE__ */

/* TextUtils.h is from QTdev */
#  include "TextUtils.h"
#  define OffsetRect MacOffsetRect
#  define FILMQT_DEFAULT_PIXELFORMAT k32RGBAPixelFormat

static bool filmQT_initQT(void) {
  // Initialize QuickTime Media Layer
  OSErr		err = noErr;
  if ((err = InitializeQTML(0))) {
    error("filmQT: Could not initialize quicktime: error %d\n", err);
    return false;
  }

  // Initialize QuickTime
  if (err = EnterMovies()) {
    error("filmQT: Could not initialize quicktime: error %d\n", err);
    return false;
  }
  return true;
}

static bool filmQT_deinitQT(void) { 
  // Deinitialize QuickTime Media Layer
  ExitMovies();
  // Deinitialize QuickTime Media Layer
  TerminateQTML();

  return true;
}

# endif /* w32 */
#endif /* HAVE_QUICKTIME */

/////////////////////////////////////////////////////////
//
// filmQT
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////

filmQT :: filmQT(int format) : film(format)
#ifdef HAVE_QUICKTIME
                ,
				m_movie(NULL),
				m_srcGWorld(NULL),
				m_movieTime(0),
				m_movieTrack(0),
				m_timeScale(1), 
				duration(0),
				m_bInit(false)
{
  static bool first_time=true;
  if(!filmQT_initQT())return;

  if (first_time) {
    post("pix_film:: QuickTime support");
    first_time = false;
  }

  m_image.image.setCsizeByFormat(GL_RGBA);
  m_bInit = true;
}
#else /* !HAVE_QUICKTIME */
{/* nop */}
#endif

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
filmQT :: ~filmQT()
{
  close();

#if defined HAVE_QUICKTIME
  /* should we check whether "m_bInit" is true? */
  filmQT_deinitQT();
#endif // HAVE_QT
}

#ifdef HAVE_QUICKTIME
void filmQT :: close(void)
{
  if (!m_bInit){
    return;
  }

  DisposeMovie(m_movie);

  //in QT Bizzaro World GWorlds dispose themselves!  And leak about 4k per call
  //	::DisposeGWorld(m_srcGWorld);
  //	m_srcGWorld = NULL;
}

bool filmQT :: open(char*filename, int format) {
  FSSpec	theFSSpec;
  OSErr		err = noErr;
  Rect		m_srcRect;
  long		m_rowBytes;

  Str255	pstrFilename;

  short	refnum = 0;
  long	movieDur, movieScale;
  OSType	whichMediaType;
  short		flags = 0;
  int wantedFormat;


  if (filename==NULL)return false;
  if (!m_bInit){
    error("filmQT: object not correctly initialized\n");
    goto unsupported;
  }
  if (format>0)m_wantedFormat=format;
  wantedFormat= (m_wantedFormat)?m_wantedFormat:GL_RGBA;
  // Clean up any open files:  closeMess();

  CopyCStringToPascal(filename, pstrFilename);           // Convert to Pascal string

  err = FSMakeFSSpec (0, 0L, pstrFilename, &theFSSpec);  // Make specification record
  if (err) {
    error("GEM: pix_film: Unable to find file: %s", filename);
    goto unsupported;
  }
  err = ::OpenMovieFile(&theFSSpec, &refnum, fsRdPerm);
  if (err) {
    error("GEM: pix_movie: Couldn't open the movie file: %#s (%d)", theFSSpec.name, err);
    if (refnum) ::CloseMovieFile(refnum);
    goto unsupported;
  }
  err = ::NewMovieFromFile(&m_movie, refnum, NULL, NULL, newMovieActive, NULL);
  if (err) {
    error("GEM: pix_movie: Couldn't open the movie file: %#s (%d)", theFSSpec.name, err);
    if (refnum) ::CloseMovieFile(refnum);
	m_movie=NULL;
    goto unsupported;
  }
  if (refnum) ::CloseMovieFile(refnum);


  m_curFrame = -1;
  m_numTracks = (int)GetMovieTrackCount(m_movie);

  // Get the length of the movie
  movieDur = (long)GetMovieDuration(m_movie);
  movieScale = (long)GetMovieTimeScale(m_movie);

  whichMediaType = VisualMediaCharacteristic;

  // shouldn't the flags be OR'ed instead of ADDed ? (jmz)
  flags = nextTimeMediaSample | nextTimeEdgeOK;

  GetMovieNextInterestingTime( m_movie, flags, (TimeValue)1, &whichMediaType, 0,
			       (Fixed)1<<16, NULL, &duration);
  m_numFrames = movieDur/duration;

  // Get the bounds for the movie
  ::GetMovieBox(m_movie, &m_srcRect);
  // OffsetRect(&m_srcRect,  -m_srcRect.left,  -m_srcRect.top);
  SetMovieBox(m_movie, &m_srcRect);	
  m_image.image.xsize = m_srcRect.right - m_srcRect.left;
  m_image.image.ysize = m_srcRect.bottom - m_srcRect.top;

  m_image.image.setCsizeByFormat(GL_RGBA);
  m_image.image.allocate();

  m_rowBytes = m_image.image.xsize * 4;
  // SetMoviePlayHints(m_movie, hintsHighQuality, hintsHighQuality);
  err = SetMovieAudioMute(m_movie, true, 0);
  if(noErr!=err) {
    error("GEM: filmQT: unable to mute movie...");
  }

  err = QTNewGWorldFromPtr(	&m_srcGWorld,
				FILMQT_DEFAULT_PIXELFORMAT,
				&m_srcRect,
				NULL,
				NULL,
				0,
				m_image.image.data,
				m_rowBytes);
  if (err) {
    error("GEM: filmQT: Couldn't make QTNewGWorldFromPtr %d", err);
    goto unsupported;
  }

  // *** set the graphics world for displaying the movie ***
  ::SetMovieGWorld(m_movie, m_srcGWorld, GetGWorldDevice(m_srcGWorld));
  if(GetMoviesError()){
    close();
    goto unsupported;
  }

  SetMovieRate(m_movie,X2Fix(1.0));
  ::MoviesTask(m_movie, 0);	// *** this does the actual drawing into the GWorld ***

  return true;

 unsupported:
  startpost("QuickTime failed ...");
  return false;

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
  Ptr		m_baseAddr;

  ::GetGWorld(&savedPort, &savedDevice);
  ::SetGWorld(m_srcGWorld, NULL);
  ::GetMovieBox(m_movie, &m_srcRect);

  m_pixMap = ::GetGWorldPixMap(m_srcGWorld);
  m_baseAddr = ::GetPixBaseAddr(m_pixMap);
  // set the time for the frame and give time to the movie toolbox
  SetMovieTimeValue(m_movie, m_movieTime);
  MoviesTask(m_movie, 0);	// *** this does the actual drawing into the GWorld ***
  m_image.image.setCsizeByFormat(m_wantedFormat);
  m_image.newimage = 1;
  m_image.image.upsidedown=true;

  return &m_image;
}


double filmQT :: getFPS() {
  m_fps = m_numFrames;
  return m_fps;
}

int filmQT :: changeImage(int imgNum, int trackNum){
  m_readNext = false;
  if (imgNum  ==-1)  imgNum=m_curFrame;
  if (m_numFrames>1 && imgNum>=m_numFrames){
    m_movieTime=0;
    return FILM_ERROR_FAILURE;
  }
  if (trackNum==-1||trackNum>m_numTracks)trackNum=m_curTrack;
  m_readNext=true;
  m_curFrame = imgNum;

  /* i have moved the "auto"-thing into [pix_film].
   * this is good because the decoder-class need not care about auto-play anylonger
   * this is bad, because we might do it more sophisticated in the decoder-class
   */
  m_movieTime = (long)(m_curFrame * duration);

  return FILM_ERROR_SUCCESS;
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
