////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.at
//
// Implementation file
//
//    Copyright (c) 2014 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    based on ofDirectShowPlayer
//        Copyright (c) 2014 Theodore Watson
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

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
// DirectShow includes and helper methods
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

#define MARK_HR(hr)if(hr)printf("%s:%d (%s)\t: 0x%X", __FILE__, __LINE__, __FUNCTION__, hr);else printf("%s:%d (%s)", __FILE__, __LINE__, __FUNCTION__);

#include <dshow.h>
#pragma include_alias( "dxtrans.h", "qedit.h" )
#define __IDxtCompositor_INTERFACE_DEFINED__
#define __IDxtAlphaSetter_INTERFACE_DEFINED__
#define __IDxtJpeg_INTERFACE_DEFINED__
#define __IDxtKey_INTERFACE_DEFINED__
#include <uuids.h>
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
    // Unecessary because pUnk should not be used, but safest.
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
// DirectShowVideo - contains a simple directshow video player implementation
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

static int comRefCount = 0;

static void retainCom()
{
  if( comRefCount == 0 ) {
    //printf("com is initialized!\n");
    CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
  }
  comRefCount++;
}

static void releaseCom()
{
  comRefCount--;
  if( comRefCount == 0 ) {
    //printf("com is uninitialized!\n");
    CoUninitialize();
  }
}

class gem::plugins::filmDS::DirectShowVideo : public ISampleGrabberCB
{
public:
  IGraphBuilder *m_pGraph;                // Graph Builder interface
  IMediaControl *m_pControl;        // Media Control interface
  IMediaEvent   *m_pEvent;                // Media Event interface
  IMediaSeeking *m_pSeek;                // Media Seeking interface
  IMediaPosition * m_pPosition;
  IBasicAudio   *m_pAudio;                // Audio Settings interface
  ISampleGrabber * m_pGrabber;
  IBaseFilter * m_pSourceFile;
  IBaseFilter * m_pGrabberF;
  IBasicVideo * m_pBasicVideo;
  IBaseFilter * m_pNullRenderer;

  REFERENCE_TIME timeNow;                                // Used for FF & REW of movie, current time
  LONGLONG lPositionInSecs;                // Time in  seconds
  LONGLONG lDurationInNanoSecs;                // Duration in nanoseconds
  LONGLONG lTotalDuration;                // Total duration
  REFERENCE_TIME rtNew;                                // Reference time of movie
  long lPosition;                                        // Desired position of movie used in FF & REW
  long lvolume;                                        // The volume level in 1/100ths dB Valid values range from -10,000 (silence) to 0 (full volume), 0 = 0 dB -10000 = -100 dB
  long evCode;                                        // event variable, used to in file to complete wait.

  long width, height;
  long videoSize;

  double averageTimePerFrame;

  bool bFrameNew;
  bool bNewPixels;
  bool bVideoOpened;
  bool bPlaying;
  bool bPaused;
  bool bLoop;
  bool bEndReached;
  double movieRate;
  int curMovieFrame;
  int frameCount;

  CRITICAL_SECTION critSection;
  unsigned char * rawBuffer;

  DirectShowVideo()
  {
    retainCom();
    clearValues();
    InitializeCriticalSection(&critSection);
  }

  ~DirectShowVideo()
  {
    tearDown();
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

    if(rawBuffer) {
      delete rawBuffer;
    }
    clearValues();
  }

  void clearValues()
  {
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

    rawBuffer = NULL;

    timeNow = 0;
    lPositionInSecs = 0;
    lDurationInNanoSecs = 0;
    lTotalDuration = 0;
    rtNew = 0;
    lPosition = 0;
    lvolume = -1000;
    evCode = 0;
    width = height = 0;
    videoSize = 0;
    bVideoOpened = false;
    bLoop = true;
    bPaused = false;
    bPlaying = false;
    bEndReached = false;
    bNewPixels = false;
    bFrameNew = false;
    curMovieFrame = -1;
    frameCount = -1;

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

    BYTE * ptrBuffer = NULL;
    HRESULT hr = pSample->GetPointer(&ptrBuffer);

    if(hr == S_OK) {
      long latestBufferLength = pSample->GetActualDataLength();
      if(latestBufferLength == videoSize ) {
        EnterCriticalSection(&critSection);
        memcpy(rawBuffer, ptrBuffer, latestBufferLength);
        bNewPixels = true;

        //this is just so we know if there is a new frame
        frameCount++;

        LeaveCriticalSection(&critSection);
      } else {
        printf("ERROR: SampleCB() - buffer sizes do not match\n");
      }
    }

    return S_OK;
  }

