/*
 *  videoDarwin.cpp
 *  gem_darwin
 *
 *  Created by James Tittle on Fri Jul 12 2002.
 *  Copyright (c) 2002-2005 James Tittle & Chris Clepper
 *  Copyright (c) 2010 IOhannes m zmölnig
 *
 */
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#if defined HAVE_CARBON && defined HAVE_QUICKTIME
# define HAVE_VIDEODARWIN
#endif

#include "videoDarwin.h"
using namespace gem;
#include "Gem/RTE.h"



#ifdef HAVE_VIDEODARWIN
#include <unistd.h> // needed for Unix file open() type functions
#include <stdio.h>
#include <fcntl.h> /* JMZ thinks that _this_ is needed for open() */

#define DEFAULT_WIDTH        320
#define DEFAULT_HEIGHT        240
  
REGISTER_VIDEOFACTORY("Darwin", videoDarwin);

videoDarwin :: videoDarwin() 
    : video(0),
      m_srcGWorld(NULL)
{
  m_width= DEFAULT_WIDTH;
  m_height=DEFAULT_HEIGHT;

  m_image.image.xsize = 800;
  m_image.image.ysize = 600;
  m_image.image.setCsizeByFormat(GL_BGRA_EXT);
  m_image.image.allocate();

  m_quality = 0; //normal quality gives non-interlaced images from DV cams
  m_colorspace = GL_YCBCR_422_GEM; //default to YUV

  //set to the first input device
  m_inputDevice = 0;
  initSeqGrabber();

  provide("darwin");
  provide("dv");
  provide("iidc");
  provide("analog");
}

////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
videoDarwin :: ~videoDarwin()
{
  if (m_vc) {
    if (::SGDisposeChannel(m_sg, m_vc)) {
      error ("Unable to dispose a video channel");
    }
    m_vc = NULL;
  }
  if (m_sg) {
    if (::CloseComponent(m_sg)) {
      error("Unable to dispose a sequence grabber component");
    }
    m_sg = NULL;
    if (m_srcGWorld) {
      ::DisposeGWorld(m_srcGWorld);
      m_pixMap = NULL;
      m_srcGWorld = NULL;
      m_baseAddr = NULL;
    }
  }
}
bool videoDarwin :: openDevice(void) {
  initSeqGrabber();
  return (NULL!=m_sg);
}
void videoDarwin :: closeDevice(void) {
  destroySeqGrabber();
}

////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
bool videoDarwin :: grabFrame()
{
  OSErr    err;

  err = SGIdle(m_sg);

  if (err != noErr){
    error("SGIdle failed with error %d",err);
    m_haveVideo = 0;
//    return false;
  } else {
    //this doesn't do anything so far
    //VDCompressDone(m_vdig,frameCount,data,size,similar,time);
    //err = SGGrabFrameComplete(m_vc,frameCount,done);
    //if (err != noErr) error("SGGrabCompressComplete failed with error %d",err);
    //post("SGGrabFramecomplete done %d framecount = %d",done[0],frameCount);

    m_haveVideo = 1;
    m_newFrame = 1;
  }
  if (!m_haveVideo)
    {
      post("no video yet");
      return true;
    }
  m_image.newimage = m_newFrame;
  m_newFrame = 0;

 return true;
}

////////////////////////////////////////////////////////
// startTransfer
//
/////////////////////////////////////////////////////////
bool videoDarwin :: startTransfer()
{
  OSErr    err = noErr;

  SGStartPreview(m_sg);
  m_haveVideo = 1;
  m_image.newimage = 1;
  return true;
}

////////////////////////////////////////////////////////
// stopTransfer
//
/////////////////////////////////////////////////////////
bool videoDarwin :: stopTransfer()
{
  OSErr    err = noErr;

  //might need SGPause or SGStop here
  err = SGStop(m_sg);
  if (err != noErr)error("SGStop failed with error %d",err);
  return true;
}
#if 0
////////////////////////////////////////////////////////
// startrender
//
/////////////////////////////////////////////////////////
void videoDarwin :: stopRendering()
{
  setupCapture();
}
#endif

