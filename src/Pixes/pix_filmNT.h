/*-----------------------------------------------------------------

GEM - Graphics Environment for Multimedia

Load an digital video (like AVI, Mpeg, Quicktime) into a pix block (Windows(tm))

Copyright (c) 1997-1999 Mark Danks. mark@danks.org
Copyright (c) G�nther Geiger. geiger@epy.co.at
Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f�r::uml�ute. IEM. zmoelnig@iem.kug.ac.at
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_PIX_FILMNT_H_
#define INCLUDE_PIX_FILMNT_H_


#include "Pixes/pix_film.h"

#include <vfw.h>
/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  pix_filmNT
    
  Loads in a film
    
  KEYWORDS
  pix
    
  DESCRIPTION

  -----------------------------------------------------------------*/
class GEM_EXTERN pix_filmNT : public pix_film
{
  CPPEXTERN_HEADER(pix_filmNT, GemBase)

    public:

  //////////
  // Constructor
  pix_filmNT(t_symbol *filename);

 protected:
  //////////
  // Destructor
  virtual ~pix_filmNT();

  //////////
  // close the movie file
  virtual void closeMess(void);
  //////////
  // open a movie up
  virtual void realOpen(char *filename);

  //////////
  // Do the rendering
  virtual void getFrame();    

  //-----------------------------------
  // GROUP:	Movie data
  //-----------------------------------

  PAVISTREAM	m_streamVid;		// the stream itself

	int		m_nRawBuffSize;

	BITMAPINFOHEADER* m_pbmihRaw;
	BITMAPINFOHEADER* m_pbmihDst;

	HIC m_hic;

	unsigned char*	m_RawBuffer;

	imageStruct m_decodedFrame; // the decoded frame, in a "AVI-native" colorspace
	imageStruct m_gemFrame;     // the decoded frame, in a "GEM-native" colorspace


 protected:
	
  //////////
  // static member functions
  static void changeImageCallback(void *data, t_symbol *, int argc, t_atom *argv);
  static void autoCallback       (void *data, t_floatarg state);
};

#endif	// for header file
