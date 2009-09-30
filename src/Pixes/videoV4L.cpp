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
#include "Base/GemConfig.h"
#include "videoV4L.h"

#ifndef HAVE_LIBV4L1
# define v4l1_open open
# define v4l1_close close
# define v4l1_dup dup
# define v4l1_ioctl ioctl
# define v4l1_read read
# define v4l1_mmap mmap
# define v4l1_munmap munmap
#endif /* libv4l-1 */

#if 0
# define debug ::post
#else
# define debug
#endif

/////////////////////////////////////////////////////////
//
// videoV4L
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
videoV4L :: videoV4L(int format) : video(format)
#ifdef HAVE_VIDEO4LINUX
                                 ,
                                   tvfd(0),
                                   frame(0),last_frame(0), 
                                   videobuf(NULL), 
                                   skipnext(0),
                                   mytopmargin(0), mybottommargin(0), 
                                   myleftmargin(0), myrightmargin(0),
                                   m_gotFormat(0),m_colorConvert(false),
                                   m_thread_id(0),
                                   m_continue_thread(false), m_frame_ready(false),
                                   m_rendering(false),
                                   m_stopTransfer(false)
#endif
{
#ifdef HAVE_VIDEO4LINUX
  if (!m_width)m_width=64;
  if (!m_height)m_height=64;

  m_capturing=false;
  m_channel=V4L_COMPOSITEIN; 
  m_norm=VIDEO_MODE_AUTO; 
  m_devicenum=V4L_DEVICENO;

  provide("video4linux");
  provide("v4l");
#endif /* HAVE_VIDEO4LINUX */
}

////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
videoV4L :: ~videoV4L()
{
#ifdef HAVE_VIDEO4LINUX
  if (m_haveVideo)stopTransfer();
#endif /* HAVE_VIDEO4LINUX */
}

#ifdef HAVE_VIDEO4LINUX


////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////


////////////////////////////////////////////////////////
// frame grabber
//
/////////////////////////////////////////////////////////
void *videoV4L::capturing_(void*you)
{
  videoV4L *me=(videoV4L *)you;
  return me->capturing();
}
void *videoV4L :: capturing()
{
  m_capturing=true;
  int errorcount=0;
  debug("v4l::thread starting");
  while(m_continue_thread){
    bool noerror=true;
  
    //debug("thread %d\t%x %x", frame, tvfd, vmmap);

    //  frame = !frame;
    frame++;
    frame%=V4L_NBUF;

    vmmap[frame].width = m_image.image.xsize + 
      myleftmargin + myrightmargin;
    vmmap[frame].height = m_image.image.ysize + 
      mytopmargin + mybottommargin;
    
    /* syncing */
    if (v4l1_ioctl(tvfd, VIDIOCSYNC, &vmmap[frame].frame) < 0)
      {
        perror("v4l: VIDIOCSYNC");
        noerror=false;
        //m_haveVideo = 0;stopTransfer();
      }

    /* capturing */
    if (v4l1_ioctl(tvfd, VIDIOCMCAPTURE, &vmmap[frame]) < 0)
      {
        if (errno == EAGAIN)
          error("v4l: can't sync (no v4l source?)");
        else 
          perror("v4l: VIDIOCMCAPTURE1");

        /* let's try again... */
        if (v4l1_ioctl(tvfd, VIDIOCMCAPTURE, &vmmap[frame]) < 0) {
          perror("v4l: VIDIOCMCAPTURE2");
          noerror=false;
        }
        /*
          verbose(1, "frame %d %d, format %d, width %d, height %d\n",
          frame, vmmap[frame].frame, vmmap[frame].format,
          vmmap[frame].width, vmmap[frame].height);
        */
        //stopTransfer();m_haveVideo = 0;
      }
    if(noerror){
      errorcount=0;
    } else {
      errorcount++;
      if(errorcount>1000) {
        error("v4L: %d capture errors in a row... I think I better stop now...", errorcount);
        m_continue_thread=false;
        m_stopTransfer=true;
      }
    }

    m_frame_ready = 1;
    last_frame=frame;
  }
  debug("v4l::thread exiting");
  m_capturing=false;
  return NULL;
}

