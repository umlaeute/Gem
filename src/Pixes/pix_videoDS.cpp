////////////////////////////////////////////////////////
//
// pix_videoDS - Graphics Environment for Multimedia
//
// daniel@bogusfront.org
//
// Implementation file 
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    Copyright (c) 2003 Daniel Heckenberg.
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
#include "Base/config.h"

#define DO_AUTO_REGISTER_CLASS
// JMZ: disabled the helpsymbol for now, 
// since the help for pix_videoDS ha to be QUITE different...
/*
#define HELPSYMBOL "pix_video"
*/

#if defined(_WINDOWS) && defined(HAVE_DIRECTSHOW)

#include "pix_videoDS.h"
#include <memory>
#include <dshow.h>
#include <qedit.h>
#include <Dvdmedia.h>
#include <streams.h>
#include "DSgrabber.h"

#define COMRELEASE(x) { if (x) x->Release(); x = NULL; }
#define REGISTER_FILTERGRAPH 1

// Utility functions
void SetupCaptureDevice(ICaptureGraphBuilder2* pCG, IBaseFilter * pCDbase);
HRESULT FindCaptureDevice(int device, IBaseFilter ** ppSrcFilter);
HRESULT ConnectFilters(IGraphBuilder *pGraph, IBaseFilter *pFirst, IBaseFilter *pSecond);
void GetBitmapInfoHdr(AM_MEDIA_TYPE* pmt, BITMAPINFOHEADER** ppbmih);
HRESULT GetPin(IBaseFilter *, PIN_DIRECTION, IPin **);
HRESULT AddGraphToRot(IUnknown *pUnkGraph, DWORD *pdwRegister);
void RemoveGraphFromRot(DWORD pdwRegister);

///////////////////////////////////////////////////////////
//
// Callback for SampleGrabber filter
//
/////////////////////////////////////////////////////////
static HRESULT Callback(void* pUser, IMediaSample* pSample, REFERENCE_TIME* StartTime,
			REFERENCE_TIME* StopTime, BOOL TypeChanged)
{
  ((pix_videoDS*)pUser)->copyBuffer(pSample);

  return NOERROR; /* jmz: why should we stop delivering ? */
  // Tell the source to stop delivering samples.
  return S_FALSE; 
}

CPPEXTERN_NEW_WITH_ONE_ARG(pix_videoDS, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// pix_videoDS
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_videoDS :: pix_videoDS(t_floatarg num)
  : m_haveVideo(0),
    m_colorSwap(0),
    m_hWndC(NULL),
    m_newFrame(0),
    m_RawBuffer(NULL),
    m_nRawBuffSize(0),
    m_pGB(NULL),
    m_pMC(NULL),
    m_pME(NULL),
    m_pMF(NULL),
    m_pVW(NULL),
    m_pBA(NULL),
    m_pBV(NULL),
    m_pMS(NULL),
    m_pMP(NULL),
    m_pSG(NULL),
    m_pFG(NULL),
    m_pCDbase(NULL),
    m_pCG(NULL),
    m_pVC(NULL),
    m_GraphRegister(0),
    m_rendering(0),
    m_capturing(0),
    m_captureOnOff(1),
    m_bInitCOM(false)
{
  // Initialize COM
  if(FAILED(CoInitialize(NULL)))
    {
      error("pix_video: could not initialise COM.");
      return;
    }

  m_bInitCOM = true;

  // Initialize the input buffers
  for (int i = 0; i <= 2; i++)
    {
      m_pixBlockBuf[i].image.data = NULL;
      m_pixBlockBuf[i].newimage = 0;
      m_nPixDataSize[i] = 0;
    }

  m_pixBlock.image.setCsizeByFormat(GL_RGBA);

  openMess(num);
}

////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_videoDS :: ~pix_videoDS()
{
  stopTransfer();  

  // Clean up the movie
  closeMess();
  cleanPixBlock();

  // Finished with COM
  if (m_bInitCOM)
    {
      CoUninitialize();
      m_bInitCOM = false;
    }
}

/////////////////////////////////////////////////////////
// open message
//
/////////////////////////////////////////////////////////
void pix_videoDS :: openMess(int device)
{
  HRESULT hr;
  IBaseFilter		*pSGbase = NULL;
  IBaseFilter		*pNRbase = NULL;

  if (!m_bInitCOM)
    {
      error("pix_videoDS: COM has not been initialized.");
      return;
    }

  if (m_haveVideo) closeMess();

  do  {
    // Get the interface for DirectShow's GraphBuilder
    if (FAILED(hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void **)&m_pGB)))  {
      error("pix_videoDS: Could not get DShow GraphBuilder, hr 0x%X", hr);
      break;
    }

    // Get the interface for DirectShow's CaptureGraphBuilder2
    if (	FAILED(hr = (CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL, CLSCTX_INPROC_SERVER, IID_ICaptureGraphBuilder2, (void **)&m_pCG)))
		||	FAILED(hr = m_pCG->SetFiltergraph(m_pGB))){
      error("pix_videoDS: Could not get DShow GraphBuilder, hr 0x%X", hr);
      break;
    }

    // Create the capture device.
    if (FAILED(hr = FindCaptureDevice(device, &m_pCDbase))){
      error("pix_videoDS: Could not open device: %d\n", device);
      break;
    }
		
    // Create an instance of our local sample grabber object.
    if (!(m_pSG = new CSampleGrabber(NULL, &hr, FALSE))) {
      error("pix_videoDS: Could not create a Sample Grabber filter, hr 0x%X", hr);
      break;
    }
    m_pSG->AddRef();
		
    // Set the grabber's media type
    CMediaType mtSG;
    mtSG.SetType(&MEDIATYPE_Video);
    mtSG.SetSubtype(&MEDIASUBTYPE_RGB24);
    if (FAILED(hr = m_pSG->SetAcceptedMediaType(&mtSG))){
      error("pix_videoDS: Could not set the MediaType of the Sample Grabber, hr 0x%X", hr);
      break;
    }

    // Set the Sample Grabber callback.
    if (FAILED(hr = m_pSG->SetCallback(&Callback, this))){	
      error("pix_videoDS: Could not set the callback of the Sample Grabber, hr 0x%X", hr);
      break;
    }

    // Create the Null Renderer.
    if (FAILED(hr = CoCreateInstance(CLSID_NullRenderer, NULL, CLSCTX_INPROC_SERVER,IID_IBaseFilter, reinterpret_cast<void**>(&pNRbase)))) {
      error("pix_videoDS: Could not create the Null Renderer, hr 0x%X", hr);
      break;
    }

    // get the base interface so we can connect the graph
    if (FAILED(hr = m_pSG->QueryInterface(IID_IBaseFilter, reinterpret_cast<void**>(&pSGbase)))){
      error("pix_videoDS: Could not get the Sample Grabber's base interface, hr 0x%X", hr);
      break;
    }

    // add the filters to the graph
    if (	FAILED(hr = m_pGB->AddFilter(m_pCDbase, L"CaptureDevice"))
		||	FAILED(hr = m_pGB->AddFilter(pNRbase, L"NullRenderer"))
		||	FAILED(hr = m_pGB->AddFilter(pSGbase, L"GrabberSample"))){
      error("pix_videoDS: Could not add the filters to the graph, hr 0x%X", hr);
      break;
    }

    // Connect the modules
    if (	FAILED(hr = ConnectFilters(m_pGB, m_pCDbase, pSGbase))
		||	FAILED(hr = ConnectFilters(m_pGB, pSGbase, pNRbase)))	{
      error("pix_videoDS: Could not connect the filters in the graph, hr 0x%X", hr);
      break;
    }

    // QueryInterface for DirectShow interfaces
    if (FAILED(hr = (m_pGB->QueryInterface(IID_IMediaFilter, (void **)&m_pMF))))     {
      error("pix_videoDS: Could not get media control interface, hr 0x%X", hr);
      break;
    }
    if (FAILED(hr = (m_pGB->QueryInterface(IID_IMediaControl, (void **)&m_pMC)))){
      error("pix_videoDS: Could not get media control interface, hr 0x%X", hr);
      break;
    }
    if (FAILED(hr = (m_pGB->QueryInterface(IID_IMediaEvent, (void **)&m_pME))))	{
      error("pix_videoDS: Could not get media event interface, hr 0x%X", hr);
      break;
    }
    if (FAILED(hr = (m_pGB->QueryInterface(IID_IMediaSeeking, (void **)&m_pMS)))){
      error("pix_videoDS: Could not get media seeking interface, hr 0x%X", hr);
      break;
    }
    if (FAILED(hr = (m_pGB->QueryInterface(IID_IMediaPosition, (void **)&m_pMP)))){
      error("pix_videoDS: Could not get media position interface, hr 0x%X", hr);
      break;
    }

    // Expose the filter graph so we can view it using GraphEdit
