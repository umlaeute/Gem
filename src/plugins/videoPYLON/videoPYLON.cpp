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

#include "videoPYLON.h"
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
// videoPYLON
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
#ifdef HAVE_PYLON

REGISTER_VIDEOFACTORY("pylon", videoPYLON);


// exception handler
static void MyPylonExceptionHandler(const Pylon::HException& except)
{
  // the exception handler is needed in order to prevent pylon from crashing
  // we just pass on the exception to upstream...
  throw except;	
}

videoPYLON :: videoPYLON() : video("pylon"),
                               m_grabber(NULL)
{
  m_width=0;
  m_height=0;

  Pylon::HException::InstallHHandler(&MyPylonExceptionHandler);

  provide("iidc");
  provide("gige");
}

////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
videoPYLON :: ~videoPYLON()
{
  close();
}


////////////////////////////////////////////////////////
// frame grabber
//
/////////////////////////////////////////////////////////
bool videoPYLON :: grabFrame() {
  Pylon::HImage img;

  if(NULL==m_grabber)
    return false;

  try {
    img=m_grabber->GrabImageAsync(-1);
  } catch (Pylon::HException& except) {
    error("Pylon::GrabImage exception: '%s'", except.message);
    return false;
  }
  Pylon::HTuple typ, W, H, pR, pG, pB;
  long r, g, b,  h, w;

  try {
    r = img.GetImagePointer3(&pG, &pB, &typ, &W, &H);
  } catch (Pylon::HException& except) {
    error("Pylon::GetImagePointer exception: '%s'", except.message);
    return false;
  }

  try {
#if 0
#define GETTUPLE(x, y) { try {x=y[0]; } catch (Pylon::HException& except) { error("HTuple exception @ %d: '%s'", __LINE__, except.message); } } while(0)
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
    //post("image[%dx%d]: %x %x %x --> %x %x %x", w, h, r, g, b, ptrR, ptrG, ptrB);
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

  } catch (Pylon::HException& except) { 
    verbose(1, "Pylon::HTuple exception: '%s'", except.message); 
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
    if(parsed[2].size()>0)
      device=parsed[2];
  case 2:
    if(parsed[1].size()>0)
      cameratype=parsed[1];
  case 1:
    name=parsed[0];
  }
  verbose(1, "PYLON: name  ='%s'", name.c_str());
  verbose(1, "PYLON: camera='%s'", cameratype.c_str());
  verbose(1, "PYLON: device='%s'", device.c_str());
  return name;
}

/////////////////////////////////////////////////////////
// openDevice
//
/////////////////////////////////////////////////////////

static void printtuple(Pylon::HTuple t) {
  int i=0;
  for(i=0; i< t.Num(); i++) {
  
    Pylon::HCtrlVal v=t[i];
    std::cerr<<"["<<i<<"]: ";
    switch(v.ValType()) {
    case Pylon::LongVal:
      std::cerr << v.L();
      break;
    case Pylon::DoubleVal:
      std::cerr << v.D();
      break;
    case Pylon::StringVal:
      std::cerr << v.S();
      break;
    case Pylon::UndefVal:
      std::cerr << "<undef>";
    }
    std::cerr << std::endl;
  }
}

static void printinfo(std::string name, std::string value) {
  try {
    Pylon::HTuple Information;
    Pylon::HTuple ValueList;
    Herror err=info_framegrabber(name.c_str(), 
                                 value.c_str(),
                                 &Information,
                                 &ValueList);

    std::cerr << "got info for "<<name<<"."<<value<<":"<<std::endl; 
    printtuple(Information);
    std::cerr << "got values: " << std::endl; 
    printtuple(ValueList); 
    std::cerr << std::endl;
  }  catch (Pylon::HException &except) {
    error("info caught exception: '%s'", except.message);
  }
}


static void getparam(Pylon::HFramegrabber*grabber, std::string name) {
  try {
    Pylon::HTuple result=grabber->GetFramegrabberParam(name.c_str());
    std::cerr << "got parm for "<<name<<std::endl; 
    printtuple(result); 
  }  catch (Pylon::HException &except) {
    error("getparam caught exception: '%s'", except.message);
  }
}

bool videoPYLON :: openDevice()
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
   * i don't think this is currently possible with pylon
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
    m_grabber = new Pylon::HFramegrabber(
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
  } catch (Pylon::HException &except) {
    error("caught exception: '%s'", except.message);
    m_grabber=NULL;
    return false;
  }

#if 0
  printinfo(name, "parameters");
  printinfo(name, "parameters_readonly");
  printinfo(name, "parameters_writeonly");

  printinfo(name, "port");

  getparam(m_grabber, "color_space_values");
  getparam(m_grabber, "revision");
#endif                              
                              
  return true;
}
/////////////////////////////////////////////////////////
// closeDevice
//
/////////////////////////////////////////////////////////
void videoPYLON :: closeDevice() {
  if(m_grabber)delete m_grabber;
  m_grabber=NULL;
}


/////////////////////////////////////////////////////////
// startTransfer
//
/////////////////////////////////////////////////////////
bool videoPYLON :: startTransfer()
{
  if(NULL!=m_grabber)
    m_grabber->GrabImageStart(-1);
  return true;
}

/////////////////////////////////////////////////////////
// stopTransfer
//
/////////////////////////////////////////////////////////
bool videoPYLON :: stopTransfer()
{
  return true;
}

std::vector<std::string> videoPYLON::enumerate() {
  std::vector<std::string> result;
  return result;
}




#else
videoPYLON :: videoPYLON() : video("")
{ }
videoPYLON :: ~videoPYLON()
{ }
#endif /* HAVE_PYLON */
