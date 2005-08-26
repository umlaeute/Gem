/*-----------------------------------------------------------------

GEM - Graphics Environment for Multimedia

Load an digital video (like AVI, Mpeg, Quicktime) into a pix block (Linux)

Copyright (c) 1997-1999 Mark Danks. mark@danks.org
Copyright (c) Günther Geiger. geiger@epy.co.at
Copyright (c) 2001-2003 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.


-----------------------------------------------------------------*/

#ifndef INCLUDE_PIX_FILMLINUX_H_
#define INCLUDE_PIX_FILMLINUX_H_

#include "Pixes/pix_film.h"
#ifdef HAVE_LIBQUICKTIME
#include <quicktime.h>
#include <colormodels.h>
#endif // QT
#ifdef HAVE_LIBMPEG3
#include <libmpeg3.h>
#else 
#ifdef HAVE_LIBMPEG
#include <mpeg.h>
#endif // MPEG
#endif // MPEG3
#ifdef HAVE_LIBAVIPLAY
#include <avifile.h>
#include <StreamInfo.h>
#ifndef IMG_FMT_RGB24
#undef HAVE_LIBAVIPLAY
#endif // IMG_FMT_RGB24
#endif

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  pix_filmLinux
  
  Loads in a film
  
  KEYWORDS
  pix
  
  DESCRIPTION

  -----------------------------------------------------------------*/
class GEM_EXTERN pix_filmLinux : public pix_film
{
  CPPEXTERN_HEADER(pix_filmLinux, GemBase)

    public:

  //////////
  // Constructor
  pix_filmLinux(t_symbol *filename);

 protected:

  //////////
  // Destructor
  virtual ~pix_filmLinux();

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

  FILE         *m_streamfile;

#ifdef HAVE_LIBQUICKTIME
  quicktime_t  *quick_file;
#endif
#ifdef HAVE_LIBMPEG3
  mpeg3_t      *mpeg_file;
#else
#ifdef HAVE_LIBMPEG
  ImageDesc     m_streamVid;
#endif
#endif
#ifdef HAVE_LIBAVIPLAY
  IAviReadFile *m_avifile;
  IAviReadStream  *m_avistream;
  CImage *m_aviimage;
#endif

  //-----------------------------------
  // GROUP:	Texturing
  //-----------------------------------

 protected:
};

#endif	// for header file
