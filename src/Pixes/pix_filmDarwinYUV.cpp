////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// chris clepper & jamie tittle
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

#include "pix_filmDarwinYUV.h"
#include "OpenGL/glext.h"

static inline int powerOfTwo( int value )
{
    int x = 1;
    while ( x<value ) x<<= 1;
    return( x );
}

 
CPPEXTERN_NEW_WITH_ONE_ARG(pix_filmDarwinYUV, t_symbol *, A_DEFSYM)

/////////////////////////////////////////////////////////
//
// pix_filmdarwinYUV
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////

pix_filmDarwinYUV :: pix_filmDarwinYUV(t_symbol *filename) :
  pix_filmYUV(filename)
{
  // make sure that there are some characters
  if (filename->s_name[0]) openMess(filename);
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_filmDarwinYUV :: ~pix_filmDarwinYUV()
{
    closeMess();
    deleteBuffer();

    outlet_free(m_outNumFrames);
    outlet_free(m_outEnd);
}

void pix_filmDarwinYUV :: closeMess(void)
{
    switch (m_haveMovie) {
        case GEM_MOVIE_MOV:
            ::DisposeMovie(m_movie);
            ::DisposeGWorld(m_srcGWorld);
            //m_pixMap = NULL;
            m_srcGWorld = NULL;
            m_haveMovie = GEM_MOVIE_NONE;
            //m_getFrame = NULL;
            break;
        case GEM_MOVIE_AVI:
            ::DisposeMovie(m_movie);
            ::DisposeGWorld(m_srcGWorld);
            //m_pixMap = NULL;
            m_srcGWorld = NULL;
            m_haveMovie = GEM_MOVIE_NONE;
            //m_getFrame = NULL;
            break;
        case GEM_MOVIE_MPG:
            ::DisposeMovie(m_movie);
            ::DisposeGWorld(m_srcGWorld);
            //m_pixMap = NULL;
            m_srcGWorld = NULL;
            m_haveMovie = GEM_MOVIE_NONE;
            //m_getFrame = NULL;
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
void pix_filmDarwinYUV :: realOpen(char *filename)
{
    FSSpec		theFSSpec;
    OSErr		err = noErr;
    FSRef		ref;
    Rect		m_srcRect;
    long		m_rowBytes;

//timer start
UnsignedWide startTime;
    ::Microseconds(&startTime);

    if (!filename[0]) {
        post("pix_filmdarwinYUV:  no filename passed");
    } else {            
        err = ::FSPathMakeRef((UInt8*)filename, &ref, NULL);
        err = ::FSGetCatalogInfo(&ref, kFSCatInfoNone, NULL, NULL, &theFSSpec, NULL);
            
        if (err) {
            error("GEM: pix_movie: Unable to find file: %#s", theFSSpec.name);
            return;
        }
        m_haveMovie = GEM_MOVIE_MOV;
    }
    m_movieTime = 0;
    duration = 0;
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
        post("GEM: pix_filmdarwinYUV:  m_numTracks = %d",m_numTracks);

	// Get the length of the movie

        long	movieDur, movieScale;
        movieDur = (long)GetMovieDuration(m_movie);
        movieScale = (long)GetMovieTimeScale(m_movie);
        
        post("Movie duration = %d timescale = %d timebase = %d",movieDur,
                                movieScale, (long)GetMovieTimeBase(m_movie));
                                            
	OSType		whichMediaType = VisualMediaCharacteristic;
        // we want to begin with the first frame (sample) in the track
	short		flags = nextTimeMediaSample + nextTimeEdgeOK;
        
       GetMovieNextInterestingTime( m_movie, flags, (TimeValue)1, &whichMediaType, 0, 
             fixed1, NULL, &duration);
        m_numFrames = movieDur/duration;
        post("pix_filmdarwinYUV:  bogus duration = %d",duration);
        post("pix_filmdarwinYUV:  bogus number of frames = %d",m_numFrames);
             
       TimeValue	theTime = 0;
	
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
		flags = nextTimeMediaSample;
	}
        
        durationf = (float)movieDur/(float)m_numFrames;
        post("pix_filmdarwinYUV:  real duration = %f",durationf);
        post("pix_filmdarwinYUV:  real number of frames = %d",m_numFrames);
        
         
       
	// Get the bounds for the movie
	::GetMovieBox(m_movie, &m_srcRect);
        OffsetRect(&m_srcRect,  -m_srcRect.left,  -m_srcRect.top);
	SetMovieBox(m_movie, &m_srcRect);	
	m_xsize = m_srcRect.right - m_srcRect.left;
	m_ysize = m_srcRect.bottom - m_srcRect.top;
        post("rect rt:%d lt:%d", m_srcRect.right, m_srcRect.left);
        post("rect top:%d bottom:%d", m_srcRect.top, m_srcRect.bottom);
	post("movie size x:%d y:%d", m_xsize, m_ysize);
 
        m_csize =2;
        m_format = GL_YCBCR_422_APPLE;
        m_pixBlock.image.type = GL_UNSIGNED_SHORT_8_8_REV_APPLE;
  
        createBuffer();
        prepareTexture();
        m_rowBytes = m_xsize * 2;
        SetMoviePlayHints(m_movie, hintsHighQuality, hintsHighQuality);
	err = QTNewGWorldFromPtr(	&m_srcGWorld, 
                                        k422YpCbCr8CodecType, 
					&m_srcRect, 
					NULL, 
					NULL, 
					0, 
					m_pixBlock.image.data, 
					m_rowBytes);
	if (err) {
		error("GEM: pix_filmdarwinYUV: Couldn't make QTNewGWorldFromPtr %d", err);
		m_haveMovie = 0;
		return;
	}
      
        m_movieTime = 0;

	// *** set the graphics world for displaying the movie ***
	::SetMovieGWorld(m_movie, m_srcGWorld, GetGWorldDevice(m_srcGWorld));
	::MoviesTask(m_movie, 0);	// *** this does the actual drawing into the GWorld ***
      
        //end timer
         UnsignedWide endTime;
        ::Microseconds(&endTime);
        float seconds = (float)(endTime.lo - startTime.lo) / 1000000.f;
        post("GEM: time to open movie: %f", seconds);
        
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void pix_filmDarwinYUV :: getFrame()
{
    if (!m_haveMovie) return;


    int num;

    // get the next frame of the source movie

    short 	flags = nextTimeStep;
    OSType	whichMediaType = VisualMediaCharacteristic;
    /*
    if (m_reqFrame > m_curFrame) {
        num = m_reqFrame - m_curFrame;
    } else {
        num = m_reqFrame;
        if (!m_auto) m_movieTime = 0;
    }
*/

    //check for last frame to loop the clip
    if (m_curFrame >= m_numFrames){
    m_curFrame = 0;
    m_movieTime = 0;
    }
    
    //check for -1
    if (m_movieTime < 0) m_movieTime = 0;
    
    // if this is the first frame, include the frame we are currently on
    if (m_curFrame == 0) flags |= nextTimeEdgeOK;


/*
//if (m_auto || (m_reqFrame - m_curFrame == 1)) {
if (m_auto) {
        ::GetMovieNextInterestingTime(	m_movie,
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
  */     
  
    m_movieTime = (long)((float)m_reqFrame * durationf);
    
    m_movieTime-=9; //total hack!! subtract an arbitrary amount and have nextinterestingtime find the exact place 
    
    ::GetMovieNextInterestingTime(	m_movie,
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
  
    // set the time for the frame and give time to the movie toolbox	

    SetMovieTimeValue(m_movie, m_movieTime); 
    MoviesTask(m_movie, 0);	// *** this does the actual drawing into the GWorld ***
    
}

void pix_filmDarwinYUV :: LoadRam()
{
    TimeValue	length;
    OSErr 	err;
    if (m_haveMovie){      
        m_movieTime = 0;
        length = GetMovieDuration(m_movie);
        err =LoadMovieIntoRam(m_movie,m_movieTime,length,keepInRam);
        if (err)
        {
            post("pix_filmYUV: LoadMovieIntoRam failed miserably");
        }
    }else{
        post("pix_filmYUV: no movie to load into RAM!");
    }
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_filmDarwinYUV :: obj_setupCallback(t_class *classPtr)
{
  class_addcreator((t_newmethod)_classpix_filmDarwinYUV, gensym("pix_filmYUV"), A_DEFSYM, A_NULL);
  pix_filmYUV::real_obj_setupCallback(classPtr);

  class_addmethod(classPtr, (t_method)&pix_filmDarwinYUV::openMessCallback,
		  gensym("open"), A_SYMBOL, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_filmDarwinYUV::changeImageCallback,
		  gensym("img_num"), A_GIMME, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_filmDarwinYUV::autoCallback,
		  gensym("auto"), A_DEFFLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_filmDarwinYUV::ramCallback,
		  gensym("ram"), A_NULL);
}

void pix_filmDarwinYUV :: openMessCallback(void *data, t_symbol *filename)
{
  GetMyClass(data)->openMess(filename);
}

void pix_filmDarwinYUV :: changeImageCallback(void *data, t_symbol *, int argc, t_atom *argv)
{
    GetMyClass(data)->changeImage((argc<1)?0:atom_getint(argv), (argc<2)?0:atom_getint(argv+1));
}

void pix_filmDarwinYUV :: autoCallback(void *data, t_floatarg state)
{
  GetMyClass(data)->m_auto=!(!(int)state);
}

void pix_filmDarwinYUV :: ramCallback(void *data)
{
  GetMyClass(data)->LoadRam();
}

#endif // MACOSX
