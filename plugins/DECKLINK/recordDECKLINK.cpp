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
//    Copyright (c) 2001-2022 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
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
#include "decklink_common.h"

// for MARK()
#include <stdio.h>

namespace {
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
  IDeckLinkVideoConversion*m_frameConverter;
  int32_t m_refCount;
  unsigned long m_totalFramesScheduled;
  BMDTimeValue m_frameDuration;
  BMDTimeScale m_frameTimescale;

  VideoOutputter(IDeckLinkOutput*output, IDeckLinkDisplayMode*displaymode, BMDPixelFormat pixelFormat)
    : m_deckLinkOutput(output)
    , m_displayMode(displaymode)
    , m_videoFrame(NULL)
    , m_frameConverter(NULL)
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
    //fprintf(stderr, "created video frame %dx%d@%s", width, height, pixformat2string(pixelFormat).c_str());
    m_videoFrame = newFrame;
    m_frameConverter = CreateVideoConversionInstance();

    m_displayMode->GetFrameRate(&m_frameDuration, &m_frameTimescale);

    m_deckLinkOutput->SetScheduledFrameCompletionCallback(this);
  };
  ~VideoOutputter(void) {
    if (m_frameConverter != NULL)
      m_frameConverter->Release();
    m_frameConverter = NULL;
  }

  virtual HRESULT STDMETHODCALLTYPE ScheduledFrameCompleted(IDeckLinkVideoFrame* completedFrame, BMDOutputFrameCompletionResult result) {
    return S_OK;
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

  bool setFrame(imageStruct*img) {
    /* convert the imageStruct into the IDeckLinkVideoFrame */
    ImageStructWrapper*isw = new ImageStructWrapper(img);
    HRESULT result = S_OK;

#if 0
    post("writing image %p[%dx%d@%s] -> %p[%dx%d@%s] || %lu*%lu/%lu"
        , isw, (int)isw->GetWidth(), (int)isw->GetHeight(), pixformat2string(isw->GetPixelFormat()).c_str()
        , m_videoFrame, (int)m_videoFrame->GetWidth(), (int)m_videoFrame->GetHeight(), pixformat2string(m_videoFrame->GetPixelFormat()).c_str()
        , m_totalFramesScheduled, m_frameDuration, m_frameTimescale
        );
#endif

    if (isw->GetPixelFormat() != m_videoFrame->GetPixelFormat()) {
      result = m_frameConverter->ConvertFrame(isw, m_videoFrame);
      if (result != S_OK)  {
        fprintf(stderr, "Failed to convert frame: 0x%X\n", (unsigned int)result);
        return false;
      }
    }

    result = m_deckLinkOutput->ScheduleVideoFrame(isw, (m_totalFramesScheduled * m_frameDuration), m_frameDuration, m_frameTimescale);
    if (result != S_OK)  {
      fprintf(stderr, "Failed to schedule frame: 0x%X\n", (unsigned int)result);
      return false;
    }
    m_totalFramesScheduled++;

    if(isw)
      isw->Release();
    return true;
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
  , m_connectionType(bmdVideoConnectionUnspecified)
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
  bool is_supported = false;
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
    //post("no displayMode!");
    goto bail;
  }

  /* negotiate the video format */
  if(m_dl) {
    IDeckLinkStatus*status;
    if (S_OK == m_dl->QueryInterface(IID_IDeckLinkStatus, (void**)&status)) {
      int64_t gotint=0;
      if (S_OK == status->GetInt(bmdDeckLinkStatusCurrentVideoInputPixelFormat, &gotint)) {
        m_pixelFormat = gotint;
        //post("got pixelformat: %d -> %s", (int)gotint, pixformat2string(m_pixelFormat).c_str());
      }
    }
  }

  if(m_dlOutput) {
    BMDDisplayMode actualMode;
    if (S_OK != m_dlOutput->DoesSupportVideoMode(
            m_connectionType /* in: BMDVideoConnection connection */
            , m_displayMode->GetDisplayMode() /* in: BMDDisplayMode requestedMode */
            , m_pixelFormat /* in: BMDPixelFormat requestedPixelFormat */
            , bmdSupportedVideoModeDefault /* flags */  /* in: BMDSupportedVideoModeFlags flags */
            , &actualMode /* out: BMDDisplayMode *actualMode */
            , &is_supported /* out: bool *supported */
            )) {
      is_supported=false;
    }
    if (!is_supported) {
      post("[GEM::recordDECKLINK] '%s' unsupported videomode...", filename.c_str());
      goto bail;
    }
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

  if (S_OK != m_dlOutput->StartScheduledPlayback(0, 100, 1.0)) {
    post("[GEM::recordDECKLINK] '%s' couldn't start VideoOutput...", filename.c_str());
    goto bail;
  }

  return true;
 bail:
  stop();
  return false;
}

/////////////////////////////////////////////////////////
// do the actual encoding and writing to file
//
/////////////////////////////////////////////////////////
bool recordDECKLINK :: write(imageStruct*img)
{
  if(m_dlCallback) {
    bool result = ((VideoOutputter*)m_dlCallback)->setFrame(img);
    return result;
  }
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