void videoDarwin :: initSeqGrabber()
{
  OSErr anErr;
  Rect m_srcRect = {0,0, m_height, m_width};

  SGDeviceList    devices;
  short            deviceIndex,inputIndex;
  short            deviceCount = 0;
  SGDeviceInputList theSGInputList = NULL;
  bool showInputsAsDevices;

  m_sg = OpenDefaultComponent(SeqGrabComponentType, 0);
  if(m_sg==NULL){
    error("could not open default component");
    return;
  }
  anErr = SGInitialize(m_sg);
  if(anErr!=noErr){
    error("could not initialize SG error %d",anErr);
    return;
  }

  anErr = SGSetDataRef(m_sg, 0, 0, seqGrabDontMakeMovie);
  if (anErr != noErr){
    error("dataref failed with error %d",anErr);
  }

  anErr = SGNewChannel(m_sg, VideoMediaType, &m_vc);
  if(anErr!=noErr){
    error("could not make new SG channnel error %d",anErr);
    return;
  }

  anErr = SGGetChannelDeviceList(m_vc, sgDeviceListIncludeInputs, &devices);
  if(anErr!=noErr){
    error("could not get SG channnel Device List");
  }else{
    deviceCount = (*devices)->count;
    deviceIndex = (*devices)->selectedIndex;
    post("SG channnel Device List count %d index %d",deviceCount,deviceIndex);
    int i;
    for (i = 0; i < deviceCount; i++){
      post("SG channnel Device List  %.*s",
           (*devices)->entry[i].name[0],
           (*devices)->entry[i].name+1);
    }
    SGGetChannelDeviceAndInputNames(m_vc, NULL, NULL, &inputIndex);

    showInputsAsDevices = ((*devices)->entry[deviceIndex].flags) & sgDeviceNameFlagShowInputsAsDevices;

    theSGInputList = ((SGDeviceName *)(&((*devices)->entry[deviceIndex])))->inputs; //fugly

    //we should have device names in big ass undocumented structs

    //walk through the list
    for (i = 0; i < inputIndex; i++){
      post("SG channnel Input Device List %d %.*s",
           i,
           (*theSGInputList)->entry[i].name[0],
           (*theSGInputList)->entry[i].name+1);
    }
  }

  //this call sets the input device
  if (m_inputDevice > 0 && m_inputDevice < deviceCount) //check that the device is not out of bounds
    post("SGSetChannelDevice trying %s", 
         (*devices)->entry[m_inputDevice].name[0],
         (*devices)->entry[m_inputDevice].name+1);
  anErr = SGSetChannelDevice(m_vc, (*devices)->entry[m_inputDevice].name);

  if(anErr!=noErr) error("SGSetChannelDevice returned error %d",anErr);

  anErr = SGSetChannelDeviceInput(m_vc,m_inputDeviceChannel);

  if(anErr!=noErr) error("SGSetChannelDeviceInput returned error %d",anErr);

  //grab the VDIG info from the SGChannel
  m_vdig = SGGetVideoDigitizerComponent(m_vc);
  vdigErr = VDGetDigitizerInfo(m_vdig,&m_vdigInfo); //not sure if this is useful

  Str255    vdigName;
  memset(vdigName,0,255);
  vdigErr = VDGetInputName(m_vdig,m_inputDevice,vdigName);
  post("vdigName is %s",vdigName); // pascal string?

  Rect vdRect;
  vdigErr = VDGetDigitizerRect(m_vdig,&vdRect);
  post("digitizer rect is top %d bottom %d left %d right %d",vdRect.top,vdRect.bottom,vdRect.left,vdRect.right);

  vdigErr = VDGetActiveSrcRect(m_vdig,0,&vdRect);
  post("active src rect is top %d bottom %d left %d right %d",vdRect.top,vdRect.bottom,vdRect.left,vdRect.right);

  anErr = SGSetChannelBounds(m_vc, &m_srcRect);
  if(anErr!=noErr){
    error("could not set SG ChannelBounds ");
  }

  anErr = SGSetVideoRect(m_vc, &m_srcRect);
  if(anErr!=noErr){
    error("could not set SG Rect ");
  }

  anErr = SGSetChannelUsage(m_vc, seqGrabPreview);
  if(anErr!=noErr){
    error("could not set SG ChannelUsage ");
  }

  switch (m_quality){
  case 0:
    anErr = SGSetChannelPlayFlags(m_vc, channelPlayNormal);
    post("set SG NormalQuality");
    break;
  case 1:
    anErr = SGSetChannelPlayFlags(m_vc, channelPlayHighQuality);
    post("set SG HighQuality");
    break;
  case 2:
    anErr = SGSetChannelPlayFlags(m_vc, channelPlayFast);
    post("set SG FastQuality");
    break;
  case 3:
    anErr = SGSetChannelPlayFlags(m_vc, channelPlayAllData);
    post("set SG PlayAlldata");
    break;
  }
  if (m_colorspace==GL_BGRA_EXT){
    m_image.image.xsize = m_width;
    m_image.image.ysize = m_height;
    m_image.image.setCsizeByFormat(GL_RGBA_GEM);
    m_image.image.reallocate();
    m_rowBytes = m_width*4;
    anErr = QTNewGWorldFromPtr (&m_srcGWorld,
                                k32ARGBPixelFormat,
                                &m_srcRect,
                                NULL,
                                NULL,
                                0,
                                m_image.image.data,
                                m_rowBytes);

    post ("using RGB");
  }else{
    m_image.image.xsize = m_width;
    m_image.image.ysize = m_height;
    m_image.image.setCsizeByFormat( GL_YCBCR_422_APPLE );

    m_image.image.reallocate();

    m_rowBytes = m_width*2;
    anErr = QTNewGWorldFromPtr (&m_srcGWorld,
                                //  k422YpCbCr8CodecType,
                                k422YpCbCr8PixelFormat, 
                                // '2vuy',
                                // kComponentVideoUnsigned,
                                &m_srcRect,
                                NULL,
                                NULL,
                                0,
                                m_image.image.data,
                                m_rowBytes);
    post ("using YUV");
  }

  if (anErr!= noErr)
    {
      error("%d error at QTNewGWorldFromPtr", anErr);
      return;
    }
  if (NULL == m_srcGWorld)
    {
      error("could not allocate off screen");
      return;
    }
  SGSetGWorld(m_sg,(CGrafPtr)m_srcGWorld, NULL);
  SGStartPreview(m_sg); //moved to starttransfer?
  m_haveVideo = 1;
}