  //This method is meant to have more overhead
  STDMETHODIMP BufferCB(double Time, BYTE *pBuffer, long BufferLen)
  {
    return E_NOTIMPL;
  }

  bool loadMovie(std::string path)
  {
    tearDown();


    // Create the Filter Graph Manager and query for interfaces.

    //printf("step 1\n");
    HRESULT hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER,
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

    hr = m_pGraph->QueryInterface(IID_IBasicAudio,(void**)&m_pAudio);
    if (FAILED(hr)) {
      tearDown();
      return false;
    }

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

    m_pGrabber->SetCallback(this, 0);
    if (FAILED(hr)) {
      tearDown();
      return false;
    }

    //MEDIA CONVERSION
    //Get video properties from the stream's mediatype and apply to the grabber (otherwise we don't get an RGB image)
    AM_MEDIA_TYPE mt;
    ZeroMemory(&mt,sizeof(AM_MEDIA_TYPE));

    mt.majortype         = MEDIATYPE_Video;
    mt.subtype           = MEDIASUBTYPE_RGB32;
    mt.formattype        = FORMAT_VideoInfo;

    //printf("step 5.5\n");
    hr = m_pGrabber->SetMediaType(&mt);
    if (FAILED(hr)) {
      tearDown();
      return false;
    }

    //printf("step 6\n");
    std::wstring filePathW = std::wstring(path.begin(), path.end());

    //this is the more manual way to do it - its a pain though because the audio won't be connected by default
    hr = m_pGraph->AddSourceFilter(filePathW.c_str(), L"Source", &m_pSourceFile);
    if (FAILED(hr)){
        printf("unable to AddSourceFilter\n");
        tearDown();
        return false;
    }

    hr = ConnectFilters(m_pGraph, m_pSourceFile, m_pGrabberF);
    if (FAILED(hr)){MARK_HR(hr);
            printf("unable to ConnectFilters(m_pGraph, m_pSourceFile, m_pGrabberF)\n");
            tearDown();
            return false;
    }
    //printf("step 7\n");
    if (SUCCEEDED(hr)) {

      //Set Params - One Shot should be false unless you want to capture just one buffer
      hr = m_pGrabber->SetOneShot(FALSE);
      if (FAILED(hr)) {
        printf("unable to set one shot\n");
        tearDown();
        return false;
      }

      hr = m_pGrabber->SetBufferSamples(TRUE);
      if (FAILED(hr)) {
        printf("unable to set buffer samples\n");
        tearDown();
        return false;
      }

      //NULL RENDERER//
      //used to give the video stream somewhere to go to.
      hr = CoCreateInstance(CLSID_NullRenderer, NULL, CLSCTX_INPROC_SERVER,
                            IID_IBaseFilter, (void**)(&m_pNullRenderer));
      if (FAILED(hr)) {
        printf("null renderer error\n");
        tearDown();
        return false;
      }

      hr = m_pGraph->AddFilter(m_pNullRenderer, L"Render");
      if (FAILED(hr)) {
        printf("unable to add null renderer\n");
        tearDown();
        return false;
      }

      hr = ConnectFilters(m_pGraph, m_pGrabberF, m_pNullRenderer);
      if (FAILED(hr)){MARK_HR(hr);
              printf("unable to ConnectFilters(m_pGraph, m_pGrabberF, m_pNullRenderer)\n");
              tearDown();
              return false;
      }

      AM_MEDIA_TYPE mt;
      ZeroMemory(&mt,sizeof(AM_MEDIA_TYPE));

      m_pGrabber->GetConnectedMediaType(&mt);
      if (FAILED(hr)) {
        printf("unable to call GetConnectedMediaType\n");
        tearDown();
        return false;
      }

      VIDEOINFOHEADER * infoheader = (VIDEOINFOHEADER*)mt.pbFormat;
      width = infoheader->bmiHeader.biWidth;
      height = infoheader->bmiHeader.biHeight;
      averageTimePerFrame = infoheader->AvgTimePerFrame / 10000000.0;

      videoSize = width * height * 4;
      //printf("video dimensions are %i %i\n", width, height);

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
        printf("Error occured while playing or pausing or opening the file\n");
        return false;
      } else {
        rawBuffer = new unsigned char[videoSize];
        //printf("success!\n");
      }
    } else {
      tearDown();
      printf("Error occured while playing or pausing or opening the file\n");
      return false;
    }

