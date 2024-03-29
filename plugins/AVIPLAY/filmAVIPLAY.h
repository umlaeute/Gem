/* -----------------------------------------------------------------

GEM - Graphics Environment for Multimedia

Load an digital video (like AVI, Mpeg, Quicktime) into a pix block
(OS-independent parent-class)

Copyright (c) 1997-1999 Mark Danks. mark@danks.org
Copyright (c) Günther Geiger. geiger@epy.co.at
Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef _INCLUDE_GEMPLUGIN__FILMAVIPLAY_FILMAVIPLAY_H_
#define _INCLUDE_GEMPLUGIN__FILMAVIPLAY_FILMAVIPLAY_H_
#include "plugins/film.h"
#include "Gem/Image.h"

#if defined (_WIN32) & defined (HAVE_LIBAVIPLAY)
// un windows there are other ways...
#undef HAVE_LIBAVIPLAY
#endif

#ifdef HAVE_LIBAVIPLAY
/* this used to be <avifile/avifile.h>
 * but on my system it changed to <avifile-0.7/avifile.h>
 * so we now find the correct path via "configure"
 */

// ugly hack, since avifile.h does weird things if HAVE_CONFIG_H is defined
# undef HAVE_CONFIG_H

# include "avifile.h"
# include "infotypes.h"
# include "image.h"

// some version checking...
# ifndef IMG_FMT_RGB24
#  undef HAVE_LIBAVIPLAY
# endif // IMG_FMT_RGB24

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
namespace gem
{
namespace plugins
{
class GEM_EXPORT filmAVIPLAY : public film
{
public:

  //////////
  // Constructor
  filmAVIPLAY(void);

  //////////
  // Destructor
  ~filmAVIPLAY(void);

#ifdef HAVE_LIBAVIPLAY
  //////////
  // open a movie up
  virtual bool open(const std::string&filename, const gem::Properties&);
  //////////
  // close the movie file
  virtual void close(void);

  //////////
  // get the next frame
  virtual pixBlock* getFrame(void);

  //////////
  // set the next frame to read;
  virtual errCode changeImage(int imgNum, int trackNum=-1);

  // can be used within a threaded context
  virtual bool isThreadable(void)
  {
    return true;
  }

  // Property handling
  virtual bool enumProperties(gem::Properties&readable,
                              gem::Properties&writeable);
  virtual void setProperties(gem::Properties&props);
  virtual void getProperties(gem::Properties&props);

protected:
  unsigned int  m_wantedFormat; // format requested by the user
  double m_fps;  // the frame-rate
  int  m_numFrames, m_numTracks; // number of frames in video
  int m_curFrame, m_curTrack;
  pixBlock m_image; // output image

  bool m_readNext; // indicates whether we should perform a seek/read
  bool m_newfilm;


  IAviReadFile *m_avifile;
  IAviReadStream  *m_avistream;


# ifdef AVM_BEGIN_NAMESPACE
  avm::CImage *m_aviimage;
# else
  CImage *m_aviimage;
# endif
#endif //AVIPLAY
  unsigned char *m_rawdata;
  long           m_rawlength;
};
};
};

#endif  // for header file
