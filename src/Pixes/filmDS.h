/* -----------------------------------------------------------------

GEM - Graphics Environment for Multimedia

Load an digital video (like AVI, Mpeg, Quicktime) into a pix block 
(OS independant parent-class)

Copyright (c) 1997-1999 Mark Danks. mark@danks.org
Copyright (c) Günther Geiger. geiger@epy.co.at
Copyright (c) 2001-2003 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_filmDS_H_
#define INCLUDE_filmDS_H_
  
#include "Pixes/film.h"
#if defined(__WIN32__) && defined(HAVE_DIRECTSHOW)
#include <dshow.h>
#include <qedit.h>
#endif

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  filmDS
    
  film-loader class for AVI(windoze)
    
  KEYWORDS
  pix film movie
    
  DESCRIPTION

  -----------------------------------------------------------------*/
class GEM_EXTERN filmDS : public film {
 public:
  
  //////////
  // Constructor
  filmDS(int format=0);

  //////////
  // Destructor
  virtual ~filmDS();

#if defined(__WIN32__) && defined(HAVE_DIRECTSHOW)
  //////////
  // open a movie up
  virtual bool open(char *filename, int format=0);
  //////////
  // close the movie file
  virtual void close(void);

  //////////
  // get the next frame
  virtual pixBlock* getFrame();

  //////////
  // set the next frame to read;
  virtual int changeImage(int imgNum, int trackNum=-1);


  // the raw buffer for decoding...
  int			m_nRawBuffSize;
  unsigned char*	m_RawBuffer;
  GLint                 m_format;

  int 	    	m_reqFrame;

  pixBlock		*m_pixBlock;

  unsigned char*  m_frame;  /* this points to a buffer for decompression */

  int			m_xsize;
  int			m_ysize;
  int			m_csize;

 protected:

  private:
	IBaseFilter				*VideoFilter;		// Base Filter for video
	IBaseFilter				*SampleFilter;		// Sample filter
	IBaseFilter				*NullFilter;		// Null render base Filter for video
	ISampleGrabber			*SampleGrabber;		// Sample grabber
	IGraphBuilder			*FilterGraph;		// Filter Graph for movie playback
	ISampleGrabber			*VideoGrabber;		// Video grabber
	IMediaControl			*MediaControl;		// MediaControl interface
	IMediaSeeking			*MediaSeeking;		// MediaSeeking interface
	IMediaPosition			*MediaPosition;		// MediaPosition interface
	LONGLONG				m_Duration;			// Duration of video
	LONGLONG				m_LastFrame;		// Last frame

	unsigned long		m_GraphRegister;

#endif //AVI

};

#endif	// for header file
