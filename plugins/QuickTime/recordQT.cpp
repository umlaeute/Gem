/*
 *  recordQT.cpp
 *  GEM_darwin
 *
 */
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "recordQT.h"
#include "plugins/PluginFactory.h"
#include "Gem/Exception.h"
#include "Gem/RTE.h"
#include "Utils/wstring.h"

using namespace gem::plugins;

#ifdef __APPLE__
# include <sys/types.h>
# include <unistd.h>
# include <fcntl.h>
#elif defined _WIN32
//apparently on OSX there is no member portPixMap in a GWorld so a function is used instead
# define GetPortPixMap(x) (x)->portPixMap
#endif

#ifdef _MSC_VER  /* This is only for Microsoft's compiler, not cygwin, e.g. */
# define snprintf _snprintf
# define vsnprintf _vsnprintf
#endif


#include <stdio.h>

/* for post() and pd_error() */
#include <m_pd.h>

static char* FourCC2Str(int code, char*char5)
{
  char5[0] = (code >> 24) & 0xFF;
  char5[1] = (code >> 16) & 0xFF;
  char5[2] = (code >>  8) & 0xFF;
  char5[3] = (code >>  0) & 0xFF;
  char5[4] = 0;
  return char5;
}


static bool touch(const std::string&filename)
{
#ifdef __APPLE__
  int fd;
  fd = open(filename.c_str(), O_CREAT | O_RDWR, 0600);
  if (fd < 0) {
    return false;
  }
  write(fd, " ", 1);
  close(fd);

  return true;
#elif defined _WIN32
  FILE*outfile=fopen(filename.c_str(), "");
  if(NULL==outfile) {
    return false;
  }
  fclose(outfile);
  return true;
#endif
  return false;
}

REGISTER_RECORDFACTORY("QuickTime", recordQT);
/////////////////////////////////////////////////////////
//
// recordQT
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
recordQT :: recordQT(void)
  : m_recordSetup(false)
  , m_recordStart(false)
  , m_recordStop(false)
  , m_width(-1), m_height(-1)
  , m_prevWidth(0), m_prevHeight(0)
  , m_compressImage(NULL)
  , seconds(0.f)
  , m_ticks(20)
  , m_firstRun(true)
  , m_rowBytes(0)
  , dataSize(0)
  , m_depth(0)
  , m_frameRate(0), m_keyFrameRate(0)
  , m_spatialQuality(codecNormalQuality)
  , nFileRefNum(0), nResID(0)
{
  static bool firsttime=true;
  if(firsttime) {
    OSErr               err = noErr;
#ifdef _WIN32
    // Initialize QuickTime Media Layer
    if ((err = InitializeQTML(0))) {
      throw(GemException("unable to initialize QuickTime"));
    }
#endif // WINDOWS
    // start QuickTime
    if (err = EnterMovies()) {
      throw(GemException("unable to initialize QuickTime/Movies"));
    }
    logpost(0, 3+1, "[GEM:recordQT] QT init done");
    firsttime=false;
  }

  //get list of codecs installed  -- useful later
  CodecNameSpecListPtr codecList;
  CodecNameSpec codecName;
  int count;

  GetCodecNameList(&codecList,1);
  count=codecList->count;
  codecContainer.clear();
  logpost(0, 3+0, "[GEM:recordQT] %i codecs installed",codecList->count);
  for (int i = 0; i < count; i++) {
    codecName = codecList->list[i];
    std::string typeName = std::string((char*)codecName.typeName + 1, ((char*)codecName.typeName)[0]);
    codecListStorage cod = {i, codecName.cType, typeName, codecName.codec};
    codecContainer.push_back(cod);
  }

  //initialize member variables
  stdComponent = NULL;
  hImageDesc = NULL;
  nResID = movieInDataForkResID;
  m_codecType = kJPEGCodecType;

  for(int i = 0; i < count; i++) {
    if (codecContainer[i].ctype == kJPEGCodecType) {
      m_codec = codecContainer[i].codec;
      logpost(0, 3+1, "[GEM:recordQT] found pjpeg codec %i %lu %p ctype",
              i, m_codecType, m_codec);
      break;
    }
  }

  stdComponent = OpenDefaultComponent(StandardCompressionType,
                                      StandardCompressionSubType);
  if (stdComponent == NULL) {
    logpost(0, 3+0, "[GEM:recordQT] failed to open compressor component");
  }
}

