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
#if defined(__APPLE__) || defined(HAVE_QUICKTIME)

#define DO_AUTO_REGISTER_CLASS

#include "pix_filmQT.h"
#ifdef __APPLE__

#else /* w32 */
#	include "TextUtils.h"
#endif

CPPEXTERN_NEW_WITH_ONE_ARG(pix_filmQT, t_symbol *, A_DEFSYM)

/////////////////////////////////////////////////////////
//
// pix_filmQT
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////

pix_filmQT :: pix_filmQT(t_symbol *filename) :
  pix_filmOS(filename)
, m_bInit(false)
{

#ifndef __APPLE__
    OSErr		err = noErr;

	// Initialize QuickTime Media Layer
	err = InitializeQTML(0);
	if (err)
	{
		error("Could not initialize quicktime: error %d\n", err);
		return;
	}	
	
	// Initialize QuickTime
	EnterMovies();
	if (err)
	{
		error("Could not initialize quicktime: error %d\n", err);
		return;
	}	
#endif

 m_pixBlock.image.csize = 4;
 m_pixBlock.image.format = GL_BGRA_EXT;
 m_format = GL_BGRA_EXT;
#ifndef __APPLE__
 m_pixBlock.image.type = GL_UNSIGNED_BYTE;
#else
 m_pixBlock.image.type = GL_UNSIGNED_INT_8_8_8_8_REV;
#endif

m_bInit = true;

  // make sure that there are some characters
  if (filename->s_name[0]) openMess(filename);
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_filmQT :: ~pix_filmQT()
{
    closeMess();
    deleteBuffer();

    outlet_free(m_outNumFrames);
    outlet_free(m_outEnd);

#ifndef __APPLE__
	// Deinitialize QuickTime Media Layer
	ExitMovies();

	// Deinitialize QuickTime Media Layer
	TerminateQTML();
#endif
}

void pix_filmQT :: closeMess(void)
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

void pix_filmQT :: openMess(t_symbol *filename)
{
	if (!m_bInit)
	{
		error("object not correctly initialized\n");
		return;
	}
  //  if (filename==x_filename)return;
  x_filename=filename;

  char buf[MAXPDSTRING];
  canvas_makefilename(getCanvas(), filename->s_name, buf, MAXPDSTRING);

  // Clean up any open files
  closeMess();

  m_haveMovie = GEM_MOVIE_NONE;
  realOpen(buf);
  if (m_haveMovie == GEM_MOVIE_NONE)return;
  
#ifndef __APPLE__
//  createBuffer();
//  prepareTexture();
#endif

  t_atom ap[3];
  SETFLOAT(ap, m_numFrames);
  SETFLOAT(ap+1, m_xsize);
  SETFLOAT(ap+2, m_ysize);


  //outlet_float(m_outNumFrames, (float)m_numFrames);
  post("loaded file: %s with %d frames (%dx%d)", buf, m_numFrames, m_xsize, m_ysize);
  outlet_list(m_outNumFrames, 0, 3, ap);
}

/////////////////////////////////////////////////////////
// really open the file ! (OS dependent)
//
/////////////////////////////////////////////////////////
void pix_filmQT :: realOpen(char *filename)
{
    FSSpec		theFSSpec;
    OSErr		err = noErr;
    FSRef		ref;
	Rect		m_srcRect;
	long		m_rowBytes;

    if (!filename[0]) {
        error("no filename passed");
    } else {  
		Str255	pstrFilename;
        CopyCStringToPascal(filename, pstrFilename);                          // Convert to Pascal string
        //c2pstrcpy(pstrFilename, filename);
		err = FSMakeFSSpec (0, 0L, pstrFilename, &theFSSpec);  // Make specification record

//        err = ::FSPathMakeRef((Str255)filename, &ref, NULL);
//        err = ::FSGetCatalogInfo(&ref, kFSCatInfoNone, NULL, NULL, &theFSSpec, NULL);
            
        if (err) {
            error("Unable to find file: %s", filename);
            return;
        }
        m_haveMovie = GEM_MOVIE_MOV;
    }
    
    short	refnum = 0;
    err = ::OpenMovieFile(&theFSSpec, &refnum, fsRdPerm);
    if (err) {
        error("Couldn't open the movie file: %#s (%d)", theFSSpec.name, err);
        if (refnum) ::CloseMovieFile(refnum);
        return;
    }

	::NewMovieFromFile(&m_movie, refnum, NULL, NULL, newMovieActive, NULL);
	if (refnum) ::CloseMovieFile(refnum);

	m_reqFrame = 0;
	m_curFrame = -1;
        m_numTracks = (int)GetMovieTrackCount(m_movie);
        post("m_numTracks = %d",m_numTracks);

	// Get the length of the movie
        long	movieDur, movieScale;
        movieDur = (long)GetMovieDuration(m_movie);
        movieScale = (long)GetMovieTimeScale(m_movie);
        
        post("Movie duration = %d timescale = %d timebase = %d",movieDur,
                                            movieScale,
                                            (long)GetMovieTimeBase(m_movie));
                                            
	OSType		whichMediaType = VisualMediaCharacteristic;
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
	m_xsize = m_srcRect.right - m_srcRect.left;
	m_ysize = m_srcRect.bottom - m_srcRect.top;
        post("rect rt:%d lt:%d", m_srcRect.right, m_srcRect.left);
        post("rect top:%d bottom:%d", m_srcRect.top, m_srcRect.bottom);
	post("movie size x:%d y:%d", m_xsize, m_ysize);

        m_csize = 4;
        m_format = GL_BGRA_EXT;
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
		error("Couldn't make QTNewGWorldFromPtr %d", err);
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
void pix_filmQT :: render(GemState *state)
{
  /* get the current frame from the file */
  int newImage = 0;
  if (!m_haveMovie || !m_pixBlock.image.data)return;
  // do we actually need to get a new frame from the movie ?
  if (m_reqFrame != m_curFrame) {
    newImage = 1;
    getFrame();
    m_curFrame = m_reqFrame;
	m_pixBlock.image.upsidedown=1;
    if (m_film)m_pixBlock.image.data = m_frame; // this is mainly for windows
  }
  m_pixBlock.newimage = newImage;
  state->image = &m_pixBlock;

  if (m_reqFrame == m_curFrame)
        ::MoviesTask(NULL, 0);

  /* texture it, if needed */
  texFrame(state, newImage);

  // automatic proceeding
  if (m_auto)m_reqFrame++;
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void pix_filmQT :: getFrame()
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
        }

    // set the time for the frame and give time to the movie toolbox	
    SetMovieTimeValue(m_movie, m_movieTime); 
    MoviesTask(m_movie, 0);	// *** this does the actual drawing into the GWorld ***
    
    m_frame = (unsigned char *)m_baseAddr;
}

void pix_filmQT :: LoadRam()
{
    TimeValue	length;
    OSErr err;
    if (m_haveMovie){      
        m_movieTime = 0;
        length = GetMovieDuration(m_movie);
        err =LoadMovieIntoRam(m_movie,m_movieTime,length,keepInRam);
        if (err)
        {
            post("LoadMovieIntoRam failed miserably");
        }
    }else{
        post("no movie to load into RAM!");
    }
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_filmQT :: obj_setupCallback(t_class *classPtr)
{
 class_addcreator((t_newmethod)create_pix_filmQT, gensym("pix_film"), A_DEFSYM, A_NULL);
  pix_filmOS::real_obj_setupCallback(classPtr);

  class_addmethod(classPtr, (t_method)&pix_filmQT::openMessCallback,
		  gensym("open"), A_SYMBOL, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_filmQT::changeImageCallback,
		  gensym("img_num"), A_GIMME, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_filmQT::autoCallback,
		  gensym("auto"), A_DEFFLOAT, A_NULL);
}

void pix_filmQT :: openMessCallback(void *data, t_symbol *filename)
{
  GetMyClass(data)->openMess(filename);
}

void pix_filmQT :: changeImageCallback(void *data, t_symbol *, int argc, t_atom *argv)
{
    GetMyClass(data)->changeImage((argc<1)?0:atom_getint(argv), (argc<2)?0:atom_getint(argv+1));
}

void pix_filmQT :: autoCallback(void *data, t_floatarg state)
{
  GetMyClass(data)->m_auto=!(!(int)state);
}
#endif // __APPLE__