#ifdef REGISTER_FILTERGRAPH
    if (FAILED(hr = AddGraphToRot(m_pGB, &m_GraphRegister))){
      error("pix_videoDS: failed to register filter graph with ROT!  hr=0x%X", hr);
      m_GraphRegister = 0;
    }
#endif
    // Turn off the reference clock.
    if (FAILED(hr = m_pMF->SetSyncSource(NULL))){
      error("pix_videoDS: failed to turn off the reference clock  hr=0x%X", hr);
      break;
    }

    m_haveVideo = 1;
    startTransfer();

  } while (0);

  COMRELEASE(pSGbase);
  COMRELEASE(pNRbase);

  if (!m_haveVideo)closeMess();

  post("opened device %d", device);
}

////////////////////////////////////////////////////////
// close message
//
/////////////////////////////////////////////////////////
void pix_videoDS :: closeMess()
{
  m_haveVideo = 0;

  COMRELEASE(m_pGB);
  COMRELEASE(m_pMC);
  COMRELEASE(m_pME);
  COMRELEASE(m_pMF);
  COMRELEASE(m_pVW);
  COMRELEASE(m_pBA);
  COMRELEASE(m_pBV);
  COMRELEASE(m_pMS);
  COMRELEASE(m_pMP);
  COMRELEASE(m_pSG);
  COMRELEASE(m_pFG);
  COMRELEASE(m_pCDbase);
  COMRELEASE(m_pCG);
  COMRELEASE(m_pVC);

#ifdef REGISTER_FILTERGRAPH
  if (m_GraphRegister)
    {	
      HRESULT hr;

      RemoveGraphFromRot(m_GraphRegister);
      m_GraphRegister = 0;
    }
#endif
}

