////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 2010 IOhannes m zmoelnig. forum::für::umläute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "LICENSE.txt" 
//
/////////////////////////////////////////////////////////
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "videoAVT.h"
#include <sstream>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <errno.h>

using namespace gem;

#include "Gem/RTE.h"
#include "Base/GemException.h"

#if 0
# define debug ::post
#else
# define debug
#endif

/////////////////////////////////////////////////////////
//
// videoAVT
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
#ifdef HAVE_AVT

#define MAX_CAMERA_LIST 20



REGISTER_VIDEOFACTORY("avt", videoAVT);

struct PvApiInitClass {
 PvApiInitClass(void) {
   unsigned long major=0, minor=0;
   PvVersion(&major, &minor);
   //   post("Prosilica AVT SDK %d.%d", major, minor);

   if(ePvErrResources==PvInitialize()) {
     throw(GemException("unable to initialization PvAPI"));
   }
 }
 virtual ~PvApiInitClass(void) {
   PvUnInitialize();
 }
};

videoAVT :: videoAVT() : video("avt"),
                               m_grabber(NULL)
{
  m_width=0;
  m_height=0;

  static PvApiInitClass paic;

  int i=0;
  for(i=0;i<AVT_FRAMESCOUNT;i++) {
    m_frames[i].ImageBuffer = NULL;
    m_frames[i].ImageBufferSize = 0;
  }

  provide("gige");
}

////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
videoAVT :: ~videoAVT()
{
  close();

  resizeFrames(0);
}

void videoAVT::resizeFrames(unsigned long int size) {
  int i=0;
  for(i=0;i<AVT_FRAMESCOUNT;i++) {
    if(m_frames[i].ImageBuffer) {
      unsigned char*frame=(unsigned char*)m_frames[i].ImageBuffer;
      delete[]frame;
      m_frames[i].ImageBuffer = NULL;
    }
    m_frames[i].ImageBufferSize = 0;

    if(size) {
      m_frames[i].ImageBuffer = new unsigned char[size];
      if(m_frames[i].ImageBuffer)
        m_frames[i].ImageBufferSize = size;
    }

    m_frames[0].AncillaryBuffer=NULL;
    m_frames[0].AncillaryBufferSize=0;
  }
}


////////////////////////////////////////////////////////
// frame grabber
//
/////////////////////////////////////////////////////////
bool videoAVT :: grabFrame() {
  return true;
}

void videoAVT::grabbedFrame(const tPvFrame&pFrame) {
  /* frame successfully grabbed -> convert it into a pixbuf */
  bool success=true;

  lock();
  m_image.image.xsize=pFrame.Width;
  m_image.image.ysize=pFrame.Height;
  m_image.image.setCsizeByFormat(GL_RGBA);
  m_image.image.reallocate();

  switch(pFrame.Format) {
  case(ePvFmtMono8) : m_image.image.fromGray((unsigned char *)pFrame.ImageBuffer);break;
    //  case(ePvFmtMono16): m_image.image.fromGray((unsigned short*)pFrame.ImageBuffer);break;
  case(ePvFmtRgb24) : m_image.image.fromRGB ((unsigned char *)pFrame.ImageBuffer);break;
  case(ePvFmtBgr24) : m_image.image.fromBGR ((unsigned char *)pFrame.ImageBuffer);break;
  case(ePvFmtRgba32): m_image.image.fromRGBA((unsigned char *)pFrame.ImageBuffer);break;
  case(ePvFmtBgra32): m_image.image.fromBGRA((unsigned char *)pFrame.ImageBuffer);break;
  default:
    // ouch
    success=false;
  }

  if(success) {
    m_image.image.upsidedown=true;
    m_image.newimage=true;
  }

  unlock();
}
void videoAVT::grabCB(tPvFrame*pFrame) {
  videoAVT*me=(videoAVT*)pFrame->Context[0];

  if(me && ePvErrSuccess==pFrame->Status)
    me->grabbedFrame(*pFrame);

  // if the frame was completed we re-enqueue it
  if(pFrame->Status != ePvErrUnplugged && pFrame->Status != ePvErrCancelled)
    {
      PvCaptureQueueFrame(me->m_grabber, pFrame, grabCB);
    }
}

pixBlock* videoAVT::getFrame(void) {
  if(!(m_haveVideo && m_capturing))return NULL;

  lock();
  return &m_image;
}



