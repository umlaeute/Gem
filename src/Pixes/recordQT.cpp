/*
 *  recordQT.cpp
 *  GEM_darwin
 *
 */

#include "recordQT.h"

#ifdef __APPLE__
# include <sys/types.h>
# include <unistd.h>
# include <fcntl.h>
#elif defined __WIN32__
//apparently on OSX there is no member portPixMap in a GWorld so a function is used instead
# define GetPortPixMap(x) (x)->portPixMap
#endif

#include <stdio.h>

/* for post() and error() */
#include "m_pd.h"

/////////////////////////////////////////////////////////
//
// recordQT
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
#ifndef HAVE_QUICKTIME
recordQT :: recordQT(int x, int y, int w, int h) : record(x, y, w, h){}
recordQT :: ~recordQT(){}
#else
recordQT :: recordQT(int x, int y, int w, int h)
  : record(x, y, w, h),
    m_recordSetup(false),
    m_dialog(false),
    m_prevHeight(0), m_prevWidth(0),
    m_compressImage(NULL),
#ifdef __APPLE__
    startTime(0), endTime(0),
#elif __WIN32__
    freq(0), startTime(0), endTime(0),
#endif
    seconds(0.f),
    m_ticks(20),
    m_firstRun(true),
    m_rowBytes(0),
    dataSize(0),
    m_depth(0),
    m_codecSet(false),
    m_codecQualitySet(false),
    nFileRefNum(0), mResID(0),
    codecContainer(NULL), numCodecContainer(0)
{
  post("using recordQT");

  m_filename[0] = 0;

# ifdef __WIN32__
  // Initialize QuickTime Media Layer
  /* isn't this done in GemMan/GemWinCreateNT already? */
  OSErr		err = noErr;
  if ((err = InitializeQTML(0))) {
    post("recordQT: Could not initialize quicktime: error %d", err);
    return;
  }
	
  // Initialize QuickTime
  if (err = EnterMovies()) {
    error("recordQT: Could not initialize quicktime: error %d", err);
    return;
  }
  post("recordQT: QT init done");
# endif // WINDOWS

  //get list of codecs installed  -- useful later
  CodecNameSpecListPtr codecList;
  CodecNameSpec	codecName;
  int	i;
  int count;
	
  GetCodecNameList(&codecList,1);
  count=codecList->count;
  codecContainer=new codecListStorage[count];
  numCodecContainer=count;
  post("recordQT: %i codecs installed",codecList->count);
  for (i = 0; i < count; i++){
    codecName = codecList->list[i];
    codecContainer[i].position = i;
    codecContainer[i].ctype = codecName.cType;
    codecContainer[i].codec = codecName.codec;
    if(codecName.typeName) {
	  int namelength=*(codecName.typeName);
	  char*name=new char[namelength+1];
	  strncpy(name, ((char*)codecName.typeName)+1, namelength);
	  name[namelength]=0;
	  t_symbol*s=gensym(name);
      codecContainer[i].name = s->s_name;
	  //post("codec: '%s' %d", name, namelength);
	  free(name);
    } else {
      codecContainer[i].name = NULL;
    }
  }

  //initialize member variables
  stdComponent = NULL;
  hImageDesc = NULL;
  nResID = movieInDataForkResID;
  m_recordStart = 0;
  m_recordStop = 0;
  m_codecType = kJPEGCodecType;

  for(i = 0; i < count; i++){
    if (codecContainer[i].ctype == kJPEGCodecType) {
      m_codec = codecContainer[i].codec;
      post("recordQT: found pjpeg codec %i %i %i ctype", i, m_codecType, m_codec);
      break;
    }
  }

  m_codecSet = true;
  m_spatialQuality = codecNormalQuality; //codecHighQuality;
  m_codecQualitySet = true;
  m_currentFrame = 0;

  stdComponent = OpenDefaultComponent(StandardCompressionType,StandardCompressionSubType);
  if (stdComponent == NULL){
    error("recordQT failed to open compressor component");
    return;
  }
}

////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
recordQT :: ~recordQT()
{
  ComponentResult			compErr = noErr;
  if (stdComponent != NULL){
    compErr = CloseComponent(stdComponent);
    if (compErr != noErr) error("recordQT: CloseComponent failed with error %d",compErr);
  }
}

