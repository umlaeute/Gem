////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-1998 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
/*
    this is an attempt at a Linux version of pix_video by Miller Puckette.
    Anyone conversant in c++ will probably howl at this.  I'm uncertain of
    several things.
    
    First, the #includes I threw in pix_video.h may not all be necessary; I
    notice that far fewer are needed for the other OSes.
    
    Second, shouldn't the os-dependent state variables be "private"?  I
    followed the lead of the other os-dependent state variables.  Also,
    I think the indentation is goofy but perhaps there's some reason for it.

    Third, I probably shouldn't be using sprintf to generate filenames; I
    don't know the "modern" c++ way to do this.
    
    Fourth, I don't know why some state variables 
    show up as "arguments" in the pix_video :: pix_video().
     
    This code is written with the "bttv" device in mind, which memory mapes
    images up to 24 bits per pixel.  So we request the whole 24 and don't
    settle for anything of lower quality (nor do we offer anything of higher
    quality; it seems that Gem is limited to 32 bits per pixel including
    alpha.)  We take all video images to be opaque by setting the alpha
    channel to 255.

*/
#ifdef __linux__
    
#include "videoDV4L.h"

#define N_BUF 4

#define PAL 0
#define NTSC 1

/////////////////////////////////////////////////////////
//
// videoDV4L
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
videoDV4L :: videoDV4L(int format) : video(format)
{
  post("ieee1394 for linux");
  m_channel = 0;//0x63;
  m_devicenum  = 0;
  m_norm = PAL;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
videoDV4L :: ~videoDV4L()
{
    if (m_haveVideo)stopTransfer();
}
#ifdef HAVE_DV
/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void *videoDV4L :: capturing(void*you)
{
  videoDV4L *me=(videoDV4L *)you;
  me->m_capturing=true;
  while(me->m_continue_thread){
    //post("thread %d\t%x %x", me->frame, me->tvfd, me->vmmap);
    if (ioctl(me->dvfd, DV1394_RECEIVE_FRAMES, 1) < 0)    {
      perror("receiving...");
    }
    me->m_lastframe=me->m_frame;
    me->m_frame++;
    me->m_frame%=N_BUF;
    me->m_frame_ready = true;
  }  
  me->m_capturing=false;
  return NULL;
}
pixBlock *videoDV4L :: getFrame(){
  //  post("getting frame %d", m_frame_ready);
  if (!m_frame_ready) m_image.newimage = 0;
  else {
    struct dv1394_status state;    
    ioctl(dvfd, DV1394_GET_STATUS, &state);
    post("STATUS: %d (%d, %d, %d)", state.active_frame, 
	 state.first_clear_frame, state.n_clear_frames, state.dropped_frames);
    m_image.image.data=videobuf+(m_lastframe);
    m_image.newimage = 1;
    m_frame_ready = false;
  }
  return &m_image;
}

/////////////////////////////////////////////////////////
// startTransfer
//
/////////////////////////////////////////////////////////
int videoDV4L :: startTransfer(int format)
{
  char buf[255];
  static struct dv1394_init init = {
    DV1394_API_VERSION, // api version 
    m_channel,              // isochronous transmission channel
    N_BUF,                  // number of frames in ringbuffer
    (m_norm==NTSC)?DV1394_NTSC:DV1394_PAL,         // PAL or NTSC
    0, 0 , 0                // default packet rate
  };
  post("starting transfer");
  if (m_devicenum<0){
    sprintf(buf, "/dev/dv1394");
  } else sprintf(buf, "/dev/video1394/%d", m_devicenum);
  int i;
  if ((dvfd = open(buf, O_RDWR)) < 0)    {
    perror(buf);
    goto closit;
  }

  post("init %s", buf);
  if (ioctl(dvfd, DV1394_INIT, &init) < 0)    {
    perror("initializing");
    goto closit;
  }

  if (!(videobuf = (unsigned char *)
    	mmap(0,
	     N_BUF*(m_norm==NTSC)?DV1394_NTSC_FRAME_SIZE:DV1394_PAL_FRAME_SIZE,
	     PROT_READ|PROT_WRITE,
	     MAP_SHARED, 
	     dvfd, 
	     0)))    {
    perror("mmap");
    goto closit;
  }
  post("ok");
  return 1;
closit:
  post("closed");
    return(0);
}

/////////////////////////////////////////////////////////
// stopTransfer
//
/////////////////////////////////////////////////////////
int videoDV4L :: stopTransfer()
{
  /* close the dv4l device and dealloc buffer */
  /* terminate thread if there is one */
  m_continue_thread=false;
  while(m_capturing){};
  ioctl(dvfd, DV1394_SHUTDOWN);

  return(1);
}

/////////////////////////////////////////////////////////
// dimenMess
//
/////////////////////////////////////////////////////////
int videoDV4L :: setDimen(int x, int y, int leftmargin, int rightmargin,
    int topmargin, int bottommargin)
{
    return 0;
}

int videoDV4L :: setNorm(char*norm)
{
  return 0;
}

int videoDV4L :: setChannel(int c, t_float f){
  return 0;
}

int videoDV4L :: setDevice(int d)
{
  if (d==m_devicenum)return 0;
  return 0;
}


int videoDV4L :: setColor(int format)
{
  if (format<=0 || format==m_reqFormat)return -1;
  return 0;
}

#endif // HAVE_DV
#endif // linux
