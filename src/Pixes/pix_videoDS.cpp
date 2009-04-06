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

#if defined(__WIN32__) && defined(HAVE_DIRECTSHOW)

#include "pix_videoDS.h"
#include <memory>
#include <dshow.h>
#include <qedit.h>
#include <Dvdmedia.h>
#include <streams.h>
#include <atlbase.h>
#include "DSgrabber.h"

#include <strsafe.h>

#define COMRELEASE(x) { if (x) x->Release(); x = NULL; }
//#define REGISTER_FILTERGRAPH 1

// Utility functions
void SetupCaptureDevice(ICaptureGraphBuilder2* pCG, IBaseFilter * pCDbase);
HRESULT FindCaptureDevice(int device, IBaseFilter ** ppSrcFilter);
HRESULT ConnectFilters(IGraphBuilder *pGraph, IBaseFilter *pFirst, IBaseFilter *pSecond);
void GetBitmapInfoHdr(AM_MEDIA_TYPE* pmt, BITMAPINFOHEADER** ppbmih);
HRESULT GetPin(IBaseFilter *, PIN_DIRECTION, IPin **);
HRESULT AddGraphToRot(IUnknown *pUnkGraph, DWORD *pdwRegister);
HRESULT SetAviOptions(IBaseFilter *ppf, InterleavingMode INTERLEAVE_MODE);
void RemoveGraphFromRot(DWORD pdwRegister);

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
    m_pGB(NULL),
    m_pMC(NULL),
    m_pME(NULL),
    m_pMF(NULL),
    m_pBA(NULL),
    m_pBV(NULL),
    m_pMS(NULL),
    m_pMP(NULL),
	SampleFilter(NULL),
	NullFilter(NULL),
	FileFilter(NULL),
    m_pCDbase(NULL),
    m_pCG(NULL),
    m_GraphRegister(0),
    m_rendering(0),
    m_capturing(0),
	m_recording(1),
    m_captureOnOff(1),
#ifdef DIRECTSHOW_LOGGING
	LogFileHandle(NULL),
