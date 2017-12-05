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

#include "filmAVFoundation.h"
#include "plugins/PluginFactory.h"
#include "Gem/Properties.h"
#include "Gem/RTE.h"
#include "Gem/Exception.h"

using namespace gem::plugins;

REGISTER_FILMFACTORY("AVFoundation", filmAVFoundation);
#define FILMQTKIT_DEFAULT_PIXELFORMAT GL_YCBCR_422_APPLE

static bool filmAVFoundation_initAVFoundation(void) { return true; }
static bool filmAVFoundation_deinitAVFoundation(void) { return true; }

/////////////////////////////////////////////////////////
//
// filmAVFoundation
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////

filmAVFoundation :: filmAVFoundation(void) :
  m_wantedFormat(FILMQTKIT_DEFAULT_PIXELFORMAT),
  m_fps(-1.),
  m_numFrames(-1), //m_numTracks(-1),
  m_curFrame(-1), //m_curTrack(-1),
  m_readNext(false),
  m_auto(0.f),
  //m_movie(NULL),
  //m_srcGWorld(NULL),
  //m_movieTime(0),
  //m_movieTrack(0),
  //m_timeScale(1),
  m_frameDuration(0),
  m_bInit(false),
  m_moviePlayer(NULL)
{
  if(!filmAVFoundation_initAVFoundation()) {
    throw(GemException("unable to initialize AVFoundation"));
  }
  m_image.image.setCsizeByFormat(m_wantedFormat);
  m_bInit = true;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
filmAVFoundation :: ~filmAVFoundation(void)
{
  close();
  /* should we check whether "m_bInit" is true? */
  filmAVFoundation_deinitAVFoundation();
}

void filmAVFoundation :: close(void)
{
  if (!m_bInit){
    return;
  }

  //DisposeMovie(m_movie);
  m_moviePlayer = nil;

  //in QT Bizzaro World GWorlds dispose themselves!  And leak about 4k per call
  //	::DisposeGWorld(m_srcGWorld);
  //	m_srcGWorld = NULL;
}

bool filmAVFoundation :: open(const std::string&filename, const gem::Properties&wantProps) {

  if (filename.empty()) return false;
  if (!m_bInit) {
    error("filmAVFoundation: object not correctly initialized\n");
    return false;
  }

  // simple check to see if this is a URL
  bool isURL = false;
  if ((filename.length() > 7 && filename.substr(0,8) == "https://") ||
      (filename.length() > 6 && (filename.substr(0,7) == "http://" || filename.substr(0,7) == "rtsp://"))) {
    isURL = true;
  }

  m_moviePlayer = [[AVFMoviePlayer alloc] init];
  m_moviePlayer.useTexture = false;
  m_moviePlayer.useAlpha = true;
  if(isURL) {
    [m_moviePlayer loadFilePath:[NSString stringWithCString:filename.c_str() encoding:NSUTF8StringEncoding]];
  }
  else {
    [m_moviePlayer loadURLPath:[NSString stringWithCString:filename.c_str() encoding:NSUTF8StringEncoding]];
  }
  if(m_moviePlayer.isLoaded) {
    //m_moviePlayer.synchronousSeek = true;//bSynchronousSeek;
    //reallocatePixels();
    //moviePath = movieFilePath;
    //duration = moviePlayer.duration;


    //setLoopState(currentLoopState);
    //setSpeed(1.0f);
    //firstFrame(); // will load the first frame
  }
  else {
    error("filmAVFoundation: Unable to open file: %s", filename.c_str());
    //ofLogError("ofAVFoundationPlayer") << "loadMovie(): couldn't load \"" << movieFilePath << "\"";
    m_moviePlayer = nil;
    return false;
  }
  m_image.newfilm = true;

  // setup movie data
  m_movieTime = 0;
  m_curFrame = -1;

  if(m_moviePlayer.totalFrames <= 0) {
    m_numFrames=-1;
  }
  else {
    m_numFrames = m_moviePlayer.totalFrames;
  }

  m_frameDuration = m_moviePlayer.duration;
  if(m_frameDuration <= 0) {
    m_fps = 30.f;
    m_frameDuration = 0;
    m_numFrames = -1;
  }
  else {
    m_fps = (float)m_numFrames/(float)m_frameDuration;
  }

  m_image.image.xsize = m_moviePlayer.width;
  m_image.image.ysize = m_moviePlayer.height;

  switch(m_wantedFormat) {
    default: // if no other format is requested, use YUV
    // case GL_YCBCR_422_APPLE:
    //   post("YUV");
    //   m_image.image.format = m_wantedFormat;
    //   hints |= hintsHighQuality | hintsDeinterlaceFields;
    //   pixelformat=k422YpCbCr8CodecType;
    //   break;
    // case GL_BGRA_EXT:
    //   post("BGRA");
    //   m_image.image.format = GL_BGRA_EXT;
    //   hints |= hintsHighQuality;
    //   pixelformat=k32ARGBPixelFormat;
    //   break;
    case GL_RGBA:
      post("RGBA");
      m_image.image.format = GL_RGBA;
      // hints |= hintsHighQuality;
      // pixelformat=k32RGBAPixelFormat;
      break;
  }
  m_image.image.setCsizeByFormat();
  m_image.image.allocate();

  //?m_rowBytes = m_image.image.xsize * m_image.image.csize;
  m_moviePlayer.volume = 0; // mute

  m_readNext = true;

  return true;



 //  FSSpec	theFSSpec;
 //  OSErr		err = noErr;
 //  Rect		m_srcRect;
 //  long		m_rowBytes;

 //  short	refnum = 0;
 //  long	movieDur, movieTimeScale;
 //  OSType	whichMediaType;
 //  short		flags = 0;
 //  double d;

 //  OSType pixelformat=0;
 //  long hints=0;

 //  if (filename.empty())return false;
 //  if (!m_bInit){
 //    error("filmAVFoundation: object not correctly initialized\n");
 //    return false;
 //  }
 //  // Clean up any open files:  closeMess();

 //  Str255	pstrFilename;
 //  CopyCStringToPascal(filename.c_str(), pstrFilename);           // Convert to Pascal string

 //  err = FSMakeFSSpec (0, 0L, pstrFilename, &theFSSpec);  // Make specification record
 //  if (err != noErr) {
 //    FSRef		ref;
 //    err = ::FSPathMakeRef((const UInt8*)filename.c_str(), &ref, NULL);
 //    err = ::FSGetCatalogInfo(&ref, kFSCatInfoNone, NULL, NULL, &theFSSpec, NULL);
 //  }

 //  if (err != noErr) {
 //    error("filmAVFoundation: Unable to find file: %s (%d)", filename.c_str(), err);
 //    //goto unsupported;
 //  }
 //  err = ::OpenMovieFile(&theFSSpec, &refnum, fsRdPerm);
 //  if (err) {
 //    error("filmAVFoundation: Couldn't open the movie file: %s (%d)", filename.c_str(), err);
 //    if (refnum) ::CloseMovieFile(refnum);
 //    goto unsupported;
 //  }
 //  err = ::NewMovieFromFile(&m_movie, refnum, NULL, NULL, newMovieActive, NULL);
 //  if (err) {
 //    error("filmAVFoundation: Couldn't make a movie from file: %s (%d)", filename.c_str(), err);
 //    if (refnum) ::CloseMovieFile(refnum);
 //    m_movie=NULL;
 //    goto unsupported;
 //  }
 //  if (refnum) ::CloseMovieFile(refnum);

 //  m_curFrame = -1;
 //  m_numTracks = static_cast<int>(GetMovieTrackCount(m_movie));

 //  // Get the length of the movie
 //  movieDur = static_cast<long>(GetMovieDuration(m_movie));
 //  movieTimeScale = static_cast<long>(GetMovieTimeScale(m_movie));

 //  whichMediaType = VisualMediaCharacteristic;

 //  flags = nextTimeMediaSample | nextTimeEdgeOK;

 //  m_frameDuration=0;

 //  GetMovieNextInterestingTime( m_movie, flags,
	// 		       static_cast<TimeValue>(1),
	// 		       &whichMediaType, 0,
	// 		       static_cast<Fixed>(1<<16), NULL, &m_frameDuration);
 //  if(movieDur<=0)
 //    m_numFrames=-1;
 //  else
 //    m_numFrames = movieDur/m_frameDuration;

 //  if(m_frameDuration>=movieDur) {
 //    m_fps=30.f;
 //    //m_frameDuration=(float)movieTimeScale/m_fps;
 //    m_frameDuration=0;
 //    m_numFrames=-1;
 //  } else {
 //    m_fps = (float)movieTimeScale/(float)m_frameDuration;
 //  }

 //  // Get the bounds for the movie
 //  ::GetMovieBox(m_movie, &m_srcRect);
 //  // OffsetRect(&m_srcRect,  -m_srcRect.left,  -m_srcRect.top);
 //  SetMovieBox(m_movie, &m_srcRect);
 //  m_image.image.xsize = m_srcRect.right - m_srcRect.left;
 //  m_image.image.ysize = m_srcRect.bottom - m_srcRect.top;

 //  switch(m_wantedFormat) {
 //    default: // if no other format is requested, use YUV
 //    case GL_YCBCR_422_APPLE:
 //      post("YUV");
 //      m_image.image.format = m_wantedFormat;
 //      hints |= hintsHighQuality | hintsDeinterlaceFields;
 //      pixelformat=k422YpCbCr8CodecType;
 //      break;
 //    case GL_BGRA_EXT:
 //      post("BGRA");
 //      m_image.image.format = GL_BGRA_EXT;
 //      hints |= hintsHighQuality;
 //      pixelformat=k32ARGBPixelFormat;
 //      break;
 //    case GL_RGBA:
 //      post("RGBA");
 //      m_image.image.format = GL_RGBA;
 //      hints |= hintsHighQuality;
 //      pixelformat=k32RGBAPixelFormat;
 //      break;
 //  }
 //  m_image.image.setCsizeByFormat();
 //  m_image.image.allocate();

 //  m_rowBytes = m_image.image.xsize * m_image.image.csize;
 //  SetMoviePlayHints(m_movie, hints, hints);
 //  err = SetMovieAudioMute(m_movie, true, 0);
 //  if(noErr!=err) {
 //    error("filmAVFoundation: unable to mute movie...");
 //  }

 //  err = QTNewGWorldFromPtr(	&m_srcGWorld,
	// 			pixelformat,
	// 			&m_srcRect,
	// 			NULL,
	// 			NULL,
	// 			0,
	// 			m_image.image.data,
	// 			m_rowBytes);
 //  if (err) {
 //    error("filmAVFoundation: Couldn't make QTNewGWorldFromPtr %d", err);
 //    goto unsupported;
 //  }

 //  // *** set the graphics world for displaying the movie ***
 //  ::SetMovieGWorld(m_movie, m_srcGWorld, GetGWorldDevice(m_srcGWorld));
 //  if(GetMoviesError()){
 //    close();
 //    goto unsupported;
 //  }

 //  if(m_frameDuration)
 //    SetMovieRate(m_movie,X2Fix(m_auto));
 //  else if (m_auto == 0.f)
 //    SetMovieRate(m_movie,X2Fix(1.f));

 //  ::MoviesTask(m_movie, 0);	// *** this does the actual drawing into the GWorld ***

 //  m_readNext=true;

 //  return true;

 // unsupported:
 //  return false;

}

/////////////////////////////////////////////////////////
// getFrame
//
/////////////////////////////////////////////////////////
pixBlock* filmAVFoundation :: getFrame(void)
{
  m_image.newimage = [m_moviePlayer update];
  if(m_image.newimage) {
    [m_moviePlayer pixels:m_image.image.data];
  }

  // set the time for the frame and give time to the movie toolbox
  if(m_frameDuration > 0) {
    m_moviePlayer.position = m_movieTime/m_moviePlayer.duration;
  }
  m_image.image.upsidedown=true;
  m_moviePlayer.playbackRate = (float) m_auto;

  m_readNext = !m_auto;
  return &m_image;


  // CGrafPtr	savedPort;
  // GDHandle     	savedDevice;
  // Rect		m_srcRect;

  // ::GetGWorld(&savedPort, &savedDevice);
  // ::SetGWorld(m_srcGWorld, NULL);
  // ::GetMovieBox(m_movie, &m_srcRect);

  // // set the time for the frame and give time to the movie toolbox
  // if(m_frameDuration>0)
  //   SetMovieTimeValue(m_movie, m_movieTime);
  // MoviesTask(m_movie, 0);	// *** this does the actual drawing into the GWorld ***
  // m_image.newimage = m_readNext;

  // m_image.image.upsidedown=true;
  // SetMovieRate(m_movie,X2Fix(m_auto));

  // m_readNext=(m_auto!=0.f);
  // return &m_image;
}

film::errCode filmAVFoundation :: changeImage(int imgNum, int trackNum){
  m_readNext = false;
  if(imgNum == -1) {
    imgNum = m_curFrame;
  }
  if(m_numFrames > 1 && imgNum >= m_numFrames) {
    m_movieTime = 0;
    return film::FAILURE;
  }
  //if(trackNum == -1 || trackNum>m_numTracks)trackNum=m_curTrack;
  m_readNext=true;

  if(imgNum != m_curFrame && m_frameDuration < 1) {
  //  SetMovieRate(m_movie,X2Fix(1.0));
    m_moviePlayer.playbackRate = 1.0;
  }

  m_curFrame = imgNum;

  /* i have moved the "auto"-thing into [pix_film].
   * this is good because the decoder-class need not care about auto-play anylonger
   * this is bad, because we might do it more sophisticated in the decoder-class
   */
  m_movieTime = static_cast<long>(m_curFrame * m_frameDuration);

  return film::SUCCESS;
}

///////////////////////////////
// Properties
bool filmAVFoundation::enumProperties(gem::Properties&readable,
			      gem::Properties&writeable) {
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

void filmAVFoundation::setProperties(gem::Properties&props) {
}

void filmAVFoundation::getProperties(gem::Properties&props) {
  std::vector<std::string> keys=props.keys();
  gem::any value;
  double d;
  unsigned int i=0;
  for(i=0; i<keys.size(); i++) {
    std::string key=keys[i];
    props.erase(key);
    if("fps"==key) {
      d=m_fps;
      value=d; props.set(key, value);
    }
    if("frames"==key && m_numFrames>=0) {
      d=m_numFrames;
      value=d; props.set(key, value);
    }
    if("tracks"==key) {// && m_numTracks>=0) {
      //d=m_numTracks;
      d = 0;
      value=d; props.set(key, value);
    }
    if("width"==key) {
      d=m_image.image.xsize;
      value=d; props.set(key, value);
    }
    if("height"==key) {
      d=m_image.image.ysize;
      value=d; props.set(key, value);
    }
  }
}


// #ifdef LOADRAM
// //////////
// // load film into RAM
// void filmAVFoundation :: LoadRam(void){
//   TimeValue	length;
//   OSErr err;
//   if (m_haveMovie){
//     m_movieTime = 0;
//     length = GetMovieDuration(m_movie);
//     err =LoadMovieIntoRam(m_movie,m_movieTime,length,keepInRam);
//     if (err) {
//       post("filmAVFoundation: LoadMovieIntoRam failed miserably");
//     }
//   }else{
//     post("filmAVFoundation: no movie to load into RAM!");
//   }
// }
// #endif // LoadRAM
// #endif // HAVE_QT