void videoDarwin :: setupCapture()
{
  if(stop()) {
    SGSetChannelUsage(m_vc, 0);
    SGSetChannelUsage(m_vc, seqGrabPreview);
    SGUpdate(m_sg,0);
    start();
  }
}

void videoDarwin :: destroySeqGrabber()
{
  if (m_vc) {
    if (::SGDisposeChannel(m_sg, m_vc)) {
      error ("Unable to dispose a video channel");
    }
    m_vc = NULL;
  }
  if (m_sg) {
    if (::CloseComponent(m_sg)) {
      error("Unable to dispose a sequence grabber component");
    }
    m_sg = NULL;
    if (m_srcGWorld) {
      ::DisposeGWorld(m_srcGWorld);
      m_pixMap = NULL;
      m_srcGWorld = NULL;
      m_baseAddr = NULL;
    }
  }
}

void videoDarwin :: resetSeqGrabber()
{
  OSErr anErr;
  post ("starting reset");

  destroySeqGrabber();
  initSeqGrabber();
}

bool videoDarwin :: setQuality(int X)
{
  OSErr anErr;

  if (X < 0) X = 0;
  if (X > 3) X = 3;
  m_quality = X;

  switch (m_quality){
  case 0:
    anErr = SGSetChannelPlayFlags(m_vc, channelPlayNormal);
    post("set SG NormalQuality");
    break;
  case 1:
    anErr = SGSetChannelPlayFlags(m_vc, channelPlayHighQuality);
    post("set SG HighQuality");
    break;
  case 2:
    anErr = SGSetChannelPlayFlags(m_vc, channelPlayFast);
    post("set SG FastQuality");
    break;
  case 3:
    anErr = SGSetChannelPlayFlags(m_vc, channelPlayAllData);
    post("set SG PlayAlldata");
    break;
  }
 return true;
}

/////////////////////////////////////////////////////////
// dimenMess
//
/////////////////////////////////////////////////////////
bool videoDarwin :: setDimen(int x, int y, int leftmargin, int rightmargin,
                              int topmargin, int bottommargin)
{
  if (x > 0 )
    m_width = (int)x;
  if (y > 0)
    m_height = (int)y;

  if(stop()) {
    resetSeqGrabber();
    start();
  }
  post("height %d width %d",m_height,m_width);
  return true;
}

