/*
 *  pix_record.cpp
 *  GEM_darwin
 *
 *  Created by chris clepper on 7/18/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

#include "pix_record.h"
#include "Base/GemMan.h"
#include "Base/GemCache.h"

#ifdef __APPLE__
#include <Quicktime/Quicktime.h>
#include <Carbon/Carbon.h>

#include <unistd.h> //needed for Unix file open() type functions
#include <stdio.h>
#include <string.h>
#include <fcntl.h> 

CPPEXTERN_NEW_WITH_GIMME(pix_record)

  /////////////////////////////////////////////////////////
//
// pix_record
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_record :: pix_record(int argc, t_atom *argv)
{
 //cna ditch the offsets
  m_xoff = m_yoff = 0;
  m_width = m_height = 0;
  m_prevHeight = m_prevWidth = 0;
  if (argc == 4) {
    m_xoff = (int)atom_getfloat(&argv[0]);
    m_yoff = (int)atom_getfloat(&argv[1]);
    m_width = (int)atom_getfloat(&argv[2]);
    m_height = (int)atom_getfloat(&argv[3]);
  } else if (argc == 2) {
    m_width = (int)atom_getfloat(&argv[0]);
    m_height = (int)atom_getfloat(&argv[1]);
  } else if (argc != 0){
    error("GEM: pix_record: needs 0, 2, or 4 values");
    m_xoff = m_yoff = 0;
    m_width = m_height = 128;
	 
  }

  inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("list"), gensym("vert_pos"));
  inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("list"), gensym("size"));
  
  m_outNumFrames = outlet_new(this->x_obj, 0);

  m_automatic = false;
  m_autocount = 0;
  m_filetype=0;
  sprintf(m_pathname, "/Users/lincoln/Movies/temp");
  m_filename[0] = NULL;
    
  m_banged = false;
  
  
  /* */
  //get list of codecs installed  -- useful later
	CodecNameSpecListPtr codecList;
	CodecNameSpec	codecName;
	int	i;
	int count;
	
	GetCodecNameList(&codecList,1);
	post("pix_record : %i codecs installed",codecList->count);
	if (codecList->count < 64) count = codecList->count; else count = 64;
	for (i = 0; i < count; i++){
		codecName = codecList->list[i];
	//	post("pix_record : codec %i %s %i ctype",i,codecName.typeName, codecName.cType);
		codecContainer[i].position = i;
		codecContainer[i].ctype = codecName.cType;
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
  m_codec = (CodecComponent)65708; //this is pjpeg????
  m_codecSet = true;
  m_spatialQuality = codecHighQuality;
  m_codecQualitySet = true;
  m_dialog = 0;
  m_currentFrame = 0;
  
  post("pix_record : anyCodec %d bestSpeedCodec %d bestFidelityCodec %d bestCompressionCodec %d",anyCodec,bestSpeedCodec,bestFidelityCodec,bestCompressionCodec);
     
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_record :: ~pix_record()
{
}

/////////////////////////////////////////////////////////
// Prepares QT for recording
//
/////////////////////////////////////////////////////////
void pix_record :: setupQT()
{
	FSSpec		theFSSpec;
    OSErr		err = noErr;
    FSRef		ref;
	
	ComponentResult			compErr = noErr;

	m_recordSetup = 0; //if it fails then there is no setup
	
	//this mess should create and open a file for QT to use
	post("filename %s",m_filename);
	if (!m_filename[0]) {
        post("pix_filmDarwin:  no filename passed");
		return;
		} else {            
			err = ::FSPathMakeRef((UInt8*)m_filename, &ref, NULL);
			if (err == fnfErr) {
				// if the file does not yet exist, then let's create the file
				int fd;
                fd = open(m_filename, O_CREAT | O_RDWR, 0600);
                if (fd < 0){
                    post("pix_record : problem with fd");
					return ;
					}
                        write(fd, " ", 1);
                        close(fd);
						err = FSPathMakeRef((UInt8*)m_filename, &ref, NULL);
						post("pix_record : made new file %s",m_filename);
			}

            
			if (err) {
				error("GEM: pix_record: Unable to make file ref from filename %s", m_filename);
				return;
			}
			
			//err = ::FSsetCatalogInfo(&ref, kFSCatInfoSettableInfo, NULL);
			err = FSGetCatalogInfo(&ref, kFSCatInfoNodeFlags, NULL, NULL, &theFSSpec, NULL);

			if (err != noErr){
					error("GEM: pix_record: error %d in FSGetCatalogInfo()", err);
				}
		
		
		//	err = FSMakeFSSpec(theFSSpec.vRefNum, theFSSpec.parID, (UInt8*)m_filename, &theFSSpec);
			
			if (err != noErr && err != -37){
					error("GEM: pix_record: error %d in FSMakeFSSpec()", err);
				}

		}


	//create the movie from the file 
	err = CreateMovieFile(	&theFSSpec,
							FOUR_CHAR_CODE('TVOD'),
							smSystemScript,
							createMovieFileDeleteCurFile |
							createMovieFileDontCreateResFile,
							&nFileRefNum,
							&m_movie);

	if (err != noErr) post("pix_record : CreateMovieFile failed with error %d",err);
	


	//give QT the dimensions of the image to compress
	m_srcRect.top = 0;
	m_srcRect.left = 0;
	m_srcRect.bottom = m_height;
	m_srcRect.right = m_width;
	
	//give QT the length of each pixel row in bytes (2 for 4:2:2 YUV)
	m_rowBytes = m_width * 2;
	
	m_srcGWorld = NULL;
	err = QTNewGWorldFromPtr(&m_srcGWorld,
							k422YpCbCr8CodecType,
							//k32ARGBPixelFormat,
							&m_srcRect,
							NULL,
							NULL,
							0,
							m_compressImage.data,
							m_rowBytes);
	
	if (err != noErr) post("pix_record : QTNewGWorldFromPtr failed with error %d",err);
	
	SetMovieGWorld(m_movie,m_srcGWorld,GetGWorldDevice(m_srcGWorld));
	
	track = NewMovieTrack(m_movie,FixRatio(m_srcRect.right, 1),FixRatio(m_srcRect.bottom, 1),kNoVolume);
	
	media = NewTrackMedia(track,VideoMediaType,600,NULL,0);
	
	stdComponent = OpenDefaultComponent(StandardCompressionType,StandardCompressionSubType);
	
	if (stdComponent == NULL){
		post("pix_record failed to open compressor component");
		return;
	}
	
	//if the settings aren't already set then go ahead and do them
	//if (!m_spatialQuality || !m_codecType || m_dialog ){
	if (m_dialog ){
		post("pix_record : opening settings Dialog");
		compErr = SCRequestSequenceSettings(stdComponent);
	
		if (compErr != noErr) post("pix_record : SCRequestSequenceSettings failed with error %d",compErr);
	
		compErr = SCGetInfo(stdComponent, scTemporalSettingsType, &TemporalSettings);
		compErr = SCGetInfo(stdComponent, scSpatialSettingsType, &SpatialSettings);
	
		if (compErr != noErr) post("pix_record : SCGetInfo failed with error %d",compErr);
		
		m_codecType = SpatialSettings.codecType;
		m_depth = SpatialSettings.depth;
		m_spatialQuality = SpatialSettings.spatialQuality;
		m_codec = SpatialSettings.codec;
		
		post("pix_record : Dialog returned SpatialSettings.codecType %d",SpatialSettings.codecType);
		post("pix_record : Dialog returned SpatialSettings.codec %d",SpatialSettings.codec);
		post("pix_record : Dialog returned SpatialSettings.depth %d",SpatialSettings.depth);
		post("pix_record : Dialog returned SpatialSettings.spatialQuality %d",SpatialSettings.spatialQuality);
		post("pix_record : Dialog returned TemporalSettings.temporalQualitye %d",TemporalSettings.temporalQuality);
		post("pix_record : Dialog returned TemporalSettings.frameRate %d",TemporalSettings.frameRate);
		post("pix_record : Dialog returned TemporalSettings.keyFrameRate %d",TemporalSettings.keyFrameRate);
		
		m_dialog = false; //don't keep doing it again
		
	}else{
	
	/*
		compErr = SCGetInfo(stdComponent, scTemporalSettingsType, &TemporalSettings);
		compErr = SCGetInfo(stdComponent, scSpatialSettingsType, &SpatialSettings);
		compErr = SCGetInfo(stdComponent, scDataRateSettingsType, &datarate);
	
		if (compErr != noErr) post("pix_record : SCGetInfo failed with error %d",compErr);
	*/	
		post("pix_record : manually filling in codec info");
		//fill in manually
		SpatialSettings.codecType = m_codecType;
		SpatialSettings.codec = m_codec;
		SpatialSettings.depth = 0; //should choose best depth
		SpatialSettings.spatialQuality = m_spatialQuality;
		
		TemporalSettings.temporalQuality = m_spatialQuality;
		TemporalSettings.frameRate = 0;
		TemporalSettings.keyFrameRate = 0;
		
		post("pix_record : manual returned SpatialSettings.codecType %d",SpatialSettings.codecType);
		post("pix_record : manual returned SpatialSettings.codec %d",SpatialSettings.codec);
		post("pix_record : manual returned SpatialSettings.depth %d",SpatialSettings.depth);
		post("pix_record : manual returned SpatialSettings.spatialQuality %d",SpatialSettings.spatialQuality);
		post("pix_record : manual returned TemporalSettings.temporalQualitye %d",TemporalSettings.temporalQuality);
		post("pix_record : manual returned TemporalSettings.frameRate %d",TemporalSettings.frameRate);
		post("pix_record : manual returned TemporalSettings.keyFrameRate %d",TemporalSettings.keyFrameRate);
		
		
	}
	
	if (m_codecType == kJPEGCodecType)
	post("pix_record : SCSpatialSettings CodecType %d is p-jpeg",m_codecType);
	//m_codec = SpatialSettings.codec;
	//post("pix_record : SCSpatialSettings Codec %s",m_codec);
	
	post("pix_record : SCSpatialSettings depth %d",m_depth);
	
	if (m_spatialQuality == codecHighQuality) post("pix_record : SCSpatialSettings SpatialQuality codecHighQuality");
	
	datarate.frameDuration = 33;
	
	compErr = SCSetInfo(stdComponent, scTemporalSettingsType, &TemporalSettings);
	compErr = SCSetInfo(stdComponent, scSpatialSettingsType, &SpatialSettings);
	compErr = SCSetInfo(stdComponent, scDataRateSettingsType, &datarate);
	
	if (compErr != noErr) post("pix_record : SCSetInfo failed with error %d",compErr);
	
	compErr = SCCompressSequenceBegin(stdComponent,GetPortPixMap(m_srcGWorld),&m_srcRect,&hImageDesc);
	
	if (compErr != noErr) {
		post("pix_record : SCCompressSequenceBegin failed with error %d",compErr);
		return;
		}
	
	err = BeginMediaEdits(media);
	if (err != noErr) post("pix_record : BeginMediaEdits failed with error %d",err);
	

	//this will show that everything is OK for recording
	m_recordSetup = 1;
	
	//set the previous dimensions for the sanity check during compression
	m_prevWidth = m_width;
	m_prevHeight = m_height;
	
	//reset frame counter for new movie file
	m_currentFrame = 0;

}



//
// stops recording into the QT movie
//
void pix_record :: stopRecording()
{
	ComponentResult			compErr = noErr;
	OSErr					err;
	
	
	err = EndMediaEdits(media);
	if (err != noErr) post("pix_record : EndMediaEdits failed with error %d",err);
	
	err = InsertMediaIntoTrack(track,0,0,GetMediaDuration(media),0x00010000);
	if (err != noErr) post("pix_record : InsertMediaIntoTrack failed with error %d",err);

	err = AddMovieResource(m_movie,nFileRefNum,&nResID,NULL);
	if (err != noErr) post("pix_record : AddMovieResource failed with error %d",err);
	
	err = CloseMovieFile(nFileRefNum);
	if (err != noErr) post("pix_record : CloseMovieFile failed with error %d",err);
	
	DisposeMovie(m_movie);
		
	compErr = SCCompressSequenceEnd(stdComponent);
	
	if (compErr != noErr) post("pix_record : SCCompressSequenceEnd failed with error %d",compErr);
	
	compErr = CloseComponent(stdComponent);
	
	if (compErr != noErr) post("pix_record : CloseComponent failed with error %d",compErr);
	
	m_recordStop = 0;
	m_recordSetup = 0;
	m_recordStart = 0; //just to be sure
	
	m_currentFrame = 0; //reset the frame counter?
	
	outlet_float(m_outNumFrames,m_currentFrame);
	
	post("pix_record : movie written to %s",m_filename);

}

void pix_record :: compressFrame()
{
	OSErr					err;

	Handle					compressedData; //data to put in QT mov
	
	ComponentResult			compErr = noErr;

	short					syncFlag; //flag for keyframes
	
	
	
	compErr = SCCompressSequenceFrame(	stdComponent,
										GetPortPixMap(m_srcGWorld),
										&m_srcRect,
										&compressedData,
										&dataSize,
										&syncFlag);
										
	if (compErr != noErr) post("pix_record : SCCompressSequenceFrame failed with error %d",compErr);
										
	err = AddMediaSample(media,
							compressedData,
							0,
							dataSize,
							20, //this should not be a fixed value but vary with framerate
							(SampleDescriptionHandle)hImageDesc,
							1,
							syncFlag,
							NULL);
							
	if (err != noErr) post("pix_record : AddMediaSample failed with error %d",err);
							
	m_currentFrame++;
	
	outlet_float(m_outNumFrames,m_currentFrame);
	
}



/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void pix_record :: render(GemState *state)
{
	//check if state exists
	if (state->image){
		m_pixBlock = state->image;
		m_compressImage = m_pixBlock->image;
		m_height = m_pixBlock->image.ysize;
		m_width = m_pixBlock->image.xsize;
			

  if (m_automatic || m_banged) {
  /*
    char *extension;
    if (m_filetype<0)m_filetype=0;
    if (m_filetype==0) extension="tif";
    else {
#if 0
      post("pix_record: you can only write TIFF-files ! (forcing to TIFF)");
      m_filetype=0;
      extension="tif";
#else
      extension="jpg";
#endif
    }
    
    sprintf(m_filename, "%s%d.%s", m_pathname, m_autocount+10000, extension);
    m_filename[strlen(m_pathname)]-=1;
    */
    m_autocount++;
    m_banged = false;
	
	
	
	//record
	if (m_recordStart) {
		//if setupQT() has not been run do that first
		if (!m_recordSetup) setupQT();
		
		//should check if the size has changed or else we will freak the compressor's trip out
		if (m_width == m_prevWidth && m_height == m_prevHeight) {
			//go ahead and grab a frame if everything is ready to go
			if (m_recordSetup) 
				compressFrame();
			//	post("grabbing frame");
			}else{
				post("pix_record: movie dimensions changed prev %dx%d now %dx%d stopping recording",m_prevWidth,m_prevHeight,m_width,m_height);
				m_recordStop = 1;
				m_prevWidth = m_width;
				m_prevHeight = m_height; //go ahead and change dimensions
			}
	}
	
	//if recording is stopped and everything is setup then stop recording
	if (m_recordStop){
		//guard against someone not setting up QT beforehand
		if (!m_recordSetup)	return;
		stopRecording();
	}
	

  }
  }
}


/////////////////////////////////////////////////////////
// sizeMess
//
/////////////////////////////////////////////////////////
void pix_record :: sizeMess(int width, int height)
{
  m_width = width;
  m_height = height;
}

/////////////////////////////////////////////////////////
// posMess
//
/////////////////////////////////////////////////////////
void pix_record :: posMess(int x, int y)
{
  m_xoff = x;
  m_yoff = y;
}

/////////////////////////////////////////////////////////
// dialogMess
//
/////////////////////////////////////////////////////////
void pix_record :: dialogMess()
{
	//if recording is going do not open the dialog
  if (!m_recordStart) {
		post("pix_record : opening compression dialog");
		m_dialog = true;
		setupQT();
  }else{
		post("pix_record : cannot open compression dialog while recording");
  }
}

/////////////////////////////////////////////////////////
// spits out a list of installed codecs and stores them
//
/////////////////////////////////////////////////////////
void pix_record :: getCodecList()
{
	  //get list of codecs installed  -- useful later
	CodecNameSpecListPtr codecList;
	CodecNameSpec	codecName;
	int	i;
	int count;
	
	GetCodecNameList(&codecList,1);
	post("pix_record : %i codecs installed",codecList->count);
	if (codecList->count < 64) count = codecList->count; else count = 64;
	for (i = 0; i < count; i++){
		codecName = codecList->list[i];
		post("pix_record : codec %i %s %i ctype %d",i,codecName.typeName, codecName.cType,codecName.codec);
		codecContainer[i].position = i;
		codecContainer[i].ctype = codecName.cType;
		
		}
}


/////////////////////////////////////////////////////////
// deals with the name of a codec
//
/////////////////////////////////////////////////////////
void pix_record :: codecMess(int argc, t_atom *argv)
{

	char codecName[80];

	//might be nice to allow both a symbol corresponding to the codecType and a number from the list
	if (argc) {
		if (argv->a_type == A_SYMBOL) {
			atom_string(argv++, codecName, 80);
			argc--;
		}
     }
	
	if (!strncmp(codecName,"jpeg",4)) {
		//have to put the right things in here
		m_codecType = kJPEGCodecType;
		m_codec = (CodecComponent)65708; //this is pjpeg?!? 
		post("pix_record : kJPEGCodecType");
	}
	//do the same for these
	if (!strcmp(codecName,"animation")) post("pix_record : kAnimationCodecType");
	if (!strncmp(codecName,"yuv2",4)) post("pix_record : kComponentVideoCodecType");
	if (!strncmp(codecName,"yuvu",4)) post("pix_record : kComponentVideoSigned");
	if (!strncmp(codecName,"raw",3)) post("pix_record : kRawCodecType");
	if (!strncmp(codecName,"dvc",3)) post("pix_record : kDVCNTSCCodecType");
	if (!strncmp(codecName,"dvcp",4)) post("pix_record : kDVCPALCodecType");
	if (!strncmp(codecName,"y420",4)) post("pix_record : kYUV420CodecType");
	post("pix_record : codecName %s",codecName);

}


void pix_record :: fileMess(int argc, t_atom *argv)
{

//  char *extension = ".mov";
  if (argc) {
    if (argv->a_type == A_SYMBOL) {
      atom_string(argv++, m_pathname, 80);
      argc--;
      sprintf(m_filename, "%s", m_pathname);
    }
    if (argc>0)
      m_filetype = atom_getint(argv);
  }

  m_autocount = 0;

post("pix_record : filename %s",m_filename);

}

/////////////////////////////////////////////////////////
// cleanImage
//
/////////////////////////////////////////////////////////
/*
void pix_record :: cleanImage()
{
  // release previous data
  if (m_originalImage)
    {
      delete m_originalImage;
      m_originalImage = NULL;
    }
}
*/

/////////////////////////////////////////////////////////
// static member functions
//
/////////////////////////////////////////////////////////
void pix_record :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr, (t_method)&pix_record::fileMessCallback,
		  gensym("file"), A_GIMME, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_record::autoMessCallback,
		  gensym("auto"), A_FLOAT, A_NULL);
  class_addbang(classPtr, (t_method)&pix_record::bangMessCallback);

  class_addmethod(classPtr, (t_method)&pix_record::sizeMessCallback,
		  gensym("size"), A_FLOAT, A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_record::posMessCallback,
		  gensym("vert_pos"), A_FLOAT, A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_record::recordMessCallback,
		  gensym("record"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_record::dialogMessCallback,
		  gensym("dialog"),  A_NULL);
  class_addmethod(classPtr, (t_method)&pix_record::codeclistMessCallback,
		  gensym("codeclist"),  A_NULL);
	class_addmethod(classPtr, (t_method)&pix_record::codecMessCallback,
		  gensym("codec"), A_GIMME, A_NULL);
}

