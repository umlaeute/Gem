/*-----------------------------------------------------------------

GEM - Graphics Environment for Multimedia

Load a video into a pix block

Copyright (c) 1997-1999 Mark Danks. mark@danks.org
Copyright (c) G�nther Geiger. geiger@epy.co.at
Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f�r::uml�ute. IEM. zmoelnig@iem.kug.ac.at
Copyright (c) 2003 Daniel Heckenberg
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_PIX_VIDEODS_H_
#define INCLUDE_PIX_VIDEODS_H_

#include <Pixes/pix_video.h>

#include <dshow.h>
#include <qedit.h>

class CSampleGrabber;

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  pix_videoDS

  Loads in an video using DirectShow

  KEYWORDS
  pix

  DESCRIPTION

  -----------------------------------------------------------------*/
class pix_videoDS : public pix_video
{
  CPPEXTERN_HEADER(pix_videoDS, pix_video)

    public:

  //////////
  // Constructor
  pix_videoDS(t_floatarg num);

  //////////
  //  Accept new sample buffer
  void			copyBuffer(IMediaSample* pSample);

 protected:

  //////////
  // Destructor
  virtual ~pix_videoDS();

  //////////
  // Do the rendering
  virtual void 	render(GemState *state);

  //////////
  // Clear the dirty flag on the pixBlock
  virtual void 	postrender(GemState *state);

  //////////
  virtual void	startRendering();

  //////////
  // If you care about the stop of rendering
  virtual void	stopRendering();

  //////////
  // Clean up the pixBlock
  void	    	cleanPixBlock();

  // Open the video device
  void	    	openMess(int device);

  // List the available devices
  virtual void	    	enumerateMess();

  // Setup the video input
  virtual void	    	setupMess();

  // fire the format dialogs
  virtual void		dialogMess(int,t_atom*);

  // fire the format dialogs
  virtual void		dvMess(int argc, t_atom *argv);

  // Close the video device
  void	    	closeMess();

  // Set live capturing
  void		captureOnOff(int n);

  // set the colorspace (RGBA/YUV/Grey)
  void		csMess(int);

  //////////
  // Start up the video device
  // [out] int - returns 0 if bad
  int	    	startTransfer();

  //////////
  // Stop the video device
  // [out] int - returns 0 if bad
  int	    	stopTransfer();

  //-----------------------------------
  // GROUP:	Video data
  //-----------------------------------

  //////////
  // If video is connected
  int 	    	m_haveVideo;

  //////////
  // The pixBlocks for the captured and rendered image
  pixBlock    	m_pixBlockBuf[3];
  int		m_nPixDataSize[3];
  ////////
  // the pixBlock that gets the actually displayed data:
  // this is : m_pixBlockBuf is written by the capture-device, with some format
  // on "render()" we copy/convert the current data into m_pixBlock 
  // and pass it to the state-pointer
  pixBlock      m_pixBlock;

  // Index to read latest image.
  int		m_readIdx;
  int		m_lastreadIdx;
  int		m_writeIdx;
  int		m_lastwriteIdx;

  //////////
  // Do we have to color swap?
  int 	    	m_colorSwap;

  HWND		m_hWndC;
  //	void	videoFrame(LPVIDEOHDR lpVHdr);
  int		m_vidXSize;
  int		m_vidYSize;
  int		m_newFrame;
  int		m_format;

  int		m_rendering;
  int		m_capturing;
  int		m_captureOnOff;

  //  filmds parameters
  int		m_xsize;
  int		m_ysize;
  int		m_csize;

  unsigned char*m_RawBuffer;
  int		m_nRawBuffSize;

  // DirectShow Interfaces that we may need
  IGraphBuilder*	m_pGB;
  IMediaControl*	m_pMC;
  IMediaEvent*		m_pME;
  IMediaFilter*		m_pMF;
  IVideoWindow*		m_pVW;
  IBasicAudio*		m_pBA;
  IBasicVideo*		m_pBV;
  IMediaSeeking*	m_pMS;
  IMediaPosition*	m_pMP;
  CSampleGrabber*	m_pSG;

  IAMVideoControl*	m_pVC;
  IFilterGraph*		m_pFG;
  IBaseFilter*		m_pCDbase;
  ICaptureGraphBuilder2*m_pCG;

  unsigned long		m_GraphRegister;
  bool			m_bInitCOM;

 private:

  //////////
  // static member functions

  static void openMessCallback(void *data, t_floatarg device);
  static void closeMessCallback(void *data);
  static void setupMessCallback(void *data);
  static void floatMessCallback(void *data, float n);
  static void dvMessCallback(void *data, t_symbol *type, int argc, t_atom *argv);

  //	static void videoFrameCallback(HWND hWnd, LPVIDEOHDR lpVHdr);
};

#endif	// for header file