////////////////////////////////////////////////////////
// Prepares QT for recording
//
/////////////////////////////////////////////////////////
void recordQT :: setupQT() //this only needs to be done when codec info changes
{
  FSSpec	theFSSpec;
  OSErr		err = noErr;
  FSRef		ref;
  OSType		colorspace;
  ComponentResult			compErr = noErr;

  m_recordSetup = 0; //if it fails then there is no setup
	
  //this mess should create and open a file for QT to use
  //probably should be a separate function
  //post("filename %s",m_filename);

  if (!m_filename || !m_filename[0]) {
    error("recordQT:  no filename passed");
    return;
  }
  if (!m_compressImage) {
    error("recordQT:  no image to record");
    return;
  }

#ifdef __APPLE__
  else {
    err = ::FSPathMakeRef((UInt8*)m_filename, &ref, NULL);
    if (err == fnfErr) {
      // if the file does not yet exist, then let's create the file
      int fd;
      fd = ::open(m_filename, O_CREAT | O_RDWR, 0600);
      if (fd < 0){
        error("recordQT: problem with fd");
        return ;
      }
      ::write(fd, " ", 1);
      ::close(fd);
      err = FSPathMakeRef((UInt8*)m_filename, &ref, NULL);
    }
    if (err) {
      error("GEM: recordQT: Unable to make file ref from filename %s", m_filename);
      return ;
    }
    err = FSGetCatalogInfo(&ref, kFSCatInfoNodeFlags, NULL, NULL, &theFSSpec, NULL);
    if (err != noErr){
      error("GEM: recordQT: error %d in FSGetCatalogInfo()", err);
      return ;
    }
    err = FSMakeFSSpec(theFSSpec.vRefNum, theFSSpec.parID, (UInt8*)m_filename, &theFSSpec);		
    if (err != noErr && err != -37){ /* what is -37 */
      error("GEM: recordQT: error %d in FSMakeFSSpec()", err);
      return;
    }
  }
#elif defined __WIN32__
  else {
    c2pstr(m_filename);
    FSMakeFSSpec (0, 0L, (UInt8*)m_filename, &theFSSpec);
    if (err != noErr && err != -37){
      error("GEM: recordQT: error %d in FSMakeFSSpec()", err);
      return;
    }
  }
#endif    /* OS */

  //create the movie from the file
  err = CreateMovieFile(	&theFSSpec,
                          FOUR_CHAR_CODE('TVOD'),
                          smSystemScript,
                          createMovieFileDeleteCurFile |
                          createMovieFileDontCreateResFile,
                          &nFileRefNum,
                          &m_movie);
  if (err != noErr) {
    error("recordQT: CreateMovieFile failed with error %d",err);
    return;
  }

  //give QT the dimensions of the image to compress
  m_srcRect.top = 0;
  m_srcRect.left = 0;
  m_srcRect.bottom = m_height;
  m_srcRect.right = m_width;
	
	if (m_compressImage->format == GL_YUV422_GEM){
    m_rowBytes = m_width * 2;
		colorspace = k422YpCbCr8CodecType;
		post("recordQT: using YUV");
  }
	if (m_compressImage->format == GL_BGRA){
    colorspace = k32BGRAPixelFormat;// k32RGBAPixelFormat;
		m_rowBytes = m_width * 4;
		post("recordQT: using BGRA");
	}
#ifdef __WIN32__
	colorspace = k32RGBAPixelFormat;
	m_rowBytes = m_width*4;
#endif
  //give QT the length of each pixel row in bytes (2 for 4:2:2 YUV)
  err = QTNewGWorldFromPtr(&m_srcGWorld,
                           colorspace,
                           &m_srcRect,
                           NULL,
                           NULL,
                           0,
                           m_compressImage->data,
                           m_rowBytes);

  if (err != noErr){
    error("recordQT: QTNewGWorldFromPtr failed with error %d",err);
    return;
  }
  SetMovieGWorld(m_movie,m_srcGWorld,GetGWorldDevice(m_srcGWorld));

#ifdef __APPLE__
  //there is a discrepency between what is really upside down and not.
  //since QT has flipped Y compared to GL it is upside down to GL but not to itself
  //so while the upsidedown flag is set for QT images sent to GL it is not correct for pix_ processing.
  //this is a hack on OSX since the native is YUV for pix_ and the only BGRA will usually be from pix_snap
	if (m_compressImage->upsidedown && m_compressImage->format == GL_BGRA) {
		MatrixRecord	aMatrix;
		GetMovieMatrix(m_movie,&aMatrix);
		verbose(2,"upside down");
		ScaleMatrix(&aMatrix,Long2Fix(1),Long2Fix(-1),0,0);
		SetMovieMatrix(m_movie,&aMatrix);
	}
#elif defined __WIN32__
	MatrixRecord	aMatrix;
	GetMovieMatrix(m_movie,&aMatrix);
	ScaleMatrix(&aMatrix,Long2Fix(1),Long2Fix(-1),0,0);
	SetMovieMatrix(m_movie,&aMatrix);
#endif

  track = NewMovieTrack(m_movie,FixRatio(m_srcRect.right, 1),FixRatio(m_srcRect.bottom, 1),kNoVolume);
  media = NewTrackMedia(track,VideoMediaType,600,NULL,0);

  if (m_dialog ){	
    //close the component if already open
    if (stdComponent) compErr = CloseComponent(stdComponent);
    if (compErr != noErr) error("recordQT: CloseComponent failed with error %d",compErr);		

    //open a new component from scratch
    stdComponent = OpenDefaultComponent(StandardCompressionType,StandardCompressionSubType);
	
    if (stdComponent == NULL){
      error("recordQT: failed to open compressor component");
      return;
    }

    post("recordQT: opening settings Dialog");
    compErr = SCRequestSequenceSettings(stdComponent);

    if (compErr != noErr) error("recordQT: SCRequestSequenceSettings failed with error %d",compErr);

    compErr = SCGetInfo(stdComponent, scTemporalSettingsType, &TemporalSettings);
    compErr = SCGetInfo(stdComponent, scSpatialSettingsType, &SpatialSettings);

    if (compErr != noErr) error("recordQT: SCGetInfo failed with error %d",compErr);

    m_codecType = SpatialSettings.codecType;
    m_depth = SpatialSettings.depth;
    m_spatialQuality = SpatialSettings.spatialQuality;
    m_codec = SpatialSettings.codec;

    post("recordQT: Dialog returned SpatialSettings.codecType %d",SpatialSettings.codecType);
    post("recordQT: Dialog returned SpatialSettings.codec %d",SpatialSettings.codec);
    post("recordQT: Dialog returned SpatialSettings.depth %d",SpatialSettings.depth);
    post("recordQT: Dialog returned SpatialSettings.spatialQuality %d",SpatialSettings.spatialQuality);
    post("recordQT: Dialog returned TemporalSettings.temporalQualitye %d",TemporalSettings.temporalQuality);
    post("recordQT: Dialog returned TemporalSettings.frameRate %d",TemporalSettings.frameRate);
    post("recordQT: Dialog returned TemporalSettings.keyFrameRate %d",TemporalSettings.keyFrameRate);

    m_dialog = false; //don't keep doing it again
  }else{
    //fill in manually
    SpatialSettings.codecType = m_codecType;
    SpatialSettings.codec = m_codec;
    SpatialSettings.depth = 0; //should choose best depth
    SpatialSettings.spatialQuality = m_spatialQuality;
		
    TemporalSettings.temporalQuality = m_spatialQuality;
    TemporalSettings.frameRate = 0;
    TemporalSettings.keyFrameRate = 0;
  }

  datarate.frameDuration = 33;

  compErr = SCSetInfo(stdComponent, scTemporalSettingsType, &TemporalSettings);
  compErr = SCSetInfo(stdComponent, scSpatialSettingsType, &SpatialSettings);
  compErr = SCSetInfo(stdComponent, scDataRateSettingsType, &datarate);

  if (compErr != noErr) error("recordQT: SCSetInfo failed with error %d",compErr);

  compErr = SCCompressSequenceBegin(stdComponent,GetPortPixMap(m_srcGWorld),&m_srcRect,&hImageDesc);
  if (compErr != noErr) {
    error("recordQT: SCCompressSequenceBegin failed with error %d",compErr);
    return;
  }
	
  err = BeginMediaEdits(media);
  if (err != noErr) {
    error("recordQT: BeginMediaEdits failed with error %d",err);
    return;
  }

  //this will show that everything is OK for recording
  m_recordSetup = 1;
	
  //set the previous dimensions for the sanity check during compression
  m_prevWidth = m_width;
  m_prevHeight = m_height;
	
  //reset frame counter for new movie file
  m_currentFrame = 0;
  post("recordQT: setup end");
}

