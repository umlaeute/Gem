////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.at
//
// Implementation file
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "recordDECKLINK.h"

// for verbose():
#include "Gem/RTE.h"
#include "Gem/Exception.h"


#include "plugins/PluginFactory.h"

#include <pthread.h>

#include <algorithm>
#include <cctype>
#include <string>
#include <map>

#include <stdio.h>
#define MARK() printf("%s:%d\t%s\n", __FILE__, __LINE__, __FUNCTION__)

#ifdef _WIN32
#include <windows.h>
#include "DeckLinkAPI_i.c"
typedef BOOL deckbool_t;
typedef wchar_t* deckstring_t;
namespace {
  deckstring_t string2deckstring(const std::string&s) {
    int n = MultiByteToWideChar(CP_UTF8, 0, s.data(), s.size(), NULL, 0);
    deckstring_t buf = new wchar_t[n];
    MultiByteToWideChar(CP_UTF8, 0, s.data(), s.size(), &buf[0], n);
    return buf;
  }
  std::string deckstring2string(deckstring_t s) {
    if (!s){
      return std::string();
    }
    size_t l = wcslen(s);
    int n = WideCharToMultiByte(CP_UTF8, 0, s, l, NULL, 0, NULL, NULL);
    std::string buf;
    buf.resize(n);
    WideCharToMultiByte(CP_UTF8, 0, s, l, &buf[0], n, NULL, NULL);
    return buf;
  }
  void free_deckstring(deckstring_t s) {
    wchar_t*b=s;
    delete[]b;
  }
};
#elif defined __APPLE__
typedef bool deckbool_t;
typedef CFStringRef deckstring_t;
namespace {
  deckstring_t string2deckstring(const std::string&s) {
    return CFStringCreateWithCString(NULL, s.c_str(), kCFStringEncodingUTF8);
  }
  std::string deckstring2string(deckstring_t s) {
    if (const char* fastCString = CFStringGetCStringPtr(s, kCFStringEncodingUTF8))
    {
      return std::string(fastCString);
    }
    CFIndex utf16length = CFStringGetLength(s);
    CFIndex maxUtf8len = CFStringGetMaximumSizeForEncoding(utf16length, kCFStringEncodingUTF8);
    char*cstr = new char[maxUtf8len+1];
    cstr[maxUtf8len] = 0;
    CFStringGetCString(s, cstr, maxUtf8len, kCFStringEncodingUTF8);
    std::string converted(cstr);
    delete[]cstr;
    return converted;
  }
  void free_deckstring(deckstring_t s) {
    CFRelease(s);
  }
};
#else /* linux */
# include <string.h>
typedef bool deckbool_t;
typedef const char* deckstring_t;
namespace {
  deckstring_t string2deckstring(const std::string&s) {
    return strdup(s.c_str());
  }
  std::string deckstring2string(deckstring_t s) {
    return std::string(s);
  }
  void free_deckstring(deckstring_t s) {
    free((void*)s);
  }
};
#endif


static std::map<std::string, BMDVideoConnection> s_connectionstrings;
namespace {
  BMDVideoConnection string2connection(std::string Name) {
    static bool done = false;
    if(!done) {
      s_connectionstrings["auto"] = (BMDVideoConnection)-1;
      s_connectionstrings["sdi"] = bmdVideoConnectionSDI;
      s_connectionstrings["hdmi"] = bmdVideoConnectionHDMI;
      s_connectionstrings["opticalsdi"] = bmdVideoConnectionOpticalSDI;
      s_connectionstrings["component"] = bmdVideoConnectionComponent;
      s_connectionstrings["composite"] = bmdVideoConnectionComposite;
      s_connectionstrings["svideo"] = bmdVideoConnectionSVideo;
#if __cplusplus >= 201103L || (defined(_MSC_VER) && _MSC_VER >= 1900)
#else
      verbose(0, "[GEM:videoDECKLINK] lacking C++11 support requires connections to be lower-case");
#endif
    }
    done=true;
    std::string name = std::string(Name);
#if __cplusplus >= 201103L || (defined(_MSC_VER) && _MSC_VER >= 1900)
    std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c){ return std::tolower(c); });
#else
#endif
    std::map<std::string, BMDVideoConnection>::iterator it = s_connectionstrings.find(name);
    if(s_connectionstrings.end() != it)
      return it->second;
    return (BMDVideoConnection)0;
  }
  std::string connection2string(BMDVideoConnection conn) {
    static bool done = false;
    if(!done) string2connection("");
    done = true;
    for (std::map<std::string, BMDVideoConnection>::iterator it = s_connectionstrings.begin(); it != s_connectionstrings.end(); ++it)
      if (it->second == conn)
        return it->first;
    return "";
  }
};
static std::map<std::string, BMDPixelFormat> s_pixformatstrings;
namespace {
  BMDPixelFormat string2pixformat(std::string Name) {
    static bool done = false;
    if(!done) {
      s_pixformatstrings["yuv8"] = bmdFormat8BitYUV;
      s_pixformatstrings["yuv10"] = bmdFormat10BitYUV;
      s_pixformatstrings["argb8"] = bmdFormat8BitARGB;
      s_pixformatstrings["bgra8"] = bmdFormat8BitBGRA;
      s_pixformatstrings["yuv"] = bmdFormat8BitYUV;
      s_pixformatstrings["argb"] = bmdFormat8BitARGB;
      s_pixformatstrings["bgra"] = bmdFormat8BitBGRA;
#if __cplusplus >= 201103L || (defined(_MSC_VER) && _MSC_VER >= 1900)
#else
      verbose(0, "[GEM:videoDECKLINK] lacking C++11 support requires pixformats to be lower-case");
#endif
    }
    done=true;
    std::string name = std::string(Name);
#if __cplusplus >= 201103L || (defined(_MSC_VER) && _MSC_VER >= 1900)
    std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c){ return std::tolower(c); });