////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
recordQT :: ~recordQT(void)
{
  ComponentResult                       compErr = noErr;
  if (stdComponent != NULL) {
    compErr = CloseComponent(stdComponent);
    if (compErr != noErr) {
      logpost(0, 3+0, "[GEM:recordQT] CloseComponent failed with error %ld", compErr);
    }
  }
}

////////////////////////////////////////////////////////
// Prepares QT for recording
//
/////////////////////////////////////////////////////////
void recordQT :: setupQT(
  void) //this only needs to be done when codec info changes
{
  FSSpec        theFSSpec;
  OSErr         err = noErr;
  FSRef         ref;
  OSType                colorspace;
  ComponentResult                       compErr = noErr;

  m_recordSetup = false; //if it fails then there is no setup

  //this mess should create and open a file for QT to use
  //probably should be a separate function

  if (m_filename.empty()) {
    pd_error(0, "[GEM:recordQT] no filename passed");
    return;
  }
  std::string filename = gem::string::utf8string_to_nativestring(m_filename);

  if (!m_compressImage) {
    pd_error(0, "[GEM:recordQT] no image to record");
    return;
  }
  touch (filename);
#ifdef __APPLE__
  do {
    const UInt8*filename8=reinterpret_cast<const UInt8*>(filename.c_str());
    err = ::FSPathMakeRef(filename8, &ref, NULL);
    if (err) {
      pd_error(0, "[GEM:recordQT] Unable to make file ref from filename %s",
               m_filename.c_str());
      return ;
    }
    err = FSGetCatalogInfo(&ref, kFSCatInfoNodeFlags, NULL, NULL, &theFSSpec,
                           NULL);
    if (err != noErr) {
      pd_error(0, "[GEM:recordQT] error#%d in FSGetCatalogInfo()", err);
      return ;
    }
    err = FSMakeFSSpec(theFSSpec.vRefNum, theFSSpec.parID, filename8,
                       &theFSSpec);
    if (err != noErr && err != -37) { /* -37: invalid filename (e.g.  non-existent) */
      pd_error(0, "[GEM:recordQT] error#%d in FSMakeFSSpec()", err);
      return;
    }
  } while(0);
#elif defined _WIN32
  do {
    err = ::NativePathNameToFSSpec (const_cast<char*>(filename.c_str()), &theFSSpec, 0);
    if (err != noErr && err != -37) {
      pd_error(0, "[GEM:recordQT] error#%d in NativePathNameToFSSpec()", err);
      return;
    }
    err = CreateMovieFile(        &theFSSpec,
                                  FOUR_CHAR_CODE('TVOD'),
                                  smSystemScript,
                                  createMovieFileDeleteCurFile |
                                  createMovieFileDontCreateResFile,
                                  &nFileRefNum,
                                  &m_movie);
    err = CloseMovieFile(nFileRefNum);
    err = ::NativePathNameToFSSpec (const_cast<char*>(filename.c_str()), &theFSSpec, 0);

  } while(0);
#endif    /* OS */

  //create the movie from the file
  err = CreateMovieFile(        &theFSSpec,
                                FOUR_CHAR_CODE('TVOD'),
                                smSystemScript,
                                createMovieFileDeleteCurFile |
                                createMovieFileDontCreateResFile,
                                &nFileRefNum,
                                &m_movie);
  switch(err) {
  case noErr:
    break;
  case -37:
    pd_error(0, "[GEM:recordQT] invalid filename '%s'", m_filename.c_str());
    return;
  default:
    pd_error(0, "[GEM:recordQT] CreateMovieFile failed with error %d",err);
    return;
  }

  //give QT the dimensions of the image to compress
  m_srcRect.top = 0;
  m_srcRect.left = 0;
  m_srcRect.bottom = m_height;
  m_srcRect.right = m_width;

  if (m_compressImage->format == GEM_YUV) {
    logpost(0, 3+0, "[GEM:recordQT] using YUV");
    colorspace = k422YpCbCr8CodecType;
  }
  if (m_compressImage->format == GEM_RGBA) {
    logpost(0, 3+0, "[GEM:recordQT] using BGRA");
    colorspace = k32ARGBPixelFormat;
  }
#ifdef _WIN32
  colorspace = k32RGBAPixelFormat;
#endif

  char char5[5];
  switch(colorspace) {
  case k32ARGBPixelFormat:
  case k32BGRAPixelFormat:
  case k32RGBAPixelFormat:
    m_rowBytes = m_width * 4;
    break;
  case k422YpCbCr8CodecType:
    m_rowBytes = m_width * 2;
    break;
  default:
    pd_error(0, "[GEM:recordQT] unknown colorspace '%s'", FourCC2Str(colorspace, char5));
    m_rowBytes = m_width;
    break;
  }

  //give QT the length of each pixel row in bytes (2 for 4:2:2 YUV)
  err = QTNewGWorldFromPtr(&m_srcGWorld,
                           colorspace,
                           &m_srcRect,
                           NULL,
                           NULL,
                           0,
                           m_compressImage->data,
                           m_rowBytes);

  if (err != noErr) {
    pd_error(0, "[GEM:recordQT] QTNewGWorldFromPtr failed with error %d",err);
    return;
  }
  SetMovieGWorld(m_movie,m_srcGWorld,GetGWorldDevice(m_srcGWorld));

#ifdef __APPLE__
  //there is a discrepancy between what is really upside down and not.
  //since QT has flipped Y compared to GL it is upside down to GL but not to itself
  //so while the upsidedown flag is set for QT images sent to GL it is not correct for pix_ processing.
  //this is a hack on OSX since the native is YUV for pix_ and the only BGRA will usually be from pix_snap
  if (m_compressImage->upsidedown && m_compressImage->format == GEM_RGBA) {
    MatrixRecord        aMatrix;
    GetMovieMatrix(m_movie,&aMatrix);
    logpost(0, 3+1, "[GEM:recordQT] upside down");
    ScaleMatrix(&aMatrix,Long2Fix(1),Long2Fix(-1),0,0);
    SetMovieMatrix(m_movie,&aMatrix);
  }
#elif defined _WIN32
  MatrixRecord  aMatrix;
  GetMovieMatrix(m_movie,&aMatrix);
  ScaleMatrix(&aMatrix,Long2Fix(1),Long2Fix(-1),0,0);
  SetMovieMatrix(m_movie,&aMatrix);
#endif

  track = NewMovieTrack(m_movie,FixRatio(m_srcRect.right, 1),
                        FixRatio(m_srcRect.bottom, 1),kNoVolume);
  media = NewTrackMedia(track,VideoMediaType,600,nil,0);

  SpatialSettings.codecType = m_codecType;
  SpatialSettings.codec = m_codec;
  SpatialSettings.depth = m_depth;
  SpatialSettings.spatialQuality = m_spatialQuality;

  TemporalSettings.temporalQuality = m_spatialQuality;
  TemporalSettings.frameRate = m_frameRate;
  TemporalSettings.keyFrameRate = m_keyFrameRate;

  datarate.frameDuration = 33;

  compErr = SCSetInfo(stdComponent, scTemporalSettingsType,
                      &TemporalSettings);
  compErr = SCSetInfo(stdComponent, scSpatialSettingsType, &SpatialSettings);
  compErr = SCSetInfo(stdComponent, scDataRateSettingsType, &datarate);

  if (compErr != noErr) {
    pd_error(0, "[GEM:recordQT] SCSetInfo failed with error#%ld", compErr);
  }

  compErr = SCCompressSequenceBegin(stdComponent,GetPortPixMap(m_srcGWorld),
                                    &m_srcRect,&hImageDesc);
  if (compErr != noErr) {
    pd_error(0, "[GEM:recordQT] SCCompressSequenceBegin failed with error#%ld",
             compErr);
    return;
  }

  err = BeginMediaEdits(media);
  if (err != noErr) {
    pd_error(0, "[GEM:recordQT] BeginMediaEdits failed with error#%d",err);
    return;
  }

  //this will show that everything is OK for recording
  m_recordSetup = true;

  //set the previous dimensions for the sanity check during compression
  m_prevWidth = m_width;
  m_prevHeight = m_height;

  //reset frame counter for new movie file
}

