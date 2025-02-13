////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.at
//
// Implementation file
//
//    Copyright © 2014-2019 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    based on ofDirectShowPlayer, Copyright © 2014 Theodore Watson
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "filmDS.h"
#include "plugins/PluginFactory.h"
#include "Gem/Properties.h"
#include "Gem/RTE.h"
#include "Utils/wstring.h"
/* C++11 */
#include <memory>
#include <functional>

#if 0
# define MARK_HR(hr) if(hr)printf("%s:%d (%s)\t: 0x%X\n", __FILE__, __LINE__, __FUNCTION__, hr);else printf("%s:%d (%s)\n", __FILE__, __LINE__, __FUNCTION__)
#else
# define MARK_HR(hr) do{;} while(0)
#endif
#define MARK() MARK_HR(0)


#define USE_CALLBACKS 1
#define USE_AUTO 0

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
// DirectShow includes and helper methods
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

#include <dshow.h>
#pragma include_alias( "dxtrans.h", "qedit.h" )
#define __IDxtCompositor_INTERFACE_DEFINED__
#define __IDxtAlphaSetter_INTERFACE_DEFINED__
#define __IDxtJpeg_INTERFACE_DEFINED__
#define __IDxtKey_INTERFACE_DEFINED__
#include <aviriff.h>
#include <windows.h>

//for threading
#include <process.h>

// Due to a missing qedit.h in recent Platform SDKs, we've replicated the relevant contents here
// #include <qedit.h>
MIDL_INTERFACE("0579154A-2B53-4994-B0D0-E773148EFF85")
ISampleGrabberCB :
public IUnknown {
public:
  virtual HRESULT STDMETHODCALLTYPE SampleCB(
    double SampleTime,
    IMediaSample *pSample) = 0;

  virtual HRESULT STDMETHODCALLTYPE BufferCB(
    double SampleTime,
    BYTE *pBuffer,
    long BufferLen) = 0;

};

MIDL_INTERFACE("6B652FFF-11FE-4fce-92AD-0266B5D7C78F")
ISampleGrabber :
public IUnknown {
public:
  virtual HRESULT STDMETHODCALLTYPE SetOneShot(
    BOOL OneShot) = 0;

  virtual HRESULT STDMETHODCALLTYPE SetMediaType(
    const AM_MEDIA_TYPE *pType) = 0;

  virtual HRESULT STDMETHODCALLTYPE GetConnectedMediaType(
    AM_MEDIA_TYPE *pType) = 0;

  virtual HRESULT STDMETHODCALLTYPE SetBufferSamples(
    BOOL BufferThem) = 0;

  virtual HRESULT STDMETHODCALLTYPE GetCurrentBuffer(
    /* [out][in] */ long *pBufferSize,
    /* [out] */ long *pBuffer) = 0;

  virtual HRESULT STDMETHODCALLTYPE GetCurrentSample(
    /* [retval][out] */ IMediaSample **ppSample) = 0;

  virtual HRESULT STDMETHODCALLTYPE SetCallback(
    ISampleGrabberCB *pCallback,
    long WhichMethodToCallback) = 0;

};
EXTERN_C const CLSID CLSID_SampleGrabber;
EXTERN_C const IID IID_ISampleGrabber;
EXTERN_C const CLSID CLSID_NullRenderer;

// GetUnconnectedPin
//    Finds an unconnected pin on a filter in the desired direction
HRESULT GetUnconnectedPin(
  IBaseFilter *pFilter,   // Pointer to the filter.
  PIN_DIRECTION PinDir,   // Direction of the pin to find.
  IPin **ppPin)           // Receives a pointer to the pin.
{
  *ppPin = 0;
  IEnumPins *pEnum = 0;
  IPin *pPin = 0;
  HRESULT hr = pFilter->EnumPins(&pEnum);
  if (FAILED(hr)) {
    return hr;
  }
  while (pEnum->Next(1, &pPin, NULL) == S_OK) {
    PIN_DIRECTION ThisPinDir;
    pPin->QueryDirection(&ThisPinDir);
    if (ThisPinDir == PinDir) {
      IPin *pTmp = 0;
      hr = pPin->ConnectedTo(&pTmp);
      if (SUCCEEDED(hr)) { // Already connected, not the pin we want.
        pTmp->Release();
      } else { // Unconnected, this is the pin we want.
        pEnum->Release();
        *ppPin = pPin;
        return S_OK;
      }
    }
    pPin->Release();
  }
  pEnum->Release();
  // Did not find a matching pin.
  return E_FAIL;
}