////////////////////////////////////////////////////////
// enumerate message
//
/////////////////////////////////////////////////////////
void pix_videoDS :: enumerateMess()
{
  HRESULT hr;
  IBaseFilter * pSrc = NULL;

  IMoniker* pMoniker =NULL;
  ULONG cFetched;

  ICreateDevEnum* pDevEnum =NULL;
  IEnumMoniker* pClassEnum = NULL;

  do 
    {
      // Create the system device enumerator
      hr = CoCreateInstance (CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC,
			     IID_ICreateDevEnum, (void ** ) &pDevEnum);
      if (FAILED(hr))
	{
	  error("pix_videoDS: Couldn't create system enumerator!");
	  break;
	}

      // Create an enumerator for the video capture devices
      hr = pDevEnum->CreateClassEnumerator (CLSID_VideoInputDeviceCategory, &pClassEnum, 0);
      if (FAILED(hr))
	{
	  error("pix_videoDS: Couldn't create class enumerator!");
	  break;
	}

      // If there are no enumerators for the requested type, then 
      // CreateClassEnumerator will succeed, but pClassEnum will be NULL.
      if (pClassEnum == NULL)
	{
	  error("pix_videoDS: No video capture devices found!");
	  break;
	}

      // Use the first video capture device on the device list.
      // Note that if the Next() call succeeds but there are no monikers,
      // it will return S_FALSE (which is not a failure).  Therefore, we
      // check that the return code is S_OK instead of using SUCCEEDED() macro.
      int devIndex = 0;
      while (S_OK == (pClassEnum->Next (1, &pMoniker, &cFetched)))
	{
	  IPropertyBag *pPropBag;
	  if (SUCCEEDED(hr = pMoniker->BindToStorage(0, 0, IID_IPropertyBag, (void **)&pPropBag)))
	    {
	      // To retrieve the friendly name of the filter, do the following:
	      VARIANT varName;
	      VariantInit(&varName);
	      hr = pPropBag->Read(L"FriendlyName", &varName, 0);
	      if (SUCCEEDED(hr))
		{
		  post("Dev %d: %S", devIndex, varName.bstrVal);
		}
	      VariantClear(&varName);

	      COMRELEASE(pPropBag);
	    }
	  COMRELEASE(pMoniker);
	  devIndex++;
	}
    } while (0);

  // Copy the found filter pointer to the output parameter.
  // Do NOT Release() the reference, since it will still be used
  // by the calling function.
  COMRELEASE(pDevEnum);
  COMRELEASE(pClassEnum);
}

////////////////////////////////////////////////////////
// setup message
//
/////////////////////////////////////////////////////////
void pix_videoDS :: setupMess()
{
  if (!m_haveVideo)
    {
      error("pix_videoDS: No open video device.");
      return;
    }
	
  SetupCaptureDevice(m_pCG, m_pCDbase);
}
////////////////////////////////////////////////////////
// colorspace message
//
/////////////////////////////////////////////////////////
void pix_videoDS :: csMess(int format)
{
  if(format)m_pixBlock.image.setCsizeByFormat(format);
}
////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void pix_videoDS :: render(GemState *state)
{
  if (!m_haveVideo || !m_capturing)  {
    //      error("pix_videoDS: Video not active");
    state->image = NULL;	
    return;
  }

  m_readIdx = m_lastwriteIdx;
  if (m_nPixDataSize[m_readIdx]){
    m_pixBlock.newimage=m_pixBlockBuf[m_readIdx].newimage;
    m_pixBlock.image.xsize=m_pixBlockBuf[m_readIdx].image.xsize;
    m_pixBlock.image.ysize=m_pixBlockBuf[m_readIdx].image.ysize;
    switch (m_pixBlockBuf[m_readIdx].image.format){
    case GL_BGR_EXT:
    default:
      m_pixBlock.image.fromBGR(m_pixBlockBuf[m_readIdx].image.data);
    }
    
    state->image = &m_pixBlock;
  }
  else 
    state->image = NULL;
}

////////////////////////////////////////////////////////
// postrender
//
/////////////////////////////////////////////////////////
void pix_videoDS :: postrender(GemState *state)
{
  if (!m_haveVideo || !m_capturing)return;

  m_pixBlockBuf[m_readIdx].newimage = 0;
  state->image = NULL;
  m_lastreadIdx = m_readIdx;
}


////////////////////////////////////////////////////////
// startRendering
//
/////////////////////////////////////////////////////////
void pix_videoDS :: startRendering()
{
  m_rendering = 1;
  startTransfer();
}

////////////////////////////////////////////////////////
// stopRendering
//
/////////////////////////////////////////////////////////
void pix_videoDS :: stopRendering()
{
  m_rendering = 0;
  stopTransfer();
}


////////////////////////////////////////////////////////
// startTransfer
//
/////////////////////////////////////////////////////////
int pix_videoDS :: startTransfer()
{
  m_capturing = 0;

  if (m_haveVideo && m_captureOnOff)   {
      HRESULT hr;

      m_readIdx = 0;
      m_lastreadIdx = 0;
      m_writeIdx = 1;
      m_lastwriteIdx = 0;

      // Get the stream characteristics
      CMediaType mt;
      BITMAPINFOHEADER* pbmih;
      if (FAILED(hr = m_pSG->GetConnectedMediaType(&mt)))
	{
	  error("pix_videoDS: Could not get connect media type, hr 0x%X", hr);
	  return 0;
	}
      GetBitmapInfoHdr(&mt, &pbmih);
      m_xsize = pbmih->biWidth;
      m_ysize = pbmih->biHeight;
      m_csize = 3;
      m_format = GL_BGR_EXT;

      if (FAILED(hr = m_pMC->Run()))
	{
	  error("pix_videoDS: Could not start graph playback, hr 0x%X", hr);
	} else
	m_capturing = 1;
    }

  return m_capturing;
}

/////////////////////////////////////////////////////////
// stopTransfer
//
/////////////////////////////////////////////////////////
int pix_videoDS :: stopTransfer()
{
  if (m_haveVideo)
    {
      HRESULT hr;
      if (FAILED(hr = m_pMC->Stop()))
	{
	  error("pix_videoDS: Could not stop graph playback, hr 0x%X", hr);
	} else
	m_capturing = 0;
    }

  return !(!m_capturing);
}


/////////////////////////////////////////////////////////
    // captureOnOff
    //
    /////////////////////////////////////////////////////////
    void pix_videoDS :: captureOnOff(int n)
{
  m_captureOnOff = n;

  if (n)
    startTransfer();
  else
    stopTransfer();
}