//
// stops recording into the QT movie
//
void recordQT :: close()
{
  ComponentResult			compErr = noErr;
  OSErr					err;
  m_recordStart = 0; //just to be sure
	
  err = EndMediaEdits(media);

  if (err != noErr) {
    error("recordQT: EndMediaEdits failed with error %d",err);
    return;  //no sense in crashing after this
	}

  err = InsertMediaIntoTrack(track,0,0,GetMediaDuration(media),0x00010000);
  if (err != noErr) error("recordQT: InsertMediaIntoTrack failed with error %d",err);

  err = AddMovieResource(m_movie,nFileRefNum,&nResID,NULL);
  if (err != noErr) error("recordQT: AddMovieResource failed with error %d",err);

  err = CloseMovieFile(nFileRefNum);
  if (err != noErr) error("recordQT: CloseMovieFile failed with error %d",err);

  DisposeMovie(m_movie);
  DisposeGWorld(m_srcGWorld);
  m_srcGWorld = NULL;
		
  compErr = SCCompressSequenceEnd(stdComponent);
	
  if (compErr != noErr) error("recordQT: SCCompressSequenceEnd failed with error %d",compErr);

  m_recordStop = 0;
  m_recordSetup = 0;
	
  m_currentFrame = 0; //reset the frame counter?

  m_firstRun = 1;

  post("recordQT: movie written to %s",m_filename);
}