//
// stops recording into the QT movie
//
void recordQT :: stop(void)
{
  ComponentResult                       compErr = noErr;
  OSErr                                 err;
  m_recordStart = false; //just to be sure

  err = EndMediaEdits(media);

  if (err != noErr) {
    pd_error(0, "[GEM:recordQT] EndMediaEdits failed with error %d",err);
    return;  //no sense in crashing after this
  }

  err = InsertMediaIntoTrack(track,0,0,GetMediaDuration(media),0x00010000);
  if (err != noErr) {
    pd_error(0, "[GEM:recordQT] InsertMediaIntoTrack failed with error %d", err);
  }

  err = AddMovieResource(m_movie,nFileRefNum,&nResID,NULL);
  if (err != noErr) {
    pd_error(0, "[GEM:recordQT] AddMovieResource failed with error %d", err);
  }

  err = CloseMovieFile(nFileRefNum);
  if (err != noErr) {
    pd_error(0, "[GEM:recordQT] CloseMovieFile failed with error %d", err);
  }

  DisposeMovie(m_movie);
  DisposeGWorld(m_srcGWorld);
  m_srcGWorld = NULL;

  compErr = SCCompressSequenceEnd(stdComponent);

  if (compErr != noErr) {
    pd_error(0, "[GEM:recordQT] SCCompressSequenceEnd failed with error %ld",
             compErr);
  }

  m_recordStop = false;
  m_recordSetup = false;
  m_firstRun = 1;

  logpost(0, 3+0, "[GEM:recordQT] movie written to %s",m_filename.c_str());
  m_filename.clear();
}