////////////////////////////////////////////////////////
// dvMess
//
/////////////////////////////////////////////////////////
void pix_videoDS :: dvMess(int argc, t_atom *argv)
{
  HRESULT hr = S_OK;

  // Set DV interface to full resolution
  if (argc < 1)
    {
      error("dv: Missing required argument");
      return;
    }

  if (!m_haveVideo)
    {
      error("No open video device.");
      return;
    }

  stopTransfer();

  const char* szCmd = atom_getsymbolarg(0, argc, argv)->s_name;

  if (!strcmp(szCmd,"res"))
    {
      if (argc >= 2)
	{
	  const char* szRes = atom_getsymbolarg(1, argc, argv)->s_name;

	  IIPDVDec* pDV;
	  if (SUCCEEDED(hr = (m_pCG->FindInterface(NULL, NULL, m_pCDbase, IID_IIPDVDec, (void **)&pDV))))
	    {
	      if (!strcmp(szRes, "full"))
		{
		  hr = pDV->put_IPDisplay(DVRESOLUTION_FULL);
		} else if (!strcmp(szRes, "half"))
		{
		  hr = pDV->put_IPDisplay(DVRESOLUTION_HALF);
		} else if (!strcmp(szRes, "quarter"))
		{
		  hr = pDV->put_IPDisplay(DVRESOLUTION_QUARTER);
		} else if (!strcmp(szRes, "dc"))
		{
		  hr = pDV->put_IPDisplay(DVRESOLUTION_DC);
		} else
		{
		  error("pix_videoDS: unknown DV decoder setting");
		}
	      if (FAILED(hr))
		{
		  error("pix_videoDS: Could not set decoder resolution.");
		}
	      pDV->Release();
	    } else
	    {
	      error("pix_videoDS: Device does not have DV decoder interface");
	    }
	} else
	{
	  error("pix_videoDS: 'dv' Missing required argument(s)");
	}
    }

  startTransfer();
}

////////////////////////////////////////////////////
// dialogMess
//
/////////////////////////////////////////////////////////
void pix_videoDS :: dialogMess(int argc, t_atom*argv)
{
  HRESULT hr;
  if (argc==0)setupMess();
  t_symbol *type=atom_getsymbol(argv);

  if (!m_haveVideo)
    {
      error("pix_video: No open video device.");
      return;
    }

  stopTransfer();

  // Check for old style VFW dialogs
  IAMVfwCaptureDialogs *pDlg = NULL;
  hr = m_pCG->FindInterface(NULL, NULL, m_pCDbase, IID_IAMVfwCaptureDialogs, (void **)&pDlg);

  ISpecifyPropertyPages *pSpec;
  CAUUID cauuid;

  if (!strcmp(type->s_name, "source")) 
    {
      if (pDlg)
	{
	  if (S_OK == (pDlg->HasDialog(VfwCaptureDialog_Source)))
	    {
	      if (FAILED(hr = pDlg->ShowDialog(VfwCaptureDialog_Source, NULL)))
		{
		  error("pix_videoDS: Could not show VFW Capture Source Dialog");
		}
	    }
	} else
	{
	  hr = m_pCDbase->QueryInterface(IID_ISpecifyPropertyPages, (void **)&pSpec);
	  if(hr == S_OK) 
	    {
	      hr = pSpec->GetPages(&cauuid);
	      if (hr == S_OK && cauuid.cElems > 0) 
		{
		  hr = OleCreatePropertyFrame(NULL, 30, 30, NULL, 1,
					      (IUnknown **)&m_pCDbase, cauuid.cElems,
					      (GUID *)cauuid.pElems, 0, 0, NULL);
		  CoTaskMemFree(cauuid.pElems);
		}
	      pSpec->Release();
	    }
	}
    } else if(!strcmp(type->s_name, "format"))
    {
      if (pDlg)
	{
	  if (S_OK == (pDlg->HasDialog(VfwCaptureDialog_Format)))
	    if (FAILED(hr = pDlg->ShowDialog(VfwCaptureDialog_Format, NULL)))
	      {
		error("pix_videoDS: Could not show VFW Capture Format Dialog");
	      }
	  
	} else
	{
	  IAMStreamConfig *pSC;
	  hr = m_pCG->FindInterface(&PIN_CATEGORY_CAPTURE,
				    &MEDIATYPE_Interleaved,
				    m_pCDbase, IID_IAMStreamConfig, (void **)&pSC);
	  if(hr != S_OK)
	    hr = m_pCG->FindInterface(&PIN_CATEGORY_CAPTURE,
				      &MEDIATYPE_Video, m_pCDbase,
				      IID_IAMStreamConfig, (void **)&pSC);

	  if(hr == S_OK) {
	    hr = pSC->QueryInterface(IID_ISpecifyPropertyPages, (void **)&pSpec);
	    if(hr == S_OK) {
	      hr = pSpec->GetPages(&cauuid);
	      if(hr == S_OK && cauuid.cElems > 0) {
		hr = OleCreatePropertyFrame(NULL, 30, 30, NULL, 1,
					    (IUnknown **)&pSC, cauuid.cElems,
					    (GUID *)cauuid.pElems, 0, 0, NULL);
		CoTaskMemFree(cauuid.pElems);
	      }
	      pSpec->Release();
	    }
	    pSC->Release();
	  }
	}
    } else if(!strcmp(type->s_name, "display"))
    {
      if (pDlg)
	{
	  if (S_OK == (pDlg->HasDialog(VfwCaptureDialog_Display)))
	    {
	      if FAILED((hr = pDlg->ShowDialog(VfwCaptureDialog_Display, NULL)))
	      {
		error("pix_videoDS: Could not show VFW Capture Display Dialog");
	      }
	    } else
	    post("pix_videoDS: No display dialog for this device");
	} 
    } else if(!strcmp(type->s_name, "crossbar")) 
    {
      IAMCrossbar *pX, *pX2;
      IBaseFilter *pXF;
      hr = m_pCG->FindInterface(&PIN_CATEGORY_CAPTURE,
				&MEDIATYPE_Interleaved, m_pCDbase,
				IID_IAMCrossbar, (void **)&pX);
      if(hr != S_OK)
	hr = m_pCG->FindInterface(&PIN_CATEGORY_CAPTURE,
				  &MEDIATYPE_Video, m_pCDbase,
				  IID_IAMCrossbar, (void **)&pX);

      if(hr == S_OK) {
	hr = pX->QueryInterface(IID_IBaseFilter, (void **)&pXF);
	if(hr == S_OK) {
	  hr = pX->QueryInterface(IID_ISpecifyPropertyPages, (void **)&pSpec);
	  if(hr == S_OK) {
	    hr = pSpec->GetPages(&cauuid);
	    if(hr == S_OK && cauuid.cElems > 0) {
	      hr = OleCreatePropertyFrame(NULL, 30, 30, NULL, 1,
					  (IUnknown **)&pX, cauuid.cElems,
					  (GUID *)cauuid.pElems, 0, 0, NULL);
	      CoTaskMemFree(cauuid.pElems);
	    }
	    pSpec->Release();
	  }
	  hr = m_pCG->FindInterface(&LOOK_UPSTREAM_ONLY, NULL, pXF,
				    IID_IAMCrossbar, (void **)&pX2);
	  if(hr == S_OK) {
	    hr = pX2->QueryInterface(IID_ISpecifyPropertyPages,
				     (void **)&pSpec);
	    if(hr == S_OK) {
	      hr = pSpec->GetPages(&cauuid);
	      if(hr == S_OK && cauuid.cElems > 0) {
		hr = OleCreatePropertyFrame(NULL, 30, 30, NULL, 1,
					    (IUnknown **)&pX2, cauuid.cElems,
					    (GUID *)cauuid.pElems, 0, 0, NULL);
		CoTaskMemFree(cauuid.pElems);
	      }
	      pSpec->Release();
	    }
	    pX2->Release();
	  }
	  pXF->Release();
	}
	pX->Release();
      }
    } else
    {
      error ("pix_videoDS: dialog not known");
    }
  setModified();
  startTransfer();
}


