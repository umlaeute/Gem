/* -----------------------------------------------------------------

GEM - Graphics Environment for Multimedia

Load an digital video (like AVI, Mpeg, Quicktime) into a pix block 
(OS independant parent-class)

Copyright (c) 1997-1999 Mark Danks. mark@danks.org
Copyright (c) G�nther Geiger. geiger@epy.co.at
Copyright (c) 2001-2003 IOhannes m zmoelnig. forum::f�r::uml�ute. IEM. zmoelnig@iem.kug.ac.at
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_FILMAVIPLAY_H_
#define INCLUDE_FILMAVIPLAY_H_
  
#include "Pixes/film.h"

#if defined (_WINDOWS) & defined (HAVE_LIBAVIPLAY)
   // un windows there are other ways...
#undef HAVE_LIBAVIPLAY
#endif

#ifdef HAVE_LIBAVIPLAY
   /* this used to be <avifile/avifile.h> 
    * but on my system it changed to <avifile-0.7/avifile.h>
    * so we now find the correct path via "configure"
    */
#include "avifile.h"
#include "StreamInfo.h"

   // some version checking...
#ifndef IMG_FMT_RGB24
#undef HAVE_LIBAVIPLAY
#endif // IMG_FMT_RGB24
#endif

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  filmAVIPLAY
    
  film-loader class for AVIPLAY(linux)
    
  KEYWORDS
  pix film movie
    
  DESCRIPTION

  -----------------------------------------------------------------*/
class GEM_EXTERN filmAVIPLAY : public film {
 public:
  
  //////////
  // Constructor
  filmAVIPLAY(int format=0);

  //////////
  // Destructor
  ~filmAVIPLAY();

#ifdef HAVE_LIBAVIPLAY
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

 protected:
  IAviReadFile *m_avifile;
  IAviReadStream  *m_avistream;
  CImage *m_aviimage;
#endif //AVIPLAY
  unsigned char *m_rawdata;
  long           m_rawlength;
};

#endif	// for header file
