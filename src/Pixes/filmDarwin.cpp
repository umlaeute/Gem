////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file 
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2003 IOhannes m zmoelnig. forum::für::umläute. IEM
//    Copyright (c) 2002 James Tittle & Chris Clepper
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
#include "Pixes/filmDarwin.h"


   
/////////////////////////////////////////////////////////
//
// filmDarwin
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////

filmDarwin :: filmDarwin(int format) : film(format) {
  static bool first_time=true;
  if (first_time) {
#ifdef HAVE_CARBONQUICKTIME
    post("pix_film:: Darwin support");
#endif /*  HAVE_CARBONQUICKTIME */
    first_time = false;
  }
}

////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
filmDarwin :: ~filmDarwin()
{
  close();
}

void filmDarwin :: close(void)
{
#ifdef HAVE_CARBONQUICKTIME
  if(m_srcGWorld){
    ::DisposeMovie(m_movie);
    ::DisposeGWorld(m_srcGWorld);
    m_srcGWorld = NULL;
  }
#endif /*  HAVE_CARBONQUICKTIME */
}

/////////////////////////////////////////////////////////
// open the file
//
/////////////////////////////////////////////////////////
bool filmDarwin :: open(char *filename, int format)
{
  if (format>0)m_wantedFormat=format;
#ifdef HAVE_CARBONQUICKTIME
  FSSpec		theFSSpec;
  OSErr		err = noErr;
  FSRef		ref;
  Rect		m_srcRect;
  long		m_rowBytes;
  OSType		whichMediaType = VisualMediaCharacteristic;
  short		flags = nextTimeMediaSample + nextTimeEdgeOK;
  
  if (!filename[0]) {
    post("pix_film:  no filename passed");
  } else { 
    err = ::FSPathMakeRef((const UInt8*)filename, &ref, NULL);
    err = ::FSGetCatalogInfo(&ref, kFSCatInfoNone, NULL, NULL, &theFSSpec, NULL);
    if (err) {
      error("GEM: pix_film: Unable to find file: %#s", theFSSpec.name);
      //goto unsupported;
    }
  }
  
  short	refnum = 0;
  err = ::OpenMovieFile(&theFSSpec, &refnum, fsRdPerm);
  if (err) {
    error("GEM: pix_film: Couldn't open the movie file: %#s (%d)", theFSSpec.name, err);
    if (refnum) ::CloseMovieFile(refnum);
    goto unsupported;
  }
  
  ::NewMovieFromFile(&m_movie, refnum, NULL, NULL, newMovieActive, NULL);
  if (refnum) ::CloseMovieFile(refnum);
  
  // m_curFrame = -1;
  m_numTracks = (int)GetMovieTrackCount(m_movie);
  post("GEM: pix_film:  m_numTracks = %d",m_numTracks);

  // Get the length of the movie
  long	movieDur, movieScale;
  movieDur = (long)GetMovieDuration(m_movie);
  movieScale = (long)GetMovieTimeScale(m_movie);
        
  post("Movie duration = %d timescale = %d timebase = %d",movieDur,
       movieScale,
       (long)GetMovieTimeBase(m_movie));
  
   GetMovieNextInterestingTime( m_movie, flags, (TimeValue)1, &whichMediaType, 0, 
                               fixed1, NULL, &duration);
  m_numFrames = movieDur/duration;

  // Get the bounds for the movie
  ::GetMovieBox(m_movie, &m_srcRect);
  OffsetRect(&m_srcRect,  -m_srcRect.left,  -m_srcRect.top);
  SetMovieBox(m_movie, &m_srcRect);	
  m_image.image.xsize = m_srcRect.right - m_srcRect.left;
  m_image.image.ysize = m_srcRect.bottom - m_srcRect.top;
  post("rect rt:%d lt:%d", m_srcRect.right, m_srcRect.left);
  post("rect top:%d bottom:%d", m_srcRect.top, m_srcRect.bottom);
  post("movie size x:%d y:%d", m_image.image.xsize, m_image.image.ysize);
 
  m_image.image.csize = 4;
  m_image.image.format = GL_BGRA_EXT;
  m_image.image.type = GL_UNSIGNED_INT_8_8_8_8_REV;

  m_image.image.data = new unsigned char [m_image.image.xsize*m_image.image.ysize*m_image.image.csize]; 
  m_rowBytes = m_image.image.xsize * m_image.image.csize;
  SetMoviePlayHints(m_movie, hintsHighQuality, hintsHighQuality);
  err = QTNewGWorldFromPtr(	&m_srcGWorld, 
                            k32ARGBPixelFormat,
                            &m_srcRect, 
                            NULL, 
                            NULL, 
                            0, 
                            m_image.image.data, 
                            m_rowBytes);
  if (err) {
    error("GEM: pix_film: Couldn't make QTNewGWorldFromPtr %d", err);
    goto unsupported;
  }
  m_movieTime = 0;
  // *** set the graphics world for displaying the movie ***
  ::SetMovieGWorld(m_movie, m_srcGWorld, GetGWorldDevice(m_srcGWorld));
  ::MoviesTask(m_movie, 0);	// *** this does the actual drawing into the GWorld ***
  return true;
#endif /*  HAVE_CARBONQUICKTIME */
  goto unsupported;
 unsupported:
  post("Darwin: unsupported!");
  return false;
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
pixBlock* filmDarwin :: getFrame(){
#ifdef HAVE_CARBONQUICKTIME
  CGrafPtr	 	savedPort;
  GDHandle     	savedDevice;
  Rect		m_srcRect;
  PixMapHandle	m_pixMap;
  Ptr			m_baseAddr;
    
  ::GetGWorld(&savedPort, &savedDevice);
  ::SetGWorld(m_srcGWorld, NULL);
  ::GetMovieBox(m_movie, &m_srcRect);
    
  m_pixMap = ::GetGWorldPixMap(m_srcGWorld);
  m_baseAddr = ::GetPixBaseAddr(m_pixMap);

  // get the next frame of the source movie
  short 	flags = nextTimeStep;
  OSType	whichMediaType = VisualMediaCharacteristic;
  TimeValue	duration;
    
  //check for last frame to loop the clip
  if (m_curFrame >= m_numFrames){
    m_curFrame = 0;
    m_movieTime = 0;
  }
    
  //check for -1
  if (m_movieTime < 0) m_movieTime = 0;
       
  // if this is the first frame, include the frame we are currently on
  if (m_curFrame == 0) flags |= nextTimeEdgeOK;

  // if (m_auto) {
  if (1) {
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
    // m_curFrame++;
  }else{
  }

  post("filmDarwin: curFrame %d",m_curFrame);
  // set the time for the frame and give time to the movie toolbox	
  SetMovieTimeValue(m_movie, m_movieTime); 
  MoviesTask(m_movie, 0);	// *** this does the actual drawing into the GWorld ***
    
  //  m_image.image.data = (unsigned char *)m_baseAddr;
  m_image.newimage=1;
#endif /*  HAVE_CARBONQUICKTIME */
  return &m_image;
}

int filmDarwin :: changeImage(int imgNum, int trackNum){
  m_curFrame=imgNum;
  //  return 0;
  return FILM_ERROR_SUCCESS;
}
