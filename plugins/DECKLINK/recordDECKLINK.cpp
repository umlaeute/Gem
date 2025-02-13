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

// for logpost(0, 3+):
#include "Gem/RTE.h"
#include "Gem/Exception.h"


#include "plugins/PluginFactory.h"
#include "decklink_common.h"

// for MARK()
#include <stdio.h>

namespace
{
  static std::map<std::string, BMDDisplayMode> s_name2mode;
  static bool name2mode(const std::string&name, BMDDisplayMode&mode)
  {
    static bool done=false;
    if(!done) {
      done = true;
      /* fourcc-like codes from DeckLink API */
      s_name2mode["ntsc"] = bmdModeNTSC;
      s_name2mode["nt23"] = bmdModeNTSC2398;
      s_name2mode["pal "] = bmdModePAL;
      s_name2mode["ntsp"] = bmdModeNTSCp;
      s_name2mode["palp"] = bmdModePALp;
      s_name2mode["23ps"] = bmdModeHD1080p2398;
      s_name2mode["24ps"] = bmdModeHD1080p24;
      s_name2mode["Hp25"] = bmdModeHD1080p25;
      s_name2mode["Hp29"] = bmdModeHD1080p2997;
      s_name2mode["Hp30"] = bmdModeHD1080p30;
      s_name2mode["Hp47"] = bmdModeHD1080p4795;
      s_name2mode["Hp48"] = bmdModeHD1080p48;
      s_name2mode["Hp50"] = bmdModeHD1080p50;
      s_name2mode["Hp59"] = bmdModeHD1080p5994;
      s_name2mode["Hp60"] = bmdModeHD1080p6000;
      s_name2mode["Hp95"] = bmdModeHD1080p9590;
      s_name2mode["Hp96"] = bmdModeHD1080p96;
      s_name2mode["Hp10"] = bmdModeHD1080p100;
      s_name2mode["Hp11"] = bmdModeHD1080p11988;
      s_name2mode["Hp12"] = bmdModeHD1080p120;
      s_name2mode["Hi50"] = bmdModeHD1080i50;
      s_name2mode["Hi59"] = bmdModeHD1080i5994;
      s_name2mode["Hi60"] = bmdModeHD1080i6000;
      s_name2mode["hp50"] = bmdModeHD720p50;
      s_name2mode["hp59"] = bmdModeHD720p5994;
      s_name2mode["hp60"] = bmdModeHD720p60;
      s_name2mode["2k23"] = bmdMode2k2398;
      s_name2mode["2k24"] = bmdMode2k24;
      s_name2mode["2k25"] = bmdMode2k25;
      s_name2mode["2d23"] = bmdMode2kDCI2398;
      s_name2mode["2d24"] = bmdMode2kDCI24;
      s_name2mode["2d25"] = bmdMode2kDCI25;
      s_name2mode["2d29"] = bmdMode2kDCI2997;
      s_name2mode["2d30"] = bmdMode2kDCI30;
      s_name2mode["2d47"] = bmdMode2kDCI4795;
      s_name2mode["2d48"] = bmdMode2kDCI48;
      s_name2mode["2d50"] = bmdMode2kDCI50;
      s_name2mode["2d59"] = bmdMode2kDCI5994;
      s_name2mode["2d60"] = bmdMode2kDCI60;
      s_name2mode["2d95"] = bmdMode2kDCI9590;
      s_name2mode["2d96"] = bmdMode2kDCI96;
      s_name2mode["2d10"] = bmdMode2kDCI100;
      s_name2mode["2d11"] = bmdMode2kDCI11988;
      s_name2mode["2d12"] = bmdMode2kDCI120;
      s_name2mode["4k23"] = bmdMode4K2160p2398;
      s_name2mode["4k24"] = bmdMode4K2160p24;
      s_name2mode["4k25"] = bmdMode4K2160p25;
      s_name2mode["4k29"] = bmdMode4K2160p2997;
      s_name2mode["4k30"] = bmdMode4K2160p30;
      s_name2mode["4k47"] = bmdMode4K2160p4795;
      s_name2mode["4k48"] = bmdMode4K2160p48;
      s_name2mode["4k50"] = bmdMode4K2160p50;
      s_name2mode["4k59"] = bmdMode4K2160p5994;
      s_name2mode["4k60"] = bmdMode4K2160p60;
      s_name2mode["4k95"] = bmdMode4K2160p9590;
      s_name2mode["4k96"] = bmdMode4K2160p96;
      s_name2mode["4k10"] = bmdMode4K2160p100;
      s_name2mode["4k11"] = bmdMode4K2160p11988;
      s_name2mode["4k12"] = bmdMode4K2160p120;
      s_name2mode["4d23"] = bmdMode4kDCI2398;
      s_name2mode["4d24"] = bmdMode4kDCI24;
      s_name2mode["4d25"] = bmdMode4kDCI25;
      s_name2mode["4d29"] = bmdMode4kDCI2997;
      s_name2mode["4d30"] = bmdMode4kDCI30;
      s_name2mode["4d47"] = bmdMode4kDCI4795;
      s_name2mode["4d48"] = bmdMode4kDCI48;
      s_name2mode["4d50"] = bmdMode4kDCI50;
      s_name2mode["4d59"] = bmdMode4kDCI5994;
      s_name2mode["4d60"] = bmdMode4kDCI60;
      s_name2mode["4d95"] = bmdMode4kDCI9590;
      s_name2mode["4d96"] = bmdMode4kDCI96;
      s_name2mode["4d10"] = bmdMode4kDCI100;
      s_name2mode["4d11"] = bmdMode4kDCI11988;
      s_name2mode["4d12"] = bmdMode4kDCI120;
      s_name2mode["8k23"] = bmdMode8K4320p2398;
      s_name2mode["8k24"] = bmdMode8K4320p24;
      s_name2mode["8k25"] = bmdMode8K4320p25;
      s_name2mode["8k29"] = bmdMode8K4320p2997;
      s_name2mode["8k30"] = bmdMode8K4320p30;
      s_name2mode["8k47"] = bmdMode8K4320p4795;
      s_name2mode["8k48"] = bmdMode8K4320p48;
      s_name2mode["8k50"] = bmdMode8K4320p50;
      s_name2mode["8k59"] = bmdMode8K4320p5994;
      s_name2mode["8k60"] = bmdMode8K4320p60;
      s_name2mode["8d23"] = bmdMode8kDCI2398;
      s_name2mode["8d24"] = bmdMode8kDCI24;
      s_name2mode["8d25"] = bmdMode8kDCI25;
      s_name2mode["8d29"] = bmdMode8kDCI2997;
      s_name2mode["8d30"] = bmdMode8kDCI30;
      s_name2mode["8d47"] = bmdMode8kDCI4795;
      s_name2mode["8d48"] = bmdMode8kDCI48;
      s_name2mode["8d50"] = bmdMode8kDCI50;
      s_name2mode["8d59"] = bmdMode8kDCI5994;
      s_name2mode["8d60"] = bmdMode8kDCI60;
      s_name2mode["vga6"] = bmdMode640x480p60;
      s_name2mode["svg6"] = bmdMode800x600p60;
      s_name2mode["wxg5"] = bmdMode1440x900p50;
      s_name2mode["wxg6"] = bmdMode1440x900p60;
      s_name2mode["sxg5"] = bmdMode1440x1080p50;
      s_name2mode["sxg6"] = bmdMode1440x1080p60;
      s_name2mode["uxg5"] = bmdMode1600x1200p50;
      s_name2mode["uxg6"] = bmdMode1600x1200p60;
      s_name2mode["wux5"] = bmdMode1920x1200p50;
      s_name2mode["wux6"] = bmdMode1920x1200p60;
      s_name2mode["1945"] = bmdMode1920x1440p50;
      s_name2mode["1946"] = bmdMode1920x1440p60;
      s_name2mode["wqh5"] = bmdMode2560x1440p50;
      s_name2mode["wqh6"] = bmdMode2560x1440p60;
      s_name2mode["wqx5"] = bmdMode2560x1600p50;
      s_name2mode["wqx6"] = bmdMode2560x1600p60;
      s_name2mode["rwci"] = bmdModeCintelRAW;
      s_name2mode["rwcc"] = bmdModeCintelCompressedRAW;
      s_name2mode["iunk"] = bmdModeUnknown;

      /* GStreamer short names */
      s_name2mode["ntsc2398"] = bmdModeNTSC2398;
      s_name2mode["pal"] = bmdModePAL;
      s_name2mode["ntsc-p"] = bmdModeNTSCp;
      s_name2mode["pal-p"] = bmdModePALp;
      s_name2mode["1080p2398"] = bmdModeHD1080p2398;
      s_name2mode["1080p24"] = bmdModeHD1080p24;
      s_name2mode["1080p25"] = bmdModeHD1080p25;
      s_name2mode["1080p2997"] = bmdModeHD1080p2997;
      s_name2mode["1080p30"] = bmdModeHD1080p30;
      s_name2mode["1080i50"] = bmdModeHD1080i50;
      s_name2mode["1080i5994"] = bmdModeHD1080i5994;
      s_name2mode["1080i60"] = bmdModeHD1080i6000;
      s_name2mode["1080p50"] = bmdModeHD1080p50;
      s_name2mode["1080p5994"] = bmdModeHD1080p5994;
      s_name2mode["1080p60"] = bmdModeHD1080p6000;
      s_name2mode["720p50"] = bmdModeHD720p50;
      s_name2mode["720p5994"] = bmdModeHD720p5994;
      s_name2mode["720p60"] = bmdModeHD720p60;
      s_name2mode["1556p2398"] = bmdMode2k2398;
      s_name2mode["1556p24"] = bmdMode2k24;
      s_name2mode["1556p25"] = bmdMode2k25;
      s_name2mode["2kdcip2398"] = bmdMode2kDCI2398;
      s_name2mode["2kdcip24"] = bmdMode2kDCI24;
      s_name2mode["2kdcip25"] = bmdMode2kDCI25;
      s_name2mode["2kdcip2997"] = bmdMode2kDCI2997;
      s_name2mode["2kdcip30"] = bmdMode2kDCI30;
      s_name2mode["2kdcip50"] = bmdMode2kDCI50;
      s_name2mode["2kdcip5994"] = bmdMode2kDCI5994;
      s_name2mode["2kdcip60"] = bmdMode2kDCI60;
      s_name2mode["2160p2398"] = bmdMode4K2160p2398;
      s_name2mode["2160p24"] = bmdMode4K2160p24;
      s_name2mode["2160p25"] = bmdMode4K2160p25;
      s_name2mode["2160p2997"] = bmdMode4K2160p2997;
      s_name2mode["2160p30"] = bmdMode4K2160p30;
      s_name2mode["2160p50"] = bmdMode4K2160p50;
      s_name2mode["2160p5994"] = bmdMode4K2160p5994;
      s_name2mode["2160p60"] = bmdMode4K2160p60;
      s_name2mode["4kdcip2398"] = bmdMode4kDCI2398;
      s_name2mode["8kdcip2398"] = bmdMode8kDCI2398;
      s_name2mode["4kdcip24"] = bmdMode4kDCI24;
      s_name2mode["8kdcip24"] = bmdMode8kDCI24;
      s_name2mode["4kdcip25"] = bmdMode4kDCI25;
      s_name2mode["8kdcip25"] = bmdMode8kDCI25;
      s_name2mode["4kdcip2997"] = bmdMode4kDCI2997;
      s_name2mode["8kdcip2997"] = bmdMode8kDCI2997;
      s_name2mode["4kdcip30"] = bmdMode4kDCI30;
      s_name2mode["8kdcip30"] = bmdMode8kDCI30;
      s_name2mode["4kdcip50"] = bmdMode4kDCI50;
      s_name2mode["8kdcip50"] = bmdMode8kDCI50;
      s_name2mode["4kdcip5994"] = bmdMode4kDCI5994;
      s_name2mode["8kdcip5994"] = bmdMode8kDCI5994;
      s_name2mode["4kdcip60"] = bmdMode4kDCI60;
      s_name2mode["8kdcip60"] = bmdMode8kDCI60;
      s_name2mode["8kp2398"] = bmdMode8K4320p2398;
      s_name2mode["8kp24"] = bmdMode8K4320p24;
      s_name2mode["8kp25"] = bmdMode8K4320p25;
      s_name2mode["8kp2997"] = bmdMode8K4320p2997;
      s_name2mode["8kp30"] = bmdMode8K4320p30;
      s_name2mode["8kp50"] = bmdMode8K4320p50;
      s_name2mode["8kp5994"] = bmdMode8K4320p5994;
      s_name2mode["8kp60"] = bmdMode8K4320p60;

      /* GStreamer long names */
      s_name2mode["NTSC SD 60i"] = bmdModeNTSC;
      s_name2mode["NTSC SD 60i (24 fps)"] = bmdModeNTSC2398;
      s_name2mode["PAL SD 50i"] = bmdModePAL;
      s_name2mode["NTSC SD 60p"] = bmdModeNTSCp;
      s_name2mode["PAL SD 50p"] = bmdModePALp;
      s_name2mode["HD1080 23.98p"] = bmdModeHD1080p2398;
      s_name2mode["HD1080 24p"] = bmdModeHD1080p24;
      s_name2mode["HD1080 25p"] = bmdModeHD1080p25;
      s_name2mode["HD1080 29.97p"] = bmdModeHD1080p2997;
      s_name2mode["HD1080 30p"] = bmdModeHD1080p30;
      s_name2mode["HD1080 50i"] = bmdModeHD1080i50;
      s_name2mode["HD1080 59.94i"] = bmdModeHD1080i5994;
      s_name2mode["HD1080 60i"] = bmdModeHD1080i6000;
      s_name2mode["HD1080 50p"] = bmdModeHD1080p50;
      s_name2mode["HD1080 59.94p"] = bmdModeHD1080p5994;
      s_name2mode["HD1080 60p"] = bmdModeHD1080p6000;
      s_name2mode["HD720 50p"] = bmdModeHD720p50;
      s_name2mode["HD720 59.94p"] = bmdModeHD720p5994;
      s_name2mode["HD720 60p"] = bmdModeHD720p60;
      s_name2mode["2k 23.98p"] = bmdMode2k2398;
      s_name2mode["2k 24p"] = bmdMode2k24;
      s_name2mode["2k 25p"] = bmdMode2k25;
      s_name2mode["2k dci 23.98p"] = bmdMode2kDCI2398;
      s_name2mode["2k dci 24p"] = bmdMode2kDCI24;
      s_name2mode["2k dci 25p"] = bmdMode2kDCI25;
      s_name2mode["2k dci 29.97p"] = bmdMode2kDCI2997;
      s_name2mode["2k dci 30p"] = bmdMode2kDCI30;
      s_name2mode["2k dci 50p"] = bmdMode2kDCI50;
      s_name2mode["2k dci 59.94p"] = bmdMode2kDCI5994;
      s_name2mode["2k dci 60p"] = bmdMode2kDCI60;
      s_name2mode["4k 23.98p"] = bmdMode4K2160p2398;
      s_name2mode["4k 24p"] = bmdMode4K2160p24;
      s_name2mode["4k 25p"] = bmdMode4K2160p25;
      s_name2mode["4k 29.97p"] = bmdMode4K2160p2997;
      s_name2mode["4k 30p"] = bmdMode4K2160p30;
      s_name2mode["4k 50p"] = bmdMode4K2160p50;
      s_name2mode["4k 59.94p"] = bmdMode4K2160p5994;
      s_name2mode["4k 60p"] = bmdMode4K2160p60;
      s_name2mode["4k dci 23.98p"] = bmdMode4kDCI2398;
      s_name2mode["8k dci 23.98p"] = bmdMode8kDCI2398;
      s_name2mode["4k dci 24p"] = bmdMode4kDCI24;
      s_name2mode["8k dci 24p"] = bmdMode8kDCI24;
      s_name2mode["4k dci 25p"] = bmdMode4kDCI25;
      s_name2mode["8k dci 25p"] = bmdMode8kDCI25;
      s_name2mode["4k dci 29.97p"] = bmdMode4kDCI2997;
      s_name2mode["8k dci 29.97p"] = bmdMode8kDCI2997;
      s_name2mode["4k dci 30p"] = bmdMode4kDCI30;
      s_name2mode["8k dci 30p"] = bmdMode8kDCI30;
      s_name2mode["4k dci 50p"] = bmdMode4kDCI50;
      s_name2mode["8k dci 50p"] = bmdMode8kDCI50;
      s_name2mode["4k dci 59.94p"] = bmdMode4kDCI5994;
      s_name2mode["8k dci 59.94p"] = bmdMode8kDCI5994;
      s_name2mode["4k dci 60p"] = bmdMode4kDCI60;
      s_name2mode["8k dci 60p"] = bmdMode8kDCI60;
      s_name2mode["8k 23.98p"] = bmdMode8K4320p2398;
      s_name2mode["8k 24p"] = bmdMode8K4320p24;
      s_name2mode["8k 25p"] = bmdMode8K4320p25;
      s_name2mode["8k 29.97p"] = bmdMode8K4320p2997;
      s_name2mode["8k 30p"] = bmdMode8K4320p30;
      s_name2mode["8k 50p"] = bmdMode8K4320p50;
      s_name2mode["8k 59.94p"] = bmdMode8K4320p5994;
      s_name2mode["8k 60p"] = bmdMode8K4320p60;


    }
    std::map<std::string, BMDDisplayMode>::iterator it = s_name2mode.find(name);
    if(s_name2mode.end() != it) {
      mode=it->second;
      return true;
    }
    return false;
  }




IDeckLinkDisplayMode*getDisplayMode(IDeckLinkOutput*dlo,
                                    const std::string&formatname, int formatnum)
{
  IDeckLinkDisplayModeIterator*dmi = NULL;
  IDeckLinkDisplayMode*displayMode = NULL;
  int count=formatnum;
  BMDDisplayMode mode = bmdModeUnknown;
  bool have_mode = name2mode(formatname, mode);

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
          logpost(0, 3+1, "[GEM:videoDECKLINK] checking format '%s'", dmns.c_str());
          free_deckstring(dmn);
          if(found) {
            break;
          }
          if(have_mode && displayMode->GetDisplayMode() == mode) {
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
#ifdef _WIN32
    m_frameConverter = NULL;
#else
    m_frameConverter = CreateVideoConversionInstance();
#endif

    m_displayMode->GetFrameRate(&m_frameDuration, &m_frameTimescale);

    m_deckLinkOutput->SetScheduledFrameCompletionCallback(this);
  };
  ~VideoOutputter(void)
  {
    if (m_frameConverter) {
      m_frameConverter->Release();
    }
    m_frameConverter = NULL;
  }

  virtual HRESULT STDMETHODCALLTYPE ScheduledFrameCompleted(IDeckLinkVideoFrame* completedFrame, BMDOutputFrameCompletionResult result)
  {
    return S_OK;
  };
  virtual HRESULT STDMETHODCALLTYPE ScheduledPlaybackHasStopped()
  {
    return S_OK;
  };
  virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, LPVOID *ppv)
  {
    *ppv = NULL;
    return E_NOINTERFACE;
  };