////////////////////////////////////////////////////////
// cleanPixBlock
//
/////////////////////////////////////////////////////////
    void pix_videoDS :: cleanPixBlock()
{
  for (int i = 0; i <= 2; i++)
    {
      if (m_pixBlockBuf[i].image.data)
	{
	  delete [] m_pixBlockBuf[i].image.data;
	  m_pixBlockBuf[i].image.data = NULL;
	}
      m_pixBlockBuf[i].newimage = 0;
      m_nPixDataSize[i] = 0;
    }
}

void pix_videoDS :: copyBuffer(IMediaSample* pSample)
{
  HRESULT hr;
  unsigned char* pBuffer;
	
  // Get the media type
  AM_MEDIA_TYPE* pmt;
  int readIdx = m_readIdx;

  if ((m_writeIdx = (m_lastwriteIdx + 1) % 3) == readIdx)
    m_writeIdx = (readIdx + 1) % 3;

  int nRawBuffSize = pSample->GetSize();

  // Check for a format change.
  if (FAILED(hr = pSample->GetMediaType(&pmt)))
    {
      error("pix_videoDS : could not get sample media type.");
      closeMess();
      return;
    }

  if (S_OK == hr)
    {
      BITMAPINFOHEADER* pbmih;
      GetBitmapInfoHdr(pmt, &pbmih);
      m_xsize = pbmih->biWidth;
      m_ysize = pbmih->biHeight;
      m_csize = 3;
      m_format = GL_BGR_EXT;
      DeleteMediaType(pmt);	// is this necessary?!	
    }

  if (m_nPixDataSize[m_writeIdx] != nRawBuffSize)
    {
      m_pixBlockBuf[m_writeIdx].image.xsize = m_xsize;
      m_pixBlockBuf[m_writeIdx].image.ysize = m_ysize;
      m_pixBlockBuf[m_writeIdx].image.csize = m_csize;
      m_pixBlockBuf[m_writeIdx].image.format = m_format;
      m_pixBlockBuf[m_writeIdx].image.type = GL_UNSIGNED_BYTE;

      // setup the buffer
      if (m_pixBlockBuf[m_writeIdx].image.data)
	{
	  delete[] m_pixBlockBuf[m_writeIdx].image.data;
	}
      m_pixBlockBuf[m_writeIdx].image.data = new unsigned char[nRawBuffSize];
      m_nPixDataSize[m_writeIdx] = nRawBuffSize;
    }

  pSample->GetPointer(&pBuffer);

  memcpy(m_pixBlockBuf[m_writeIdx].image.data, pBuffer, nRawBuffSize);

  m_pixBlockBuf[m_writeIdx].newimage = 1;
  m_lastwriteIdx = m_writeIdx;
}


/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_videoDS :: obj_setupCallback(t_class *classPtr)
{
  class_addcreator((t_newmethod)_classpix_videoDS, gensym("pix_video"), A_DEFFLOAT, A_NULL);
  class_addcreator((t_newmethod)_classpix_videoDS, gensym("pix_videoDS"), A_DEFFLOAT, A_NULL);
  class_addcreator((t_newmethod)_classpix_videoDS, gensym("pix_video_ds"), A_DEFFLOAT, A_NULL);  // alias to old external
  class_addcreator((t_newmethod)_classpix_videoDS, gensym("pix_dv"), A_DEFFLOAT, A_NULL);

  class_addmethod(classPtr, (t_method)&pix_videoDS::openMessCallback,
		  gensym("device"), A_DEFFLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_videoDS::openMessCallback,
		  gensym("open"), A_DEFFLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_videoDS::closeMessCallback,
		  gensym("close"), A_NULL);
  class_addmethod(classPtr, (t_method)&pix_videoDS::setupMessCallback,
		  gensym("setup"), A_NULL);
  class_addmethod(classPtr, (t_method)&pix_videoDS::dvMessCallback,
		  gensym("dv"), A_GIMME, A_NULL);
  class_addfloat(classPtr, (t_method)&pix_videoDS::floatMessCallback);

  //  pix_video::real_obj_setupCallback(classPtr);
} 