/////////////////////////////////////////////////////////
// colorspaceMess
//
/////////////////////////////////////////////////////////
bool videoDarwin :: setColor(int format)
{
  m_colorspace = format;
  if(stop()) {
    resetSeqGrabber();
    start();
  }
}

void videoDarwin :: brightnessMess(float X)
{

  QTAtomContainer         atomContainer;
  QTAtom                  featureAtom;
  VDIIDCFeatureSettings   settings;
  ComponentDescription    desc;
  ComponentResult         result = paramErr;

  //check if device is IIDC
  GetComponentInfo((Component)m_vdig, &desc, NULL, NULL, NULL);
  if (vdSubtypeIIDC != desc.componentSubType){

    unsigned short brightness = (unsigned short)(65536. * X);
    VDSetBrightness(m_vdig,&brightness);

    VDGetBrightness(m_vdig,&brightness);
    post("brightness is %d",brightness);
  }
  else
    {
      //IIDC stuff
      //these things are as stubborn as they are stupid - find one that conforms to spec!

      result = VDIIDCGetFeaturesForSpecifier(m_vdig, vdIIDCFeatureBrightness, &atomContainer);
      if (noErr != result) {
        error("VDIIDCGetFeaturesForSpecifier returned %d",result);
      }

      featureAtom = QTFindChildByIndex(atomContainer, kParentAtomIsContainer,
                                       vdIIDCAtomTypeFeature, 1, NULL);
      if (0 == featureAtom) error("featureAtom not found");

      result = QTCopyAtomDataToPtr(atomContainer,
                                   QTFindChildByID(atomContainer, featureAtom,
                                                   vdIIDCAtomTypeFeatureSettings,
                                                   vdIIDCAtomIDFeatureSettings, NULL),
                                   true, sizeof(settings), &settings, NULL);

      settings.state.flags = (vdIIDCFeatureFlagOn |
                              vdIIDCFeatureFlagManual |
                              vdIIDCFeatureFlagRawControl);

      settings.state.value = X;

      result = QTSetAtomData(atomContainer,
                             QTFindChildByID(atomContainer, featureAtom,
                                             vdIIDCAtomTypeFeatureSettings,
                                             vdIIDCAtomIDFeatureSettings, NULL),
                             sizeof(settings), &settings);

      result = VDIIDCSetFeatures(m_vdig, atomContainer);
    }
}

void videoDarwin :: saturationMess(float X)
{

  QTAtomContainer         atomContainer;
  QTAtom                  featureAtom;
  VDIIDCFeatureSettings   settings;
  ComponentDescription    desc;
  ComponentResult         result = paramErr;

  //check if device is IIDC
  GetComponentInfo((Component)m_vdig, &desc, NULL, NULL, NULL);
  if (vdSubtypeIIDC != desc.componentSubType){

    unsigned short saturation = (unsigned short)(65536. * X);
    VDSetSaturation(m_vdig,&saturation);

    VDGetSaturation(m_vdig,&saturation);
    post("saturation is %d",saturation);
  }
  else
    {
      //IIDC stuff
      result = VDIIDCGetFeaturesForSpecifier(m_vdig, vdIIDCFeatureSaturation, &atomContainer);
      if (noErr != result) {
        error("VDIIDCGetFeaturesForSpecifier vdIIDCFeatureSaturation returned %d",result);
      }

      featureAtom = QTFindChildByIndex(atomContainer, kParentAtomIsContainer,
                                       vdIIDCAtomTypeFeature, 1, NULL);
      if (0 == featureAtom) error("featureAtom vdIIDCFeatureSaturation not found");

      result = QTCopyAtomDataToPtr(atomContainer,
                                   QTFindChildByID(atomContainer, featureAtom,
                                                   vdIIDCAtomTypeFeatureSettings,
                                                   vdIIDCAtomIDFeatureSettings, NULL),
                                   true, sizeof(settings), &settings, NULL);

      settings.state.flags = (vdIIDCFeatureFlagOn |
                              vdIIDCFeatureFlagManual |
                              vdIIDCFeatureFlagRawControl);

      settings.state.value = X;

      result = QTSetAtomData(atomContainer,
                             QTFindChildByID(atomContainer, featureAtom,
                                             vdIIDCAtomTypeFeatureSettings,
                                             vdIIDCAtomIDFeatureSettings, NULL),
                             sizeof(settings), &settings);

      result = VDIIDCSetFeatures(m_vdig, atomContainer);
    }
}