void pix_record :: fileMessCallback(void *data, t_symbol *s, int argc, t_atom *argv)
{
  GetMyClass(data)->fileMess(argc, argv);
}
void pix_record :: autoMessCallback(void *data, t_floatarg on)
{
  GetMyClass(data)->m_automatic=(on!=0);
}
void pix_record :: bangMessCallback(void *data)
{
  GetMyClass(data)->m_banged=true;
}

void pix_record :: sizeMessCallback(void *data, t_floatarg width, t_floatarg height)
{
  GetMyClass(data)->sizeMess((int)width, (int)height);
}
void pix_record :: posMessCallback(void *data, t_floatarg x, t_floatarg y)
{
  GetMyClass(data)->posMess((int)x, (int)y);
}

void pix_record :: recordMessCallback(void *data, t_floatarg on)
{
	if (on) {
		GetMyClass(data)->m_recordStart=1;
		GetMyClass(data)->m_recordStop=0;
	}else{
		GetMyClass(data)->m_recordStart=0;
		GetMyClass(data)->m_recordStop=1;
		}
}

void pix_record :: dialogMessCallback(void *data)
{
	GetMyClass(data)->dialogMess();
}

void pix_record :: codeclistMessCallback(void *data)
{
	GetMyClass(data)->getCodecList();
}

void pix_record :: codecMessCallback(void *data, t_symbol *s, int argc, t_atom *argv)
{
  GetMyClass(data)->codecMess(argc, argv);
 // if (s->s_name == kJPEGCodecType) post("pix_record : photo-jpeg codec"); else post("pix_record : not photo-jpeg");
}

#endif // __APPLE__