pixBlock *videoV4L :: getFrame(){
  if(!m_haveVideo)return NULL;
  if(m_stopTransfer) {
    /* transfer has been stopped dues to errors in the thread
     * it should be resumed whenever possible 
     */
    bool rendering=m_rendering;
    stopTransfer();
    m_rendering=rendering;
    return NULL;
  }
  m_image.newfilm=0;
  if (!m_frame_ready) m_image.newimage = 0;
  else {
    if (m_colorConvert){
      m_image.image.notowned = false;
      switch(m_gotFormat){
      case VIDEO_PALETTE_YUV420P: m_image.image.fromYUV420P(videobuf + vmbuf.offsets[last_frame]); break;
      case VIDEO_PALETTE_RGB24:   m_image.image.fromBGR    (videobuf + vmbuf.offsets[last_frame]); break;
      case VIDEO_PALETTE_RGB32:   m_image.image.fromBGRA   (videobuf + vmbuf.offsets[last_frame]); break;
      case VIDEO_PALETTE_GREY:    m_image.image.fromGray   (videobuf + vmbuf.offsets[last_frame]); break;
      case VIDEO_PALETTE_YUV422:  m_image.image.fromYUV422 (videobuf + vmbuf.offsets[last_frame]); break;

      default: // ? what should we do ?
        m_image.image.data=videobuf + vmbuf.offsets[last_frame];
        m_image.image.notowned = true;
      }
    } else {
      m_image.image.data=videobuf + vmbuf.offsets[last_frame];
      m_image.image.notowned = true;
    }
    m_image.image.upsidedown=true;
    
    m_image.newimage = 1;
    m_frame_ready = false;
  }
  return &m_image;
}