void recordQT :: compressFrame()
{
  OSErr			err;
  Handle		compressedData; //data to put in QT mov
  ComponentResult	compErr = noErr;
  short			syncFlag; //flag for keyframes

#ifdef __APPLE__
	//fakes the first run time
	if (m_firstRun){
	  ::Microseconds(&startTime);
		m_firstRun = 0;	
	}
	::Microseconds(&endTime);

	seconds = (float)(endTime.lo - startTime.lo) / 1000000.f;
	m_ticks = (int)(600 * seconds);
	if (m_ticks < 20) m_ticks = 20;
	
#endif //timers
	
#ifdef __WIN32__
	static int firstTime = 1;
	static float countFreq = 0;
	if (m_firstRun)
    {
      // LARGE_INTEGER freq;
      if (!QueryPerformanceFrequency(&freq))
        countFreq = 0;
      else
        countFreq = (float)(freq.QuadPart);
      QueryPerformanceCounter(&startTime);//fakes the time of the first frame
      m_ticks = 20;
      m_firstRun = 0;
    }else{
		QueryPerformanceCounter(&endTime);
		float fps = 1000 / ((float)(endTime.QuadPart - startTime.QuadPart)/countFreq * 1000.f);
		seconds = ((float)(endTime.QuadPart - startTime.QuadPart)/countFreq * 1.f);
		// post("pix_recordQT: freq %f countFreq %f startTime %d endTime %d fps %f seconds %f ",freq, countFreq,(int)startTime.QuadPart,(int)endTime.QuadPart,fps,seconds);

		m_ticks = (int)(600 * seconds);
		if (m_ticks < 20) m_ticks = 20;
	}
#endif

  //post("recordQT: compressing frame");
  compErr = SCCompressSequenceFrame(stdComponent,
                                    GetPortPixMap(m_srcGWorld),
                                    &m_srcRect,
                                    &compressedData,
                                    &dataSize,
                                    &syncFlag);

  if (compErr != noErr) error("recordQT: SCCompressSequenceFrame failed with error %d",compErr);
										
  err = AddMediaSample(media,
                       compressedData,
                       0,
                       dataSize,
                       m_ticks, //this should not be a fixed value but vary with framerate
                       (SampleDescriptionHandle)hImageDesc,
                       1,
                       syncFlag,
                       NULL);
							
  if (err != noErr) error("recordQT: AddMediaSample failed with error %d",err);

#ifdef __APPLE__
	::Microseconds(&startTime);					
#endif //timer		

#ifdef __WIN32__
  QueryPerformanceCounter(&startTime);
#endif

  m_currentFrame++;
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
int recordQT :: putFrame(imageStruct*img)
{
  //check if state exists
  if(!img)return (-1);
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
            m_recordStop = 1;
            return -1;
          }
      }
		
    //should check if the size has changed or else we will freak the compressor's trip out
    if (m_width == m_prevWidth && m_height == m_prevHeight) {
      //go ahead and grab a frame if everything is ready to go
      if (m_recordSetup)
        compressFrame();
    }else{
      error("recordQT: movie dimensions changed prev %dx%d now %dx%d stopping recording",m_prevWidth,m_prevHeight,m_width,m_height);
      m_recordStop = 1;
      m_prevWidth = m_width;
      m_prevHeight = m_height; //go ahead and change dimensions
    }
  }
	
  //if recording is stopped and everything is setup then stop recording
  if (m_recordStop){
    //guard against someone not setting up QT beforehand
    if (!m_recordSetup)	return(-1);
    close();
  }
  return (m_currentFrame);
}

