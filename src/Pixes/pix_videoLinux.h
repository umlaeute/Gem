/*-----------------------------------------------------------------

    GEM - Graphics Environment for Multimedia

    Load an video into a pix block

    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyright (c) Günther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

    Linux version by Miller Puckette. msp@ucsd.edu
	
-----------------------------------------------------------------*/

#ifndef INCLUDE_PIX_VIDEOLINUX_H_
#define INCLUDE_PIX_VIDEOLINUX_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/time.h>
#include <linux/types.h>
#include <linux/videodev.h>
#include <sys/mman.h>
#include <pthread.h>

#define DEVICENO 0
#define NBUF 2
#define COMPOSITEIN 1

#include "Pixes/pix_video.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
	pix_video
    
    Loads in a video
    
KEYWORDS
    pix
    
DESCRIPTION

    "dimen" (int, int) - set the x,y dimensions
    "zoom" (int, int) - the zoom factor (1.0 is nominal) (num / denom)
    "bright" (int) - the brightnes
    "contrast" (int) - the contrast
    "hue" (int) - the hue
    "sat" (int) - the saturation
    
-----------------------------------------------------------------*/
class GEM_EXTERN pix_videoLinux : public pix_video
{
    CPPEXTERN_HEADER(pix_videoLinux, GemBase)

    public:

        //////////
        // Constructor
    	pix_videoLinux(t_floatarg, t_floatarg);

    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~pix_videoLinux();

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
    
    	//////////
    	// Set the video dimensions

	/* in Linux the camera's own dimensions are specifiable; the
	   image is resized appropriately.  So we have 6 independent components
	   to specify.  X and Y give the OUTPUT dimensions as a pix object;
	   the margins are added to that to make the rectangle filled by the
	   video input. */
    	void	    	dimenMess(int x, int y, int leftmargin, int rightmargin,
    	    	    	    int topmargin, int bottommargin);
    	void	    	channelMess(int f);
    	void	    	freqMess(t_floatarg f);
    	void	    	modeMess(int argc, t_atom *argv);
    	void	    	deviceMess(int d);

    	//////////
    	// Start up the video device
    	// [out] int - returns 0 if bad
    	int	    	startTransfer();
    
    	//////////
    	// Stop the video device
    	// [out] int - returns 0 if bad
    	int	   	stopTransfer();
    
	//-----------------------------------
	// GROUP:	Linux specific video data
	//-----------------------------------
	
	struct video_tuner vtuner;
	struct video_picture vpicture;
	struct video_buffer vbuffer;
	struct video_capability vcap;
	struct video_channel vchannel;
	struct video_audio vaudio;
	struct video_mbuf vmbuf;
	struct video_mmap vmmap[NBUF];
	int tvfd;
	int frame, last_frame;
	unsigned char *videobuf;
	int skipnext;
	int mytopmargin, mybottommargin;
	int myleftmargin, myrightmargin;

	int m_width;
	int m_height;
	int m_channel;
	int m_norm;

	int m_devicenum;

	pthread_t m_thread_id;
	bool      m_continue_thread;
	bool      m_frame_ready;

    private:
    	
    	//////////
    	// static member functions

    	static void freqMessCallback(void *data, t_floatarg f);
    	static void channelMessCallback(void *data, t_floatarg f);
    	static void deviceMessCallback(void *data, t_floatarg f);
     	static void modeMessCallback(void *data, t_symbol* norm, int argc, t_atom *argv);

	//////////
	// the capturing thread
	static void*capturing(void*);


};

#endif	// for header file