#endif
    m_bInitCOM(false)
{
  // Initialize COM
  if(FAILED(CoInitialize(NULL)))
    {
      error("could not initialise COM.");
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
  
  memset(m_filename, 0, MAXPDSTRING);

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
	HRESULT			hr;
	AM_MEDIA_TYPE	MediaType;

  if (!m_bInitCOM)
    {
      error("COM has not been initialized.");
      return;
    }

  if (m_haveVideo) closeMess();

  do  {
    // Get the interface for DirectShow's GraphBuilder
    if (FAILED(hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void **)&m_pGB)))  {
      error("Could not get DShow GraphBuilder, hr 0x%X", hr);
      break;
    }

#ifdef DIRECTSHOW_LOGGING
	OFSTRUCT	OFStruct;
	
	LogFileHandle	= OpenFile("DirectShow.log", &OFStruct, OF_CREATE);
	
	if (LogFileHandle != NULL)
	{
		m_pGB->SetLogFile(LogFileHandle);
	}
#endif

    // Get the interface for DirectShow's CaptureGraphBuilder2 which allows the use of capture devices instead of file sources
    if (	FAILED(hr = (CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL, CLSCTX_INPROC_SERVER, IID_ICaptureGraphBuilder2, (void **)&m_pCG)))
		||	FAILED(hr = m_pCG->SetFiltergraph(m_pGB))){
      error("Could not get DShow GraphBuilder, hr 0x%X", hr);
      break;
    }

    // Create the capture device.
    if (FAILED(hr = FindCaptureDevice(device, &m_pCDbase)))
    {
      error("Could not open device: %d\n", device);
      break;
    }

	// Create the SampleGrabber filter
	hr	= CoCreateInstance(CLSID_SampleGrabber, NULL, CLSCTX_INPROC_SERVER,
		IID_IBaseFilter, (void**)&SampleFilter);
    
	if (hr != S_OK || NULL == SampleFilter)
	{
		error("Unable to create SampleFilter interface %d", hr);
		
		return;
	}

	// Query for the SampleGrabber interface in the SampleGrabber filter
	// Needed to grab the buffer using GetCurrentBuffer()
	hr	= SampleFilter->QueryInterface(IID_ISampleGrabber, (void **)&SampleGrabber);

	if (hr != S_OK || NULL == SampleGrabber)
	{
		error("Unable to create SampleGrabber interface %d", hr);
		
		return;
	}

	// Set the media type that the SampleGrabber wants.
	// MEDIATYPE_Video selects only video and not interleaved audio and video
	// MEDIASUBTYPE_RGB24 is the colorspace and format to deliver frames
	// MediaType.formattype is GUID_NULLsince it is handled later to get file info
	memset(&MediaType, 0, sizeof(AM_MEDIA_TYPE));
	MediaType.majortype		= MEDIATYPE_Video;
	MediaType.subtype		= MEDIASUBTYPE_RGB24;
	MediaType.formattype	= GUID_NULL;
	hr						= SampleGrabber->SetMediaType(&MediaType);
	
	// Set the SampleGrabber to return continuous frames
	hr	= SampleGrabber->SetOneShot(FALSE);
	
	if (hr != S_OK)
	{
		error("Unable to setup sample grabber %d", hr);
		
		return;
	}

	// Set the SampleGrabber to copy the data to a buffer. This only set to FALSE when a 
	// callback is used.
	hr	= SampleGrabber->SetBufferSamples(TRUE);
	
	if (hr != S_OK)
	{
		error("Unable to setup sample grabber %d", hr);
		
		return;
	}

	// Create the Null Renderer
	hr	= CoCreateInstance(CLSID_NullRenderer, NULL, CLSCTX_INPROC_SERVER,
		IID_IBaseFilter, (void**)&NullFilter);
    
	if (hr != S_OK || NULL == NullFilter)
	{
		error("Unable to create NullFilter interface %d", hr);
		
		return;
	}

    // add the filters to the graph
    if (FAILED(hr = m_pGB->AddFilter(m_pCDbase, L"Capture Device")) || 
		FAILED(hr = m_pGB->AddFilter(SampleFilter, L"Sample Grabber")) ||
		FAILED(hr = m_pGB->AddFilter(NullFilter, L"Null Renderer")))
	{
      error("Could not add the filters to the graph, hr 0x%X", hr);
      break;
    }

	// Automatically connect the Device filter to the NullFilter through the SampleFilter.
	// Additional filters may be added.
	// Try Interleaved Audio and Video first for DV input
	if (FAILED(hr = m_pCG->RenderStream(&PIN_CATEGORY_PREVIEW, &MEDIATYPE_Interleaved, 
		m_pCDbase, SampleFilter, NullFilter)))
	{
		//try Video only for devices with no audio
		if (FAILED(hr = m_pCG->RenderStream(&PIN_CATEGORY_PREVIEW, &MEDIATYPE_Video, 
			m_pCDbase, SampleFilter, NullFilter)))
		{
			error("Unable to connect to SampleGrabber.");
	
			return;
		}
	}

    // QueryInterface for DirectShow interfaces
    if (FAILED(hr = (m_pGB->QueryInterface(IID_IMediaFilter, (void **)&m_pMF))))
    {
      error("Could not get media control interface, hr 0x%X", hr);
      break;
    }

	//MediaControl is used for Run, Stop, Pause and running state queries
    if (FAILED(hr = (m_pGB->QueryInterface(IID_IMediaControl, (void **)&m_pMC))))
    {
      error("Could not get media control interface, hr 0x%X", hr);
      break;
    }

	//not used right now
    if (FAILED(hr = (m_pGB->QueryInterface(IID_IMediaEvent, (void **)&m_pME))))	{
      error("Could not get media event interface, hr 0x%X", hr);
      break;
    }

	//MediaSeeking for the end of a clip.  not really used here
    if (FAILED(hr = (m_pGB->QueryInterface(IID_IMediaSeeking, (void **)&m_pMS)))){
      error("Could not get media seeking interface, hr 0x%X", hr);
      break;
    }

	//for the position of a clip.  not really used for device capture
    if (FAILED(hr = (m_pGB->QueryInterface(IID_IMediaPosition, (void **)&m_pMP)))){
      error("Could not get media position interface, hr 0x%X", hr);
      break;
    }

    // Expose the filter graph so we can view it using GraphEdit
#ifdef REGISTER_FILTERGRAPH
    if (FAILED(hr = AddGraphToRot(m_pGB, &m_GraphRegister))){
      error("failed to register filter graph with ROT!  hr=0x%X", hr);
      m_GraphRegister = 0;
    }