/////////////////////////////////////////////////////////
// startTransfer
//
/////////////////////////////////////////////////////////
int videoV4L :: startTransfer(int format)
{
  if(m_capturing)stopTransfer(); // just in case we are already running!
  m_rendering=true;
  m_stopTransfer=false;
  if (format>1)m_reqFormat=format;
  //  verbose(1, "starting transfer");
  char buf[256];
  int i;
  int width, height;

  frame = 0;
  skipnext = 0;
  last_frame = 0;

  if(m_devicename){
    snprintf(buf,256,"%s", m_devicename);
    buf[255]=0;
  } else {
    if (m_devicenum<0){
      sprintf(buf, "/dev/video");
    } else {
      snprintf(buf, 256, "/dev/video%d", m_devicenum);
      buf[255]=0;
    }
  }
  
  if ((tvfd = v4l1_open(buf, O_RDWR)) < 0)
    {
      error("v4l: failed opening device: '%s'", buf);
      perror(buf);
      goto closit;
    }
  if (v4l1_ioctl(tvfd, VIDIOCGCAP, &vcap) < 0)
    {
      perror("v4l: get capabilities");
      goto closit;
    }
  /*
    verbose(1, "cap: name %s type %d channels %d maxw %d maxh %d minw %d minh %d\n",
    vcap.name, vcap.type,  vcap.channels,  vcap.maxwidth,  vcap.maxheight,
    vcap.minwidth,  vcap.minheight);
  */
  if (v4l1_ioctl(tvfd, VIDIOCGPICT, &vpicture) < 0)
    {
      perror("v4l: VIDIOCGPICT");
      goto closit;
    }
  /*
    verbose(1, "picture: brightness %d depth %d palette %d\n",
    vpicture.brightness, vpicture.depth, vpicture.palette);
  */
  for (i = 0; i < vcap.channels; i++)
    {
      vchannel.channel = i;
      if (v4l1_ioctl(tvfd, VIDIOCGCHAN, &vchannel) < 0)
        {
          perror("v4l: VDIOCGCHAN");
          goto closit;
        }
      /*
        verbose(1, "channel %d name %s type %d flags %d\n",
        vchannel.channel, vchannel.name, 
        vchannel.type, vchannel.flags);
      */
    }
  vchannel.channel = ((vcap.channels-1)<m_channel)?(vcap.channels-1):m_channel;
  //verbose(1, "setting to channel %d", vchannel.channel);
  if (v4l1_ioctl(tvfd, VIDIOCGCHAN, &vchannel) < 0)
    {
      perror("v4l: VDIOCGCHAN");
      goto closit;
    }

  vchannel.norm = m_norm;
  if (v4l1_ioctl(tvfd, VIDIOCSCHAN, &vchannel) < 0)
    {
      perror("v4l: VDIOCSCHAN");
      goto closit;
    }


  /* get mmap numbers */
  if (v4l1_ioctl(tvfd, VIDIOCGMBUF, &vmbuf) < 0)
    {
      perror("v4l: VIDIOCGMBUF");
      goto closit;
    }
  /*
    verbose(1, "buffer size %d, frames %d, offset %d %d\n", vmbuf.size,
    vmbuf.frames, vmbuf.offsets[0], vmbuf.offsets[1]);
  */
  if (!(videobuf = (unsigned char *)
        v4l1_mmap(0, vmbuf.size, PROT_READ|PROT_WRITE, MAP_SHARED, tvfd, 0)))
    {
      perror("v4l: mmap");
      goto closit;
    }

  width = (m_width > vcap.minwidth) ? m_width :  vcap.minwidth;   
  width = (width > vcap.maxwidth) ? vcap.maxwidth : width;
  height = m_height > vcap.minheight ? m_height : vcap.minheight;
  height = (height > vcap.maxheight) ? vcap.maxheight : height;

  //verbose(1, "wanted format is 0x%X", m_reqFormat);
  for (i = 0; i < V4L_NBUF; i++)    {
    switch(m_reqFormat){
    case GL_LUMINANCE:
    	vmmap[i].format = VIDEO_PALETTE_GREY;
      break;
    case GL_RGBA:
    case GL_BGRA:
    	vmmap[i].format = VIDEO_PALETTE_RGB24;
      break;
    case GL_YCBCR_422_GEM:
#if 0
      VIDEO_PALETTE_GREY	1	/* Linear greyscale */
        VIDEO_PALETTE_HI240	2	/* High 240 cube (BT848) */
        VIDEO_PALETTE_RGB565	3	/* 565 16 bit RGB */
        VIDEO_PALETTE_RGB24	4	/* 24bit RGB */
        VIDEO_PALETTE_RGB32	5	/* 32bit RGB */
        /* note: all of my devices give BGR instead of RGB (jmz) */
        /* note: RGB32 unfortunately sets the alpha-channel 0! */
        VIDEO_PALETTE_RGB555	6	/* 555 15bit RGB */
        VIDEO_PALETTE_YUV422	7	/* YUV422 capture */
        VIDEO_PALETTE_YUYV	8
        VIDEO_PALETTE_UYVY	9	/* The great thing about standards is ... */
        VIDEO_PALETTE_YUV420	10
        VIDEO_PALETTE_YUV411	11	/* YUV411 capture */
        VIDEO_PALETTE_RAW	12	/* RAW capture (BT848) */
        VIDEO_PALETTE_YUV422P	13	/* YUV 4:2:2 Planar */
        VIDEO_PALETTE_YUV411P	14	/* YUV 4:1:1 Planar */
        VIDEO_PALETTE_YUV420P	15	/* YUV 4:2:0 Planar */
        VIDEO_PALETTE_YUV410P	16	/* YUV 4:1:0 Planar */
#endif
        /* this is very unfortunate:
         * PALETTE_YUV422 obviously is something different than ours
         * although our yuv422 reads uyvy it is
         * not PALETTE_UYVY either !
         */
        vmmap[i].format = VIDEO_PALETTE_YUV420P;
      break;
    default:
    case GL_RGB:
    case GL_BGR:
    	vmmap[i].format = VIDEO_PALETTE_RGB24;
    }

    vmmap[i].width = width;
    vmmap[i].height = height;
    vmmap[i].frame  = i;
  }

  //verbose(1, "setting cmcapture to %dx%d\t%d", vmmap[0].width,  vmmap[0].height, vmmap[0].format);

  if (v4l1_ioctl(tvfd, VIDIOCMCAPTURE, &vmmap[frame]) < 0)    {
    for (i = 0; i < V4L_NBUF; i++)vmmap[i].format = vpicture.palette;
    //verbose(1, "now trying standard palette %d", vmmap[0].format);
    if (v4l1_ioctl(tvfd, VIDIOCMCAPTURE, &vmmap[frame]) < 0)    {
      if (errno == EAGAIN)
        error("v4l: can't sync (no video source?)");
      else 
        perror("v4l: VIDIOCMCAPTURE");
      //goto closit;
    }
  }
  /*
    verbose(1, "frame %d %d, format %d, width %d, height %d\n",
    frame, vmmap[frame].frame, vmmap[frame].format,
    vmmap[frame].width, vmmap[frame].height);
  */
    
  /* fill in image specifics for Gem pixel object.  Could we have
     just used RGB, I wonder? */
  m_image.image.xsize = vmmap[frame].width;
  m_image.image.ysize = vmmap[frame].height;
  m_image.image.setCsizeByFormat(m_reqFormat);
  m_image.image.reallocate();

  switch((m_gotFormat=vmmap[frame].format)){
  case VIDEO_PALETTE_GREY  : m_colorConvert=(m_reqFormat!=GL_LUMINANCE); break;
  case VIDEO_PALETTE_RGB24 : m_colorConvert=(m_reqFormat!=GL_BGR); break;
  case VIDEO_PALETTE_RGB32 : m_colorConvert=(m_reqFormat!=GL_BGRA); break;
  case VIDEO_PALETTE_YUV422: m_colorConvert=(m_reqFormat!=GL_YCBCR_422_GEM); break;
  default: m_colorConvert=true;
  }
  
  myleftmargin = 0;
  myrightmargin = 0;
  mytopmargin = 0;
  mybottommargin = 0;
    
  m_haveVideo = 1;

  /* create thread */
  m_continue_thread = true;
  m_frame_ready = 0;
  pthread_create(&m_thread_id, 0, capturing_, this);

  verbose(1, "v4l::startTransfer opened video connection %X", tvfd);

  return(1);

 closit:
  verbose(1, "v4l::startTransfer closing %d", tvfd);
  if (tvfd >= 0)
    {
    	v4l1_close(tvfd);
      tvfd = -1;
    }
  m_haveVideo = 0;
  m_frame_ready=0;
  return(0);
}