// Disconnect any connections to the filter.
HRESULT DisconnectPins(IBaseFilter *pFilter)
{
  IEnumPins *pEnum = 0;
  IPin *pPin = 0;
  HRESULT hr = pFilter->EnumPins(&pEnum);
  if (FAILED(hr)) {
    return hr;
  }

  while (pEnum->Next(1, &pPin, NULL) == S_OK) {
    pPin->Disconnect();
    pPin->Release();
  }
  pEnum->Release();

  // Did not find a matching pin.
  return S_OK;
}

// ConnectFilters
//    Connects a pin of an upstream filter to the pDest downstream filter
HRESULT ConnectFilters(
  IGraphBuilder *pGraph, // Filter Graph Manager.
  IPin *pOut,            // Output pin on the upstream filter.
  IBaseFilter *pDest)    // Downstream filter.
{
  if ((pGraph == NULL) || (pOut == NULL) || (pDest == NULL)) {
    return E_POINTER;
  }
#ifdef debug
  PIN_DIRECTION PinDir;
  pOut->QueryDirection(&PinDir);
  _ASSERTE(PinDir == PINDIR_OUTPUT);
#endif

  // Find an input pin on the downstream filter.
  IPin *pIn = 0;
  HRESULT hr = GetUnconnectedPin(pDest, PINDIR_INPUT, &pIn);
  if (FAILED(hr)) {
    return hr;
  }
  // Try to connect them.
  hr = pGraph->Connect(pOut, pIn);
  pIn->Release();
  return hr;
}



// ConnectFilters
//    Connects two filters
HRESULT ConnectFilters(
  IGraphBuilder *pGraph,
  IBaseFilter *pSrc,
  IBaseFilter *pDest)
{
  if ((pGraph == NULL) || (pSrc == NULL) || (pDest == NULL)) {
    return E_POINTER;
  }

  // Find an output pin on the first filter.
  IPin *pOut = 0;
  HRESULT hr = GetUnconnectedPin(pSrc, PINDIR_OUTPUT, &pOut);
  if (FAILED(hr)) {
    return hr;
  }
  hr = ConnectFilters(pGraph, pOut, pDest);
  pOut->Release();
  return hr;
}

// LocalFreeMediaType
//    Free the format buffer in the media type
void LocalFreeMediaType(AM_MEDIA_TYPE& mt)
{
  if (mt.cbFormat != 0) {
    CoTaskMemFree((PVOID)mt.pbFormat);
    mt.cbFormat = 0;
    mt.pbFormat = NULL;
  }
  if (mt.pUnk != NULL) {
    // Unnecessary because pUnk should not be used, but safest.
    mt.pUnk->Release();
    mt.pUnk = NULL;
  }
}

// LocalDeleteMediaType
//    Free the format buffer in the media type,
//    then delete the MediaType ptr itself
void LocalDeleteMediaType(AM_MEDIA_TYPE *pmt)
{
  if (pmt != NULL) {
    LocalFreeMediaType(*pmt); // See FreeMediaType for the implementation.
    CoTaskMemFree(pmt);
  }
}


HRESULT SaveGraphFile(IGraphBuilder *pGraph, WCHAR *wszPath)
{
  const WCHAR wszStreamName[] = L"ActiveMovieGraph";
  HRESULT hr;

  IStorage *pStorage = NULL;
  hr = StgCreateDocfile(
         wszPath,
         STGM_CREATE | STGM_TRANSACTED | STGM_READWRITE | STGM_SHARE_EXCLUSIVE,
         0, &pStorage);
  if(FAILED(hr)) {
    return hr;
  }

  IStream *pStream;
  hr = pStorage->CreateStream(
         wszStreamName,
         STGM_WRITE | STGM_CREATE | STGM_SHARE_EXCLUSIVE,
         0, 0, &pStream);
  if (FAILED(hr)) {
    pStorage->Release();
    return hr;
  }

  IPersistStream *pPersist = NULL;
  pGraph->QueryInterface(IID_IPersistStream, (void**)&pPersist);
  hr = pPersist->Save(pStream, TRUE);
  pStream->Release();
  pPersist->Release();
  if (SUCCEEDED(hr)) {
    hr = pStorage->Commit(STGC_DEFAULT);
  }
  pStorage->Release();
  return hr;
}

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
// DirectShowVideo - a simple directshow video player implementation
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

