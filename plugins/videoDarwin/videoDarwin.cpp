/*
 *  videoDarwin.cpp
 *  gem_darwin
 *
 *  Created by James Tittle on Fri Jul 12 2002.
 *  Copyright (c) 2002-2005 James Tittle & Chris Clepper
 *  Copyright (c) 2010 IOhannes m zmÃ¶lnig
 *
 */
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#if defined __APPLE__  && defined __x86_64
#warning unconditionally disabling QuickTime/Carbon on OSX/64bit
// with OSX10.6, apple has removed loads of Carbon functionality (in 64bit mode)
// LATER make this a real check in configure
# undef HAVE_QUICKTIME
# undef HAVE_CARBON
#endif

#if defined HAVE_CARBON && defined HAVE_QUICKTIME
# define HAVE_VIDEODARWIN
#endif

#include "videoDarwin.h"
#include "Gem/RTE.h"
#include "plugins/PluginFactory.h"

using namespace gem::plugins;


#ifdef HAVE_VIDEODARWIN
#include <unistd.h> // needed for Unix file open() type functions
#include <stdio.h>
#include <fcntl.h> /* JMZ thinks that _this_ is needed for open() */

#define DEFAULT_WIDTH        320
#define DEFAULT_HEIGHT        240
  
REGISTER_VIDEOFACTORY("Darwin", videoDarwin);