  virtual ULONG STDMETHODCALLTYPE AddRef(void)
  {
    // gcc atomic operation builtin
    return __sync_add_and_fetch(&m_refCount, 1);
  };

  ULONG STDMETHODCALLTYPE Release(void)
  {
    // gcc atomic operation builtin
    ULONG newRefValue = __sync_sub_and_fetch(&m_refCount, 1);
    if (!newRefValue) {
      delete this;
    }
    return newRefValue;
  };

  bool setFrame(imageStruct*img)
  {
    /* convert the imageStruct into the IDeckLinkVideoFrame */
    ImageStructWrapper*isw = new ImageStructWrapper(img);
    imageStruct dst;
    HRESULT result = S_OK;
    const BMDPixelFormat srcformat = isw->GetPixelFormat();

#if 0
    post("writing image %p[%dx%d@%s] -> %p[%dx%d@%s] || %lu*%lu/%lu"
         , isw, (int)isw->GetWidth(), (int)isw->GetHeight(), pixformat2string(srcformat).c_str()
         , m_videoFrame, (int)m_videoFrame->GetWidth(), (int)m_videoFrame->GetHeight(), pixformat2string(m_videoFrame->GetPixelFormat()).c_str()
         , m_totalFramesScheduled, m_frameDuration, m_frameTimescale
        );
#endif

    if (m_videoFrame->GetPixelFormat() != srcformat) {
      result = E_NOTIMPL;
      switch(srcformat) {
      case bmdFormatUnspecified:
      case gemBmdFormat8BitRGBA:
        break;
      default:
        result = m_frameConverter?m_frameConverter->ConvertFrame(isw, m_videoFrame):E_NOTIMPL;
      }

      if (result != S_OK)  {
        if(isw) {
          isw->Release();
        }

        if(!dst.convertFrom(img, GEM_YUV)) {
          post("unable to convert frame...");
          return false;
        }
        img = &dst;
        isw = new ImageStructWrapper(img);
      }
    }

#if 0
    post("schedule image %p[%dx%d@%s]"
         , isw, (int)isw->GetWidth(), (int)isw->GetHeight(), pixformat2string(isw->GetPixelFormat()).c_str()
         );
#endif

    result = m_deckLinkOutput->ScheduleVideoFrame(isw, (m_totalFramesScheduled * m_frameDuration), m_frameDuration, m_frameTimescale);
    if (result != S_OK)  {
      fprintf(stderr, "Failed to schedule frame: 0x%X\n", (unsigned int)result);
      return false;
    }
    m_totalFramesScheduled++;

    if(isw) {
      isw->Release();
    }
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
  m_pixBlock.image.setFormat(GEM_RGBA);
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
  HRESULT result = S_OK;
  if(m_dlOutput) {
    result = m_dlOutput->StopScheduledPlayback(0, 0, 0);
    m_dlOutput->DisableVideoOutput();
    m_dlOutput->Release();
  }
  m_dlOutput = 0;

  if(m_dlCallback) {
    m_dlCallback->Release();
  }
  m_dlCallback = 0;

  if(m_dlConfig) {
    m_dlConfig->Release();
  }
  m_dlConfig = 0;
}

bool recordDECKLINK :: start(const std::string&filename, gem::Properties&props)
{
  stop();
  deckbool_t is_supported = false;
  int formatnumber=-1;
  std::string formatname="";
  BMDVideoOutputFlags flags = bmdVideoOutputFlagDefault;
  IDeckLinkIterator*dlIterator = CreateDeckLinkIteratorInstance();
  BMDVideoConnection vconn = m_connectionType;

  std::vector<std::string>keys=props.keys();
  for(int i=0; i<keys.size(); i++) {
    std::string key=keys[i];
    if("format" == key) {
      std::string s;
      double d;
      switch(props.type(key)) {
      case gem::Properties::STRING:
        if(props.get(key, s)) {
          formatnumber =-1;
          formatname=s;
        }
        break;
      case gem::Properties::DOUBLE:
        if(props.get(key, d)) {
          formatnumber =(int)d;
          formatname="";
        }
        break;
      default:
        break;
      }
      continue;
    }
    if("connection" == key) {
      std::string s;
      double d;
      switch(props.type(key)) {
      case gem::Properties::STRING:
        if(props.get(key, s)) {
          vconn = string2connection(s);
          if(-1 == vconn) {
            vconn = bmdVideoConnectionUnspecified;
          }
        }
        post("setting 'connection' to %d '%s'", vconn, s.c_str());
        break;
      case gem::Properties::DOUBLE:
        if(props.get(key, d)) {
          vconn = id2connection((int)d);
        }
        break;
      default:
        break;
      }
      continue;
    }
  }

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
    dlIterator->Release();
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
        m_pixelFormat = (BMDPixelFormat)gotint;
        //post("got pixelformat: %d -> %s", (int)gotint, pixformat2string(m_pixelFormat).c_str());
      }
    }
  }



  if(m_dlOutput) {
    BMDDisplayMode actualMode;
    if (S_OK != m_dlOutput->DoesSupportVideoMode(
          vconn /* in: BMDVideoConnection connection */
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
                       vconn);
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
  /* mode: auto, ntsc,...,1080p50 */
  gem::any v;
  props.clear();

  /* format:
   * "auto", "automatic",
   * "NTSC", "PAL", "NTSC Progressive", "PAL Progressive", "1080p30", "720p50",...
   */
  props.set("format", std::string("auto"));

  /* connection:
   * "auto",
   * "sdi", "hdmi", "opticalsdi", "component", "composite", "svideo",
   */
  props.set("connection", std::string("auto"));
  return true;
}