namespace
{
int comRefCount = 0;

void retainCom()
{
  if( comRefCount == 0 ) {
    //printf("com is initialized!\n");
    CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
  }
  comRefCount++;
}

void releaseCom()
{
  comRefCount--;
  if( comRefCount == 0 ) {
    //printf("com is uninitialized!\n");
    CoUninitialize();
  }
}

void releaseSample(IMediaSample * sample)
{
  sample->Release();
}
}


class gem::plugins::filmDS::DirectShowVideo : public ISampleGrabberCB
{
public:

  DirectShowVideo()
  {
    retainCom();
    clearValues();
    InitializeCriticalSection(&critSection);
  }

  ~DirectShowVideo()
  {
    stop();
    tearDown();
    middleSample.reset();
    backSample.reset();
    releaseCom();
    DeleteCriticalSection(&critSection);
  }

  void tearDown()
  {
    //printf("tearDown\n");

    if(m_pControl) {
      m_pControl->Release();
    }
    if(m_pEvent) {
      m_pEvent->Release();
    }
    if(m_pSeek) {
      m_pSeek->Release();
    }
    if(m_pAudio) {
      m_pAudio->Release();
    }
    if(m_pBasicVideo) {
      m_pBasicVideo->Release();
    }
    if(m_pGrabber) {
      m_pGrabber->Release();
    }
    if(m_pGrabberF) {
      m_pGrabberF->Release();
    }
    if(m_pGraph) {
      m_pGraph->Release();
    }
    if(m_pNullRenderer) {
      m_pNullRenderer->Release();
    }
    if( m_pSourceFile ) {
      m_pSourceFile->Release();
    }
    if( m_pPosition ) {
      m_pPosition->Release();
    }
    clearValues();
  }

  void clearValues()
  {
    hr = 0;

    m_pGraph = NULL;
    m_pControl = NULL;
    m_pEvent = NULL;
    m_pSeek = NULL;
    m_pAudio = NULL;
    m_pGrabber = NULL;
    m_pGrabberF = NULL;
    m_pBasicVideo = NULL;
    m_pNullRenderer = NULL;
    m_pSourceFile = NULL;
    m_pPosition = NULL;

    timeNow = 0;
    lPositionInSecs = 0;
    lDurationInNanoSecs = 0;
    lTotalDuration = 0;
    rtNew = 0;
    lPosition = 0;
    lvolume = -1000;
    evCode = 0;
    width = height = 0;
    bVideoOpened = false;
    bLoop = false;
    bPaused = false;
    bPlaying = false;
    bEndReached = false;
    bNewPixels = false;
    bFrameNew = false;
    curMovieTime = -1.;
    bNewlyOpened = false;

    movieRate = 1.0;
    averageTimePerFrame = 1.0/30.0;
  }

  //------------------------------------------------
  STDMETHODIMP_(ULONG) AddRef()
  {
    return 1;
  }
  STDMETHODIMP_(ULONG) Release()
  {
    return 2;
  }


  //------------------------------------------------
  STDMETHODIMP QueryInterface(REFIID riid, void **ppvObject)
  {
    *ppvObject = static_cast<ISampleGrabberCB*>(this);
    return S_OK;
  }


  //------------------------------------------------
  STDMETHODIMP SampleCB(double Time, IMediaSample *pSample)
  {
    MARK();
    BYTE * ptrBuffer = NULL;
    HRESULT hr = pSample->GetPointer(&ptrBuffer);

    if(hr == S_OK) {
      long latestBufferLength = pSample->GetActualDataLength();
      int currentBufferLength = pix.image.xsize * pix.image.ysize * pix.image.csize;
      if(latestBufferLength == currentBufferLength ) {
        EnterCriticalSection(&critSection);
        pSample->AddRef();
        backSample =
          std::unique_ptr<IMediaSample, std::function<void(IMediaSample*)>>(pSample, releaseSample);
        bNewPixels = true;

        //this is just so we know if there is a new frame
        if(Time != curMovieTime) {
          bFrameNew = true;
        }
        curMovieTime = Time;

        LeaveCriticalSection(&critSection);
      } else {
        pd_error(0, "[GEM:videoDS] SampleCB() - buffer sizes do not match %d != %d",
                 latestBufferLength, currentBufferLength);
      }
    }

    MARK();
    return S_OK;
  }

  //This method is meant to have more overhead
  STDMETHODIMP BufferCB(double Time, BYTE *pBuffer, long BufferLen)
  {
    return E_NOTIMPL;
  }