videoDarwin :: videoDarwin() 
  : video("darwin", 0),
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
  //initSeqGrabber();

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
  close();
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
bool videoDarwin :: openDevice(gem::Properties&props) {
  double d;
  if(props.get("width", d))
    m_width=d;
  if(props.get("height", d))
    m_height=d;
  if(props.get("channel", d))
    m_inputDeviceChannel=d;

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


static std::string pascal2str(const Str255 pstr) {
  const unsigned char*cstr=static_cast<const unsigned char*>(pstr);
  const char*str=(const char*)(cstr+1);
  const size_t length=cstr[0];
  return std::string(str, length);
}
void videoDarwin :: initSeqGrabber()
{
  OSErr anErr;
  Rect m_srcRect = {0,0, m_height, m_width};
post("srcRect=%dx%d", m_width, m_height);
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
    m_devices.clear();
    for (i = 0; i < deviceCount; i++){
      m_devices.push_back(pascal2str((*devices)->entry[i].name));
      post("SG channnel Device List[%d]  %s", i, m_devices[i].c_str());
    }
    SGGetChannelDeviceAndInputNames(m_vc, NULL, NULL, &inputIndex);

    showInputsAsDevices = ((*devices)->entry[deviceIndex].flags) & sgDeviceNameFlagShowInputsAsDevices;

    theSGInputList = ((SGDeviceName *)(&((*devices)->entry[deviceIndex])))->inputs; //fugly

    //we should have device names in big ass undocumented structs

    //walk through the list
    for (i = 0; i < inputIndex; i++){
      std::string input=pascal2str((*theSGInputList)->entry[i].name);
      post("SG channnel Input Device List %d %s",
           i, input.c_str());
    }
  }

  if(m_devicenum>=0)
    m_inputDevice=m_devicenum;
  else {
    int i;
    for(i=0; i<deviceCount; i++) {
      if(m_devicename==m_devices[i]) {
        m_inputDevice=i;
        break;
      }
    }
  }

  //this call sets the input device
  if (m_inputDevice >= 0 && m_inputDevice < deviceCount) {//check that the device is not out of bounds
    std::string devname=pascal2str((*devices)->entry[m_inputDevice].name);
    post("SGSetChannelDevice trying[%d] %s", m_inputDevice, devname.c_str());
  }
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
  post("vdigName is %s",pascal2str(vdigName).c_str());

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


/////////////////////////////////////////////////////////
// dialog
//
/////////////////////////////////////////////////////////
bool videoDarwin :: dialog(std::vector<std::string>dlg)
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

std::vector<std::string>videoDarwin :: dialogs(void) {
    std::vector<std::string>result;
    return result;
}

bool videoDarwin::enumProperties(gem::Properties&readable,
                                gem::Properties&writeable) {
  bool iidc=false;
  gem::any typ;

  readable.clear();
  writeable.clear();

  typ=1.;
  if(m_vdig) {
    ComponentDescription    desc;
    GetComponentInfo((Component)m_vdig, &desc, NULL, NULL, NULL);

    iidc=vdSubtypeIIDC != desc.componentSubType;
  }
#define SETPROP(key, value) typ=value; readable.set(key, typ); writeable.set(key, typ)
#define SETRPROP(key, value) typ=value; readable.set(key, typ)
#define SETWPROP(key, value) typ=value; writeable.set(key, typ)
  SETPROP("Brightness", 1);
  SETPROP("Saturation", 1);
  if(iidc) {
   SETPROP("Contrast", 1);
  } else {
   SETWPROP("Exposure", 1);
   SETWPROP("WhitebalanceU", 1);
   SETWPROP("WhitebalanceV", 1);
  }
 return true;
}

bool videoDarwin::setIIDCProperty(OSType specifier, double value) {
    QTAtomContainer         atomContainer;
    QTAtom                  featureAtom;
    VDIIDCFeatureSettings   settings;
    ComponentDescription    desc;
    ComponentResult         result = paramErr;
    
    //IIDC stuff
    result = VDIIDCGetFeaturesForSpecifier(m_vdig, specifier, &atomContainer);
    if (noErr != result) {
     return false;
    }
    
    featureAtom = QTFindChildByIndex(atomContainer, kParentAtomIsContainer,
                                     vdIIDCAtomTypeFeature, 1, NULL);
    if (0 == featureAtom) return false;//error("featureAtom vdIIDCFeatureSaturation not found");
    
    result = QTCopyAtomDataToPtr(atomContainer,
                                 QTFindChildByID(atomContainer, featureAtom,
                                                 vdIIDCAtomTypeFeatureSettings,
                                                 vdIIDCAtomIDFeatureSettings, NULL),
                                 true, sizeof(settings), &settings, NULL);
    
    settings.state.flags = (vdIIDCFeatureFlagOn |
                            vdIIDCFeatureFlagManual |
                            vdIIDCFeatureFlagRawControl);
    
    settings.state.value = value;
    
    result = QTSetAtomData(atomContainer,
                           QTFindChildByID(atomContainer, featureAtom,
                                           vdIIDCAtomTypeFeatureSettings,
                                           vdIIDCAtomIDFeatureSettings, NULL),
                           sizeof(settings), &settings);
    
    result = VDIIDCSetFeatures(m_vdig, atomContainer);
    
    return true;
}
void videoDarwin::setProperties(gem::Properties&props) {
  ComponentDescription    desc;
  GetComponentInfo((Component)m_vdig, &desc, NULL, NULL, NULL);
  double d;

  std::vector<std::string>keys=props.keys();
    int i=0;
    bool iidc=(vdSubtypeIIDC == desc.componentSubType);
    for(i=0; i<keys.size(); i++) {
        std::string key=keys[i];
        if("Contrast"==key) {
            if(props.get(key, d)) {
                unsigned short contrast = (unsigned short)(65536. * d);
                post("setting contrast to %f -> %d", d, contrast);
                VDSetContrast(m_vdig,&contrast);
            }
        } else if("Saturation"==key) {
            if(props.get(key, d)) {                
                if (!iidc){
                    unsigned short saturation = (unsigned short)(65536. * d);
                    post("setting saturation to %f -> %d", d, saturation); 
                    VDSetSaturation(m_vdig,&saturation);
                    post("set saturation to %f -> %d", d, saturation); 
                    VDGetSaturation(m_vdig,&saturation);
                    post("saturation is %d",saturation);
                } else {
                if(!setIIDCProperty(vdIIDCFeatureSaturation, d)) {
                    error("failed setting Saturation using IIDC");
                }
             }
            }
        } else if("Brightness"==key) {
            if(props.get(key, d)) {                
                if (!iidc){
                    unsigned short brightness = (unsigned short)(65536. * d);
                    post("setting brightness to %f -> %d", d, brightness); 
                    VDSetBrightness(m_vdig,&brightness);
                    post("set brightness to %f -> %d", d, brightness); 
                    VDGetBrightness(m_vdig,&brightness);
                    post("brightness is %d",brightness);
                } else {
                if(!setIIDCProperty(vdIIDCFeatureBrightness, d)) {
                    error("failed setting Brightness using IIDC");
                }
             }
            }
        } else if("Exposure"==key) {
            if(props.get(key, d)) {                
                if (iidc){
                 if(!setIIDCProperty(vdIIDCFeatureExposure, d)) {
                    error("failed setting Exposure using IIDC");
                }
             }
            }
        } else if("WhiteBalanceU"==key) {
            if(props.get(key, d)) {                
                if (iidc){
                 if(!setIIDCProperty(vdIIDCFeatureWhiteBalanceU, d)) {
                    error("failed setting WhiteBalanceU using IIDC");
                }
             }
            }
        } else if("WhiteBalanceV"==key) {
            if(props.get(key, d)) {                
                if (iidc){
                 if(!setIIDCProperty(vdIIDCFeatureWhiteBalanceV, d)) {
                    error("failed setting WhiteBalanceV using IIDC");
                }
             }
            }
        }

    }

}
void videoDarwin::getProperties(gem::Properties&props)
{
  std::vector<std::string>keys=props.keys();

  ComponentDescription    desc;
  GetComponentInfo((Component)m_vdig, &desc, NULL, NULL, NULL);
  bool iidc=(vdSubtypeIIDC == desc.componentSubType);
  int i=0;
  for(i=0; i<keys.size(); i++) {
    std::string key=keys[i];
    if("Brightness"==key) {
      if (!iidc){
        unsigned short brightness = 0;
        VDGetBrightness(m_vdig,&brightness);
        double d=brightness/65536.;
        props.set(key, d);
      } else {
        post("how to get IIDC/brightness?");
      }
    } else if("Saturation"==key) {
      if (!iidc){
        unsigned short saturation = 0;
        VDGetSaturation(m_vdig,&saturation);
        double d=saturation/65536.;
        props.set(key, d);
      } else {
        post("how to get IIDC/saturation?");
      }
    } else if("Contrast"==key) {
    if (!iidc){
        unsigned short contrast = 0;
        VDGetContrast(m_vdig,&contrast);
        double d=contrast/65536.;
        props.set(key, d);
      } else {
        post("how to get IIDC/contrast?");
      }
#if 0
    } else if("Exposure"==key) {
        post("how to get IIDC/exposure?");
    } else if("WhitebalanceU"==key) {
        post("how to get IIDC/whitebalanceU?");
    } else if("WhitebalanceV"==key) {
        post("how to get IIDC/whitebalanceV?");
#endif
    }
  }
}

std::vector<std::string> videoDarwin::enumerate() {
  std::vector<std::string> result;
  OSErr anErr;
  SGDeviceList    devices;

  anErr = SGGetChannelDeviceList(m_vc, sgDeviceListIncludeInputs, &devices);
  if(anErr!=noErr){
    error("could not get SG channnel Device List");
  }else{
    short deviceCount = (*devices)->count;
    short deviceIndex = (*devices)->selectedIndex;
    short inputIndex;
    post("SG channnel Device List count %d index %d",deviceCount,deviceIndex);
    int i;
    m_devices.clear();
    for (i = 0; i < deviceCount; i++){
      m_devices.push_back(pascal2str((*devices)->entry[i].name));
      post("SG channnel Device List[%d]  %s", i, m_devices[i].c_str());
    }
    SGGetChannelDeviceAndInputNames(m_vc, NULL, NULL, &inputIndex);

    bool showInputsAsDevices = ((*devices)->entry[deviceIndex].flags) & sgDeviceNameFlagShowInputsAsDevices;

    SGDeviceInputList theSGInputList = ((SGDeviceName *)(&((*devices)->entry[deviceIndex])))->inputs; //fugly

    //we should have device names in big ass undocumented structs
    //walk through the list
    for (i = 0; i < inputIndex; i++){
      std::string input=pascal2str((*theSGInputList)->entry[i].name);
      post("SG channnel Input Device List %d %s",
           i, input.c_str());
    }
  }

  result=m_devices;
  return result;
}
#else
videoDarwin ::  videoDarwin() : video("") {}
videoDarwin :: ~videoDarwin() {}
#endif // __APPLE__
