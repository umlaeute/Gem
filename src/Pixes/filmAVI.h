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

#ifndef INCLUDE_FILMAVI_H_
#define INCLUDE_FILMAVI_H_
  
#include "Pixes/film.h"

#ifdef _WINDOWS
#include <vfw.h>
#endif

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  filmAVI
    
  film-loader class for AVI(windoze)
    
  KEYWORDS
  pix film movie
    
  DESCRIPTION

  -----------------------------------------------------------------*/
class GEM_EXTERN filmAVI : public film {
 public:
  
  //////////
  // Constructor
  filmAVI(int format=0);

  //////////
  // Destructor
  virtual ~filmAVI();

#ifdef _WINDOWS
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

  unsigned char*  m_frame;  /* this points to a buffer for decompression */

 protected:
  BITMAPINFOHEADER* m_pbmihRaw;
  BITMAPINFOHEADER* m_pbmihDst;

  HIC m_hic;

  PGETFRAME	m_getFrame;		// the frame information
  PAVISTREAM	m_streamVid;		// the stream itself
#endif //AVI

};

#endif	// for header file
