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


namespace
{
int GetRowBytes(BMDPixelFormat pixelFormat, int frameWidth)
{
        int bytesPerRow;

        // Refer to DeckLink SDK Manual - 2.7.4 Pixel Formats
        switch (pixelFormat)
        {
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
        default:
                bytesPerRow = frameWidth * 4;
                break;
        }

        return bytesPerRow;
}

IDeckLinkDisplayMode*getDisplayMode(IDeckLinkOutput*dlo,
                                    const std::string&formatname, int formatnum)
{
  IDeckLinkDisplayModeIterator*dmi = NULL;
  IDeckLinkDisplayMode*displayMode = NULL;
  int count=formatnum;
  if(S_OK == dlo->GetDisplayModeIterator(&dmi)) {
    while(S_OK == dmi->Next(&displayMode)) {
      if (formatnum<0 && formatname.empty()) {
        // we don't care for the format; accept the first one
        break;
      }

      // if we have set the format name, check that
      if(!formatname.empty()) {
        deckstring_t dmn = NULL;
        if (S_OK == displayMode->GetName(&dmn)) {
          std::string dmns = deckstring2string(dmn);
          bool found=(formatname == dmns);
          verbose(1, "[GEM:videoDECKLINK] checking format '%s'", dmns.c_str());
          free_deckstring(dmn);
          if(found) {
            break;
          }
        }
      }
      // else check the format index
      if(formatnum>=0 && 0 == count) {
        break;
      }
      --count;

      displayMode->Release();
      displayMode=NULL;
    }
    dmi->Release();
  }
  return displayMode;
}
};


class VideoOutputter : public IDeckLinkVideoOutputCallback
{
public:
  IDeckLinkOutput*m_deckLinkOutput;
  IDeckLinkDisplayMode*m_displayMode;
  IDeckLinkConfiguration *m_config;
  IDeckLinkVideoFrame*m_videoFrame;
  int32_t m_refCount;
  unsigned long m_totalFramesScheduled;
  BMDTimeValue m_frameDuration;
  BMDTimeScale m_frameTimescale;

  VideoOutputter(IDeckLinkOutput*output, IDeckLinkDisplayMode*displaymode, BMDPixelFormat pixelFormat)
    : m_deckLinkOutput(output)
    , m_displayMode(displaymode)
    , m_videoFrame(NULL)
    , m_refCount(1)
    , m_totalFramesScheduled(0)
  {
    /*
IDeckLinkStatus::GetInt(bmdDeckLinkStatusCurrentVideoInputPixelFormat)
     */
    IDeckLinkMutableVideoFrame*newFrame = NULL;
    int width = m_displayMode->GetWidth(), height = m_displayMode->GetHeight();
    HRESULT result = m_deckLinkOutput->CreateVideoFrame(
        width, height,
        GetRowBytes(pixelFormat, width),
        pixelFormat,
        bmdFrameFlagDefault,
        &newFrame
        );
    if (result != S_OK) {
      fprintf(stderr, "Failed to create video frame\n");
    }
    m_videoFrame = newFrame;

    m_displayMode->GetFrameRate(&m_frameDuration, &m_frameTimescale);

    m_deckLinkOutput->SetScheduledFrameCompletionCallback(this);
  };

  virtual HRESULT STDMETHODCALLTYPE ScheduledFrameCompleted(IDeckLinkVideoFrame* completedFrame, BMDOutputFrameCompletionResult result) {
    HRESULT res = S_OK;
    fprintf(stderr, "scheduledframecompleted");
    if(m_videoFrame)
      res = m_deckLinkOutput->ScheduleVideoFrame(m_videoFrame, (m_totalFramesScheduled * m_frameDuration), m_frameDuration, m_frameTimescale);
    if(S_OK == res)
      m_totalFramesScheduled += 1;
    return res;
  };
  virtual HRESULT STDMETHODCALLTYPE ScheduledPlaybackHasStopped() {
    return S_OK;
  };

  virtual HRESULT QueryInterface(REFIID iid, LPVOID *ppv) {
    *ppv = NULL;
    return E_NOINTERFACE;
  };

  virtual ULONG AddRef(void) {
    // gcc atomic operation builtin
    return __sync_add_and_fetch(&m_refCount, 1);
  };

