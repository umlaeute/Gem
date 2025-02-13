////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.at
//
// Implementation file
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    Copyright (c) 2002 James Tittle & Chris Clepper
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif
#include "filmDarwin.h"
#ifdef HAVE_CARBONQUICKTIME

#include "plugins/PluginFactory.h"
#include "Gem/RTE.h"
#include "Gem/Properties.h"
#include "Gem/Exception.h"

using namespace gem::plugins;

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


REGISTER_FILMFACTORY("Darwin", filmDarwin);

/////////////////////////////////////////////////////////
//
// filmDarwin
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////

filmDarwin :: filmDarwin(void) :
  m_wantedFormat(GEM_YUV),
  m_auto(false),
  m_numFrames(-1), m_numTracks(-1),
  m_curFrame(-1), m_lastFrame(-1),
  m_movie(NULL),
  m_srcGWorld(NULL),
  m_movieTime(0),
  m_movieTrack(0),
  m_movieMedia(0),
  m_timeScale(0),
  m_durationf(0.),
  m_fps(1.)
{
  if(!filmQT_initQT()) {
    throw(GemException("unable to initialize QuickTime"));
  }
}

////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
filmDarwin :: ~filmDarwin(void)
{
  close();
  filmQT_deinitQT();
}

void filmDarwin :: close(void)
{
  if(m_srcGWorld) {
    ::DisposeMovie(m_movie);
    ::DisposeGWorld(m_srcGWorld);
    m_srcGWorld = NULL;
  }
}

