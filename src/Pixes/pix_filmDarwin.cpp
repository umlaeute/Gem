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
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
#ifdef MACOSX

#include "pix_filmDarwin.h"
#include "OpenGL/glext.h"

static inline int powerOfTwo( int value )
{
    int x = 1;
    while ( x<value ) x<<= 1;
    return( x );
}

CPPEXTERN_NEW_WITH_ONE_ARG(pix_filmDarwin, t_symbol *, A_DEFSYM)

/////////////////////////////////////////////////////////
//
// pix_filmDarwin
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////

pix_filmDarwin :: pix_filmDarwin(t_symbol *filename) :
  pix_film(filename)
{
  // make sure that there are some characters
  if (filename->s_name[0]) openMess(filename);
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_filmDarwin :: ~pix_filmDarwin()
{
    closeMess();
    deleteBuffer();

    outlet_free(m_outNumFrames);
    outlet_free(m_outEnd);
}

void pix_filmDarwin :: closeMess(void)
{
    switch (m_haveMovie) {
        case GEM_MOVIE_MOV:
            ::DisposeMovie(m_movie);
            ::DisposeGWorld(m_srcGWorld);
            m_srcGWorld = NULL;
            m_haveMovie = GEM_MOVIE_NONE;
            break;
        case GEM_MOVIE_AVI:
            ::DisposeMovie(m_movie);
            ::DisposeGWorld(m_srcGWorld);
            m_srcGWorld = NULL;
            m_haveMovie = GEM_MOVIE_NONE;
            break;
        case GEM_MOVIE_MPG:
            ::DisposeMovie(m_movie);
            ::DisposeGWorld(m_srcGWorld);
            m_srcGWorld = NULL;
            m_haveMovie = GEM_MOVIE_NONE;
            break;
        case GEM_MOVIE_NONE:
        default:
            break;
  }
}
/////////////////////////////////////////////////////////
// really open the file ! (OS dependent)
//
/////////////////////////////////////////////////////////
void pix_filmDarwin :: realOpen(char *filename)
{
    FSSpec		theFSSpec;
    OSErr		err = noErr;
    FSRef		ref;
    Rect		m_srcRect;
    long		m_rowBytes;

    if (!filename[0]) {
        post("pix_filmDarwin:  no filename passed");
    } else {            
        err = ::FSPathMakeRef((Str255)filename, &ref, NULL);
        err = ::FSGetCatalogInfo(&ref, kFSCatInfoNone, NULL, NULL, &theFSSpec, NULL);
            
        if (err) {
            error("GEM: pix_movie: Unable to find file: %#s", theFSSpec.name);
            return;
        }
        m_haveMovie = GEM_MOVIE_MOV;
    }
    
    short	refnum = 0;
    err = ::OpenMovieFile(&theFSSpec, &refnum, fsRdPerm);
    if (err) {
        error("GEM: pix_movie: Couldn't open the movie file: %#s (%d)", theFSSpec.name, err);
        if (refnum) ::CloseMovieFile(refnum);
        return;
    }

	::NewMovieFromFile(&m_movie, refnum, NULL, NULL, newMovieActive, NULL);
	if (refnum) ::CloseMovieFile(refnum);

	m_reqFrame = 0;
	m_curFrame = -1;
        m_numTracks = (int)GetMovieTrackCount(m_movie);
        post("GEM: pix_filmDarwin:  m_numTracks = %d",m_numTracks);

	// Get the length of the movie
        long	movieDur, movieScale, mediaDur, mediaScale;
        movieDur = (long)GetMovieDuration(m_movie);
        movieScale = (long)GetMovieTimeScale(m_movie);
        
        post("Movie duration = %d timescale = %d timebase = %d",movieDur,
                                            movieScale,
                                            (long)GetMovieTimeBase(m_movie));
                                            
	OSType		whichMediaType = VisualMediaCharacteristic;
	short		flags = nextTimeMediaSample + nextTimeEdgeOK;
	//TimeValue	duration;
	//TimeValue	theTime = 0;
        
        //m_movieTrack = GetMovieIndTrack( m_movie, 1);
        m_movieMedia = GetTrackMedia( m_movieTrack );
        mediaDur = (long)GetMediaDuration(m_movieMedia);
        mediaScale = (long)GetMediaTimeScale(m_movieMedia);
        post("Media duration = %d timescale = %d", mediaDur, mediaScale);
        m_timeScale = mediaScale/movieScale;
        
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
	m_xsize = m_srcRect.right - m_srcRect.left;
	m_ysize = m_srcRect.bottom - m_srcRect.top;
        post("rect rt:%d lt:%d", m_srcRect.right, m_srcRect.left);
        post("rect top:%d bottom:%d", m_srcRect.top, m_srcRect.bottom);
	post("movie size x:%d y:%d", m_xsize, m_ysize);

        m_csize = 4;
        m_format = GL_BGRA_EXT;
        m_pixBlock.image.type = GL_UNSIGNED_INT_8_8_8_8_REV;
        
        createBuffer();
        prepareTexture();
        m_rowBytes = m_xsize * 4;
        SetMoviePlayHints(m_movie, hintsHighQuality, hintsHighQuality);
	err = QTNewGWorldFromPtr(	&m_srcGWorld, 
                                        k32ARGBPixelFormat,	// gives noErr
					&m_srcRect, 
					NULL, 
					NULL, 
					0, 
					m_pixBlock.image.data, 
					m_rowBytes);
	if (err) {
		error("GEM: pix_filmDarwin: Couldn't make QTNewGWorldFromPtr %d", err);
		m_haveMovie = 0;
		return;
	}
        m_movieTime = 0;
	// *** set the graphics world for displaying the movie ***
	::SetMovieGWorld(m_movie, m_srcGWorld, GetGWorldDevice(m_srcGWorld));
	::MoviesTask(m_movie, 0);	// *** this does the actual drawing into the GWorld ***
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void pix_filmDarwin :: getFrame()
{
    if (!m_haveMovie) return;
    
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
            //SampleNumToMediaTime( m_movieMedia, m_reqFrame, &mFrame, NULL );
            //m_movieTime = mFrame/m_timeScale;
/*        for (int i=0; i<num; i++) {
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

       post("pix_film: m_movieTIme: %d",m_movieTime);
    // set the time for the frame and give time to the movie toolbox	
    SetMovieTimeValue(m_movie, m_movieTime); 
    MoviesTask(m_movie, 0);	// *** this does the actual drawing into the GWorld ***
    
    m_frame = (unsigned char *)m_baseAddr;
}

void pix_filmDarwin :: LoadRam()
{
      TimeValue	length;
      OSErr err;
if (m_haveMovie){      
m_movieTime = 0;
 length = GetMovieDuration(m_movie);
 err =LoadMovieIntoRam(m_movie,m_movieTime,length,keepInRam);
 if (err)
 {
 post("pix_film: LoadMovieIntoRam failed miserably");
 }
 }else{
 post("pix_film: no movie to load into RAM!");
 }
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_filmDarwin :: obj_setupCallback(t_class *classPtr)
{
  class_addcreator((t_newmethod)_classpix_filmDarwin, gensym("pix_film"), A_DEFSYM, A_NULL);
  pix_film::real_obj_setupCallback(classPtr);

  class_addmethod(classPtr, (t_method)&pix_filmDarwin::openMessCallback,
		  gensym("open"), A_SYMBOL, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_filmDarwin::changeImageCallback,
		  gensym("img_num"), A_GIMME, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_filmDarwin::autoCallback,
		  gensym("auto"), A_DEFFLOAT, A_NULL);
}

void pix_filmDarwin :: openMessCallback(void *data, t_symbol *filename)
{
  GetMyClass(data)->openMess(filename);
}

void pix_filmDarwin :: changeImageCallback(void *data, t_symbol *, int argc, t_atom *argv)
{
    GetMyClass(data)->changeImage((argc<1)?0:atom_getint(argv), (argc<2)?0:atom_getint(argv+1));
}

void pix_filmDarwin :: autoCallback(void *data, t_floatarg state)
{
  GetMyClass(data)->m_auto=!(!(int)state);
}
#endif // MACOSX