  bool loadMovie(std::string path, int format)
  {
    tearDown();
    pixelFormat = format;

    // Create the Filter Graph Manager and query for interfaces.

    //printf("step 1\n");
    hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER,
                          IID_IGraphBuilder, (void **)&m_pGraph);
    if (FAILED(hr)) {
      tearDown();
      return false;
    }

    //printf("step 2\n");
    hr = m_pGraph->QueryInterface(IID_IMediaSeeking, (void**)&m_pSeek);
    if (FAILED(hr)) {
      tearDown();
      return false;
    }

    hr = m_pGraph->QueryInterface(IID_IMediaPosition, (LPVOID *)&m_pPosition);
    if (FAILED(hr)) {
      tearDown();
      return false;
    }
#if 0
    hr = m_pGraph->QueryInterface(IID_IBasicAudio,(void**)&m_pAudio);
    if (FAILED(hr)) {
      tearDown();
      return false;
    }
#endif

    // Use IGraphBuilder::QueryInterface (inherited from IUnknown) to get the IMediaControl interface.
    //printf("step 4\n");
    hr = m_pGraph->QueryInterface(IID_IMediaControl, (void **)&m_pControl);
    if (FAILED(hr)) {
      tearDown();
      return false;
    }

    // And get the Media Event interface, too.
    //printf("step 5\n");
    hr = m_pGraph->QueryInterface(IID_IMediaEvent, (void **)&m_pEvent);
    if (FAILED(hr)) {
      tearDown();
      return false;
    }

    //SAMPLE GRABBER (ALLOWS US TO GRAB THE BUFFER)//
    // Create the Sample Grabber.
    hr = CoCreateInstance(CLSID_SampleGrabber, NULL, CLSCTX_INPROC_SERVER,
                          IID_IBaseFilter, (void**)&m_pGrabberF);
    if (FAILED(hr)) {
      tearDown();
      return false;
    }

    hr = m_pGraph->AddFilter(m_pGrabberF, L"Sample Grabber");
    if (FAILED(hr)) {
      tearDown();
      return false;
    }

    hr = m_pGrabberF->QueryInterface(IID_ISampleGrabber, (void**)&m_pGrabber);
    if (FAILED(hr)) {
      tearDown();
      return false;
    }

#if USE_CALLBACKS
    hr = m_pGrabber->SetCallback(this, 0);
    if (FAILED(hr)) {
      tearDown();
      return false;
    }
#endif
    //MEDIA CONVERSION
    //Get video properties from the stream's mediatype and apply to the grabber (otherwise we don't get an RGB image)
    AM_MEDIA_TYPE mt;
    ZeroMemory(&mt,sizeof(AM_MEDIA_TYPE));

    mt.majortype    = MEDIATYPE_Video;
    switch (format) {
    case GEM_RGB:
      mt.subtype = MEDIASUBTYPE_RGB24;
      break;
    case GEM_RGBA:
      mt.subtype = MEDIASUBTYPE_RGB32;
      break;
    default:
      logpost(0, 3+1,
              "[GEM:videoDS] Trying to set unsupported format this is an internal bug, using default RGBA");
      mt.subtype = MEDIASUBTYPE_RGB32;
    }

    mt.formattype   = FORMAT_VideoInfo;
    //printf("step 5.5\n");
    hr = m_pGrabber->SetMediaType(&mt);
    if (FAILED(hr)) {
      tearDown();
      return false;
    }

    //printf("step 6\n");
    std::wstring filePathW = std::wstring(path.begin(), path.end());

    //this is the easier way to connect the graph, but we have to remove the video window manually
    hr = m_pGraph->RenderFile(filePathW.c_str(), NULL);

