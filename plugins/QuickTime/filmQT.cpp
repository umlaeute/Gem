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
//    Copyright (c) 2003-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "filmQT.h"
#include "plugins/PluginFactory.h"
#include "Gem/Properties.h"
#include "Gem/RTE.h"
#include "Gem/Exception.h"
#include "Utils/wstring.h"

using namespace gem::plugins;

REGISTER_FILMFACTORY("QuickTime", filmQT);
# ifdef __APPLE__
#  define FILMQT_DEFAULT_PIXELFORMAT GEM_RAW_UYVY

static bool filmQT_initQT(void)
{
  // Initialize QuickTime Media Layer
  OSErr         err = noErr;
  // Initialize QuickTime
  if (err = EnterMovies()) {
    logpost(0, 3+0, "[GEM:filmQT]] Could not initialize quicktime: error %d\n",
            err);
    return false;
  }
  return true;
}
static bool filmQT_deinitQT(void)
{
  // Deinitialize QuickTime Media Layer
  ExitMovies();

  return true;
}

# else /* !__APPLE__ */

/* TextUtils.h is from QTdev */
#  include "TextUtils.h"
#  define OffsetRect MacOffsetRect
#  define FILMQT_DEFAULT_PIXELFORMAT GEM_RAW_RGBA

static bool filmQT_initQT(void)
{
  // Initialize QuickTime Media Layer
  OSErr         err = noErr;
  if ((err = InitializeQTML(0))) {
    logpost(0, 3+0, "[GEM:filmQT]] Could not initialize quicktime: error %d\n",
            err);
    return false;
  }

  // Initialize QuickTime
  if (err = EnterMovies()) {
    logpost(0, 3+0, "[GEM:filmQT]] Could not initialize quicktime: error %d\n",
            err);
    return false;
  }
  return true;
}

static bool filmQT_deinitQT(void)
{
  // Deinitialize QuickTime Media Layer
  ExitMovies();
  // Deinitialize QuickTime Media Layer
  TerminateQTML();

  return true;
}

# endif /* w32 */

/////////////////////////////////////////////////////////
//
// filmQT
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////

