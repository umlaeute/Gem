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
#endif

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
    m_prevHeight(0), m_prevWidth(0), 
    m_compressImage(NULL)
{
  m_filename[0] = NULL;

# ifdef __WIN32__
  // Initialize QuickTime Media Layer
  OSErr		err = noErr;
  if ((err = InitializeQTML(0))) {
    printf("filmQT: Could not initialize quicktime: error %d\n", err);
    return;
  }
	
  // Initialize QuickTime
  if (err = EnterMovies()) {
    printf("filmQT: Could not initialize quicktime: error %d\n", err);
    return;
  }
  printf("pix_video: QT init done\n");
# endif // WINDOWS

  /* */
  //get list of codecs installed  -- useful later
  CodecNameSpecListPtr codecList;
  CodecNameSpec	codecName;
  int	i;
  int count;
	
  GetCodecNameList(&codecList,1);
  printf("recordQT : %i codecs installed\n",codecList->count);
  if (codecList->count < 64) count = codecList->count; else count = 64;
  for (i = 0; i < count; i++){
    codecName = codecList->list[i];
    //	printf("recordQT : codec %i %s %i ctype",i,codecName.typeName, codecName.cType);
    codecContainer[i].position = i;
    codecContainer[i].ctype = codecName.cType;
    codecContainer[i].codec = codecName.codec;
  }
  
  //initialize member variables
  stdComponent = NULL;
  hImageDesc = NULL;
  nFileRefNum = 0;
  nResID = movieInDataForkResID;
  m_recordStart = 0;
  m_recordStop = 0;
  m_recordSetup = 0;
  m_codecType = kJPEGCodecType;
  
  for(i = 0; i < count; i++){
    if (codecContainer[i].ctype == kJPEGCodecType) m_codec = codecContainer[i].codec;
  }
  printf("recordQT : pjpeg codec %i %i %i ctype\n",i,m_codecType, m_codec);
  // m_codec = (CodecComponent)65731;//65719;//65708; //this is pjpeg????
  m_codecSet = true;
  m_spatialQuality = codecNormalQuality; //codecHighQuality;
  m_codecQualitySet = true;
  m_dialog = 0;
  m_currentFrame = 0;
  
  
  // post("recordQT : anyCodec %d bestSpeedCodec %d bestFidelityCodec %d bestCompressionCodec %d",anyCodec,bestSpeedCodec,bestFidelityCodec,bestCompressionCodec);
  stdComponent = OpenDefaultComponent(StandardCompressionType,StandardCompressionSubType);
	
  if (stdComponent == NULL){
    printf("recordQT failed to open compressor component\n");
    return;
  }
	   
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
recordQT :: ~recordQT()
{
  ComponentResult			compErr = noErr;

  printf("recordQT: deconstructor\n");
  if (stdComponent != NULL){
    compErr = CloseComponent(stdComponent);
    
    if (compErr != noErr) printf("recordQT : CloseComponent failed with error %d\n",compErr);
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
	
  ComponentResult			compErr = noErr;

  m_recordSetup = 0; //if it fails then there is no setup
	
  //this mess should create and open a file for QT to use
  //probably should be a separate function
  //post("filename %s",m_filename);

  if (!m_filename[0]) {
    printf("recordQT:  no filename passed\n");
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
	printf("recordQT : problem with fd\n");
	return ;
      }
      ::write(fd, " ", 1);
      ::close(fd);
      err = FSPathMakeRef((UInt8*)m_filename, &ref, NULL);
      //printf("recordQT : made new file %s\n",m_filename);
    }


    if (err) {
      printf("GEM: recordQT: Unable to make file ref from filename %s", m_filename);
      return ;
    }
			
    //err = ::FSsetCatalogInfo(&ref, kFSCatInfoSettableInfo, NULL);
    err = FSGetCatalogInfo(&ref, kFSCatInfoNodeFlags, NULL, NULL, &theFSSpec, NULL);

    if (err != noErr){
      printf("GEM: recordQT: error %d in FSGetCatalogInfo()", err);
      return ;
    }
		
		
    err = FSMakeFSSpec(theFSSpec.vRefNum, theFSSpec.parID, (UInt8*)m_filename, &theFSSpec);
			
    if (err != noErr && err != -37){
      printf("GEM: recordQT: error %d in FSMakeFSSpec()", err);
      return;
    }

  }
#elif defined __WIN32__
  else {
    c2pstr(m_filename);
    
    FSMakeFSSpec (0, 0L, (UInt8*)m_filename, &theFSSpec);
    //err = ::FSPathMakeRef((UInt8*)m_filename, &ref, NULL);
    if (err != noErr && err != -37){
      printf("GEM: recordQT: error %d in FSMakeFSSpec()", err);
      return;
    }
  }
#endif    //APPLE 

  //create the movie from the file 
  err = CreateMovieFile(	&theFSSpec,
				FOUR_CHAR_CODE('TVOD'),
				smSystemScript,
				createMovieFileDeleteCurFile |
				createMovieFileDontCreateResFile,
				&nFileRefNum,
				&m_movie);

  if (err != noErr) {
    printf("recordQT : CreateMovieFile failed with error %d\n",err);
    return;
  }
	
  //give QT the dimensions of the image to compress
  m_srcRect.top = 0;
  m_srcRect.left = 0;
  m_srcRect.bottom = m_height;
  m_srcRect.right = m_width;
	

#ifdef __APPLE__
  //give QT the length of each pixel row in bytes (2 for 4:2:2 YUV)
  m_rowBytes = m_width * 2;
	
  //m_srcGWorld = NULL;//probably a memory leak
  err = QTNewGWorldFromPtr(&m_srcGWorld,
			   k422YpCbCr8CodecType,
			   //k32ARGBPixelFormat,
			   &m_srcRect,
			   NULL,
			   NULL,
			   0,
			   m_compressImage->data,
			   m_rowBytes);

#elif defined __WIN32__
  //give QT the length of each pixel row in bytes (2 for 4:2:2 YUV)
  m_rowBytes = m_width * 4;
	
  //m_srcGWorld = NULL;//probably a memory leak
  err = QTNewGWorldFromPtr(&m_srcGWorld,
			   k32RGBAPixelFormat,
			   //k32ARGBPixelFormat,
			   &m_srcRect,
			   NULL,
			   NULL,
			   0,
			   m_compressImage->data,
			   m_rowBytes);

#endif
  if (err != noErr){
    printf("recordQT : QTNewGWorldFromPtr failed with error %d\n",err);
    return;
  }
	
  SetMovieGWorld(m_movie,m_srcGWorld,GetGWorldDevice(m_srcGWorld));
	
  track = NewMovieTrack(m_movie,FixRatio(m_srcRect.right, 1),FixRatio(m_srcRect.bottom, 1),kNoVolume);
	
  media = NewTrackMedia(track,VideoMediaType,600,NULL,0);
	
  //moved to constructor
  /*
    stdComponent = OpenDefaultComponent(StandardCompressionType,StandardCompressionSubType);
	
    if (stdComponent == NULL){
    post("recordQT failed to open compressor component");
    return;
    }*/
	
  //if the settings aren't already set then go ahead and do them
  //if (!m_spatialQuality || !m_codecType || m_dialog ){
  if (m_dialog ){
	
    //close the component if already open
    if (stdComponent) compErr = CloseComponent(stdComponent);
	
    if (compErr != noErr) printf("recordQT : CloseComponent failed with error %d\n",compErr);
		
    //open a new component from scratch
    stdComponent = OpenDefaultComponent(StandardCompressionType,StandardCompressionSubType);
	
    if (stdComponent == NULL){
      printf("recordQT failed to open compressor component\n");
      return;
    }
		
    printf("recordQT : opening settings Dialog\n");
    compErr = SCRequestSequenceSettings(stdComponent);
	
    if (compErr != noErr) printf("recordQT : SCRequestSequenceSettings failed with error %d\n",compErr);
	
    compErr = SCGetInfo(stdComponent, scTemporalSettingsType, &TemporalSettings);
    compErr = SCGetInfo(stdComponent, scSpatialSettingsType, &SpatialSettings);
	
    if (compErr != noErr) printf("recordQT : SCGetInfo failed with error %d\n",compErr);
		
    m_codecType = SpatialSettings.codecType;
    m_depth = SpatialSettings.depth;
    m_spatialQuality = SpatialSettings.spatialQuality;
    m_codec = SpatialSettings.codec;
		
    printf("recordQT : Dialog returned SpatialSettings.codecType %d\n",SpatialSettings.codecType);
    printf("recordQT : Dialog returned SpatialSettings.codec %d\n",SpatialSettings.codec);
    printf("recordQT : Dialog returned SpatialSettings.depth %d\n",SpatialSettings.depth);
    printf("recordQT : Dialog returned SpatialSettings.spatialQuality %d\n",SpatialSettings.spatialQuality);
    printf("recordQT : Dialog returned TemporalSettings.temporalQualitye %d\n",TemporalSettings.temporalQuality);
    printf("recordQT : Dialog returned TemporalSettings.frameRate %d\n",TemporalSettings.frameRate);
    printf("recordQT : Dialog returned TemporalSettings.keyFrameRate %d\n",TemporalSettings.keyFrameRate);
		
    m_dialog = false; //don't keep doing it again
		
  }else{
    //post("recordQT : manually filling in codec info");
    //fill in manually
    SpatialSettings.codecType = m_codecType;
    SpatialSettings.codec = m_codec;
    SpatialSettings.depth = 0; //should choose best depth
    SpatialSettings.spatialQuality = m_spatialQuality;
		
    TemporalSettings.temporalQuality = m_spatialQuality;
    TemporalSettings.frameRate = 0;
    TemporalSettings.keyFrameRate = 0;
		
    /*
      post("recordQT : manual returned SpatialSettings.codecType %d",SpatialSettings.codecType);
      post("recordQT : manual returned SpatialSettings.codec %d",SpatialSettings.codec);
      post("recordQT : manual returned SpatialSettings.depth %d",SpatialSettings.depth);
      post("recordQT : manual returned SpatialSettings.spatialQuality %d",SpatialSettings.spatialQuality);
      post("recordQT : manual returned TemporalSettings.temporalQualitye %d",TemporalSettings.temporalQuality);
      post("recordQT : manual returned TemporalSettings.frameRate %d",TemporalSettings.frameRate);
      post("recordQT : manual returned TemporalSettings.keyFrameRate %d",TemporalSettings.keyFrameRate);
    */
		
  }
	
  datarate.frameDuration = 33;
	
  compErr = SCSetInfo(stdComponent, scTemporalSettingsType, &TemporalSettings);
  compErr = SCSetInfo(stdComponent, scSpatialSettingsType, &SpatialSettings);
  compErr = SCSetInfo(stdComponent, scDataRateSettingsType, &datarate);
	
  if (compErr != noErr) printf("recordQT : SCSetInfo failed with error %d\n",compErr);
	
#ifdef __APPLE__
  compErr = SCCompressSequenceBegin(stdComponent,GetPortPixMap(m_srcGWorld),&m_srcRect,&hImageDesc);
#elif defined __WIN32__
  compErr = SCCompressSequenceBegin(stdComponent,m_srcGWorld->portPixMap,&m_srcRect,&hImageDesc);
#endif
  if (compErr != noErr) {
    printf("recordQT : SCCompressSequenceBegin failed with error %d\n",compErr);
    return;
  }
	
  err = BeginMediaEdits(media);
  if (err != noErr) {
    printf("recordQT : BeginMediaEdits failed with error %d\n",err);
    return;
  }
	

  //this will show that everything is OK for recording
  m_recordSetup = 1;
	
  //set the previous dimensions for the sanity check during compression
  m_prevWidth = m_width;
  m_prevHeight = m_height;
	
  //reset frame counter for new movie file
  m_currentFrame = 0;
  printf("recordQT: setup end\n");
}