    //printf("step 7\n");
    if (SUCCEEDED(hr)) {

      //Set Params - One Shot should be false unless you want to capture just one buffer
      hr = m_pGrabber->SetOneShot(FALSE);
      if (FAILED(hr)) {
        logpost(0, 3+1, "[GEM:videoDS] unable to set one shot");
        tearDown();
        return false;
      }

      //apparently setting to TRUE causes a small memory leak
      hr = m_pGrabber->SetBufferSamples(FALSE);
      if (FAILED(hr)) {
        logpost(0, 3+1, "[GEM:videoDS] unable to set buffer samples");
        tearDown();
        return false;
      }

      //NULL RENDERER//
      //used to give the video stream somewhere to go to.
      hr = CoCreateInstance(CLSID_NullRenderer, NULL, CLSCTX_INPROC_SERVER,
                            IID_IBaseFilter, (void**)(&m_pNullRenderer));
      if (FAILED(hr)) {
        logpost(0, 3+1, "[GEM:videoDS] null renderer error");
        tearDown();
        return false;
      }

      hr = m_pGraph->AddFilter(m_pNullRenderer, L"Render");
      if (FAILED(hr)) {
        logpost(0, 3+1, "[GEM:videoDS] unable to add null renderer");
        tearDown();
        return false;
      }

      AM_MEDIA_TYPE mt;
      ZeroMemory(&mt,sizeof(AM_MEDIA_TYPE));

      hr = m_pGrabber->GetConnectedMediaType(&mt);
      if (FAILED(hr)) {
        logpost(0, 3+1, "[GEM:videoDS] unable to call GetConnectedMediaType");
        tearDown();
        return false;
      }

      VIDEOINFOHEADER * infoheader = (VIDEOINFOHEADER*)mt.pbFormat;
      width = infoheader->bmiHeader.biWidth;
      height = infoheader->bmiHeader.biHeight;
      averageTimePerFrame = infoheader->AvgTimePerFrame / 10000000.0;
      pix.image.xsize = width;
      pix.image.ysize = height;
      pix.image.setFormat(pixelFormat);
      pix.image.reallocate();

      //we need to manually change the output from the renderer window to the null renderer
      IBaseFilter * m_pVideoRenderer;
      IPin* pinIn = 0;
      IPin* pinOut = 0;

      hr = m_pGraph->FindFilterByName(L"Video Renderer", &m_pVideoRenderer);
      if (FAILED(hr)) {
        logpost(0, 3+1, "[GEM:videoDS] failed to find the video renderer");
        tearDown();
        return false;
      }

      //we disconnect the video renderer window by finding the output pin of the sample grabber
      hr = m_pGrabberF->FindPin(L"Out", &pinOut);
      if (FAILED(hr)) {
        logpost(0, 3+1, "[GEM:videoDS] failed to find the sample grabber output pin");
        tearDown();
        return false;
      }

      hr = pinOut->Disconnect();
      if (FAILED(hr)) {
        logpost(0, 3+1, "[GEM:videoDS] failed to disconnect grabber output pin");
        tearDown();
        return false;
      }

      //we have to remove it as well otherwise the graph builder will reconnect it
      hr = m_pGraph->RemoveFilter(m_pVideoRenderer);
      if (FAILED(hr)) {
        logpost(0, 3+1, "[GEM:videoDS] failed to remove the default renderer");
        tearDown();
        return false;
      } else {
        m_pVideoRenderer->Release();
      }

      //now connect the null renderer to the grabber output, if we don't do this not frames will be captured
      hr = m_pNullRenderer->FindPin(L"In", &pinIn);
      if (FAILED(hr)) {
        logpost(0, 3+1,
                "[GEM:videoDS] failed to find the input pin of the null renderer");
        tearDown();
        return false;
      }

      hr = pinOut->Connect(pinIn, NULL);
      if (FAILED(hr)) {
        logpost(0, 3+1, "[GEM:videoDS] failed to connect the null renderer");
        tearDown();
        return false;
      }

      //printf("step 8\n");


      // Run the graph.

      //SaveGraphFile(m_pGraph, L"test2.grf");
      hr = m_pControl->Run();
      //SaveGraphFile(m_pGraph, L"test3.grf");

      // Now pause the graph.
      hr = m_pControl->Stop();
      updatePlayState();

      if( FAILED(hr) || width == 0 || height == 0 ) {
        tearDown();
        logpost(0, 3+1,
                "[GEM:videoDS] Error occurred while playing or pausing or opening the file");
        return false;
      }
    } else {
      tearDown();
      logpost(0, 3+1,
              "[GEM:videoDS] Error occurred while playing or pausing or opening the file");
      return false;
    }

