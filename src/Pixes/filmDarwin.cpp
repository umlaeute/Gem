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
#ifdef MACOSX
    post("pix_film:: Darwin support");
#endif
    first_time = false;
  }
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
filmDarwin :: ~filmDarwin()
{
  close();
}

void filmDarwin :: close(void)
{
#ifdef MACOSX
  if(m_srcGWorld){
    ::DisposeMovie(m_movie);
    ::DisposeGWorld(m_srcGWorld);
    m_srcGWorld = NULL;
  }
#endif
}

/////////////////////////////////////////////////////////
// open the file
//
/////////////////////////////////////////////////////////
bool filmDarwin :: open(char *filename, int format)
{
  if (format>0)m_wantedFormat=format;
#ifdef MACOSX
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
    err = ::FSPathMakeRef((Str255)filename, &ref, NULL);
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
  long	movieDur, movieScale, mediaDur, mediaScale;
  movieDur = (long)GetMovieDuration(m_movie);
  movieScale = (long)GetMovieTimeScale(m_movie);
        
  post("Movie duration = %d timescale = %d timebase = %d",movieDur,
       movieScale,
       (long)GetMovieTimeBase(m_movie));
  
 
  //TimeValue	duration;
  //TimeValue	theTime = 0;
        
  //m_movieTrack = GetMovieIndTrack( m_movie, 1);
 /* m_movieMedia = GetTrackMedia( m_movieTrack );
  mediaDur = (long)GetMediaDuration(m_movieMedia);
  mediaScale = (long)GetMediaTimeScale(m_movieMedia);
  post("Media duration = %d timescale = %d", mediaDur, mediaScale);
  m_timeScale = mediaScale/movieScale; */
        
  GetMovieNextInterestingTime( m_movie, flags, (TimeValue)1, &whichMediaType, 0, 
			       fixed1, NULL, &duration);
  m_numFrames = movieDur/duration;
  /*	
  m_numFrames = -1;
  while (theTime >= 0) {
    m_numFrames++;
    ::GetMovieNextInterestingTime(m_movie,
				  flags,
				  1,
				  &whichMediaType,
				  theTime,
				  0,
				  &theTime,
				  &duration);
    // after the first interesting time, don't include the time we
    //  are currently at.
    //flags = nextTimeMediaSample;
    flags = 0;
    flags = nextTimeStep;
  }
*/
  // Get the bounds for the movie
  ::GetMovieBox(m_movie, &m_srcRect);
  OffsetRect(&m_srcRect,  -m_srcRect.left,  -m_srcRect.top);
  SetMovieBox(m_movie, &m_srcRect);	
  m_image.image.xsize = m_srcRect.right - m_srcRect.left;
  m_image.image.ysize = m_srcRect.bottom - m_srcRect.top;
  post("rect rt:%d lt:%d", m_srcRect.right, m_srcRect.left);
  post("rect top:%d bottom:%d", m_srcRect.top, m_srcRect.bottom);
  post("movie size x:%d y:%d", m_image.image.xsize, m_image.image.ysize);
  /*
  int qt_format=0; // i guess this will give an error. but what is it ?
  switch (format){
  case GL_RGBA:case GL_BGRA:
    m_image.image.csize = 4;
    m_image.image.format = GL_BGRA_EXT;
    m_pixBlock.image.type = GL_UNSIGNED_INT_8_8_8_8_REV;
    qt_format=k32ARGBPixelFormat; 
    break;
  case GL_YCBCR_422_GEM: default:
    m_image.image.csize = 2;
    m_image.image.format = GL_YCBCR_422_GEM;
    m_pixBlock.image.type = GL_UNSIGNED_SHORT_8_8_REV_APPLE;
    qt_format=k422YpCbCr8CodecType;
  } */
  
 // createBuffer();
 // prepareTexture();
 m_image.image.csize = 4;
    m_image.image.format = GL_BGRA_EXT;
    m_image.image.type = GL_UNSIGNED_INT_8_8_8_8_REV;
  //  m_image.allocate();
 // m_pixBlock.image.allocate();
 m_image.image.data = new unsigned char [m_image.image.xsize*m_image.image.ysize*m_image.image.csize]; 
  m_rowBytes = m_image.image.xsize * m_image.image.csize;
  SetMoviePlayHints(m_movie, hintsHighQuality, hintsHighQuality);
  err = QTNewGWorldFromPtr(	&m_srcGWorld, 
				//qt_format,
                             //   k422YpCbCr8CodecType,
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
#endif
  goto unsupported;
 unsupported:
  post("Darwin: unsupported!");
//  close();
  return false;
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
pixBlock* filmDarwin :: getFrame(){
#ifdef MACOSX
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

    int num;

    // get the next frame of the source movie
    short 	flags = nextTimeStep;
    OSType	whichMediaType = VisualMediaCharacteristic;
    TimeValue	duration;
    /*if (m_reqFrame > m_curFrame) {
        num = m_reqFrame - m_curFrame;
    } else {
        num = m_reqFrame;
        if (!m_auto) m_movieTime = 0;
    }*/
    
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
     // m_movieTime = m_reqFrame * duration;
      //SampleNumToMediaTime( m_movieMedia, m_reqFrame, &mFrame, NULL );
      //m_movieTime = mFrame/m_timeScale;
      /*
      for (int i=0; i<num; i++) {
	// skip to the next interesting time and get the duration for that frame
	::GetMovieNextInterestingTime(m_movie,
				      flags,
				      1,
				      &whichMediaType,
				      m_movieTime,
				      0,
				      &m_movieTime,
				      &duration);
      }*/
    }

    post("filmDarwin: curFrame %d",m_curFrame);
    // set the time for the frame and give time to the movie toolbox	
    SetMovieTimeValue(m_movie, m_movieTime); 
    MoviesTask(m_movie, 0);	// *** this does the actual drawing into the GWorld ***
    
  //  m_image.image.data = (unsigned char *)m_baseAddr;
  m_image.newimage=1;
#endif
  return &m_image;
}

int filmDarwin :: changeImage(int imgNum, int trackNum){
  m_curFrame=imgNum;
//  return 0;
return FILM_ERROR_SUCCESS;
}