/////////////////////////////////////////////////////////
// open the file
//
/////////////////////////////////////////////////////////
bool filmDarwin :: open(const std::string&filename,
                        const gem::Properties&wantProps)
{
  double d;
  if(wantProps.get("colorspace", d) && d>0) {
    m_wantedFormat=d;
  }
  FSSpec                theFSSpec;
  OSErr         err = noErr;
  FSRef         ref;
  Rect          m_srcRect;
  long          m_rowBytes;
  OSType                whichMediaType = VisualMediaCharacteristic;
  short         flags = nextTimeMediaSample + nextTimeEdgeOK;
  short refnum = 0;
  OSType pixelformat=0;
  long hints;
  Track movieTrack;
  Media trackMedia;

  if(filename.empty()) {
    goto unsupported;
  } else {
    err = ::FSPathMakeRef((const UInt8*)filename.c_str(), &ref, NULL);
    err = ::FSGetCatalogInfo(&ref, kFSCatInfoNone, NULL, NULL, &theFSSpec,
                             NULL);
    if (err) {
      logpost(0, 3+0, "[GEM:filmDarwin] Unable to find file: %s", filename.c_str());
      goto unsupported;
    }
  }

  err = ::OpenMovieFile(&theFSSpec, &refnum, fsRdPerm);
  if (err) {
    logpost(0, 3+0, "[GEM:filmDarwin] Couldn't open the movie file: %#s (%d)",
            filename.c_str(), err);
    if (refnum) {
      ::CloseMovieFile(refnum);
    }
    goto unsupported;
  }

  ::NewMovieFromFile(&m_movie, refnum, NULL, NULL, newMovieActive, NULL);
  if (refnum) {
    ::CloseMovieFile(refnum);
  }

  // m_curFrame = -1;
  if(m_curFrame==-1) {
    m_curFrame=-2;
  }
  m_lastFrame=-1;
  m_numTracks = (int)GetMovieTrackCount(m_movie);
  logpost(0, 3+1, "[GEM:filmDarwin] %d tracks", (int)m_numTracks);

  // Get the length of the movie
  long  movieDur, movieScale;
  movieDur = (long)GetMovieDuration(m_movie);
  movieScale = (long)GetMovieTimeScale(m_movie);

  logpost(0, 3+1,
          "[GEM:filmDarwin] duration = %d timescale = %d timebase = %d",
          movieDur, movieScale, (long)GetMovieTimeBase(m_movie));

  movieTrack = GetMovieIndTrackType(m_movie,
                                    1,
                                    VideoMediaType,
                                    movieTrackMediaType);  //get first video track
  trackMedia = GetTrackMedia(movieTrack);
  m_numFrames= GetMediaSampleCount(trackMedia);
  if(m_numFrames>0) {
    m_durationf = static_cast<double>(movieDur)/static_cast<double>
                  (m_numFrames);
    m_fps=static_cast<double>(m_numFrames)*static_cast<double>
          (movieScale)/static_cast<double>(movieDur);
  } else {
    m_numFrames=-1;
    m_fps=30.f;
    m_durationf=static_cast<double>(movieScale)/m_fps;
  }
  logpost(0, 3+1, "[GEM:filmDarwin] %d frames @ %f", (int)m_numFrames,
          (float)m_durationf);

  // Get the bounds for the movie
  ::GetMovieBox(m_movie, &m_srcRect);
  OffsetRect(&m_srcRect,  -m_srcRect.left,  -m_srcRect.top);
  SetMovieBox(m_movie, &m_srcRect);
  m_image.image.xsize = m_srcRect.right - m_srcRect.left;
  m_image.image.ysize = m_srcRect.bottom - m_srcRect.top;
  logpost(0, 3+1, "[GEM:filmDarwin] rect rt:%d lt:%d", m_srcRect.right,
          m_srcRect.left);
  logpost(0, 3+1, "[GEM:filmDarwin] rect top:%d bottom:%d", m_srcRect.top,
          m_srcRect.bottom);
  logpost(0, 3+1, "[GEM:filmDarwin] movie size x:%d y:%d", m_image.image.xsize,
          m_image.image.ysize);

  switch(m_wantedFormat) {
  case 0: // if no other format is requested, use YUV
  case GEM_YUV:
    m_image.image.format = m_wantedFormat;
    hints |= hintsHighQuality | hintsDeinterlaceFields;
    pixelformat=k422YpCbCr8CodecType;
    break;
  default:
    m_image.image.format = GEM_RGBA;
    hints |= hintsHighQuality;
    pixelformat=k32ARGBPixelFormat;
    break;
  }
  m_image.image.setFormat();

  m_image.image.data = new unsigned
  char [m_image.image.xsize*m_image.image.ysize*m_image.image.csize];
  m_rowBytes = m_image.image.xsize * m_image.image.csize;
  SetMoviePlayHints(m_movie, hints, hints);
  err = QTNewGWorldFromPtr(     &m_srcGWorld,
                                pixelformat,
                                &m_srcRect,
                                NULL,
                                NULL,
                                0,
                                m_image.image.data,
                                m_rowBytes);
  if (err) {
    logpost(0, 3+0, "[GEM:filmDarwin] Couldn't make QTNewGWorldFromPtr %d", err);
    goto unsupported;
  }
  m_movieTime = 0;
  // *** set the graphics world for displaying the movie ***
  ::SetMovieGWorld(m_movie, m_srcGWorld, GetGWorldDevice(m_srcGWorld));
  ::MoviesTask(m_movie,
               0);     // *** this does the actual drawing into the GWorld ***
  return true;

unsupported:
  return false;
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
pixBlock* filmDarwin :: getFrame(void)
{
  CGrafPtr      savedPort;
  GDHandle      savedDevice;
  Rect          m_srcRect;
  PixMapHandle  m_pixMap;

  ::GetGWorld(&savedPort, &savedDevice);
  ::SetGWorld(m_srcGWorld, NULL);
  ::GetMovieBox(m_movie, &m_srcRect);

  m_pixMap = ::GetGWorldPixMap(m_srcGWorld);

  // get the next frame of the source movie
  short         flags = nextTimeStep;
  OSType        whichMediaType = VisualMediaCharacteristic;
  TimeValue     duration;

  /* check whether we reached the end of the clip */
  if (IsMovieDone(m_movie)) {
    GoToBeginningOfMovie(m_movie);
    return NULL;
  }

  if(m_numFrames<0) {
    /* the opened media doesn't support seeking
     * so we return the next frame, if a new frame was requested
     * and the last frame if no new frame was requested
     */
    m_movieTime = GetMovieTime(m_movie, NULL);


    if(m_curFrame<0 && m_lastFrame<0) {
      m_curFrame=0;
    }

    /* if we have a valid curFrame (>=0) and curFrame progresses (curFrame!=lastFrame), get the next image
     * always get the image, if lastFrame<0
     */
    if(m_curFrame!=m_lastFrame) {
      m_image.newimage=1;
      SetMovieRate(m_movie,X2Fix(1.0));
    } else {
      m_image.newimage=0;
      SetMovieRate(m_movie,X2Fix(0.0));
    }

    MoviesTask(m_movie,
               0);     // *** this does the actual drawing into the GWorld ***
    m_lastFrame=m_curFrame;

    return &m_image;
  }

  //check for last frame to loop the clip
  if (m_curFrame >= m_numFrames) {
    return NULL;
    m_curFrame = 0;
    m_movieTime = 0;
  }
  m_movieTime = static_cast<long>(static_cast<double>(m_curFrame) *
                                  m_durationf);
  m_movieTime-=9; //total hack!! subtract an arbitrary amount and have nextinterestingtime find the exact place

  //check for -1
  if (m_movieTime < 0) {
    m_movieTime = 0;
  }

  // if this is the first frame, include the frame we are currently on
  if (m_curFrame == 0) {
    flags |= nextTimeEdgeOK;
  }

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
  }

  // set the time for the frame and give time to the movie toolbox
  SetMovieTimeValue(m_movie, m_movieTime);
  MoviesTask(m_movie,
             0);       // *** this does the actual drawing into the GWorld ***

  m_image.newimage=(m_lastFrame != m_curFrame);
  m_lastFrame=m_curFrame;
  return &m_image;
}

film::errCode filmDarwin :: changeImage(int imgNum, int trackNum)
{
  m_curFrame=imgNum;
  //  return 0;
  return film::SUCCESS;
}


///////////////////////////////
// Properties
bool filmDarwin::enumProperties(gem::Properties&readable,
                                gem::Properties&writeable)
{
  readable.clear();
  writeable.clear();

  gem::any value;
  value=0.;
  readable.set("frames", value);
  readable.set("tracks", value);
  readable.set("width", value);
  readable.set("height", value);
  readable.set("fps", value);

  writeable.set("auto", value);

  return false;
}

void filmDarwin::setProperties(gem::Properties&props)
{
  double d;
  if(props.get("auto", d)) {
    m_auto=(d>=0.5);
  }
}

void filmDarwin::getProperties(gem::Properties&props)
{
  std::vector<std::string> keys=props.keys();
  gem::any value;
  double d;
  for(unsigned int i=0; i<keys.size(); i++) {
    std::string key=keys[i];
    props.erase(key);
    if("frames"==key) {
      d=m_numFrames;
      value=d;
      props.set(key, value);
    }
    if("tracks"==key) {
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
    if("fps"==key) {
      d=m_fps;
      value=d;
      props.set(key, value);
    }
  }
}


#endif /* CARBONQUICKTIME */