void videoDarwin :: contrastMess(float X)
{
  unsigned short contrast = (unsigned short)(65536. * X);
  VDSetContrast(m_vdig,&contrast);
  
  VDGetContrast(m_vdig,&contrast);
  post("contrast is %d",contrast);
}

void videoDarwin :: exposureMess(float X)
{

  QTAtomContainer         atomContainer;
  QTAtom                  featureAtom;
  VDIIDCFeatureSettings   settings;
  ComponentDescription    desc;
  ComponentResult         result = paramErr;

  //check if device is IIDC
  GetComponentInfo((Component)m_vdig, &desc, NULL, NULL, NULL);
  if (vdSubtypeIIDC == desc.componentSubType){

    //IIDC stuff
    //vdIIDCFeatureExposure
    result = VDIIDCGetFeaturesForSpecifier(m_vdig, vdIIDCFeatureExposure, &atomContainer);
    if (noErr != result) {
      error("VDIIDCGetFeaturesForSpecifier vdIIDCFeatureExposure returned %d",result);
    }

    featureAtom = QTFindChildByIndex(atomContainer, kParentAtomIsContainer,
                                     vdIIDCAtomTypeFeature, 1, NULL);
    if (0 == featureAtom) error("featureAtom vdIIDCFeatureExposure not found");

    result = QTCopyAtomDataToPtr(atomContainer,
                                 QTFindChildByID(atomContainer, featureAtom,
                                                 vdIIDCAtomTypeFeatureSettings,
                                                 vdIIDCAtomIDFeatureSettings, NULL),
                                 true, sizeof(settings), &settings, NULL);

    settings.state.flags = (vdIIDCFeatureFlagOn |
                            vdIIDCFeatureFlagManual |
                            vdIIDCFeatureFlagRawControl);

    settings.state.value = X;

    result = QTSetAtomData(atomContainer,
                           QTFindChildByID(atomContainer, featureAtom,
                                           vdIIDCAtomTypeFeatureSettings,
                                           vdIIDCAtomIDFeatureSettings, NULL),
                           sizeof(settings), &settings);

    result = VDIIDCSetFeatures(m_vdig, atomContainer);
  }
}

void videoDarwin :: gainMess(float X)
{

  QTAtomContainer         atomContainer;
  QTAtom                  featureAtom;
  VDIIDCFeatureSettings   settings;
  ComponentDescription    desc;
  ComponentResult         result = paramErr;

  //check if device is IIDC
  GetComponentInfo((Component)m_vdig, &desc, NULL, NULL, NULL);
  if (vdSubtypeIIDC == desc.componentSubType){

    //IIDC stuff
    //vdIIDCFeatureGain
    result = VDIIDCGetFeaturesForSpecifier(m_vdig, vdIIDCFeatureWhiteBalanceU, &atomContainer);
    if (noErr != result) {
      error("VDIIDCGetFeaturesForSpecifier vdIIDCFeatureExposure returned %d",result);
    }

    featureAtom = QTFindChildByIndex(atomContainer, kParentAtomIsContainer,
                                     vdIIDCAtomTypeFeature, 1, NULL);
    if (0 == featureAtom) error("featureAtom vdIIDCFeatureExposure not found");

    result = QTCopyAtomDataToPtr(atomContainer,
                                 QTFindChildByID(atomContainer, featureAtom,
                                                 vdIIDCAtomTypeFeatureSettings,
                                                 vdIIDCAtomIDFeatureSettings, NULL),
                                 true, sizeof(settings), &settings, NULL);

    settings.state.flags = (vdIIDCFeatureFlagOn |
                            vdIIDCFeatureFlagManual |
                            vdIIDCFeatureFlagRawControl);

    settings.state.value = X;

    result = QTSetAtomData(atomContainer,
                           QTFindChildByID(atomContainer, featureAtom,
                                           vdIIDCAtomTypeFeatureSettings,
                                           vdIIDCAtomIDFeatureSettings, NULL),
                           sizeof(settings), &settings);

    result = VDIIDCSetFeatures(m_vdig, atomContainer);

  }
}