    bVideoOpened = true;
    return true;
  }

  void update()
  {
    if( bVideoOpened ) {

      long eventCode = 0;
      long ptrParam1 = 0;
      long ptrParam2 = 0;
      long timeoutMs = 2000;

      if( curMovieFrame != frameCount ) {
        bFrameNew = true;
      } else {
        bFrameNew = false;
      }
      curMovieFrame = frameCount;

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
      HRESULT hr = m_pSeek->SetPositions(&rtNew, AM_SEEKING_AbsolutePositioning,NULL,
					 AM_SEEKING_NoPositioning);
      if (FAILED(hr)) {
	printf("seek failed\n");
      }
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

  void processPixels(unsigned char * src, unsigned char * dst, int width,
                     int height, bool bRGB, bool bFlip)
  {

    int widthInBytes = width * 4;
    int numBytes = widthInBytes * height;

    if(!bRGB) {

      int x = 0;
      int y = 0;

      if(bFlip) {
        for(int y = 0; y < height; y++) {
          memcpy(dst + (y * widthInBytes), src + ( (height -y -1) * widthInBytes),
                 widthInBytes);
        }

      } else {
        memcpy(dst, src, numBytes);
      }
    } else {
      if(bFlip) {

        int x = 0;
        int y = (height - 1) * widthInBytes;
        src += y;

        for(int i = 0; i < numBytes; i+=4) {
          if(x >= width) {
            x = 0;
            src -= widthInBytes*2;
          }

          *dst = *(src+2);
          dst++;

          *dst = *(src+1);
          dst++;

          *dst = *src;
          dst++;

          *dst = *(src+3);
          dst++;

          src+=4;
          x++;
        }
      } else {
        for(int i = 0; i < numBytes; i+=4) {
          *dst = *(src+2);
          dst++;

          *dst = *(src+1);
          dst++;

          *dst = *src;
          dst++;

          *dst = *(src+3);
          dst++;

          src+=4;
        }
      }
    }
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
      HRESULT hr = m_pControl->GetState(4000, (OAFilterState*)&fs);
      if(!FAILED(S_OK)) {
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

  float getWidth()
  {
    return width;
  }

  float getHeight()
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

  double getApproximateNoFrames()
  {
    if( bVideoOpened && averageTimePerFrame > 0.0 ) {
      double d=getDurationInSeconds() / averageTimePerFrame;
      return d;
    }
    return 0;
  }
  double getFramesPerSecond(void)
  {
    return 1. / averageTimePerFrame;
  }
  void getPixels(unsigned char * dstBuffer)
  {

    if(bVideoOpened && bNewPixels) {

      EnterCriticalSection(&critSection);
      processPixels(rawBuffer, dstBuffer, width, height, true, true);
      bNewPixels = false;
      LeaveCriticalSection(&critSection);

    }
  }

  //this is the non-callback approach
  //void getPixels(unsigned char * dstBuffer){
  //
  //        if(bVideoOpened && isFrameNew()){
  //                long bufferSize = videoSize;
  //                HRESULT hr = m_pGrabber->GetCurrentBuffer(&bufferSize, (long *)rawBuffer);
  //
  //                if(hr==S_OK){
  //                        if (videoSize == bufferSize){
  //                                processPixels(rawBuffer, dstBuffer, width, height, true, true);
  //                        }else{
  //                                printf("ERROR: GetPixels() - bufferSizes do not match!\n");
  //                        }
  //                }else{
  //                        printf("ERROR: GetPixels() - Unable to get pixels for device  bufferSize = %i \n", bufferSize);
  //                }
  //        }
  //}
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

bool filmDS::open(const std::string path, const gem::Properties&props)
{
  close();
  player = new DirectShowVideo();
  bool res=player->loadMovie(path);
  if(res) {
    player->setPosition(0.0);
    player->play();
    double d;
    if(props.get("auto", d)) {
      player->setSpeed(d);
    }
  }

  return res;
}

void filmDS::close()
{
  if( player ) {
    player->stop();
    delete player;
    player = NULL;
  }
}

pixBlock*filmDS::getFrame(void)
{
  //printf("getting frame...%p\n", player);
  if(!player || !player->isLoaded())
    return NULL;

  if(player->isMovieDone())
    return NULL;

  m_image.newfilm=false;
  m_image.newimage=false;

  player->update();
  if(player->isFrameNew()) {
    //printf("getting new frame\n");
    m_image.newimage=true;
    int w=player->getWidth();
    int h=player->getHeight();
    if(w!=m_image.image.xsize || h!=m_image.image.ysize) {
      m_image.image.xsize=w;
      m_image.image.ysize=h;
      m_image.image.setCsizeByFormat(GL_RGBA);
      m_image.image.reallocate();

      m_image.newfilm=true;
      //printf("getting new film\n");
    }
    player->getPixels(m_image.image.data);
  }
  return&m_image;
}

film::errCode filmDS::changeImage(int imgNum, int trackNum)
{
  if( player && player->isLoaded() ) {
    if(imgNum<0)
      return FAILURE;

    //frame = ofClamp(frame, 0, getTotalNumFrames());
    //float frame=player->getCurrentFrameNo();
    player->setApproximateFrame(imgNum);
    return SUCCESS;
  }
  return FAILURE;
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

  writeable.set("auto", value);
  return true;
}
void filmDS::setProperties(gem::Properties&props)
{
  double d;
  if(props.get("auto", d)) {
    if(player && player->isLoaded()) {
      player->setSpeed(d);
    }
  }
}
void filmDS::getProperties(gem::Properties&props)
{
  std::vector<std::string> keys=props.keys();
  gem::any value;
  double d;
  unsigned int i=0;
  if(player && player->isLoaded()) {
    for(i=0; i<keys.size(); i++) {
      std::string key=keys[i];
      props.erase(key);
      if("fps"==key) {
	d=player->getFramesPerSecond();
	value=d; props.set(key, value);
      }
      if("frames"==key) {
	d=player->getApproximateNoFrames();
	value=(int)(d+0.5); props.set(key, value);
      }
      if("width"==key) {
	d=player->getWidth();
	value=d; props.set(key, value);
      }
      if("height"==key) {
	d=player->getHeight();
	value=d; props.set(key, value);
      }
    }
  }
}

#if 0
void filmDS::play()
{
  if( player && player->isLoaded() ) {
    player->play();
  }
}

void filmDS::stop()
{
  if( player && player->isLoaded() ) {
    player->stop();
  }
}

bool filmDS::setPixelFormat(ofPixelFormat pixelFormat)
{
  return (pixelFormat == OF_PIXELS_RGB);
}

//should implement!
float filmDS::getPosition()
{
  if( player && player->isLoaded() ) {
    return player->getPosition();
  }
  return 0.0;
}

void filmDS::setPosition(float pct)
{
  if( player && player->isLoaded() ) {
    player->setPosition(pct);
  }
}

int filmDS::getCurrentFrame()
{
  if( player && player->isLoaded() ) {
    return player->getCurrentFrameNo();
  }
  return 0;
}

int filmDS::getTotalNumFrames()
{
  if( player && player->isLoaded() ) {
    return player->getApproximateNoFrames();
  }
  return 0;
}

void filmDS::setFrame(int frame)
{
  if( player && player->isLoaded() ) {
    frame = ofClamp(frame, 0, getTotalNumFrames());
    return player->setApproximateFrame(frame);
  }
}

void filmDS::firstFrame()
{
  setPosition(0.0);
}
#endif