filmQT :: filmQT(void) :
  m_wantedFormat(FILMQT_DEFAULT_PIXELFORMAT),
  m_fps(-1.),
  m_numFrames(-1), m_numTracks(-1),
  m_curFrame(-1), m_curTrack(-1),
  m_readNext(false),
  m_auto(0.f),
  m_movie(NULL),
  m_srcGWorld(NULL),
  m_movieTime(0),
  m_movieTrack(0),
  m_timeScale(1),
  m_frameDuration(0),
  m_bInit(false)
{
  if(!filmQT_initQT()) {
    throw(GemException("unable to initialize QuickTime"));
  }
  m_image.image.setFormat(m_wantedFormat);
  m_bInit = true;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
filmQT :: ~filmQT(void)
{
  close();
  /* should we check whether "m_bInit" is true? */
  filmQT_deinitQT();
}

void filmQT :: close(void)
{
  if (!m_bInit) {
    return;
  }

  DisposeMovie(m_movie);

  //in QT Bizzaro World GWorlds dispose themselves!  And leak about 4k per call
  //    ::DisposeGWorld(m_srcGWorld);
  //    m_srcGWorld = NULL;
}

bool filmQT :: open(const std::string&filename_,
                    const gem::Properties&wantProps)
{
  FSSpec        theFSSpec;
  OSErr         err = noErr;
  Rect          m_srcRect;
  long          m_rowBytes;

  short refnum = 0;
  long  movieDur, movieTimeScale;
  OSType        whichMediaType;
  short         flags = 0;
  double d;

  OSType pixelformat=0;
  long hints=0;

  if (filename_.empty()) {
    return false;
  }
  if (!m_bInit) {
    logpost(0, 3+0, "[GEM:filmQT] QT object not correctly initialized\n");
    return false;
  }
  std::string filename = gem::string::utf8string_to_nativestring(filename_);

  // Clean up any open files:  closeMess();

#ifdef __APPLE__
  Str255        pstrFilename;
  CopyCStringToPascal(filename.c_str(),
                      pstrFilename);           // Convert to Pascal string

  err = FSMakeFSSpec (0, 0L, pstrFilename,
                      &theFSSpec);  // Make specification record
  if (err != noErr) {
    FSRef               ref;
    err = ::FSPathMakeRef((const UInt8*)filename.c_str(), &ref, NULL);
    err = ::FSGetCatalogInfo(&ref, kFSCatInfoNone, NULL, NULL, &theFSSpec,
                             NULL);
  }
#else /* windows */
  err = ::NativePathNameToFSSpec (const_cast<char*>(filename.c_str()), &theFSSpec, 0);
#endif

  if (err != noErr) {
    logpost(0, 3+0, "[GEM:filmQT] Unable to find file: %s (%d)", filename.c_str(),
            err);
    //goto unsupported;
  }
  err = ::OpenMovieFile(&theFSSpec, &refnum, fsRdPerm);
  if (err) {
    logpost(0, 3+0, "[GEM:filmQT] Couldn't open the movie file: %s (%d)",
            filename.c_str(), err);
    if (refnum) {
      ::CloseMovieFile(refnum);
    }
    goto unsupported;
  }
  err = ::NewMovieFromFile(&m_movie, refnum, NULL, NULL, newMovieActive,
                           NULL);
  if (err) {
    logpost(0, 3+0, "[GEM:filmQT] Couldn't make a movie from file: %s (%d)",
            filename.c_str(), err);
    if (refnum) {
      ::CloseMovieFile(refnum);
    }
    m_movie=NULL;
    goto unsupported;
  }
  if (refnum) {
    ::CloseMovieFile(refnum);
  }

  m_curFrame = -1;
  m_numTracks = static_cast<int>(GetMovieTrackCount(m_movie));

  // Get the length of the movie
  movieDur = static_cast<long>(GetMovieDuration(m_movie));
  movieTimeScale = static_cast<long>(GetMovieTimeScale(m_movie));

  whichMediaType = VisualMediaCharacteristic;

  // shouldn't the flags be OR'ed instead of ADDed ? (jmz)
  flags = nextTimeMediaSample | nextTimeEdgeOK;

  m_frameDuration=0;

  GetMovieNextInterestingTime( m_movie, flags,
                               static_cast<TimeValue>(1),
                               &whichMediaType, 0,
                               static_cast<Fixed>(1<<16), NULL, &m_frameDuration);
  if(movieDur<=0) {
    m_numFrames=-1;
  } else {
    m_numFrames = movieDur/m_frameDuration;
  }

  if(m_frameDuration>=movieDur) {
    m_fps=30.f;
    //m_frameDuration=(float)movieTimeScale/m_fps;
    m_frameDuration=0;
    m_numFrames=-1;
  } else {
    m_fps = (float)movieTimeScale/(float)m_frameDuration;
  }

  // Get the bounds for the movie
  ::GetMovieBox(m_movie, &m_srcRect);
  // OffsetRect(&m_srcRect,  -m_srcRect.left,  -m_srcRect.top);
  SetMovieBox(m_movie, &m_srcRect);
  m_image.image.xsize = m_srcRect.right - m_srcRect.left;
  m_image.image.ysize = m_srcRect.bottom - m_srcRect.top;

  switch(m_wantedFormat) {
  default: // if no other format is requested, use YUV
  case GEM_RAW_UYVY:
    m_image.image.format = m_wantedFormat;
    hints |= hintsHighQuality | hintsDeinterlaceFields;
    pixelformat=k422YpCbCr8CodecType;
    break;
  case GEM_RAW_BGRA:
    m_image.image.format = GEM_RAW_BGRA;
    hints |= hintsHighQuality;
    pixelformat=k32ARGBPixelFormat;
    break;
  case GEM_RAW_RGBA:
    m_image.image.format = GEM_RAW_RGBA;
    hints |= hintsHighQuality;
    pixelformat=k32RGBAPixelFormat;
    break;
  }
  m_image.image.setFormat();
  m_image.image.allocate();

  m_rowBytes = m_image.image.xsize * m_image.image.csize;
  SetMoviePlayHints(m_movie, hints, hints);
  err = SetMovieAudioMute(m_movie, true, 0);
  if(noErr!=err) {
    logpost(0, 3+0, "[GEM:filmQT] unable to mute movie...");
  }

  err = QTNewGWorldFromPtr(     &m_srcGWorld,
                                pixelformat,
                                &m_srcRect,
                                NULL,
                                NULL,
                                0,
                                m_image.image.data,
                                m_rowBytes);
  if (err) {
    logpost(0, 3+0, "[GEM:filmQT] Couldn't make QTNewGWorldFromPtr %d", err);
    goto unsupported;
  }

  // *** set the graphics world for displaying the movie ***
  ::SetMovieGWorld(m_movie, m_srcGWorld, GetGWorldDevice(m_srcGWorld));
  if(GetMoviesError()) {
    close();
    goto unsupported;
  }

  if(m_frameDuration) {
    SetMovieRate(m_movie,X2Fix(m_auto));
  } else if (m_auto == 0.f) {
    SetMovieRate(m_movie,X2Fix(1.f));
  }

  ::MoviesTask(m_movie,
               0);     // *** this does the actual drawing into the GWorld ***

  m_readNext=true;

  return true;

unsupported:
  return false;

}

/////////////////////////////////////////////////////////
// getFrame
//
/////////////////////////////////////////////////////////
pixBlock* filmQT :: getFrame(void)
{
  CGrafPtr      savedPort;
  GDHandle      savedDevice;
  Rect          m_srcRect;

  ::GetGWorld(&savedPort, &savedDevice);
  ::SetGWorld(m_srcGWorld, NULL);
  ::GetMovieBox(m_movie, &m_srcRect);

  // set the time for the frame and give time to the movie toolbox
  if(m_frameDuration>0) {
    SetMovieTimeValue(m_movie, m_movieTime);
  }
  MoviesTask(m_movie,
             0);       // *** this does the actual drawing into the GWorld ***
  m_image.newimage = m_readNext;

  m_image.image.upsidedown=true;
  SetMovieRate(m_movie,X2Fix(m_auto));

  m_readNext=(m_auto!=0.f);
  return &m_image;
}

film::errCode filmQT :: changeImage(int imgNum, int trackNum)
{
  m_readNext = false;
  if (imgNum  ==-1) {
    imgNum=m_curFrame;
  }
  if (m_numFrames>1 && imgNum>=m_numFrames) {
    m_movieTime=0;
    return film::FAILURE;
  }
  if (trackNum==-1||trackNum>m_numTracks) {
    trackNum=m_curTrack;
  }
  m_readNext=true;

  if(imgNum!=m_curFrame && m_frameDuration<1) {
    SetMovieRate(m_movie,X2Fix(1.0));
  }

  m_curFrame = imgNum;

  /* i have moved the "auto"-thing into [pix_film].
   * this is good because the decoder-class need not care about auto-play any longer
   * this is bad, because we might do it more sophisticated in the decoder-class
   */
  m_movieTime = static_cast<long>(m_curFrame * m_frameDuration);


  return film::SUCCESS;
}

///////////////////////////////
// Properties
bool filmQT::enumProperties(gem::Properties&readable,
                            gem::Properties&writeable)
{
  readable.clear();
  writeable.clear();

  gem::any value;
  value=0.;
  readable.set("fps", value);
  readable.set("frames", value);
  readable.set("tracks", value);
  readable.set("width", value);
  readable.set("height", value);

  return false;
}

void filmQT::setProperties(gem::Properties&props)
{
}

void filmQT::getProperties(gem::Properties&props)
{
  std::vector<std::string> keys=props.keys();
  gem::any value;
  double d;
  for(unsigned int i=0; i<keys.size(); i++) {
    std::string key=keys[i];
    props.erase(key);
    if("fps"==key) {
      d=m_fps;
      value=d;
      props.set(key, value);
    }
    if("frames"==key && m_numFrames>=0) {
      d=m_numFrames;
      value=d;
      props.set(key, value);
    }
    if("tracks"==key && m_numTracks>=0) {
      d=m_numTracks;
      value=d;
      props.set(key, value);
    }
    if("width"==key) {
      d=m_image.image.xsize;
      value=d;
      props.set(key, value);
    }
    if("height"==key) {
      d=m_image.image.ysize;
      value=d;
      props.set(key, value);
    }
  }
}


#ifdef LOADRAM
//////////
// load film into RAM
void filmQT :: LoadRam(void)
{
  TimeValue     length;
  OSErr err;
  if (m_haveMovie) {
    m_movieTime = 0;
    length = GetMovieDuration(m_movie);
    err =LoadMovieIntoRam(m_movie,m_movieTime,length,keepInRam);
    if (err) {
      pd_error(0, "[GEM:filmQT] LoadMovieIntoRam failed miserably");
    }
  } else {
    pd_error(0, "[GEM:filmQT] no movie to load into RAM!");
  }
}
#endif // LoadRAM