  ULONG Release(void) {
    // gcc atomic operation builtin
    ULONG newRefValue = __sync_sub_and_fetch(&m_refCount, 1);
    if (!newRefValue)
      delete this;
    return newRefValue;
  };

  void setFrame(imageStruct*img) {
    /* convert the imageStruct into the IDeckLinkVideoFrame */
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
  int formatnumber=-1;
  std::string formatname="";
  BMDVideoOutputFlags flags = bmdVideoOutputFlagDefault;

  IDeckLinkIterator*dlIterator = CreateDeckLinkIteratorInstance();
  if(dlIterator) {
    //setProperties(props);
    int deviceCount=0;
    while (dlIterator->Next(&m_dl) == S_OK) {
      if(!filename.empty()) {
        deckstring_t deckLinkName = NULL;
        if(S_OK == m_dl->GetDisplayName(&deckLinkName)) {
          if (filename == deckstring2string(deckLinkName)) {
            free_deckstring(deckLinkName);
            break;
          }
          free_deckstring(deckLinkName);
        }
        if(S_OK == m_dl->GetModelName(&deckLinkName)) {
          if (filename == deckstring2string(deckLinkName)) {
            free_deckstring(deckLinkName);
            break;
          }
          free_deckstring(deckLinkName);
        }
      }
      m_dl->Release();
      m_dl=NULL;
      ++deviceCount;
    }
    m_dlOutput=NULL;
    if(m_dl) {
      if (S_OK == m_dl->QueryInterface(IID_IDeckLinkOutput, (void**)&m_dlOutput)) {
        // check whether this device supports the selected format
        m_displayMode=getDisplayMode(m_dlOutput, formatname, formatnumber);
      } else {
        m_dlOutput=NULL;
      }
    }
  }
  if(!m_displayMode) {
    goto bail;
  }

  /* negotiate the video format */
  if(m_dl) {
    IDeckLinkStatus*status;
    if (S_OK == m_dl->QueryInterface(IID_IDeckLinkStatus, (void**)&status)) {
      int64_t gotint=0;
      if (S_OK == status->GetInt(bmdDeckLinkStatusCurrentVideoInputPixelFormat, &gotint)) {
        m_pixelFormat = gotint;
        post("got pixelformat: %d -> %s", (int)gotint, pixformat2string(m_pixelFormat).c_str());
      }
    }
  }


  BMDDisplayModeSupport displayModeSupported;
  if (S_OK != m_dlOutput->DoesSupportVideoMode(
        m_displayMode->GetDisplayMode(),
        m_pixelFormat,
        flags,
        &displayModeSupported,
        NULL)) {
    post("[GEM::recordDECKLINK] '%s' does not support videomode...", filename.c_str());
    goto bail;
  }
  if (displayModeSupported == bmdDisplayModeNotSupported) {
    post("[GEM::recordDECKLINK] '%s' unsupported videomode...", filename.c_str());
    goto bail;
  }
  if(S_OK != m_dl->QueryInterface (IID_IDeckLinkConfiguration,
                                   (void**)&m_dlConfig)) {
    m_dlConfig=NULL;
  }

  if(m_dlConfig) {
    m_dlConfig->SetInt(bmdDeckLinkConfigVideoOutputConnection,
                       m_connectionType);
  }


  m_dlCallback = new VideoOutputter(m_dlOutput, m_displayMode, m_pixelFormat);

  if(S_OK != m_dlOutput->EnableVideoOutput(m_displayMode->GetDisplayMode(), flags)) {
    post("[GEM::recordDECKLINK] '%s' couldn't enable VideoOutput...", filename.c_str());
    goto bail;
  }

  post("[GEM::recordDECKLINK] opened '%s'", filename.c_str());
  return true;
#warning TODO: actually open the device
 bail:
  stop();
  return false;
}

bool recordDECKLINK::init(const imageStruct* dummyImage, const int framedur)
{
  post("===========================================");
  post("[GEM::recordDECKLINK] init(%p, %d)", dummyImage, framedur);
  return true;
}



/////////////////////////////////////////////////////////
// do the actual encoding and writing to file
//
/////////////////////////////////////////////////////////
bool recordDECKLINK :: write(imageStruct*img)
{
#warning TODO write the image
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
