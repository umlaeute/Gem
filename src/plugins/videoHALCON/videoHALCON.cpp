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

#include "videoHALCON.h"
#include <sstream>
using namespace gem;

#include "Gem/RTE.h"

#if 0
# define debug ::post
#else
# define debug
#endif

/////////////////////////////////////////////////////////
//
// videoHALCON
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
#ifdef HAVE_HALCON

REGISTER_VIDEOFACTORY("halcon", videoHALCON);


// exception handler
static void MyHalconExceptionHandler(const Halcon::HException& except)
{
  // the exception handler is needed in orer to prevent halcon from crashing
  // we just pass on the exception to upstream...
  throw except;	
}




videoHALCON :: videoHALCON() : video("halcon"),
                               m_grabber(NULL)
{
  m_width=0;
  m_height=0;

  Halcon::HException::InstallHHandler(&MyHalconExceptionHandler);

  provide("iidc");
  provide("gige");
  provide("falcon");
}

////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
videoHALCON :: ~videoHALCON()
{
  close();
}


////////////////////////////////////////////////////////
// frame grabber
//
/////////////////////////////////////////////////////////
bool videoHALCON :: grabFrame() {
  Halcon::HImage img;
  try {
    img=m_grabber->GrabImage();
  } catch (Halcon::HException& except) {
    error("Halcon::GrabImage exception: '%s'", except.message);
    return false;
  }

  Halcon::HTuple typ, H, W, pR, pG, pB;
  long r, g, b,  h, w;

  try {
    r = img.GetImagePointer3(&pG, &pB, &typ, &H, &W);
  } catch (Halcon::HException& except) {
    error("Halcon::GetImagePointer exception: '%s'", except.message);
    return false;
  }

  try {
#if 0
#define GETTUPLE(x, y) { try {x=y[0]; } catch (Halcon::HException& except) { error("HTuple exception @ %d: '%s'", __LINE__, except.message); } } while(0)
    GETTUPLE(g, pG);
    GETTUPLE(b, pB);
    GETTUPLE(w, W);
    GETTUPLE(h, H);
#else
    g=pG[0];
    b=pB[0];

    w=W[0];
    h=H[0];
#endif

    const unsigned char* ptrR=(const unsigned char*)r;
    const unsigned char* ptrG=(const unsigned char*)g;
    const unsigned char* ptrB=(const unsigned char*)b;

    //    post("image[%dx%d]: %x %x %x --> %x %x %x", w, h, r, g, b, ptrR, ptrG, ptrB);
    lock();
    m_image.image.xsize=w;
    m_image.image.ysize=h;
    m_image.image.setCsizeByFormat(GL_RGBA);
    m_image.image.reallocate();

    long row, col;
    unsigned char*data=m_image.image.data;
    for(row=0; row<h; row++) {
      for(col=0; col<w; col++) {
        //        post("[%d,%d]", row, col);
        data[chRed  ]=*ptrR++;
        data[chGreen]=*ptrG++;
        data[chBlue ]=*ptrB++;
        data[chAlpha]=255;
        data+=4;
      }
    }
    m_image.newimage=true;
    m_image.image.upsidedown=true;
    unlock();

  } catch (Halcon::HException& except) { 
    verbose(1, "Halcon::HTuple exception: '%s'", except.message); 
  } 


  return true;
}


/**
 * device name parser
 */
static std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while(std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}
static std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    return split(s, delim, elems);
}

static std::string parsedevicename(std::string devicename, std::string&cameratype, std::string&device) {
  std::string name;
  if(devicename.empty())return name;

  std::vector<std::string> parsed = split(devicename, ':');
  switch(parsed.size()) {
  default:
    verbose(1, "could not parse '%s'", devicename.c_str());
    return name;
  case 3:
    device=parsed[2];
  case 2:
    cameratype=parsed[1];
  case 1:
    name=parsed[0];
  }
  verbose(1, "HALCON: name  ='%s'", name.c_str());
  verbose(1, "HALCON: camera='%s'", cameratype.c_str());
  verbose(1, "HALCON: device='%s'", device.c_str());
  return name;
}

/////////////////////////////////////////////////////////
// openDevice
//
/////////////////////////////////////////////////////////
bool videoHALCON :: openDevice()
{
  if(m_grabber)closeDevice();
  

  /* m_devicename has to provide:
   *    backendid
   *    cameratype
   *    device
   *
   * originally i though about using ":" as s delimiter, 
   *  e.g. "GigEVision::001234567890" would use the GigE-device @ "00:12:34:56:78:90" with "default" cameratype
   * however, ":" is already internally used by e.g. the "1394IIDC" backend, that uses "format:mode:fps" as cameratype
   *
   * either use another delimiter, or find some escaping mechanism (e.g. '1394IIDC:"0:4:5":0x0814436102632378'
   *
   * another idea would be to get an idea about which <driver> was selected in [pix_video] and use that as the <backendid>
   * for this to work, we would have to provide a list of valid backends (e.g. dynamically query what is installed) 
   * i don't think this is currently possible with halcon
   *
   */


  const int width=(m_width>0) ?m_width:0;
  const int height=(m_height>0)?m_height:0;
  std::string cameratype="default";
  std::string device="default";
  const int port=(m_channel>0)?m_channel:-1;

  std::string name=parsedevicename(m_devicename, cameratype, device);
  if(name.empty()) {
    return false;
  }

  try {
    m_grabber = new Halcon::HFramegrabber(
                                          name.c_str(), /* const HTuple &Name, */
                                          1, 1, /* const HTuple &HorizontalResolution = 1, const HTuple &VerticalResolution = 1, */
                                          width, height, /* const HTuple &ImageWidth = 0,           const HTuple &ImageHeight = 0, */
                                          0, 0, /* const HTuple &StartRow = 0,             const HTuple &StartColumn = 0, */
                                          "default", /* const HTuple &Field = "default", */
                                          8, /* const HTuple &BitsPerChannel = -1,  */
                                          "rgb", /* const HTuple &ColorSpace = "default", */
                                          -1, /* const HTuple &Gain = -1, */
                                          "default", /* const HTuple &ExternalTrigger = "default", */
                                          cameratype.c_str(), /* const HTuple &CameraType = "default", */
                                          device.c_str(), /* const HTuple &Device = "default", */
                                          port /* const HTuple &Port = -1, */
                                          /* const HTuple &LineIn = -1 */
                                          );
  } catch (Halcon::HException &except) {
    error("caught exception: '%s'", except.message);
    return false;
  }
                              
                              
  return true;
}
/////////////////////////////////////////////////////////
// closeDevice
//
/////////////////////////////////////////////////////////
void videoHALCON :: closeDevice() {
  if(m_grabber)delete m_grabber;
  m_grabber=NULL;
}


/////////////////////////////////////////////////////////
// startTransfer
//
/////////////////////////////////////////////////////////
bool videoHALCON :: startTransfer()
{
  return true;
}

/////////////////////////////////////////////////////////
// stopTransfer
//
/////////////////////////////////////////////////////////
bool videoHALCON :: stopTransfer()
{
  return true;
}

std::vector<std::string> videoHALCON::enumerate() {
  std::vector<std::string> result;
  return result;
}




#else
videoHALCON :: videoHALCON() : video("")
{ }
videoHALCON :: ~videoHALCON()
{ }
#endif /* HAVE_HALCON */