    bVideoOpened = true;
    bNewlyOpened = true;
    return true;
  }

  void update()
  {
    if( bVideoOpened ) {
      long eventCode = 0;
#ifdef _WIN64
      long long ptrParam1 = 0;
      long long ptrParam2 = 0;
#else
      long ptrParam1 = 0;
      long ptrParam2 = 0;
#endif

      while (S_OK == m_pEvent->GetEvent(&eventCode, &ptrParam1, &ptrParam2, 0)) {
        if (eventCode == EC_COMPLETE ) {
          if(bLoop) {
            //printf("Restarting!\n");
            setPosition(0.0);
          } else {
            bEndReached = true;
            //printf("movie end reached!\n");
            stop();
            updatePlayState();
          }
        }
        //printf("Event code: %#04x\n Params: %d, %d\n", eventCode, ptrParam1, ptrParam2);
        m_pEvent->FreeEventParams(eventCode, ptrParam1, ptrParam2);
      }
    }
  }

  bool isLoaded()
  {
    return bVideoOpened;
  }
#if 0
  //volume has to be log corrected/converted
  void setVolume(float volPct)
  {
    if( isLoaded() ) {
      if( volPct < 0 ) {
        volPct = 0.0;
      }
      if( volPct > 1 ) {
        volPct = 1.0;
      }

      long vol = log10(volPct) * 4000.0;
      if(vol < -8000) {
        vol = -10000;
      }
      m_pAudio->put_Volume(vol);
    }
  }

  float getVolume()
  {
    float volPct = 0.0;
    if( isLoaded() ) {
      long vol = 0;
      m_pAudio->get_Volume(&vol);
      volPct = powf(10, (float)vol/4000.0);
    }
    return volPct;
  }
#endif
  double getDurationInSeconds()
  {
    if( isLoaded() ) {
      long long lDurationInNanoSecs = 0;
      m_pSeek->GetDuration(&lDurationInNanoSecs);
      double timeInSeconds = (double)lDurationInNanoSecs/10000000.0;

      return timeInSeconds;
    }
    return 0.0;
  }

  double getCurrentTimeInSeconds()
  {
    if( isLoaded() ) {
      long long lCurrentTimeInNanoSecs = 0;
      m_pSeek->GetCurrentPosition(&lCurrentTimeInNanoSecs);
      double timeInSeconds = (double)lCurrentTimeInNanoSecs/10000000.0;

      return timeInSeconds;
    }
    return 0.0;
  }

  void setPosition(float pct)
  {
    if( bVideoOpened ) {
      if( pct < 0.0 ) {
        pct = 0.0;
      }
      if( pct > 1.0 ) {
        pct = 1.0;
      }

      long long lDurationInNanoSecs = 0;
      m_pSeek->GetDuration(&lDurationInNanoSecs);

      rtNew = ((float)lDurationInNanoSecs * pct);
      hr = m_pSeek->SetPositions(&rtNew, AM_SEEKING_AbsolutePositioning,NULL,
                                 AM_SEEKING_NoPositioning);
    }
  }

  float getPosition()
  {
    if( bVideoOpened ) {
      float timeDur = getDurationInSeconds();
      if( timeDur > 0.0 ) {
        return getCurrentTimeInSeconds() / timeDur;
      }
    }
    return 0.0;
  }

  void setSpeed(float speed)
  {
    if( bVideoOpened ) {
      m_pPosition->put_Rate(speed);
      m_pPosition->get_Rate(&movieRate);
    }
  }

  double getSpeed()
  {
    return movieRate;
  }

  void play()
  {
    if( bVideoOpened ) {
      m_pControl->Run();
      bEndReached = false;
      updatePlayState();
    }
  }

  void stop()
  {
    if( bVideoOpened ) {
      if( isPlaying() ) {
        setPosition(0.0);
      }
      m_pControl->Stop();
      updatePlayState();
    }
  }

  void setPaused(bool bPaused)
  {
    if( bVideoOpened ) {
      if( bPaused ) {
        m_pControl->Pause();
      } else {
        m_pControl->Run();
      }
      updatePlayState();
    }

  }

  void updatePlayState()
  {
    if( bVideoOpened ) {
      FILTER_STATE fs;
      hr = m_pControl->GetState(4000, (OAFilterState*)&fs);
      if(hr==S_OK) {
        if( fs == State_Running ) {
          bPlaying = true;
          bPaused = false;
        } else if( fs == State_Paused ) {
          bPlaying = false;
          bPaused = true;
        } else if( fs == State_Stopped ) {
          bPlaying = false;
          bPaused = false;
        }
      }
    }
  }

  bool isPlaying()
  {
    return bPlaying;
  }

  bool isPaused()
  {
    return bPaused;
  }

  bool isLooping()
  {
    return bLoop;
  }

  void setLoop(bool loop)
  {
    bLoop = loop;
  }

  bool isMovieDone()
  {
    return bEndReached;
  }

  int getWidth()
  {
    return width;
  }

  int getHeight()
  {
    return height;
  }

  bool isFrameNew()
  {
    return bFrameNew;
  }

  void nextFrame()
  {
    //we have to do it like this as the frame based approach is not very accurate
    if( bVideoOpened && ( isPlaying() || isPaused() ) ) {
      int curFrame = getCurrentFrameNo();
      float curFrameF = curFrame;
      for(int i = 1; i < 20; i++) {
        setApproximateFrameF( curFrameF + 0.3 * (float)i );
        if( getCurrentFrameNo() >= curFrame + 1 ) {
          break;
        }
      }
    }
  }

  void preFrame()
  {
    //we have to do it like this as the frame based approach is not very accurate
    if( bVideoOpened && ( isPlaying() || isPaused() ) ) {
      int curFrame = getCurrentFrameNo();
      float curFrameF = curFrame;
      for(int i = 1; i < 20; i++) {
        setApproximateFrameF( curFrameF - 0.3 * (float)i );
        if( getCurrentFrameNo() <= curFrame + 1 ) {
          break;
        }
      }
    }
  }

  void setApproximateFrameF(float frameF)
  {
    if( bVideoOpened ) {
      float pct = frameF / (float)getApproximateNoFrames();
      if( pct > 1.0 ) {
        pct = 1.0;
      }
      if( pct < 0.0 ) {
        pct = 0.0;
      }
      setPosition(pct);
    }
  }

  void setApproximateFrame(int frame)
  {
    if( bVideoOpened ) {
      float pct = (float)frame / (float)getApproximateNoFrames();
      if( pct > 1.0 ) {
        pct = 1.0;
      }
      if( pct < 0.0 ) {
        pct = 0.0;
      }
      setPosition(pct);
    }
  }

  int getCurrentFrameNo()
  {
    if( bVideoOpened ) {
      return getPosition() * (float) getApproximateNoFrames();
    }
    return 0;
  }

  int getApproximateNoFrames()
  {
    if( bVideoOpened && averageTimePerFrame > 0.0 ) {
      return getDurationInSeconds() / averageTimePerFrame;
    }
    return 0;
  }

  double getAverageTimePerFrame()
  {
    return averageTimePerFrame;
  }