void videoDarwin :: whiteBalanceMess(float U, float V)
{

  QTAtomContainer         atomContainer;
  QTAtom                  featureAtom;
  VDIIDCFeatureSettings   settings;
  ComponentDescription    desc;
  ComponentResult         result = paramErr;

  //check if device is IIDC
  GetComponentInfo((Component)m_vdig, &desc, NULL, NULL, NULL);
  if (vdSubtypeIIDC == desc.componentSubType){

    //IIDC stuff
    //vdIIDCFeatureWhiteBalanceU
    result = VDIIDCGetFeaturesForSpecifier(m_vdig, vdIIDCFeatureWhiteBalanceU, &atomContainer);
    if (noErr != result) {
      error("VDIIDCGetFeaturesForSpecifier vdIIDCFeatureExposure returned %d",result);
    }

    featureAtom = QTFindChildByIndex(atomContainer, kParentAtomIsContainer,
                                     vdIIDCAtomTypeFeature, 1, NULL);
    if (0 == featureAtom) error("featureAtom vdIIDCFeatureExposure not found");

    result = QTCopyAtomDataToPtr(atomContainer,
                                 QTFindChildByID(atomContainer, featureAtom,
                                                 vdIIDCAtomTypeFeatureSettings,
                                                 vdIIDCAtomIDFeatureSettings, NULL),
                                 true, sizeof(settings), &settings, NULL);

    settings.state.flags = (vdIIDCFeatureFlagOn |
                            vdIIDCFeatureFlagManual |
                            vdIIDCFeatureFlagRawControl);

    settings.state.value = U;

    result = QTSetAtomData(atomContainer,
                           QTFindChildByID(atomContainer, featureAtom,
                                           vdIIDCAtomTypeFeatureSettings,
                                           vdIIDCAtomIDFeatureSettings, NULL),
                           sizeof(settings), &settings);

    result = VDIIDCSetFeatures(m_vdig, atomContainer);

    //vdIIDCFeatureWhiteBalanceV
    result = VDIIDCGetFeaturesForSpecifier(m_vdig, vdIIDCFeatureWhiteBalanceV, &atomContainer);
    if (noErr != result) {
      error("VDIIDCGetFeaturesForSpecifier vdIIDCFeatureExposure returned %d",result);
    }

    featureAtom = QTFindChildByIndex(atomContainer, kParentAtomIsContainer,
                                     vdIIDCAtomTypeFeature, 1, NULL);
    if (0 == featureAtom) error("featureAtom vdIIDCFeatureExposure not found");

    result = QTCopyAtomDataToPtr(atomContainer,
                                 QTFindChildByID(atomContainer, featureAtom,
                                                 vdIIDCAtomTypeFeatureSettings,
                                                 vdIIDCAtomIDFeatureSettings, NULL),
                                 true, sizeof(settings), &settings, NULL);

    settings.state.flags = (vdIIDCFeatureFlagOn |
                            vdIIDCFeatureFlagManual |
                            vdIIDCFeatureFlagRawControl);

    settings.state.value = V;

    result = QTSetAtomData(atomContainer,
                           QTFindChildByID(atomContainer, featureAtom,
                                           vdIIDCAtomTypeFeatureSettings,
                                           vdIIDCAtomIDFeatureSettings, NULL),
                           sizeof(settings), &settings);

    result = VDIIDCSetFeatures(m_vdig, atomContainer);
  }
}

/////////////////////////////////////////////////////////
// dialog
//
/////////////////////////////////////////////////////////
bool videoDarwin :: dialog()
{
  Rect    newActiveVideoRect;
  Rect    curBounds, curVideoRect, newVideoRect;
  ComponentResult    err;

  // Get our current state - do i need this???
  err = SGGetChannelBounds (m_vc, &curBounds);
  err = SGGetVideoRect (m_vc, &curVideoRect);

  // Pause
  err = SGPause (m_sg, true);

  // Do the dialog thang
  err = SGSettingsDialog( m_sg, m_vc, 0, nil, 0, nil, 0);

  // What happened?
  err = SGGetVideoRect (m_vc, &newVideoRect);
  err = SGGetSrcVideoBounds (m_vc, &newActiveVideoRect);

  err = SGPause (m_sg, false);

  return true;
}
#else
videoDarwin ::  videoDarwin() {}
videoDarwin :: ~videoDarwin() {}
#endif // __APPLE__