/////////////////////////////////////////////////////////
// sizeMess
//
/////////////////////////////////////////////////////////
bool recordQT :: size(int width, int height)
{
  m_width = width;
  m_height = height;

  return true;
}

/////////////////////////////////////////////////////////
// posMess
//
/////////////////////////////////////////////////////////
bool recordQT :: position(int x, int y)
{
  m_xoff = x;
  m_yoff = y;
  return true;
}

/////////////////////////////////////////////////////////
// dialogMess
//
/////////////////////////////////////////////////////////
bool recordQT :: dialog()
{
  //if recording is going, do not open the dialog
  if (!m_recordStart) {
    post("recordQT: opening compression dialog");
    m_dialog = true;
    setupQT();
    return(true);
  }else{
    error("recordQT: recording is running; refusing to show up dialog...!");
    return(false);
  }
}

/////////////////////////////////////////////////////////
// spits out a list of installed codecs and stores them
//
/////////////////////////////////////////////////////////
int recordQT :: getNumCodecs()
{
  //get list of codecs installed  -- useful later
  return(numCodecContainer);
}
char*recordQT :: getCodecName(int i)
{
  if(i<0 || i>numCodecContainer)return NULL;
  return (codecContainer[i].name);
}


/////////////////////////////////////////////////////////
// deals with the name of a codec
//
/////////////////////////////////////////////////////////
bool recordQT :: setCodec(int num)
{
  if(num<0 || num>numCodecContainer)return false;

  m_codecType = codecContainer[num].ctype;
  m_codec     = codecContainer[num].codec;
  return true;
}
bool recordQT :: setCodec(char*codecName)
{
	int	i;
  int requestedCodec=0;
  if (!(strncmp(codecName,"pjpeg",5)))       requestedCodec=1;
  else if (!(strncmp(codecName,"aic",3)))    requestedCodec=2;
  else if (!(strncmp(codecName,"anim",4)))   requestedCodec=3;
  else if (!(strncmp(codecName,"dvntsc",6))) requestedCodec=4;
  else if (!(strncmp(codecName,"dvpal",5)))  requestedCodec=5;

	post("recordQT set %s",codecName);

	for(i=0; i < numCodecContainer; i++)  {
    switch(requestedCodec) {
    case 1: /* PJPEG */
      if (codecContainer[i].ctype == kJPEGCodecType) {
        post("recordQT found Photo Jpeg");
        m_codecType = codecContainer[i].ctype;
        m_codec     = codecContainer[i].codec;
        return true;
      }
      break;
    case 2: /* AIC */
      if ((int)codecContainer[i].ctype == 'icod') {
        post("recordQT found Apple Intermediate Codec");
        m_codecType = codecContainer[i].ctype;
        m_codec     = codecContainer[i].codec;
        return true;
      }
      break;
    case 3: /* Animation */
      if (codecContainer[i].ctype == kAnimationCodecType) {
        post("recordQT found Animation");
        m_codecType = codecContainer[i].ctype;
        m_codec     = codecContainer[i].codec;
        return true;
      }
      break;
    case 4: /* DV NTSC */
      if (codecContainer[i].ctype == kDVCNTSCCodecType) {
        post("recordQT found DV NTSC");
        m_codecType = codecContainer[i].ctype;
        m_codec     = codecContainer[i].codec;
        return true;
      }
      break;
    case 5: /* DV PAL */
      if (codecContainer[i].ctype == kDVCPALCodecType) {
        post("recordQT found DV PAL");
        m_codecType = codecContainer[i].ctype;
        m_codec     = codecContainer[i].codec;
        return true;
      }
      break;
    default:
      /* hmmm... */
      if(gensym(codecName)==gensym(codecContainer[i].name)) {
        post("recordQT found '%s'", codecName);
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

bool recordQT :: open(char*filename)
{
  // if recording is going, do not accept a new file name
  // on OSX changing the name while recording won't have any effect
  // but it will give the wrong message at the end if recording
    if (m_recordStart) {
        error("recordQT: cannot set filename while recording is running!");
        return false;
    }

  snprintf(m_filename, 80, "%s\0", filename);
  m_filename[79]=0;
  post("recordQT: filename '%s'", m_filename);

  return true;
}

#endif // HAVE_QUICKTIME
