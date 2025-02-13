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
#ifndef HAVE_DECKLINK_COMMON_H
#define HAVE_DECKLINK_COMMON_H
#include <algorithm>
#include <cctype>
#include <string>
#include <map>

#define MARK() printf("%s:%d\t[%s]\n", __FILE__, __LINE__, __FUNCTION__)

#ifdef _WIN32
# include <windows.h>
typedef BOOL deckbool_t;
typedef wchar_t* deckstring_t;
namespace
{
deckstring_t string2deckstring(const std::string&s)
{
  int n = MultiByteToWideChar(CP_UTF8, 0, s.data(), s.size(), NULL, 0);
  deckstring_t buf = new wchar_t[n];
  MultiByteToWideChar(CP_UTF8, 0, s.data(), s.size(), &buf[0], n);
  return buf;
}
std::string deckstring2string(deckstring_t s)
{
  if (!s) {
    return std::string();
  }
  size_t l = wcslen(s);
  int n = WideCharToMultiByte(CP_UTF8, 0, s, l, NULL, 0, NULL, NULL);
  std::string buf;
  buf.resize(n);
  WideCharToMultiByte(CP_UTF8, 0, s, l, &buf[0], n, NULL, NULL);
  return buf;
}
void free_deckstring(deckstring_t s)
{
  wchar_t*b=s;
  delete[]b;
}
};
#elif defined __APPLE__
typedef bool deckbool_t;
typedef CFStringRef deckstring_t;
namespace
{
deckstring_t string2deckstring(const std::string&s)
{
  return CFStringCreateWithCString(NULL, s.c_str(), kCFStringEncodingUTF8);
}
std::string deckstring2string(deckstring_t s)
{
  if (const char* fastCString = CFStringGetCStringPtr(s, kCFStringEncodingUTF8)) {
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
void free_deckstring(deckstring_t s)
{
  CFRelease(s);
}
};
#else /* linux */
# include <string.h>
typedef bool deckbool_t;
typedef const char* deckstring_t;
namespace
{
deckstring_t string2deckstring(const std::string&s)
{
  return strdup(s.c_str());
}
std::string deckstring2string(deckstring_t s)
{
  return std::string(s);
}
void free_deckstring(deckstring_t s)
{
  free((void*)s);
}
};
#endif /* OS specific code */

static std::map<std::string, BMDVideoConnection> s_connectionstrings;
namespace
{
  enum _gem_BMDPixelFormat {
    /* GEM specific */
    gemBmdFormat8BitRGBA                                            = /* 'RGBA' */ 0x52474241,
  };

BMDVideoConnection string2connection(std::string Name)
{
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
    logpost(0, 3+0, "[GEM:videoDECKLINK] lacking C++11 support requires connections to be lower-case");
#endif
  }
  done=true;
  std::string name = std::string(Name);
#if __cplusplus >= 201103L || (defined(_MSC_VER) && _MSC_VER >= 1900)
  std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) {
    return std::tolower(c);
  });
#else
#endif
  std::map<std::string, BMDVideoConnection>::iterator it = s_connectionstrings.find(name);
  if(s_connectionstrings.end() != it) {
    return it->second;
  }
  return (BMDVideoConnection)0;
}
std::string connection2string(BMDVideoConnection conn)
{
  static bool done = false;
  if(!done) {
    string2connection("");
  }
  done = true;
  for (std::map<std::string, BMDVideoConnection>::iterator it = s_connectionstrings.begin(); it != s_connectionstrings.end(); ++it)
    if (it->second == conn) {
      return it->first;
    }
  return "";
}

int connection2id(BMDVideoConnection conn)
{
  switch(conn) {
  default:
    break;
  case bmdVideoConnectionSDI:
    return 0;
  case bmdVideoConnectionHDMI:
    return 1;
  case bmdVideoConnectionOpticalSDI:
    return 2;
  case bmdVideoConnectionComponent:
    return 3;
  case bmdVideoConnectionComposite:
    return 4;
  case bmdVideoConnectionSVideo:
    return 5;
  }
  return bmdVideoConnectionUnspecified;
}
BMDVideoConnection id2connection(int id)
{
  switch(id) {
  default:
    break;
  case 0:
    return bmdVideoConnectionSDI;
  case 1:
    return bmdVideoConnectionHDMI;
  case 2:
    return bmdVideoConnectionOpticalSDI;
  case 3:
    return bmdVideoConnectionComponent;
  case 4:
    return bmdVideoConnectionComposite;
  case 5:
    return bmdVideoConnectionSVideo;
  }
  /* that should return an invalid connection! */
  return bmdVideoConnectionUnspecified;
}
};
static std::map<std::string, BMDPixelFormat> s_pixformatstrings;
namespace
{
BMDPixelFormat string2pixformat(std::string Name)
{
  static bool done = false;
  if(!done) {
    s_pixformatstrings["yuv8"] = bmdFormat8BitYUV;
    s_pixformatstrings["yuv10"] = bmdFormat10BitYUV;
    s_pixformatstrings["argb8"] = bmdFormat8BitARGB;
    s_pixformatstrings["bgra8"] = bmdFormat8BitBGRA;
    s_pixformatstrings["yuv"] = bmdFormat8BitYUV;
    s_pixformatstrings["argb"] = bmdFormat8BitARGB;
    s_pixformatstrings["bgra"] = bmdFormat8BitBGRA;
    s_pixformatstrings["rgba8"] = (BMDPixelFormat)((int)bmdFormat8BitYUV);
#if __cplusplus >= 201103L || (defined(_MSC_VER) && _MSC_VER >= 1900)
#else
    logpost(0, 3+0, "[GEM:videoDECKLINK] lacking C++11 support requires pixformats to be lower-case");
#endif
  }
  done=true;
  std::string name = std::string(Name);
#if __cplusplus >= 201103L || (defined(_MSC_VER) && _MSC_VER >= 1900)
  std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) {
    return std::tolower(c);
  });
