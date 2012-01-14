/* -----------------------------------------------------------------

GEM - Graphics Environment for Multimedia

Load an digital video (like AVI, Mpeg, Quicktime) into a pix block
(OS independant parent-class)

Copyright (c) 1997-1999 Mark Danks. mark@danks.org
Copyright (c) Günther Geiger. geiger@epy.co.at
Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef _INCLUDE_GEMPLUGIN__FILMDS_FILMDS_H_
#define _INCLUDE_GEMPLUGIN__FILMDS_FILMDS_H_
#include "plugins/film.h"
#include "Gem/Image.h"

#include <dshow.h>
#include <qedit.h>

   /*-----------------------------------------------------------------
     -------------------------------------------------------------------
     CLASS
     filmDS

     film-loader class for AVI(windoze)

     KEYWORDS
     pix film movie

     DESCRIPTION

     -----------------------------------------------------------------*/
namespace gem { namespace plugins {
class GEM_EXPORT filmDS : public film {
 public:

     //////////
     // Constructor
     filmDS(void);

     //////////
     // Destructor
     virtual ~filmDS(void);

     //////////
     // open a movie up
     virtual bool open(const std::string filename, const gem::Properties&);
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
     virtual bool isThreadable(void) { return true; }

     // Property handling
     virtual bool enumProperties(gem::Properties&readable,gem::Properties&writeable);
     virtual void setProperties(gem::Properties&props);
     virtual void getProperties(gem::Properties&props);


 protected:
     int m_numFrames; // number of frames in video
     pixBlock m_image; // output image

     int 	    m_reqFrame;

     unsigned char*  m_frame;  /* this points to a buffer for decompression */

     int			m_xsize;
     int			m_ysize;
     int			m_csize;

     double         m_auto;


 private:
     IBaseFilter				*VideoFilter;		// Base Filter for video
     IBaseFilter				*SampleFilter;		// Sample filter
     IBaseFilter				*NullFilter;		// Null render base Filter for video
     ISampleGrabber			*SampleGrabber;		// Sample grabber
     IGraphBuilder			*FilterGraph;		// Filter Graph for movie playback
     IMediaControl			*MediaControl;		// MediaControl interface
     IMediaSeeking			*MediaSeeking;		// MediaSeeking interface
     IMediaPosition			*MediaPosition;		// MediaPosition interface
     LONGLONG				m_Duration;			// Duration of video
     LONGLONG				m_LastFrame;		// Last frame

     unsigned long		m_GraphRegister;

};};   };

#endif	// for header file