#ifdef USE_CALLBACKS
  pixBlock*getPixels(void)
  {
    if(!bVideoOpened) {
      return 0;
    }
    pix.newfilm = bNewlyOpened;
    bNewlyOpened = false;
    if(pix.image.xsize != getWidth()  || pix.image.ysize != getHeight()) {
      pix.image.xsize = getWidth();
      pix.image.ysize = getHeight();
      pix.image.setFormat(GEM_RGBA);
      pix.image.reallocate();
      pix.newfilm = true;
    }
    if(bFrameNew) {
      pix.newimage = true;
      EnterCriticalSection(&critSection);
      std::swap(backSample, middleSample);
      bNewPixels = false;
      bFrameNew = false;
      LeaveCriticalSection(&critSection);
      BYTE * ptrBuffer = NULL;
      HRESULT hr = middleSample->GetPointer(&ptrBuffer);
      switch (pixelFormat) {
      case GEM_RGB:
        pix.image.fromBGR(ptrBuffer);
        break;
      case GEM_RGBA:
        pix.image.fromBGRA(ptrBuffer);
        break;
      }
      pix.image.upsidedown = false;
      pix.image.fixUpDown();
    }
    return &pix;
  }

#else
  //this is the non-callback approach
  bool getPixels(imageStruct&img)
  {
    if(!bVideoOpened) {
      return false;
    }
    if(isFrameNew()) {
      long bufferSize = img.xsize * img.ysize * img.csize;
      post("fetching %d bytes into %p", bufferSize, img.data);
      HRESULT hr = m_pGrabber->GetCurrentBuffer(&bufferSize, (long *)img.data);
      return (S_OK==hr);
    }
    return true;
  }
#endif