/////////////////////////////////////////////////////////
// stopTransfer
//
/////////////////////////////////////////////////////////
int videoV4L :: stopTransfer()
{
  if(m_haveVideo){ /* are we running ? */
    /* close the v4l device and dealloc buffer */
    /* terminate thread if there is one */
    if(m_continue_thread){
      void *dummy;
      m_continue_thread = 0;
      pthread_join (m_thread_id, &dummy);
    }
    while(m_capturing){verbose(1, "waiting for thread");}
    v4l1_munmap(videobuf, vmbuf.size);
  }
  if (tvfd) v4l1_close(tvfd);
  tvfd = 0;
  m_haveVideo = 0;
  m_frame_ready = 0;
  m_rendering=false;
  verbose(0, "v4l: stopped Transfer");
  return(1);
}

/////////////////////////////////////////////////////////
// dimenMess
//
/////////////////////////////////////////////////////////
int videoV4L :: setDimen(int x, int y, int leftmargin, int rightmargin,
                         int topmargin, int bottommargin)
{

  int xtotal = x + leftmargin + rightmargin;
  int ytotal = y + topmargin + bottommargin;
  if (xtotal > vcap.maxwidth) /* 844 */
    error("v4l: x dimensions too great");
  else if (xtotal < vcap.minwidth || x < 1 || leftmargin < 0 || rightmargin < 0)
    error("v4l: x dimensions too small");
  if (ytotal > vcap.maxheight)
    error("v4l: y dimensions too great");
  else if (ytotal < vcap.minheight || y < 1 ||
           topmargin < 0 || bottommargin < 0)
    error("v4l: y dimensions too small");

  myleftmargin = leftmargin;
  myrightmargin = rightmargin;
  mytopmargin = topmargin;
  mybottommargin = bottommargin;

  m_width=x;
  m_height=y;
  m_image.image.xsize = x;
  m_image.image.ysize = y;

  m_image.image.reallocate();
  skipnext = 1;
  return 0;
}

int videoV4L :: setNorm(char*norm)
{
  char c=*norm;
  int i_norm=-1;

  switch (c){
  case 'p':
  case 'P':
    i_norm = VIDEO_MODE_PAL;
    break;
  case 'n':
  case 'N':
    i_norm = VIDEO_MODE_NTSC;
    break;
  default:
    error("pix_video[v4l]: unknown norm");
    return -1;
    break;
  }
  //  if (i_norm==m_norm)return 0;
  if(m_capturing){
    debug("v4l: restarting transfer");
    stopTransfer();
    m_norm=i_norm;
    startTransfer();

  }  
  m_norm=i_norm;
  return 0;
}

int videoV4L :: setChannel(int c, t_float f){
  int freq = (int) f;
  if (freq>0){
    if (c>-1)m_channel=c;
    vtuner.tuner = m_channel;
    if (v4l1_ioctl(tvfd,VIDIOCGTUNER,&vtuner) < 0) {
      error("pix_video[v4l]: error setting frequency -- no tuner");
      return -1;
    }
    verbose(1, "setting freq: %d", freq);
    if (v4l1_ioctl(tvfd,VIDIOCSFREQ,&freq) < 0) {
      error("pix_video[v4l]: error setting frequency");
      return -1;
    }
  } else {
    if(c>-1 && m_channel!=c){
      if (m_capturing){
        stopTransfer();
        m_channel=c;
        startTransfer();
      }
      m_channel=c;
    }
  }
  return 0;
}

int videoV4L :: setDevice(int d)
{
  m_devicename=NULL;
  if (d==m_devicenum)return 0;
  m_devicenum=d;
  bool rendering=m_rendering;
  if(m_capturing)stopTransfer();
  if (rendering)startTransfer();
  //  verbose(1, "new device set %d", m_devicenum);
  return 0;
}
int videoV4L :: setDevice(char*name)
{
  m_devicenum=-1;
  m_devicename=name;
  bool rendering=m_rendering;
  if(m_capturing)stopTransfer();
  if (rendering)startTransfer();
  //  verbose(1, "new device set %d", m_devicenum);
  return 0;
}

int videoV4L :: setColor(int format)
{
  if (format<=0 || format==m_reqFormat)return -1;
  if(m_capturing){
    stopTransfer();
    startTransfer(format);
  }
  m_reqFormat=format;
  return 0;
}
#endif /* HAVE_VIDEO4LINUX */