void pix_videoDS :: openMessCallback(void *data, t_floatarg device)
{
  GetMyClass(data)->openMess((int)device);
}
void pix_videoDS :: closeMessCallback(void *data)
{
  GetMyClass(data)->closeMess();
}
void pix_videoDS :: setupMessCallback(void *data)
{
  GetMyClass(data)->setupMess();
}
void pix_videoDS :: floatMessCallback(void *data, float n)
{
  GetMyClass(data)->captureOnOff((int)n);
}
void pix_videoDS :: dvMessCallback(void *data, t_symbol *type, int argc, t_atom *argv)
{
  GetMyClass(data)->dvMess(argc, argv);
}

// From Microsoft sample:
//
// Let's talk about UI for a minute.  There are many programmatic interfaces
// you can use to program a capture filter or related filter to capture the
// way you want it to.... eg:  IAMStreamConfig, IAMVideoCompression,
// IAMCrossbar, IAMTVTuner, IAMTVAudio, IAMAnalogVideoDecoder, IAMCameraControl,
// IAMVideoProcAmp, etc.
//
// But you probably want some UI to let the user play with all these settings.
// For new WDM-style capture devices, we offer some default UI you can use.
// The code below shows how to bring up all of the dialog boxes supported 
// by any capture filters.
//
// The following code shows you how you can bring up all of the
// dialogs supported by a particular object at once on a big page with lots
// of thumb tabs.  You do this by starting with an interface on the object that
// you want, and using ISpecifyPropertyPages to get the whole list, and
// OleCreatePropertyFrame to bring them all up.  This way you will get custom
// property pages a filter has, too, that are not one of the standard pages that
// you know about.  There are at least 9 objects that may have property pages.
// Your app already has 2 of the object pointers, the video capture filter and
// the audio capture filter (let's call them pVCap and pACap)
// 1.  The video capture filter - pVCap
// 2.  The video capture filter's capture pin - get this by calling
//     FindInterface(&PIN_CATEGORY_CAPTURE, pVCap, IID_IPin, &pX);
// 3.  The video capture filter's preview pin - get this by calling
//     FindInterface(&PIN_CATEGORY_PREVIEW, pVCap, IID_IPin, &pX);
// 4.  The audio capture filter - pACap
// 5.  The audio capture filter's capture pin - get this by calling
//     FindInterface(&PIN_CATEGORY_CAPTURE, pACap, IID_IPin, &pX);
// 6.  The crossbar connected to the video capture filter - get this by calling
//     FindInterface(NULL, pVCap, IID_IAMCrossbar, &pX);
// 7.  There is a possible second crossbar to control audio - get this by 
//     looking upstream of the first crossbar like this:
//     FindInterface(&LOOK_UPSTREAM_ONLY, pX, IID_IAMCrossbar, &pX2);
// 8.  The TV Tuner connected to the video capture filter - get this by calling
//     FindInterface(NULL, pVCap, IID_IAMTVTuner, &pX);
// 9.  The TV Audio connected to the audio capture filter - get this by calling
//     FindInterface(NULL, pACap, IID_IAMTVAudio, &pX);
// 10. We have a helper class, CCrossbar, which makes the crossbar issue less
//     confusing.  In fact, although not supported here, there may be more than
//     two crossbars, arranged in many different ways.  An application may not
//     wish to have separate dialogs for each crossbar, but instead hide the
//     complexity and simply offer the user a list of inputs that can be chosen.
//     This list represents all the unique inputs from all the crossbars.
//     The crossbar helper class does this and offers that list as #10.  It is
//     expected that an application will either provide the crossbar dialogs
//     above (#6 and #7) OR provide the input list (this #10), but not both.
//     That would be confusing because if you select an input using dialog 6 or
//     7 the input list here in #10 won't know about your choice.
//
// Your last choice for UI is to make your own pages, and use the results of 
// your custom page to call the interfaces programmatically.