//
// stops recording into the QT movie
//
void recordQT :: close()
{
  ComponentResult			compErr = noErr;
  OSErr					err;
	
	
  err = EndMediaEdits(media);
  if (err != noErr) printf("recordQT : EndMediaEdits failed with error %d\n",err);
	
  err = InsertMediaIntoTrack(track,0,0,GetMediaDuration(media),0x00010000);
  if (err != noErr) printf("recordQT : InsertMediaIntoTrack failed with error %d\n",err);

  err = AddMovieResource(m_movie,nFileRefNum,&nResID,NULL);
  if (err != noErr) printf("recordQT : AddMovieResource failed with error %d\n",err);
	
  err = CloseMovieFile(nFileRefNum);
  if (err != noErr) printf("recordQT : CloseMovieFile failed with error %d\n",err);
	
  DisposeMovie(m_movie);
  DisposeGWorld(m_srcGWorld);
  m_srcGWorld = NULL;
		
  compErr = SCCompressSequenceEnd(stdComponent);
	
  if (compErr != noErr) printf("recordQT : SCCompressSequenceEnd failed with error %d\n",compErr);

  m_recordStop = 0;
  m_recordSetup = 0;
  m_recordStart = 0; //just to be sure
	
  m_currentFrame = 0; //reset the frame counter?

  printf("recordQT : movie written to %s\n",m_filename);

}
//apparently on OSX there is no member portPixMap in a GWorld so a function is used instead
#ifdef __WIN32__
# define GetPortPixMap(x) (x)->portPixMap
#endif