#endif
    // THIS KIILS FILE WRITING!! May improve latency on video preview/playback.
    // Turn off the reference clock. 
//	if (FAILED(hr = m_pMF->SetSyncSource(NULL)))
//	{
//		error("failed to turn off the reference clock  hr=0x%X", hr);
//		break;
//	}

	// Indicate that the video is ready.
    m_haveVideo = 1;

	stopCapture();
	
    startTransfer();

  } while (0);

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

#ifdef DIRECTSHOW_LOGGING
	m_pGB->SetLogFile(NULL);
	
	CloseHandle((HANDLE)LogFileHandle);
#endif

	// Release the DirectShow interfaces
  COMRELEASE(m_pGB);
  COMRELEASE(m_pMC);
  COMRELEASE(m_pME);
  COMRELEASE(m_pMF);
  COMRELEASE(m_pBA);
  COMRELEASE(m_pBV);
  COMRELEASE(m_pMS);
  COMRELEASE(m_pMP);
  COMRELEASE(SampleFilter);
  COMRELEASE(NullFilter);
  COMRELEASE(FileFilter);
  COMRELEASE(m_pCDbase);
  COMRELEASE(m_pCG);

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
	  error("Couldn't create system enumerator!");
	  break;
	}

      // Create an enumerator for the video capture devices
      hr = pDevEnum->CreateClassEnumerator (CLSID_VideoInputDeviceCategory, &pClassEnum, 0);
      if (FAILED(hr))
	{
	  error("Couldn't create class enumerator!");
	  break;
	}

      // If there are no enumerators for the requested type, then 
      // CreateClassEnumerator will succeed, but pClassEnum will be NULL.
      if (pClassEnum == NULL)
	{
	  error("No video capture devices found!");
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
      error("No open video device.");
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
    //      error("Video not active");
    state->image = NULL;	
    return;
  }

	// Copy the buffer from the camera buffer to the texture buffer
	copyBuffer();
	
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
      AM_MEDIA_TYPE mt;
      BITMAPINFOHEADER* pbmih;
      if (NULL == SampleGrabber || FAILED(hr = SampleGrabber->GetConnectedMediaType(&mt)))
	{
	  error("Could not get connect media type, hr 0x%X", hr);
	  return 0;
	}
      GetBitmapInfoHdr(&mt, &pbmih);
      m_xsize = pbmih->biWidth;
      m_ysize = pbmih->biHeight;
      m_csize = 3;
      m_format = GL_BGR_EXT;

	  //starts the graph rendering
      if (FAILED(hr = m_pMC->Run()))
	{
	  error("Could not start graph playback, hr 0x%X", hr);
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
	  error("Could not stop graph playback, hr 0x%X", hr);
	} else
	m_capturing = 0;
    }

  return !(!m_capturing);
}

void pix_videoDS :: startCapture()
{
	HRESULT	hr;
	WCHAR	WideFileName[MAXPDSTRING];
	
	if (m_filename[0] == 0)
	{
		error("No filename passed");
		return;
	}
	
	if (FALSE == m_recording && m_pCG != NULL && m_haveVideo)
	{
		// Convert filename to wide chars
		memset(&WideFileName, 0, MAXPDSTRING * 2);
	
		if (0 == MultiByteToWideChar(CP_ACP, 0, m_filename, strlen(m_filename), WideFileName, 
			MAXPDSTRING))
		{
			error("Unable to capture to %s", m_filename);
		
			return;
		}

		// Set filename of output AVI. Returns pointer to a File Writer filter.
		if (FAILED(hr = m_pCG->SetOutputFileName(&MEDIASUBTYPE_Avi, WideFileName, 
			&FileFilter, NULL)))
		{
			error("Unable to set output filename.");
		
			return;
		}
	
		// Set AVI output option for interleaving.
		if (FAILED(hr = SetAviOptions(FileFilter, INTERLEAVE_NONE)))
		{
			error("Unable to set avi options.");
		
			return;
		}

		// Connect the Capture Device filter to the File Writer filter. Try using 
		//	MEDIATYPE_Interleaved first, else default to MEDIATYPE_Video.
		if (FAILED(hr = m_pCG->RenderStream(&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Interleaved, 
			m_pCDbase, NULL, FileFilter)))
		{
			if (FAILED(hr = m_pCG->RenderStream(&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Video, 
				m_pCDbase, NULL, FileFilter)))
			{
				error("Unable to record to avi.");
	
				return;
			}
		}

		m_recording	= TRUE;
	}
}