void SetupCaptureDevice(ICaptureGraphBuilder2* pCG, IBaseFilter * pCDbase) 
{
  HRESULT hr;

  // Check for old style VFW dialogs
  IAMVfwCaptureDialogs *pDlg;
  if (SUCCEEDED(hr = (pCG->FindInterface(NULL, NULL, pCDbase, IID_IAMVfwCaptureDialogs, (void **)&pDlg))))
    {
      if (S_OK == (pDlg->HasDialog(VfwCaptureDialog_Source)))
	if (FAILED(hr = pDlg->ShowDialog(VfwCaptureDialog_Source, NULL)))
	  error("pix_videoDS: Could not show VFW Capture Source Dialog");
      if (S_OK == (pDlg->HasDialog(VfwCaptureDialog_Format)))
	if (FAILED(hr = pDlg->ShowDialog(VfwCaptureDialog_Format, NULL)))
	  error("pix_videoDS: Could not show VFW Capture Format Dialog");
      if (S_OK == (pDlg->HasDialog(VfwCaptureDialog_Display)))
	if (FAILED(hr = pDlg->ShowDialog(VfwCaptureDialog_Display, NULL)))
	  error("pix_videoDS: Could not show VFW Capture Display Dialog");
      pDlg->Release();
    }	
	
  ISpecifyPropertyPages *pSpec;
  CAUUID cauuid;
  // 1. the video capture filter itself
  hr = pCDbase->QueryInterface(IID_ISpecifyPropertyPages, (void **)&pSpec);
  if(hr == S_OK) 
    {
      hr = pSpec->GetPages(&cauuid);
      if (hr == S_OK && cauuid.cElems > 0) 
	{
	  hr = OleCreatePropertyFrame(NULL, 30, 30, NULL, 1,
				      (IUnknown **)&pCDbase, cauuid.cElems,
				      (GUID *)cauuid.pElems, 0, 0, NULL);
	  CoTaskMemFree(cauuid.pElems);
        }
      pSpec->Release();
    }


  // 2.  The video capture capture pin
  IAMStreamConfig *pSC;
  hr = pCG->FindInterface(&PIN_CATEGORY_CAPTURE,
			  &MEDIATYPE_Interleaved,
			  pCDbase, IID_IAMStreamConfig, (void **)&pSC);
  if(hr != S_OK)
    hr = pCG->FindInterface(&PIN_CATEGORY_CAPTURE,
			    &MEDIATYPE_Video, pCDbase,
			    IID_IAMStreamConfig, (void **)&pSC);

  if(hr == S_OK) {
    hr = pSC->QueryInterface(IID_ISpecifyPropertyPages, (void **)&pSpec);
    if(hr == S_OK) {
      hr = pSpec->GetPages(&cauuid);
      if(hr == S_OK && cauuid.cElems > 0) {
	hr = OleCreatePropertyFrame(NULL, 30, 30, NULL, 1,
				    (IUnknown **)&pSC, cauuid.cElems,
				    (GUID *)cauuid.pElems, 0, 0, NULL);
	CoTaskMemFree(cauuid.pElems);
      }
      pSpec->Release();
    }
    pSC->Release();
  }

  // 4 & 5.  The video crossbar, and a possible second crossbar
  IAMCrossbar *pX, *pX2;
  IBaseFilter *pXF;
  hr = pCG->FindInterface(&PIN_CATEGORY_CAPTURE,
			  &MEDIATYPE_Interleaved, pCDbase,
			  IID_IAMCrossbar, (void **)&pX);
  if(hr != S_OK)
    hr = pCG->FindInterface(&PIN_CATEGORY_CAPTURE,
			    &MEDIATYPE_Video, pCDbase,
			    IID_IAMCrossbar, (void **)&pX);

  if(hr == S_OK) {
    hr = pX->QueryInterface(IID_IBaseFilter, (void **)&pXF);
    if(hr == S_OK) {
      hr = pX->QueryInterface(IID_ISpecifyPropertyPages, (void **)&pSpec);
      if(hr == S_OK) {
	hr = pSpec->GetPages(&cauuid);
	if(hr == S_OK && cauuid.cElems > 0) {
	  hr = OleCreatePropertyFrame(NULL, 30, 30, NULL, 1,
				      (IUnknown **)&pX, cauuid.cElems,
				      (GUID *)cauuid.pElems, 0, 0, NULL);
	  CoTaskMemFree(cauuid.pElems);
	}
	pSpec->Release();
      }
      hr = pCG->FindInterface(&LOOK_UPSTREAM_ONLY, NULL, pXF,
			      IID_IAMCrossbar, (void **)&pX2);
      if(hr == S_OK) {
	hr = pX2->QueryInterface(IID_ISpecifyPropertyPages,
				 (void **)&pSpec);
	if(hr == S_OK) {
	  hr = pSpec->GetPages(&cauuid);
	  if(hr == S_OK && cauuid.cElems > 0) {
	    hr = OleCreatePropertyFrame(NULL, 30, 30, NULL, 1,
					(IUnknown **)&pX2, cauuid.cElems,
					(GUID *)cauuid.pElems, 0, 0, NULL);
	    CoTaskMemFree(cauuid.pElems);
	  }
	  pSpec->Release();
	}
	pX2->Release();
      }
      pXF->Release();
    }
    pX->Release();
  }

  /*
  // 6.  The TVTuner!!
  IAMTVTuner *pTV;
  hr = gcap.pBuilder->FindInterface(&PIN_CATEGORY_CAPTURE,
  &MEDIATYPE_Interleaved, gcap.pVCap,
  IID_IAMTVTuner, (void **)&pTV);
  if(hr != S_OK)
  hr = gcap.pBuilder->FindInterface(&PIN_CATEGORY_CAPTURE,
  &MEDIATYPE_Video, gcap.pVCap,
  IID_IAMTVTuner, (void **)&pTV);
  if(hr == S_OK) {
  hr = pTV->QueryInterface(IID_ISpecifyPropertyPages, (void **)&pSpec);
  if(hr == S_OK) {
  hr = pSpec->GetPages(&cauuid);
  if(hr == S_OK && cauuid.cElems > 0) {
  AppendMenu(hMenuSub,MF_STRING,MENU_DIALOG0+zz, TEXT("TV Tuner..."));
  gcap.iTVTunerDialogPos = zz++;
  CoTaskMemFree(cauuid.pElems);
  }
  pSpec->Release();
  }
  pTV->Release();
  }

  // 10.  Crossbar class helper menu item, to let you choose an input

  if(gcap.pCrossbar && gcap.NumberOfVideoInputs) {
  gcap.hMenuPopup = CreatePopupMenu();
  LONG j;
  LONG  PhysicalType;
  TCHAR buf[MAX_PATH];
  LONG InputToEnable = -1;

  gcap.iVideoInputMenuPos = zz++;
  AppendMenu(hMenuSub, MF_SEPARATOR, 0, NULL);

  for(j = 0; j < gcap.NumberOfVideoInputs; j++) {
  EXECUTE_ASSERT(S_OK == gcap.pCrossbar->GetInputType(j, &PhysicalType));
  EXECUTE_ASSERT(S_OK == gcap.pCrossbar->GetInputName(j, buf, sizeof (buf)));
  AppendMenu(gcap.hMenuPopup,MF_STRING,MENU_DIALOG0+zz, buf);
  zz++;

  // Route the first TVTuner by default
  if((PhysicalType == PhysConn_Video_Tuner) && InputToEnable == -1) {
  InputToEnable = j;
  }
  }

  AppendMenu(hMenuSub, MF_STRING | MF_POPUP, (UINT_PTR)gcap.hMenuPopup, TEXT("Video Input"));

  if(InputToEnable == -1) {
  InputToEnable = 0;
  }
  CheckMenuItem(gcap.hMenuPopup, InputToEnable, MF_BYPOSITION | MF_CHECKED); 

  gcap.pCrossbar->SetInputIndex(InputToEnable);
  }
  // !!! anything needed to delete the popup when selecting a new input?
  */
}


