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
#include "Gem/RTE.h"

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
// DirectShow includes and helper methods
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

#define MARK_HR(hr)if(hr)printf("%s:%d (%s)\t: 0x%X\n", __FILE__, __LINE__, __FUNCTION__, hr);else printf("%s:%d (%s)\n", __FILE__, __LINE__, __FUNCTION__)
#define MARK() MARK_HR(0)

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


HRESULT SaveGraphFile(IGraphBuilder *pGraph, const WCHAR*wszPath)
{
  const WCHAR wszStreamName[] = L"ActiveMovieGraph";
  HRESULT hr=0;
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
HRESULT SaveGraphFile(IGraphBuilder *pGraph, std::string path)
{
  std::wstring filePathW = std::wstring(path.begin(), path.end());
  const WCHAR*wszPath=filePathW.c_str();
  return SaveGraphFile(pGraph, wszPath);
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
  IGraphBuilder *m_pGraph;          // Graph Builder interface
  IMediaControl *m_pControl;        // Media Control interface
  IMediaSeeking *m_pSeek;           // Media Seeking interface
  IMediaPosition*m_pPosition;
  ISampleGrabber*m_pGrabber;
  IBaseFilter   *m_pSourceFile;
  IBaseFilter   *m_pGrabberF;
  IBasicVideo   *m_pBasicVideo;
  IBaseFilter   *m_pNullRenderer;

  REFERENCE_TIME rtNew;                    // Reference time of movie

  long width, height;
  long videoSize;

  double averageTimePerFrame;

  bool bVideoOpened;
#if 0
  bool bNewPixels;
#endif
  LONGLONG m_Duration;
  long m_numFrames;

  LONGLONG m_lastFrame;
  LONGLONG m_wantFrame;

  CRITICAL_SECTION critSection;
  unsigned char * rawBuffer;

  double m_auto;

  DirectShowVideo()
    : m_pGraph(NULL)
    , m_pControl(NULL)
    , m_pSeek(NULL)
    , m_pPosition(NULL)
    , m_pGrabber(NULL)
    , m_pSourceFile(NULL)
    , m_pGrabberF(NULL)
    , m_pBasicVideo(NULL)
    , m_pNullRenderer(NULL)
    , rtNew(0)
    , width(0)
    , height(0)
    , videoSize(0)
    , averageTimePerFrame(1.0/30.0)
#if 0
    , bNewPixels(false)
    , bVideoOpened(false)
#endif
    , m_lastFrame(-1)
    , m_wantFrame(0)
    , rawBuffer(NULL)
    , m_auto(0.)
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
    if(m_pSeek) {
      m_pSeek->Release();
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
    m_pSeek = NULL;
    m_pGrabber = NULL;
    m_pGrabberF = NULL;
    m_pBasicVideo = NULL;
    m_pNullRenderer = NULL;
    m_pSourceFile = NULL;
    m_pPosition = NULL;
    m_lastFrame=-1;
    m_wantFrame=0;

    rawBuffer = NULL;

    rtNew = 0;
    width = height = 0;
    videoSize = 0;

    bVideoOpened = false;
    //    bNewPixels = false;

    averageTimePerFrame = 1.0/30.0;
    m_auto = 0.;

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
    return E_NOTIMPL;
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
    HRESULT hr = CoCreateInstance(CLSID_FilterGraph, NULL,
                                  CLSCTX_INPROC_SERVER,
                                  IID_IGraphBuilder, (void **)&m_pGraph);
    if (FAILED(hr) || !m_pGraph) {
      verbose(1, "[GEM::filmDS] unable to create Filter Graph Manager: %d", hr);
      tearDown();
      return false;
    }

    // Use IGraphBuilder::QueryInterface (inherited from IUnknown) to get the IMediaControl interface.
    //printf("step 4\n");
    hr = m_pGraph->QueryInterface(IID_IMediaControl, (void **)&m_pControl);
    if (FAILED(hr) || !m_pControl) {
      verbose(1, "[GEM::filmDS] could not get MediaControl interface: %d", hr);
      tearDown();
      return false;
    }

    //printf("step 2\n");
    hr = m_pGraph->QueryInterface(IID_IMediaSeeking, (void**)&m_pSeek);
    if (FAILED(hr) || !m_pSeek) {
      verbose(1, "[GEM::filmDS] could not get MediaSeeking interface: %d", hr);
      tearDown();
      return false;
    }

    hr = m_pGraph->QueryInterface(IID_IMediaPosition, (LPVOID *)&m_pPosition);
    if (FAILED(hr) || !m_pPosition) {
      verbose(1, "[GEM::filmDS] could not get MediaPosition interface: %d", hr);
      tearDown();
      return false;
    }

    //SAMPLE GRABBER (ALLOWS US TO GRAB THE BUFFER)//
    // Create the Sample Grabber.
    hr = CoCreateInstance(CLSID_SampleGrabber, NULL, CLSCTX_INPROC_SERVER,
                          IID_IBaseFilter, (void**)&m_pGrabberF);
    if (FAILED(hr)) {
      verbose(1, "[GEM::filmDS] could not create SampleGrabber: %d", hr);
      tearDown();
      return false;
    }

    hr = m_pGraph->AddFilter(m_pGrabberF, L"Sample Grabber");
    if (FAILED(hr)) {
      verbose(1, "[GEM::filmDS] could not add SampleGrabber filter: %d", hr);
      tearDown();
      return false;
    }

    hr = m_pGrabberF->QueryInterface(IID_ISampleGrabber, (void**)&m_pGrabber);
    if (FAILED(hr)) {
      verbose(1, "[GEM::filmDS] could not get SampleGrabber interface: %d", hr);
      tearDown();
      return false;
    }

    //MEDIA CONVERSION
    //Get video properties from the stream's mediatype and apply to the grabber (otherwise we don't get an RGBA image)
    AM_MEDIA_TYPE mt;
    ZeroMemory(&mt,sizeof(AM_MEDIA_TYPE));

    mt.majortype         = MEDIATYPE_Video;
    mt.subtype           = MEDIASUBTYPE_RGB32;
    mt.formattype        = GUID_NULL;//FORMAT_VideoInfo;

    //printf("step 5.5\n");
    hr = m_pGrabber->SetMediaType(&mt);
    if (FAILED(hr)) {
      verbose(1, "[GEM::filmDS] could not set MediaType: %d", hr);
      tearDown();
      return false;
    }

    //printf("step 6\n");
    std::wstring filePathW = std::wstring(path.begin(), path.end());

    //this is the more manual way to do it - its a pain though because the audio won't be connected by default
    hr = m_pGraph->AddSourceFilter(filePathW.c_str(), L"Source",
                                   &m_pSourceFile);
    if (FAILED(hr)) {
      verbose(1, "[GEM::filmDS] could not add Source Filter: %d", hr);
      tearDown();
      return false;
    }

    hr = ConnectFilters(m_pGraph, m_pSourceFile, m_pGrabberF);
    if (FAILED(hr)) {
MARK_HR(hr);
      verbose(1,
              "[GEM::filmDS] unable to ConnectFilters(%p, %p, %p)", m_pGraph, m_pSourceFile, m_pGrabberF);
      tearDown();
      return false;
    }
    //printf("step 7\n");
    if(!SUCCEEDED(hr)) { // can this ever happen after we just checked for FAILED(hr)??
      verbose(1, "[GEM::filmDS] Error occured while playing or pausing or opening the file");
      tearDown();
      return false;
    }

    //Set Params - One Shot should be false unless you want to capture just one buffer
    hr = m_pGrabber->SetOneShot(FALSE);
    if (FAILED(hr)) {
      verbose(1, "[GEM::filmDS] unable to set one shot");
      tearDown();
      return false;
    }

    hr = m_pGrabber->SetBufferSamples(TRUE);
    if (FAILED(hr)) {
      verbose(1, "[GEM::filmDS] unable to set buffer samples");
      tearDown();
      return false;
    }

    //NULL RENDERER//
    //used to give the video stream somewhere to go to.
    hr = CoCreateInstance(CLSID_NullRenderer, NULL, CLSCTX_INPROC_SERVER,
                          IID_IBaseFilter, (void**)(&m_pNullRenderer));
    if (FAILED(hr) || !m_pNullRenderer) {
      verbose(1, "[GEM::filmDS] null renderer error");
      tearDown();
      return false;
    }

    hr = m_pGraph->AddFilter(m_pNullRenderer, L"Render");
    if (FAILED(hr)) {
      verbose(1, "[GEM::filmDS] unable to add null renderer");
      tearDown();
      return false;
    }

    hr = ConnectFilters(m_pGraph, m_pGrabberF, m_pNullRenderer);
    if (FAILED(hr)) {
MARK_HR(hr);
      verbose(1, "[GEM::filmDS] unable to ConnectFilters(%s, %s, %s)", m_pGraph, m_pGrabberF, m_pNullRenderer);
      tearDown();
      return false;
    }

    // Set the time format to frames
    GUID Guid = TIME_FORMAT_FRAME;
    bool bFrameTime = true;
    hr = m_pSeek->SetTimeFormat(&Guid);

    if (FAILED(hr)) {
      // If frame time format not available, default to 100 nanosecond increments.
      bFrameTime = false;
      Guid = TIME_FORMAT_MEDIA_TIME;

      hr = m_pSeek->SetTimeFormat(&Guid);

      if (FAILED(hr)) {
        verbose(1, "[GEM::filmDS] Unable to set video time format %d", hr);
        tearDown();
        return false;
      }
    }

    // Get the duration of the video. Format will be in previously set time format. This is
    // compatible with the value returned from GetCurrentPosition
    hr = m_pSeek->GetDuration(&m_Duration);
    if (FAILED(hr)) {
      verbose(1, "[GEM::filmDS] Unable to get video duration %d", hr);
      tearDown();
      return false;
    }
    // Set the number of frames based on the time format used.
    if (bFrameTime) {
      m_numFrames = m_Duration;
    } else {
      LONGLONG OutFormat;
      GUID     OutGuid = TIME_FORMAT_FRAME;
      Guid = TIME_FORMAT_MEDIA_TIME;
      //converts from 100 nanosecond format to number of frames
      m_pSeek->ConvertTimeFormat(&OutFormat, &OutGuid, m_Duration, &Guid);

      m_numFrames = OutFormat;
    }

    ZeroMemory(&mt,sizeof(AM_MEDIA_TYPE));
    m_pGrabber->GetConnectedMediaType(&mt);
    if (FAILED(hr)) {
      verbose(1, "[GEM::filmDS] unable to call GetConnectedMediaType %d", hr);
      tearDown();
      return false;
    }

    if(FORMAT_VideoInfo != mt.formattype || !mt.pbFormat) {
      verbose(1, "[GEM::filmDS] invalid media type");
      tearDown();
      return false;
    }

    VIDEOINFOHEADER * infoheader = (VIDEOINFOHEADER*)mt.pbFormat;
    width = infoheader->bmiHeader.biWidth;
    height = infoheader->bmiHeader.biHeight;
    averageTimePerFrame = infoheader->AvgTimePerFrame / 10000000.0;

    //verbose(1, "[GEM::filmDS] video dimensions are %dx%d", width, height);
    if( width == 0 || height == 0 ) {
      verbose(1, "[GEM::filmDS] illegal frame size %dx%d", width, height);
      tearDown();
      return false;
    }

    videoSize = width * height * 4;
    rawBuffer = new unsigned char[videoSize];
    if(!rawBuffer) {
      verbose(1, "[GEM::filmDS] unable to allocate memory for video buffer");
      tearDown();
      return false;
    }
    //FreeMediaType(mt); // FIXXME

    // FIXXME: original code now does some special tricks for DV video

    //printf("step 8\n");
    // Run the graph.

    std::string grfile="test_" __TIME__ "_";
    //SaveGraphFile(m_pGraph, grfile+"2.grf");
    hr = m_pControl->Run();
    //SaveGraphFile(m_pGraph, grfile+"3.grf");
    if( FAILED(hr) ) {
      verbose(1, "[GEM::filmDS] unable to start film", hr);
      tearDown();
      return false;
    }

    // Wait for the video to begin playing.
    while(true) {
      OAFilterState FilterState;

      // Get the state and ensure it's not in an intermediate state
      hr = m_pControl->GetState(0, &FilterState);
      if (FAILED(hr) && hr != VFW_S_STATE_INTERMEDIATE)  {
        verbose(1, "[GEM::filmDS] Unable to run film %d", hr);
        tearDown();
        return false;
      }

      // Ensure the video is running
      if (SUCCEEDED(hr) && State_Running == FilterState) {
        break;
      }
    }

    // FIXXME: shall we register the filtergraph?

    bVideoOpened = true;
    m_lastFrame=-1;
    m_wantFrame=0;
    return true;
  }

  void processPixels(unsigned char * src, unsigned char * dst, int width,
                     int height, bool bRGB, bool bFlip)
  {
MARK();
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

  double getFramesPerSecond(void)
  {
MARK();
    return 1. / averageTimePerFrame;
  }

  //this is the non-callback approach
  bool getPixels(unsigned char * dstBuffer)
  {
MARK();
    if(!bVideoOpened) {
      return false;
    }

    long bufferSize = videoSize;
    HRESULT hr = m_pGrabber->GetCurrentBuffer(&bufferSize, (long *)rawBuffer);

    if(FAILED(hr)) {
      verbose(1, "ERROR: GetPixels() - Unable to get pixels for device  bufferSize = %i",
             bufferSize);
      return false;
    }

    if (videoSize != bufferSize) {
      verbose(1, "ERROR: GetPixels() - bufferSizes do not match!");
      return false;
    }

    processPixels(rawBuffer, dstBuffer, width, height, true, true);
    return true;
  }
  bool getFrame(pixBlock&pb)
  {
MARK();
    if(!bVideoOpened) {
      return false;
    }

    long frameSize = width*height*4;
    HRESULT hr;
    OAFilterState State;
MARK();
    hr=m_pControl->GetState(0, &State);
    if(m_auto > 0.f) {
MARK();
      //if the video is paused then start it running again
      if (State != State_Running) {
        hr = m_pControl->Run();
        hr = m_pControl->GetState(0, &State);
      }

      //set the rate of the clip
      hr = m_pSeek->SetRate(m_auto);

      if (State == State_Running) {
        // Get the current position of the video
        LONGLONG CurrentPosition;
        hr = m_pSeek->GetCurrentPosition(&CurrentPosition);
        if (S_OK == hr)            {
          // If the current position is >= the duration, reset the position to the
          // beginning
          if (CurrentPosition >= m_numFrames)                {
            LONGLONG Current = 0;
            // Set the start position to 0, do not change the end position.
            hr = m_pSeek->SetPositions(&Current,
                                       AM_SEEKING_AbsolutePositioning | AM_SEEKING_NoFlush,
                                       NULL, AM_SEEKING_NoPositioning);
            pb.newimage = true;
          }
          // Indicate the the image has changed.
          else if (CurrentPosition > m_lastFrame)               {
            pb.newimage = true;
          }
        }

        // If the video image has changed, copy it to the pixBlock buffer.
        if (pb.newimage) {
          hr = m_pGrabber->GetCurrentBuffer(&frameSize, (long *)rawBuffer);
          if(FAILED(hr)) {
MARK_HR(hr);
            verbose(1, "[GEM::filmDS] GetCurrentBuffer(auto) failed: state=%d\n", State);
            pb.image.data = NULL; // FIXXME
MARK();
            return false;
          } else {
            //pb.image.data = rawBuffer;
            processPixels(rawBuffer, pb.image.data, width, height, true, true);
          }
        }
      }
    } else {
MARK();
      LONGLONG frameSeek;
      frameSeek = (LONGLONG) m_wantFrame;
      if (State == State_Running) {
        hr = m_pControl->Pause();
      }

      //check if the playback is 'Paused' and don't keep asking for the same frame
      hr = m_pSeek->GetCurrentPosition(&frameSeek);
      verbose(1, "[GEM::filmDS] current=%d\tlast=%d\twant=%d\n", (int)frameSeek, (int)m_lastFrame,
             (int)m_wantFrame);

      if (m_wantFrame == m_lastFrame) {
        pb.newimage = false;
        return &pb;
      }

      hr = m_pSeek->SetPositions(&frameSeek,
                                 AM_SEEKING_AbsolutePositioning,
                                 NULL, AM_SEEKING_NoPositioning);
      verbose(2, "[GEM::filmDS] new=%d", (int)frameSeek);

      if (FAILED(hr)) {
MARK_HR(hr);
        verbose(1, "[GEM::filmDS] SetPositions failed");
      }

MARK();
      hr = m_pGrabber->GetCurrentBuffer(&frameSize, (long *)rawBuffer);

      if (FAILED(hr))  {
MARK_HR(hr);
        pb.image.data = NULL; // FIXXME
        verbose(1, "[GEM::filmDS] GetCurrentBuffer failed: state=%d\n", State);
        return false;
      } else {
        //pb.image.data = rawBuffer;
        processPixels(rawBuffer, pb.image.data, width, height, true, true);

        pb.newimage = true;
        //pb.image.fromBGR(m_frame);
        //m_lastFrame = m_wantFrame;
        m_lastFrame = frameSeek;
      }
    }
    return true;
  }

  void setSpeed(double speed)
  {
    m_auto=speed;
  }
  bool seekFrame(int frame)
  {
    if(frame>m_numFrames) {
      return false;
    }
    m_wantFrame=frame;
    return true;
  }
  bool isLoaded(void)
  {
    return bVideoOpened;
  }
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
MARK();
  close();
MARK();
  player = new DirectShowVideo();
MARK();
  bool res=player->loadMovie(path);
MARK_HR(res);
  if(res) {
    player->seekFrame(0);
    double d=0.;
    if(props.get("auto", d)) {
      player->setSpeed(d);
    }
  } else {
    close();
  }

  return res;
}

void filmDS::close()
{
MARK();
  if( player ) {
MARK();
    delete player;
    player = NULL;
  }
}

pixBlock*filmDS::getFrame(void)
{
MARK();
  if(!player) {
    return 0;
  }
MARK();
#if 0
  //printf("getting frame...%p\n", player);
  if(!player || !player->isLoaded()) {
    return NULL;
  }

  if(player->isMovieDone()) {
    return NULL;
  }
#endif
  m_image.newfilm=false;
  m_image.newimage=false;

  int w=player->width;
  int h=player->height;
  if(w!=m_image.image.xsize || h!=m_image.image.ysize) {
    m_image.image.xsize=w;
    m_image.image.ysize=h;
    m_image.image.setCsizeByFormat(GL_RGBA);
    m_image.image.reallocate();

    m_image.newfilm=true;
    //printf("getting new film\n");
  }
MARK();
  bool res=player->getFrame(m_image);
MARK_HR(res);
  if(res) {
    return &m_image;
  }
  return 0;
}

film::errCode filmDS::changeImage(int imgNum, int trackNum)
{
MARK();
  if(!player) {
    return film::FAILURE;
  }
MARK();
  if(player->seekFrame(imgNum)) {
    return film::SUCCESS;
  }
MARK();
  return film::FAILURE;
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
    if(player) {
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
        value=d;
        props.set(key, value);
      }
      if("frames"==key) {
        d=player->m_numFrames;
        value=(int)(d+0.5);
        props.set(key, value);
      }
      if("width"==key) {
        d=player->width;
        value=d;
        props.set(key, value);
      }
      if("height"==key) {
        d=player->height;
        value=d;
        props.set(key, value);
      }
    }
  }
}