void pix_videoDS :: stopCapture()
{
	HRESULT	RetVal;
	
	if (TRUE == m_recording && m_haveVideo)
	{
		IBaseFilter	*Filter;
		
		// Remove the File Writer filter, if available.
		//This is probably where DS releases the written AVI file
		RetVal	= m_pGB->FindFilterByName(L"File Writer", &Filter);
		
		if (S_OK == RetVal)
		{
			m_pGB->RemoveFilter(Filter);
		}
		
		// Remove the AVI Mux filter, if available.
		RetVal	= m_pGB->FindFilterByName(L"Mux", &Filter);
		
		if (S_OK == RetVal)
		{
			m_pGB->RemoveFilter(Filter);
		}
		
		m_recording	= FALSE;
	}
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

void pix_videoDS :: fileMess(t_symbol *filename)
{
	canvas_makefilename(getCanvas(), filename->s_name, m_filename, MAXPDSTRING);
}

void pix_videoDS :: recordMess(int state)
{
	stopTransfer();
	
	if (state)
	{
		startCapture();
	}
	
	else
	{
		stopCapture();
	}
		
	startTransfer();
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

	  IIPDVDec	*pDV;
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
		  error("unknown DV decoder setting");
		}
	      if (FAILED(hr))
		{
		  error("Could not set decoder resolution.");
		}
	      pDV->Release();
	    } else
	    {
	      error("Device does not have DV decoder interface");
	    }
	} else
	{
	  error("'dv' Missing required argument(s)");
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
      error("No open video device.");
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
		  error("Could not show VFW Capture Source Dialog");
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
		error("Could not show VFW Capture Format Dialog");
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
		error("Could not show VFW Capture Display Dialog");
	      }
	    } else
	    error("No display dialog for this device");
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
      error ("dialog not known");
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

void pix_videoDS :: copyBuffer()
{
	HRESULT	hr;
	long	SampleSize;	
	
  // Get the media type
  AM_MEDIA_TYPE	pmt;
  int readIdx = m_readIdx;

  if ((m_writeIdx = (m_lastwriteIdx + 1) % 3) == readIdx)
    m_writeIdx = (readIdx + 1) % 3;

	// Get the current buffer size from the SampleGrabber.
	if (SampleGrabber != NULL)
	{
		hr	= SampleGrabber->GetCurrentBuffer(&SampleSize, NULL);
	
		if (hr != S_OK)
		{
			return;
		}
	}		

  // Check for a format change.
  if (NULL == SampleGrabber || FAILED(hr = SampleGrabber->GetConnectedMediaType(&pmt)))
    {
      error("could not get sample media type.");
      closeMess();
      return;
    }

  if (S_OK == hr)
    {
      BITMAPINFOHEADER* pbmih;
      GetBitmapInfoHdr(&pmt, &pbmih);
      m_xsize = pbmih->biWidth;
      m_ysize = pbmih->biHeight;
      m_csize = 3;
      m_format = GL_BGR_EXT;
      FreeMediaType(pmt);	// is this necessary?!	
    }

  if (m_nPixDataSize[m_writeIdx] != SampleSize)
    {
      m_pixBlockBuf[m_writeIdx].image.xsize = m_xsize;
      m_pixBlockBuf[m_writeIdx].image.ysize = m_ysize;
      m_pixBlockBuf[m_writeIdx].image.csize = m_csize;
      m_pixBlockBuf[m_writeIdx].image.format = m_format;
      m_pixBlockBuf[m_writeIdx].image.type = GL_UNSIGNED_BYTE;

      // setup the buffer
      //LATER check for memory leak and use allocate()!
      if (m_pixBlockBuf[m_writeIdx].image.data)
        {
          delete[] m_pixBlockBuf[m_writeIdx].image.data;
        }
      m_pixBlockBuf[m_writeIdx].image.data = new unsigned char[SampleSize];
      m_nPixDataSize[m_writeIdx] = SampleSize;
    }

	// Get the current buffer from the SampleGrabber.
	if (SampleGrabber != NULL)
	{
		hr	= SampleGrabber->GetCurrentBuffer(&SampleSize, 
			(long *)m_pixBlockBuf[m_readIdx].image.data);
	
		if (hr != S_OK)
		{
			return;
		}
	}		

  m_pixBlockBuf[m_writeIdx].newimage = 1;
  m_lastwriteIdx = m_writeIdx;
}