void recordQT :: compressFrame()
{
  OSErr			err;
  Handle		compressedData; //data to put in QT mov
  ComponentResult	compErr = noErr;
  short			syncFlag; //flag for keyframes

  //post("recordQT: compressing frame");
  //apparently on OSX there is no member portPixMap in a GWorld so a function is used instead
  compErr = SCCompressSequenceFrame(	stdComponent,
					GetPortPixMap(m_srcGWorld),
					&m_srcRect,
					&compressedData,
					&dataSize,
					&syncFlag);

  if (compErr != noErr) printf("recordQT : SCCompressSequenceFrame failed with error %d\n",compErr);
										
  err = AddMediaSample(media,
		       compressedData,
		       0,
		       dataSize,
		       20, //this should not be a fixed value but vary with framerate
		       (SampleDescriptionHandle)hImageDesc,
		       1,
		       syncFlag,
		       NULL);
							
  if (err != noErr) printf("recordQT : AddMediaSample failed with error %d\n",err);
							
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
    if (!m_recordSetup) setupQT();
		
    //should check if the size has changed or else we will freak the compressor's trip out
    if (m_width == m_prevWidth && m_height == m_prevHeight) {
      //go ahead and grab a frame if everything is ready to go
      if (m_recordSetup) 
	compressFrame();
      //post("grabbing frame");
    }else{
      printf("recordQT: movie dimensions changed prev %dx%d now %dx%d stopping recording\n",m_prevWidth,m_prevHeight,m_width,m_height);
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
  //if recording is going do not open the dialog
  if (!m_recordStart) {
    printf("recordQT : opening compression dialog\n");
    m_dialog = true;
    setupQT();
    return(true);
  }else{
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
  CodecNameSpecListPtr codecList;
  CodecNameSpec	codecName;
  int	i;
  int count;
	
  GetCodecNameList(&codecList,1);
  //  post("recordQT : %i codecs installed",codecList->count);
  if (codecList->count < 64) count = codecList->count; else count = 64;
  for (i = 0; i < count; i++){
    codecName = codecList->list[i];
    //post("recordQT : codec %i %s %i ctype %d",i,codecName.typeName, codecName.cType,codecName.codec);
    codecContainer[i].position = i;
    codecContainer[i].ctype = codecName.cType;
  }
  return codecList->count;
}

/////////////////////////////////////////////////////////
// deals with the name of a codec
//
/////////////////////////////////////////////////////////
bool recordQT :: setCodec(char*codecName)
{
  if (!strncmp(codecName,"jpeg",4)) {
    //have to put the right things in here
    m_codecType = kJPEGCodecType;
    m_codec = (CodecComponent)65719;//65708; //this is pjpeg?!? 
    printf("recordQT : kJPEGCodecType\n");
    return true;
  }
  //do the same for these
  if (!strcmp(codecName,"animation")) printf("recordQT : kAnimationCodecType\n");
  if (!strncmp(codecName,"yuv2",4)) printf("recordQT : kComponentVideoCodecType\n");
  if (!strncmp(codecName,"yuvu",4)) printf("recordQT : kComponentVideoSigned\n");
  if (!strncmp(codecName,"raw",3)) printf("recordQT : kRawCodecType\n");
  if (!strncmp(codecName,"dvc",3)) printf("recordQT : kDVCNTSCCodecType\n");
  if (!strncmp(codecName,"dvcp",4)) printf("recordQT : kDVCPALCodecType\n");
  if (!strncmp(codecName,"y420",4)) printf("recordQT : kYUV420CodecType\n");
  printf("recordQT : codecName %s\n",codecName);
  return false;
}


bool recordQT :: open(char*filename)
{
  //if recording is going do not accept a new file name
  //on OSX changing the name while recording won't have any effect 
  //but it will give the wrong message at the end if recording
  if (m_recordStart) return false;

  sprintf(m_filename, "%s", filename);
  printf("recordQT : filename %s\n",m_filename);
  return true;
}

#endif // HAVE_QUICKTIME