HRESULT 
FindCaptureDevice(int device, IBaseFilter ** ppSrcFilter)
{
  HRESULT hr;
  IBaseFilter * pSrc = NULL;

  IMoniker* pMoniker =NULL;
  ULONG cFetched;

  ICreateDevEnum* pDevEnum =NULL;
  IEnumMoniker* pClassEnum = NULL;

  do 
    {
      // Create the system device enumerator
      hr = CoCreateInstance (CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC,
			     IID_ICreateDevEnum, (void ** ) &pDevEnum);
      if (FAILED(hr))
	{
	  error("GEM: Couldn't create system enumerator!");
	  break;
	}

      // Create an enumerator for the video capture devices

      hr = pDevEnum->CreateClassEnumerator (CLSID_VideoInputDeviceCategory, &pClassEnum, 0);
      if (FAILED(hr))
	{
	  error("pix_videoDS: Couldn't create class enumerator!");
	  break;
	}

      // If there are no enumerators for the requested type, then 
      // CreateClassEnumerator will succeed, but pClassEnum will be NULL.
      if (pClassEnum == NULL)
	{
	  error("pix_videoDS: No video capture devices found!");
	  hr = E_FAIL;
	  break;
	}

      // Use the first video capture device on the device list.
      // Note that if the Next() call succeeds but there are no monikers,
      // it will return S_FALSE (which is not a failure).  Therefore, we
      // check that the return code is S_OK instead of using SUCCEEDED() macro.
      int devIndex = 0;
      while(		(S_OK == (pClassEnum->Next (1, &pMoniker, &cFetched)))
			&&	(devIndex <= device))
	{
	  if (devIndex == device)
	    {
	      // Bind Moniker to a filter object
	      hr = pMoniker->BindToObject(0,0,IID_IBaseFilter, (void**)&pSrc);
	      if (FAILED(hr))
		{
		  error("pix_videoDS: Couldn't bind moniker to filter object!");
		}
	    }
	  COMRELEASE(pMoniker);
	  devIndex++;
	}
    } while (0);

  // Copy the found filter pointer to the output parameter.
  // Do NOT Release() the reference, since it will still be used
  // by the calling function.
  COMRELEASE(pDevEnum);
  COMRELEASE(pClassEnum);

  *ppSrcFilter = pSrc;

  return hr;
}

HRESULT ConnectFilters(IGraphBuilder *pGraph, IBaseFilter *pFirst, IBaseFilter *pSecond)
{
  IPin *pOut = NULL, *pIn = NULL;
  HRESULT hr = GetPin(pFirst, PINDIR_OUTPUT, &pOut);
  if (FAILED(hr)) return hr;
  hr = GetPin(pSecond, PINDIR_INPUT, &pIn);
  if (FAILED(hr)) 
    {
      pOut->Release();
      return E_FAIL;
    }
  hr = pGraph->Connect(pOut, pIn);
  pIn->Release();
  pOut->Release();
  return hr;
}

void GetBitmapInfoHdr(AM_MEDIA_TYPE* pmt, BITMAPINFOHEADER** ppbmih)
{
  *ppbmih = NULL;

  if (IsEqualGUID(pmt->formattype, FORMAT_VideoInfo) ||
      IsEqualGUID(pmt->formattype, FORMAT_MPEGVideo)) 
    {

      VIDEOINFOHEADER * pVideoFormat = (VIDEOINFOHEADER *) pmt->pbFormat;
      *ppbmih = &(((VIDEOINFOHEADER *) pmt->pbFormat)->bmiHeader);
    }
  else if (	IsEqualGUID(pmt->formattype, FORMAT_MPEG2Video) ||
		IsEqualGUID(pmt->formattype, FORMAT_VideoInfo2))
    {
      *ppbmih = &(((VIDEOINFOHEADER2 *) pmt->pbFormat)->bmiHeader);
    }
  else
    {
      error("pix_videoDS: Unknown media format");
      return;
    }
}

HRESULT GetPin(IBaseFilter *pFilter, PIN_DIRECTION PinDir, IPin **ppPin)
{
  IEnumPins  *pEnum;
  IPin       *pPin;
  pFilter->EnumPins(&pEnum);
  while(pEnum->Next(1, &pPin, 0) == S_OK)
    {
      PIN_DIRECTION PinDirThis;
      pPin->QueryDirection(&PinDirThis);
      if (PinDir == PinDirThis)
        {
	  pEnum->Release();
	  *ppPin = pPin;
	  return S_OK;
        }
      pPin->Release();
    }
  pEnum->Release();
  return E_FAIL;  
}



HRESULT AddGraphToRot(IUnknown *pUnkGraph, DWORD *pdwRegister) 
{
  IMoniker * pMoniker;
  IRunningObjectTable *pROT;
  if (FAILED(GetRunningObjectTable(0, &pROT))) 
    {
      return E_FAIL;
    }

  WCHAR wsz[128];
  wsprintfW(wsz, L"FilterGraph %08x pid %08x", (DWORD_PTR)pUnkGraph, 
	    GetCurrentProcessId());

  HRESULT hr = CreateItemMoniker(L"!", wsz, &pMoniker);
  if (SUCCEEDED(hr)) 
    {
      hr = pROT->Register(0, pUnkGraph, pMoniker, pdwRegister);
      pMoniker->Release();
    }

  pROT->Release();
  return hr;
}

void RemoveGraphFromRot(DWORD pdwRegister)
{
  IRunningObjectTable *pROT;

  if (SUCCEEDED(GetRunningObjectTable(0, &pROT))) 
    {
      pROT->Revoke(pdwRegister);
      pROT->Release();
    }
}
#endif