#else
#endif
  std::map<std::string, BMDPixelFormat>::iterator it = s_pixformatstrings.find(name);
  if(s_pixformatstrings.end() != it) {
    return it->second;
  }
  return (BMDPixelFormat)0;
}
std::string pixformat2string(BMDPixelFormat conn)
{
  static bool done = false;
  if(!done) {
    string2pixformat("");
  }
  done = true;
  for (std::map<std::string, BMDPixelFormat>::iterator it = s_pixformatstrings.begin(); it != s_pixformatstrings.end(); ++it)
    if (it->second == conn) {
      return it->first;
    }
  return "";
}

class ImageStructWrapper : public IDeckLinkVideoFrame
/* wraps a Gem image into a DeckLink frame */
{
public:
  imageStruct *m_img;
  int32_t m_refCount;

  ImageStructWrapper(imageStruct*img)
    : m_img(img)
    , m_refCount(1)
  {    }
  virtual long STDMETHODCALLTYPE GetWidth()
  {
    return m_img->xsize;
  }
  virtual long STDMETHODCALLTYPE GetHeight()
  {
    return m_img->ysize;
  }
  virtual BMDPixelFormat STDMETHODCALLTYPE GetPixelFormat()
  {
    bool reverse = false;
    switch(m_img->format) {
    case GEM_RAW_RGBA:
      return (BMDPixelFormat)((int)bmdFormat8BitYUV);
    case GEM_RAW_BGRA:
      return bmdFormat8BitBGRA;
    case GEM_RAW_UYVY:
      return (BMDPixelFormat)((int)bmdFormat8BitYUV);
    default:
      break;
    }
    return bmdFormatUnspecified;
  }
  virtual long STDMETHODCALLTYPE GetRowBytes()
  {
    return m_img->xsize * m_img->csize;
  }
  virtual HRESULT STDMETHODCALLTYPE GetBytes(void **buffer)
  {
    if(m_img->data) {
      *buffer = m_img->data;
      return S_OK;
    }
    return S_FALSE;
  }
  virtual ULONG STDMETHODCALLTYPE AddRef(void)
  {
    return __sync_add_and_fetch(&m_refCount, 1);
  }
  virtual ULONG STDMETHODCALLTYPE Release(void)
  {
    ULONG newRefValue = __sync_sub_and_fetch(&m_refCount, 1);
    if (!newRefValue) {
      delete this;
    }
    return newRefValue;
  }
  virtual BMDFrameFlags STDMETHODCALLTYPE GetFlags (void)
  {
    return bmdFrameFlagDefault;
    /* return bmdFrameFlagFlipVertical */
  }
  virtual HRESULT STDMETHODCALLTYPE GetTimecode (BMDTimecodeFormat format, IDeckLinkTimecode **timecode)
  {
    return S_FALSE;
  }
  virtual HRESULT STDMETHODCALLTYPE GetAncillaryData (/* out */ IDeckLinkVideoFrameAncillary **ancillary)
  {
    return S_FALSE;
  }
  virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, LPVOID *ppv)
  {
    *ppv = NULL;
    return E_NOINTERFACE;
  }
};

int GetRowBytes(BMDPixelFormat pixelFormat, int frameWidth)
{
  int bytesPerRow;

  // Refer to DeckLink SDK Manual - 2.7.4 Pixel Formats
  switch (pixelFormat) {
  case bmdFormat8BitYUV:
    bytesPerRow = frameWidth * 2;
    break;

  case bmdFormat10BitYUV:
    bytesPerRow = ((frameWidth + 47) / 48) * 128;
    break;

  case bmdFormat10BitRGB:
    bytesPerRow = ((frameWidth + 63) / 64) * 256;
    break;

  case bmdFormat8BitARGB:
  case bmdFormat8BitBGRA:
  case gemBmdFormat8BitRGBA:
  default:
    bytesPerRow = frameWidth * 4;
    break;
  }

  return bytesPerRow;
}

void printVideoFrame(IDeckLinkVideoFrame*img, const char*prefix="")
{
  void*buffer=0;
  HRESULT res=S_OK;
  post("VideoFrame%s@%p", prefix, img);
  post("\tdimension=%dx%d", (int)img->GetWidth(), (int)img->GetHeight());
  post("\tformat=%s", pixformat2string(img->GetPixelFormat()).c_str());
  post("\trowbytes=%d", (int)img->GetRowBytes());
  res = img->GetBytes(&buffer);
  post("\tbytes=%p\t%d", buffer, (int)res);
  post("\tflags=%d", img->GetFlags());
  //res = img->GetTimecode()
}
};
#endif /* HAVE_DECKLINK_COMMON_H */