/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_videoDS :: obj_setupCallback(t_class *classPtr)
{
  class_addcreator((t_newmethod)create_pix_videoDS, gensym("pix_video"), A_DEFFLOAT, A_NULL);
  class_addcreator((t_newmethod)create_pix_videoDS, gensym("pix_videoDS"), A_DEFFLOAT, A_NULL);
  class_addcreator((t_newmethod)create_pix_videoDS, gensym("pix_video_ds"), A_DEFFLOAT, A_NULL);  // alias to old external
  class_addcreator((t_newmethod)create_pix_videoDS, gensym("pix_dv"), A_DEFFLOAT, A_NULL);

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
  
  class_addmethod(classPtr, (t_method)&pix_videoDS::recordMessCallback,
		  gensym("record"), A_DEFFLOAT, A_NULL);
		  
   class_addmethod(classPtr, (t_method)&pix_videoDS::fileMessCallback,
		  gensym("file"), A_SYMBOL, A_NULL);

  //  pix_videoOS::real_obj_setupCallback(classPtr);
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

void pix_videoDS :: recordMessCallback(void *data, t_floatarg state)
{
  GetMyClass(data)->recordMess(!(!(int)state));
}

void pix_videoDS :: fileMessCallback(void *data, t_symbol *filename)
{
  GetMyClass(data)->fileMess(filename);
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
	  error("Could not show VFW Capture Source Dialog");
      if (S_OK == (pDlg->HasDialog(VfwCaptureDialog_Format)))
	if (FAILED(hr = pDlg->ShowDialog(VfwCaptureDialog_Format, NULL)))
	  error("Could not show VFW Capture Format Dialog");
      if (S_OK == (pDlg->HasDialog(VfwCaptureDialog_Display)))
	if (FAILED(hr = pDlg->ShowDialog(VfwCaptureDialog_Display, NULL)))
	  error("Could not show VFW Capture Display Dialog");
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
	  error("Couldn't create system enumerator!");
	  break;
	}

      // Create an enumerator for the video capture devices

      hr = pDevEnum->CreateClassEnumerator (CLSID_VideoInputDeviceCategory, &pClassEnum, 0);
      if (FAILED(hr))
	{
	  error("Couldn't create class enumerator!");
	  break;
	}

      // If there are no enumerators for the requested type, then 
      // CreateClassEnumerator will succeed, but pClassEnum will be NULL.
      if (pClassEnum == NULL)
	{
	  error("No video capture devices found!");
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
		  error("Couldn't bind moniker to filter object!");
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
      error("Unknown media format");
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


HRESULT SetAviOptions(IBaseFilter *ppf, InterleavingMode INTERLEAVE_MODE)
{
    HRESULT hr;
    CComPtr<IConfigAviMux>        pMux           = NULL;
    CComPtr<IConfigInterleaving>  pInterleaving  = NULL;

    ASSERT(ppf);
    if (!ppf)
        return E_POINTER;

    // QI for interface AVI Muxer
    if (FAILED(hr = ppf->QueryInterface(IID_IConfigAviMux, reinterpret_cast<PVOID *>(&pMux))))
    {
		error("IConfigAviMux failed.");
	}
	
    if (FAILED(hr = pMux->SetOutputCompatibilityIndex(TRUE)))
    {
		error("SetOutputCompatibilityIndex failed.");
    }
    
    // QI for interface Interleaving
    if (FAILED(hr = ppf->QueryInterface(IID_IConfigInterleaving, reinterpret_cast<PVOID *>(&pInterleaving))))
    {
		error("IConfigInterleaving failed.");
	}

    // put the interleaving mode (full, none, half)
    if (FAILED(pInterleaving->put_Mode(INTERLEAVE_MODE)))
    {
		error("put_Mode failed.");
    }
       
    return hr;
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
	StringCchPrintfW(wsz, 128, L"FilterGraph %08x pid %08x", (DWORD_PTR)pUnkGraph, 
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