void recordQT :: compressFrame(void)
{
  OSErr                 err;
  Handle                compressedData; //data to put in QT mov
  ComponentResult       compErr = noErr;
  short                 syncFlag; //flag for keyframes

#ifdef __APPLE__
  //fakes the first run time
  if (m_firstRun) {
    ::Microseconds(&startTime);
    m_firstRun = 0;
  }
  ::Microseconds(&endTime);

  seconds = static_cast<float>(endTime.lo - startTime.lo) / 1000000.f;
  m_ticks = static_cast<int>(600 * seconds);
  if (m_ticks < 20) {
    m_ticks = 20;
  }

#endif //timers

#ifdef _WIN32
  static float countFreq = 0;
  if (m_firstRun) {
    // LARGE_INTEGER freq;
    if (!QueryPerformanceFrequency(&freq)) {
      countFreq = 0;
    } else {
      countFreq = static_cast<float>(freq.QuadPart);
    }
    QueryPerformanceCounter(&startTime);//fakes the time of the first frame
    m_ticks = 20;
    m_firstRun = 0;
  } else {
    QueryPerformanceCounter(&endTime);
    float fps = 1000 / (static_cast<float>(endTime.QuadPart -
                                           startTime.QuadPart)/countFreq * 1000.f);
    seconds = (static_cast<float>(endTime.QuadPart -
                                  startTime.QuadPart)/countFreq * 1.f);
    logpost(0, 3+1,
            "[GEM:recordQT] freq %f countFreq %f startTime %d endTime %d fps %f seconds %f",
            freq, countFreq, static_cast<int>(startTime.QuadPart),
            static_cast<int>(endTime.QuadPart), fps, seconds);

    m_ticks = static_cast<int>(600 * seconds);
    if (m_ticks < 20) {
      m_ticks = 20;
    }
  }
#endif

  compErr = SCCompressSequenceFrame(stdComponent,
                                    GetPortPixMap(m_srcGWorld),
                                    &m_srcRect,
                                    &compressedData,
                                    &dataSize,
                                    &syncFlag);

  if (compErr != noErr) {
    pd_error(0, "[GEM:recordQT] SCCompressSequenceFrame failed with error %ld",
             compErr);
  }

  err = AddMediaSample(media,
                       compressedData,
                       0,
                       dataSize,
                       m_ticks, //this should not be a fixed value but vary with framerate
                       reinterpret_cast<SampleDescriptionHandle>(hImageDesc),
                       1,
                       syncFlag,
                       NULL);

  if (err != noErr) {
    pd_error(0, "[GEM:recordQT] AddMediaSample failed with error %d",err);
  }

#ifdef __APPLE__
  ::Microseconds(&startTime);
#endif //timer

#ifdef _WIN32
  QueryPerformanceCounter(&startTime);
#endif
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
bool recordQT :: write(imageStruct*img)
{
  //check if state exists
  if(!img) {
    return false;
  }
  m_compressImage = img;
  m_height = img->ysize;
  m_width = img->xsize;
  //record
  if (m_recordStart) {
    //if setupQT() has not been run do that first
    if (!m_recordSetup) {
      setupQT();
      if(!m_recordSetup) {
        /* failed! */
        m_recordStop = true;
        return false;
      }
    }

    //should check if the size has changed or else we will freak the compressor's trip out
    if (m_width == m_prevWidth && m_height == m_prevHeight) {
      //go ahead and grab a frame if everything is ready to go
      if (m_recordSetup) {
        compressFrame();
      }
    } else {
      pd_error(0, "[GEM:recordQT] movie dimensions changed prev %dx%d now %dx%d stopping recording",
               m_prevWidth,m_prevHeight,m_width,m_height);
      m_recordStop = true;
      m_prevWidth = m_width;
      m_prevHeight = m_height; //go ahead and change dimensions
    }
  }

  //if recording is stopped and everything is setup then stop recording
  if (m_recordStop) {
    //guard against someone not setting up QT beforehand
    if (!m_recordSetup) {
      return false;
    }
    stop();
  }
  return true;
}

/////////////////////////////////////////////////////////
// dialogMess
//
/////////////////////////////////////////////////////////
bool recordQT :: dialog(void)
{
  //if recording is going, do not open the dialog
  if (!m_recordStart) {
    ComponentResult                     compErr = noErr;

    //close the component if already open
    if (stdComponent) {
      compErr = CloseComponent(stdComponent);
    }
    if (compErr != noErr) {
      logpost(0, 3+0, "[GEM:recordQT] CloseComponent failed with error %ld", compErr);
    }

    //open a new component from scratch
    stdComponent = OpenDefaultComponent(StandardCompressionType,
                                        StandardCompressionSubType);

    if (stdComponent == NULL) {
      pd_error(0, "[GEM:recordQT] failed to open compressor component");
      return false;
    }

    compErr = SCRequestSequenceSettings(stdComponent);

    if (compErr != noErr) {
      logpost(0, 3+0, "[GEM:recordQT] SCRequestSequenceSettings failed with error %ld",
              compErr);
    }

    compErr = SCGetInfo(stdComponent, scTemporalSettingsType,
                        &TemporalSettings);
    compErr = SCGetInfo(stdComponent, scSpatialSettingsType, &SpatialSettings);

    if (compErr != noErr) {
      logpost(0, 3+0, "[GEM:recordQT] SCGetInfo failed with error %ld", compErr);
    }

    m_codecType = SpatialSettings.codecType;
    m_depth = SpatialSettings.depth;
    m_spatialQuality = SpatialSettings.spatialQuality;
    m_codec = SpatialSettings.codec;
    m_frameRate = TemporalSettings.frameRate;
    m_keyFrameRate = TemporalSettings.keyFrameRate;

    logpost(0, 3+1, "[GEM:recordQT] Dialog returned SpatialSettings\n"
            "\tcodecType %lX\n"
            "\tcodec %p\n"
            "\tdepth %d\n"
            "\tspatialQuality %ld",
            SpatialSettings.codecType, SpatialSettings.codec, SpatialSettings.depth,
            SpatialSettings.spatialQuality);
    logpost(0, 3+1, "[GEM:recordQT] Dialog returned TemporalSettings\n"
            "\ttemporalQualitye %ld\n"
            "\tframeRate %ld\n"
            "\tkeyFrameRate %ld",
            TemporalSettings.temporalQuality, TemporalSettings.frameRate,
            TemporalSettings.keyFrameRate);
    return(true);
  }
  pd_error(0, "[GEM:recordQT] recording is running; refusing to show up dialog...!");
  return(false);
}

/////////////////////////////////////////////////////////
// spits out a list of installed codecs and stores them
//
/////////////////////////////////////////////////////////
int recordQT :: getNumCodecs(void)
{
  //get list of codecs installed  -- useful later
  return(codecContainer.size());
}
const char*recordQT :: getCodecName(int i)
{
  if(i<0 || i>codecContainer.size()) {
    return NULL;
  }
  return (codecContainer[i].name.c_str());
}

std::vector<std::string>recordQT::getCodecs(void)
{
  std::vector<std::string>result;
  for(int i=0; i<codecContainer.size(); i++) {
    result.push_back(codecContainer[i].name);
  }
  return result;
}
const std::string recordQT::getCodecDescription(const std::string&codec)
{
  return(codec);
}
bool recordQT::enumProperties(gem::Properties&props)
{
  props.clear();
  return false;
}


/////////////////////////////////////////////////////////
// deals with the name of a codec
//
/////////////////////////////////////////////////////////
bool recordQT :: setCodec(int num)
{
  if(num<0 || num>codecContainer.size()) {
    return false;
  }
  resetCodecSettings();
  m_codecType = codecContainer[num].ctype;
  m_codec     = codecContainer[num].codec;
  return true;
}
bool recordQT :: setCodec(const std::string&codecName)
{
  int requestedCodec=0;
  if(codecName=="pjpeg") {
    requestedCodec=1;
  } else if(codecName=="aic") {
    requestedCodec=2;
  } else if(codecName=="anim") {
    requestedCodec=3;
  } else if(codecName=="dvntsc") {
    requestedCodec=4;
  } else if(codecName=="dvpal") {
    requestedCodec=5;
  }

  for(int i=0; i < codecContainer.size(); i++)  {
    switch(requestedCodec) {
    case 1: /* PJPEG */
      if (codecContainer[i].ctype == kJPEGCodecType) {
        logpost(0, 3+0, "[GEM:recordQT] found Photo Jpeg");
        resetCodecSettings();
        m_codecType = codecContainer[i].ctype;
        m_codec     = codecContainer[i].codec;
        return true;
      }
      break;
    case 2: /* AIC */
      if (static_cast<int>(codecContainer[i].ctype) == 'icod') {
        logpost(0, 3+0, "[GEM:recordQT] found Apple Intermediate Codec");
        resetCodecSettings();
        m_codecType = codecContainer[i].ctype;
        m_codec     = codecContainer[i].codec;
        return true;
      }
      break;
    case 3: /* Animation */
      if (codecContainer[i].ctype == kAnimationCodecType) {
        logpost(0, 3+0, "[GEM:recordQT] found Animation");
        resetCodecSettings();
        m_codecType = codecContainer[i].ctype;
        m_codec     = codecContainer[i].codec;
        return true;
      }
      break;
    case 4: /* DV NTSC */
      if (codecContainer[i].ctype == kDVCNTSCCodecType) {
        logpost(0, 3+0, "[GEM:recordQT] found DV NTSC");
        resetCodecSettings();
        m_codecType = codecContainer[i].ctype;
        m_codec     = codecContainer[i].codec;
        return true;
      }
      break;
    case 5: /* DV PAL */
      if (codecContainer[i].ctype == kDVCPALCodecType) {
        logpost(0, 3+0, "[GEM:recordQT] found DV PAL");
        resetCodecSettings();
        m_codecType = codecContainer[i].ctype;
        m_codec     = codecContainer[i].codec;
        return true;
      }
      break;
    default:
      /* hmmm... */
      if(gensym(codecName.c_str())==gensym(codecContainer[i].name.c_str())) {
        logpost(0, 3+0, "[GEM:recordQT] found '%s'", codecName.c_str());
        resetCodecSettings();
        m_codecType = codecContainer[i].ctype;
        m_codec     = codecContainer[i].codec;
        return true;
      }
      break;
    }
  }

  //no codec found
  return false;
}

bool recordQT :: start(const std::string&filename, gem::Properties&props)
{
  // if recording is going, do not accept a new file name
  // on OSX changing the name while recording won't have any effect
  // but it will give the wrong message at the end if recording
  if (m_recordStart) {
    pd_error(0, "[GEM:recordQT] cannot set filename while recording is running!");
    return false;
  }
  m_filename = filename;
  m_recordStart=true;
  return true;
}

void recordQT :: resetCodecSettings(void)
{
  m_codecType = 0;
  m_codec = 0;

  m_depth = 0;
  m_spatialQuality = codecNormalQuality;
  m_frameRate = 0;
  m_keyFrameRate = 0;
}