#else
#endif
    std::map<std::string, BMDPixelFormat>::iterator it = s_pixformatstrings.find(name);
    if(s_pixformatstrings.end() != it)
      return it->second;
    return (BMDPixelFormat)0;
  }
  std::string pixformat2string(BMDPixelFormat conn) {
    static bool done = false;
    if(!done) string2pixformat("");
    done = true;
    for (std::map<std::string, BMDPixelFormat>::iterator it = s_pixformatstrings.begin(); it != s_pixformatstrings.end(); ++it)
      if (it->second == conn)
        return it->first;
    return "";
  }
};



using namespace gem::plugins;


REGISTER_RECORDFACTORY("decklink", recordDECKLINK);
/////////////////////////////////////////////////////////
//
// recordDECKLINK
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////

recordDECKLINK :: recordDECKLINK(void)
  :  record()
  , m_dlIterator(NULL)
  , m_dl(NULL)
  , m_dlOutput(NULL)
  , m_displayMode(NULL)
  , m_dlConfig(NULL)
  , m_connectionType((BMDVideoConnection)0)
  , m_dlCallback(NULL)
  , m_pixelFormat(bmdFormat8BitYUV)
{
  IDeckLinkIterator*dli = CreateDeckLinkIteratorInstance();
  if(!dli) {
    throw(GemException("DeckLink: unable to initialize Framework"));
  }
  dli->Release();

  m_pixBlock.image.xsize = 1920;
  m_pixBlock.image.ysize = 1080;
  m_pixBlock.image.setCsizeByFormat(GEM_RGBA);
  m_pixBlock.image.reallocate();
  m_pixBlock.image.xsize = -1;
  m_pixBlock.image.ysize = -1;
}

////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
recordDECKLINK :: ~recordDECKLINK(void)
{
  stop();
  //DECKLINK->destroy();
}

void recordDECKLINK :: stop(void)
{
#if 0
  if (m_ndi_send)
    DECKLINK->send_destroy(m_ndi_send);
  m_ndi_send = NULL;
#endif
}

bool recordDECKLINK :: start(const std::string&filename, gem::Properties&props)
{
  stop();
#if 0
  DECKLINKlib_send_create_t send_create;
  send_create.p_ndi_name = filename.c_str();
  send_create.p_groups = NULL;
  send_create.clock_video = false;
  send_create.clock_audio = false;
  m_ndi_send = DECKLINK->send_create(&send_create);
  post("[GEM::recordDECKLINK] opened '%s' as '%s'", filename.c_str(), send_create.p_ndi_name);
  if (!m_ndi_send)
    return false;
  return true;
#endif
  return false;
}

bool recordDECKLINK::init(const imageStruct* dummyImage, const int framedur)
{
  return true;
}



/////////////////////////////////////////////////////////
// do the actual encoding and writing to file
//
/////////////////////////////////////////////////////////
bool recordDECKLINK :: write(imageStruct*img)
{
#if 0
  DECKLINKlib_video_frame_v2_t video_frame_data;
  video_frame_data.xres = img->xsize;
  video_frame_data.yres = img->ysize;
  video_frame_data.p_data = img->data;
  video_frame_data.frame_rate_N = 0;
  video_frame_data.frame_rate_D = 0;
  video_frame_data.picture_aspect_ratio = 0;
  video_frame_data.frame_format_type = DECKLINKlib_frame_format_type_progressive;
  video_frame_data.timecode = 0;

  switch (img->format) {
  case GEM_RGBA:
    video_frame_data.FourCC = DECKLINKlib_FourCC_video_type_RGBA;
    break;
  case GEM_YUV:
    video_frame_data.FourCC = DECKLINKlib_FourCC_video_type_UYVY;
    break;
  default:
    return false;
  }
  //m_image.fixUpDown();
  DECKLINK->send_send_video_v2(m_ndi_send, &video_frame_data);
  return true;
#endif
  return false;
}


static const std::string s_codec_name=std::string("decklink");
static const std::string s_codec_desc=std::string("Blackmagic Decklink");



/////////////////////////////////////////////////////////
// set codec by name
//
/////////////////////////////////////////////////////////
bool recordDECKLINK :: setCodec(const std::string&name)
{
  if(name==s_codec_name) {
    return true;
  }
  return false;
}

/////////////////////////////////////////////////////////
// get codecs
//
/////////////////////////////////////////////////////////
std::vector<std::string>recordDECKLINK::getCodecs(void)
{
  std::vector<std::string>result;
  result.push_back(s_codec_name);
  return result;
}
const std::string recordDECKLINK::getCodecDescription(const std::string&codec)
{
  std::string result;
  if(codec==s_codec_name) {
    result=s_codec_desc;
  }
  return result;
}

bool recordDECKLINK::enumProperties(gem::Properties&props)
{
  props.clear();
  return false;
}