protected:

  HRESULT hr;                         // COM return value
  IGraphBuilder *m_pGraph;        // Graph Builder interface
  IMediaControl *m_pControl;  // Media Control interface
  IMediaEvent   *m_pEvent;        // Media Event interface
  IMediaSeeking *m_pSeek;     // Media Seeking interface
  IMediaPosition * m_pPosition;
  IBasicAudio   *m_pAudio;        // Audio Settings interface
  ISampleGrabber * m_pGrabber;
  IBaseFilter * m_pSourceFile;
  IBaseFilter * m_pGrabberF;
  IBasicVideo * m_pBasicVideo;
  IBaseFilter * m_pNullRenderer;

  REFERENCE_TIME
  timeNow;             // Used for FF & REW of movie, current time
  LONGLONG lPositionInSecs;       // Time in  seconds
  LONGLONG lDurationInNanoSecs;       // Duration in nanoseconds
  LONGLONG lTotalDuration;        // Total duration
  REFERENCE_TIME rtNew;               // Reference time of movie
  long lPosition;                 // Desired position of movie used in FF & REW
  long lvolume;                   // The volume level in 1/100ths dB Valid values range from -10,000 (silence) to 0 (full volume), 0 = 0 dB -10000 = -100 dB
  long evCode;                    // event variable, used to in file to complete wait.

  long width, height;

  double averageTimePerFrame;

  bool bFrameNew;
  bool bNewPixels;
  bool bVideoOpened;
  bool bPlaying;
  bool bPaused;
  bool bLoop;
  bool bEndReached;
  double movieRate;
  double curMovieTime;
  bool bNewlyOpened;

  CRITICAL_SECTION critSection;
  std::unique_ptr<IMediaSample, std::function<void(IMediaSample*)>>
      backSample;
  std::unique_ptr<IMediaSample, std::function<void(IMediaSample*)>>
      middleSample;
  pixBlock pix;
  int pixelFormat;
};


//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
// GEM SPECIFIC IMPLEMENTATION BELOW
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

using namespace gem::plugins;
REGISTER_FILMFACTORY("DirectShow", filmDS);
filmDS::filmDS()
{
  player = NULL;
}

filmDS::~filmDS()
{
  close();
}

bool filmDS::open(const std::string&path, const gem::Properties&props)
{
  close();
  player = new DirectShowVideo();
  bool res=player->loadMovie(path, GEM_RGBA);
  if(res) {
    player->setPosition(0);
#if USE_AUTO
    double d=0.;
    if(props.get("auto", d)) {
    }
    if(d>-1e-8 && d<1e-8) {
      player->setPaused(true);
    } else {
      player->setSpeed(d);
      player->play();
    }
#else
    player->setPaused(true);
#endif
  } else {
    close();
  }

  return res;
}

void filmDS::close()
{
  if( player ) {
    delete player;
    player = NULL;
  }
}

pixBlock*filmDS::getFrame(void)
{
  if(!(player && player->isLoaded())) {
    return 0;
  }
  player->update();
  if(player->isMovieDone()) {
    return 0;
  }
  return player->getPixels();
}

film::errCode filmDS::changeImage(int imgNum, int trackNum)
{
  if(!(player && player->isLoaded())) {
    return film::FAILURE;
  }

  player->setApproximateFrame(imgNum);
  player->update();
  return film::DONTKNOW;
}
// Property handling
bool filmDS::enumProperties(gem::Properties&readable,
                            gem::Properties&writeable)
{
  readable.clear();
  writeable.clear();

  gem::any value;
  value=0.;
  readable.set("fps", value);
  readable.set("frames", value);
  readable.set("width", value);
  readable.set("height", value);

#if USE_AUTO
  writeable.set("auto", value);
#endif
  return true;
}
void filmDS::setProperties(gem::Properties&props)
{
#if USE_AUTO
  double d;
  if(props.get("auto", d)) {
    if(player) {
      if(d>-1e-8 && d<1e-8) {
        player->setPaused(true);
      } else {
        player->setSpeed(d);
        player->play();
      }
    }
  }
#endif
}
void filmDS::getProperties(gem::Properties&props)
{
  std::vector<std::string> keys=props.keys();
  if(player && player->isLoaded()) {
    for(size_t i=0; i<keys.size(); i++) {
      std::string key=keys[i];
      props.erase(key);
      if("fps"==key) {
        double d=1./player->getAverageTimePerFrame();
        gem::any value=d;
        props.set(key, value);
      }
      if("frames"==key) {
        double d=player->getApproximateNoFrames();
        gem::any value=(int)(d+0.5);
        props.set(key, value);
      }
      if("width"==key) {
        double d=player->getWidth();
        gem::any value=d;
        props.set(key, value);
      }
      if("height"==key) {
        double d=player->getHeight();
        gem::any value=d;
        props.set(key, value);
      }
    }
  }
}
