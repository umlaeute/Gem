////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// cclep@artic.edu & tigital@mac.com
//
// Implementation file 
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    Copyright (c) 2002 James Tittle & Chris Clepper
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
#ifdef MACOSX

#include "yuv_filmDarwin.h"
#include "OpenGL/glext.h"

static inline int powerOfTwo( int value )
{
    int x = 1;
    while ( x<value ) x<<= 1;
    return( x );
}

CPPEXTERN_NEW_WITH_ONE_ARG(yuv_filmDarwin, t_symbol *, A_DEFSYM)

/////////////////////////////////////////////////////////
//
// yuv_filmDarwin
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////

yuv_filmDarwin :: yuv_filmDarwin(t_symbol *filename) :
  yuv_film(filename)
{
  // make sure that there are some characters
  if (filename->s_name[0]) openMess(filename);
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
yuv_filmDarwin :: ~yuv_filmDarwin()
{
    closeMess();
    deleteBuffer();

    outlet_free(m_outNumFrames);
    outlet_free(m_outEnd);
}

void yuv_filmDarwin :: closeMess(void)
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
void yuv_filmDarwin :: realOpen(char *filename)
{
    FSSpec		theFSSpec;
    OSErr		err = noErr;
    FSRef		ref;
    Rect		m_srcRect;
    long		m_rowBytes;

    if (!filename[0]) {
        post("yuv_filmDarwin:  no filename passed");
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
        post("GEM: yuv_filmDarwin:  m_numTracks = %d",m_numTracks);

	// Get the length of the movie
        post("duration = %d timescale = %d timebase = %d\n",(long)GetMovieDuration(m_movie),
                                            (long)GetMovieTimeScale(m_movie),
                                            (long)GetMovieTimeBase(m_movie));
                                            
	OSType		whichMediaType = VIDEO_TYPE;
	short		flags = nextTimeMediaSample + nextTimeEdgeOK;
	TimeValue	duration;
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

	// *** set the graphics world for displaying the movie ***
	::SetMovieGWorld(m_movie, m_srcGWorld, GetGWorldDevice(m_srcGWorld));
	::MoviesTask(m_movie, 0);	// *** this does the actual drawing into the GWorld ***
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void yuv_filmDarwin :: getFrame()
{
    if (!m_haveMovie) return;
    
    CGrafPtr	 	savedPort;
    GDHandle     	savedDevice;
    Rect		m_srcRect;
    TimeValue		m_movieTime;
    PixMapHandle	m_pixMap;
    Ptr		m_baseAddr;
    
    ::GetGWorld(&savedPort, &savedDevice);
    ::SetGWorld(m_srcGWorld, NULL);
    ::GetMovieBox(m_movie, &m_srcRect);
    
    m_pixMap = ::GetGWorldPixMap(m_srcGWorld);
    m_baseAddr = ::GetPixBaseAddr(m_pixMap);

    int num;

    // get the next frame of the source movie
    short 	flags = nextTimeMediaSample;
    OSType	whichMediaType = VIDEO_TYPE;
    
    if (m_reqFrame > m_curFrame) {
        num = m_reqFrame - m_curFrame;
    } else {
        num = m_reqFrame;
        m_movieTime = 0;
    }

    // if this is the first frame, include the frame we are currently on
    if (m_curFrame == 0) flags |= nextTimeEdgeOK;

    for (int i=0; i<num; i++) {
    // skip to the next interesting time and get the duration for that frame
        ::GetMovieNextInterestingTime(m_movie,
                                            flags,
                                                1,
                                &whichMediaType,
                                    m_movieTime,
                                                0,
                                    &m_movieTime,
                                            NULL);
        }
        
    // set the time for the frame and give time to the movie toolbox	
    ::SetMovieTimeValue(m_movie, m_movieTime);
    ::MoviesTask(m_movie, 0);	// *** this does the actual drawing into the GWorld ***
    
    m_frame = (unsigned char *)m_baseAddr;
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void yuv_filmDarwin :: obj_setupCallback(t_class *classPtr)
{
  class_addcreator((t_newmethod)_classyuv_filmDarwin, gensym("yuv_film"), A_DEFSYM, A_NULL);
  yuv_film::real_obj_setupCallback(classPtr);

  class_addmethod(classPtr, (t_method)&yuv_filmDarwin::openMessCallback,
		  gensym("open"), A_SYMBOL, A_NULL);
  class_addmethod(classPtr, (t_method)&yuv_filmDarwin::changeImageCallback,
		  gensym("img_num"), A_GIMME, A_NULL);
  class_addmethod(classPtr, (t_method)&yuv_filmDarwin::autoCallback,
		  gensym("auto"), A_DEFFLOAT, A_NULL);
}

void yuv_filmDarwin :: openMessCallback(void *data, t_symbol *filename)
{
  GetMyClass(data)->openMess(filename);
}

void yuv_filmDarwin :: changeImageCallback(void *data, t_symbol *, int argc, t_atom *argv)
{
    GetMyClass(data)->changeImage((argc<1)?0:atom_getint(argv), (argc<2)?0:atom_getint(argv+1));
}

void yuv_filmDarwin :: autoCallback(void *data, t_floatarg state)
{
  GetMyClass(data)->m_auto=!(!(int)state);
}
#endif // MACOSX