/////////////////////////////////////////////////////////
// openDevice
//
/////////////////////////////////////////////////////////
bool videoAVT :: openDevice()
{
  if(m_grabber)closeDevice();

  unsigned long cameraNum=PvCameraCount();
  tPvCameraInfo*cameraList=new tPvCameraInfo[cameraNum];

  if(m_devicenum>=0) {
    verbose(1, "AVT trying to open #%d of %d devices", m_devicenum, cameraNum);
    if(cameraNum>m_devicenum && (cameraList[m_devicenum].PermittedAccess == ePvAccessMaster)) {
      if (PvCameraOpen(cameraList[m_devicenum].UniqueId, ePvAccessMaster, &m_grabber) != ePvErrSuccess) {
        m_grabber=NULL;
      }
    }
  } else {
    verbose(1, "AVT trying to open device '%s'", m_devicename.c_str());
    /*
      cameraList[i].SerialString,
      cameraList[i].DisplayName,
      cameraList[i].UniqueId,
      IP
    */
    unsigned long i=0;
    errno=0;
    const unsigned long uid=strtoul(m_devicename.c_str(), NULL, 0);

    if(NULL==m_grabber && 0==errno) {
      verbose(1, "checking UniqueID: % 8lu", uid);
      for(i=0; i<cameraNum; i++) {
        if(uid==cameraList[i].UniqueId && PvCameraOpen(cameraList[i].UniqueId, ePvAccessMaster, &m_grabber) == ePvErrSuccess) {
          break;
        }
      }
    }

    if(NULL==m_grabber) {
      verbose(1, "checking SerialString: %s", m_devicename.c_str());
      for(i=0; i<cameraNum; i++) {
        if(m_devicename==cameraList[i].SerialString && PvCameraOpen(cameraList[i].UniqueId, ePvAccessMaster, &m_grabber) == ePvErrSuccess) {
          break;
        } 
      }
    }
    if(NULL==m_grabber) {
      verbose(1, "checking DisplayName: %s", m_devicename.c_str());
      for(i=0; i<cameraNum; i++) {
        if(m_devicename==cameraList[i].DisplayName && PvCameraOpen(cameraList[i].UniqueId, ePvAccessMaster, &m_grabber) == ePvErrSuccess) {
          break;
        } 
      }
    }

    if(NULL==m_grabber) {
      struct addrinfo *result=NULL, *rp=NULL;
      struct addrinfo hints;
      memset(&hints, 0, sizeof(struct addrinfo));
      hints.ai_family=PF_INET;
      if(0==getaddrinfo(m_devicename.c_str(), NULL, NULL, &result)) {
        unsigned long OldAddr=0;
        for(rp=result; rp!=NULL; rp=rp->ai_next) {
          struct sockaddr_in*ipv4 = (struct sockaddr_in*)result->ai_addr;
          unsigned long IpAddr=ipv4->sin_addr.s_addr ; // byte order??
          if(OldAddr==IpAddr)continue;
          OldAddr=IpAddr;
          verbose(1, "AVT trying to connect to %3d.%3d.%3d.%3d", 
                  (IpAddr & 0x0FF),
                  (IpAddr & 0x0FF00)>>8,
                  (IpAddr & 0x0FF0000)>>16,
                  (IpAddr & 0xFF000000)>>24
                  );
          
          if(ePvErrSuccess == PvCameraOpenByAddr(IpAddr, ePvAccessMaster, &m_grabber)) {
            break;
          }
          m_grabber=NULL;   
        }
        freeaddrinfo(result);
      }
    }
    
  }


  delete[]cameraList;

  if(m_grabber) {
    unsigned long FrameSize = 0;
    if(!PvAttrUint32Get(m_grabber,"TotalBytesPerFrame",&FrameSize)) {
      resizeFrames(FrameSize);
    } else {
      close();
    }


  }

  return (NULL!=m_grabber);
}
/////////////////////////////////////////////////////////
// closeDevice
//
/////////////////////////////////////////////////////////
void videoAVT :: closeDevice() {
  if(m_grabber)PvCameraClose(m_grabber);
  m_grabber=NULL;
}


/////////////////////////////////////////////////////////
// startTransfer
//
/////////////////////////////////////////////////////////
bool videoAVT :: startTransfer()
{
  PvCaptureStart(m_grabber);
  if(!PvCommandRun(m_grabber,"AcquisitionStart")) {
    error("AVT::AcquistionStart failed");
  } else {
    PvCaptureQueueFrame(m_grabber, &m_frames[0], grabCB);
  }

  return true;
}

/////////////////////////////////////////////////////////
// stopTransfer
//
/////////////////////////////////////////////////////////
bool videoAVT :: stopTransfer()
{
  PvCaptureQueueClear(m_grabber);

  if(!PvCommandRun(m_grabber,"AcquisitionStop")) {
    error("AVT::AcquistionStop failed");
  }

  PvCaptureEnd(m_grabber);
  return true;
}

std::vector<std::string> videoAVT::enumerate() {
  std::vector<std::string> result;
  unsigned long cameraNum=PvCameraCount();
  tPvCameraInfo*cameraList=new tPvCameraInfo[cameraNum];
  cameraNum = PvCameraList(cameraList,cameraNum,NULL);

  unsigned long i = 0;
  for (i = 0; i < cameraNum; i++) {
    result.push_back(cameraList[i].DisplayName);
  }
  
  delete[]cameraList;
  return result;
}




#else
videoAVT :: videoAVT() : video("")
{ }
videoAVT :: ~videoAVT()
{ }
#endif /* HAVE_AVT */
