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
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "videoV4L.h"
using namespace gem;

#include "Gem/RTE.h"

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
#ifdef HAVE_VIDEO4LINUX
REGISTER_VIDEOFACTORY("v4l", videoV4L);


videoV4L :: videoV4L() : video("v4l")
		       ,
			 tvfd(0),
			 frame(0),
			 videobuf(NULL), 
			 skipnext(0),
			 mytopmargin(0), mybottommargin(0), 
			 myleftmargin(0), myrightmargin(0),
			 m_gotFormat(0),m_colorConvert(false),
			 errorcount(0)
			 
{
  if (!m_width)m_width=64;
  if (!m_height)m_height=64;

  m_capturing=false;
  m_channel=V4L_COMPOSITEIN; 
  m_norm=VIDEO_MODE_AUTO; 
  m_devicenum=V4L_DEVICENO;

  provide("analog");
}

////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
videoV4L :: ~videoV4L()
{
  if (m_haveVideo)stopTransfer();
}


////////////////////////////////////////////////////////
// frame grabber
//
/////////////////////////////////////////////////////////
bool videoV4L :: grabFrame() {
  bool noerror=true;

  frame++;
  frame%=V4L_NBUF;

  vmmap[frame].width = m_image.image.xsize + myleftmargin + myrightmargin;
  vmmap[frame].height = m_image.image.ysize + mytopmargin + mybottommargin;
  
  /* syncing */
  if (v4l1_ioctl(tvfd, VIDIOCSYNC, &vmmap[frame].frame) < 0)
    {
      perror("v4l: VIDIOCSYNC");
      noerror=false;
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
    }
  if(noerror){
    errorcount=0;
  } else {
    errorcount++;
    if(errorcount>1000) {
      error("v4L: %d capture errors in a row... I think I better stop now...", errorcount);
      return false;
    }
  }
  
  lock();
  if (m_colorConvert){
    m_image.image.notowned = false;
    switch(m_gotFormat){
    case VIDEO_PALETTE_YUV420P: m_image.image.fromYUV420P(videobuf + vmbuf.offsets[frame]); break;
    case VIDEO_PALETTE_RGB24:   m_image.image.fromBGR    (videobuf + vmbuf.offsets[frame]); break;
    case VIDEO_PALETTE_RGB32:   m_image.image.fromBGRA   (videobuf + vmbuf.offsets[frame]); break;
    case VIDEO_PALETTE_GREY:    m_image.image.fromGray   (videobuf + vmbuf.offsets[frame]); break;
    case VIDEO_PALETTE_YUV422:  m_image.image.fromYUV422 (videobuf + vmbuf.offsets[frame]); break;
      
    default: // ? what should we do ?
      m_image.image.data=videobuf + vmbuf.offsets[frame];
      m_image.image.notowned = true;
    }
  } else {
    m_image.image.data=videobuf + vmbuf.offsets[frame];
    m_image.image.notowned = true;
  }
  m_image.image.upsidedown=true;
  
  m_image.newimage = 1;
  unlock();
  return true;
}

/////////////////////////////////////////////////////////
// openDevice
//
/////////////////////////////////////////////////////////
bool videoV4L :: openDevice()
{
  char buf[256];

  if(!m_devicename.empty()){
    snprintf(buf,256,"%s", m_devicename.c_str());
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

  return true;

 closit:
  if (tvfd >= 0)
    {
      v4l1_close(tvfd);
      tvfd = -1;
    }
  return false;
}
/////////////////////////////////////////////////////////
// closeDevice
//
/////////////////////////////////////////////////////////
void videoV4L :: closeDevice() {
  if (tvfd>0) v4l1_close(tvfd);
  tvfd = -1;
}


/////////////////////////////////////////////////////////
// startTransfer
//
/////////////////////////////////////////////////////////
bool videoV4L :: startTransfer()
{
  if(tvfd<0)return false;
  int i;
  int width, height;


  errorcount=0;

  frame = 0;
  skipnext = 0;

  if (v4l1_ioctl(tvfd, VIDIOCGPICT, &vpicture) < 0)
    {
      perror("v4l: VIDIOCGPICT");
      return false;
    }
  for (i = 0; i < vcap.channels; i++)
    {
      vchannel.channel = i;
      if (v4l1_ioctl(tvfd, VIDIOCGCHAN, &vchannel) < 0)
        {
          perror("v4l: VDIOCGCHAN");
          return false;
        }
    }
  vchannel.channel = ((vcap.channels-1)<m_channel)?(vcap.channels-1):m_channel;
  if (v4l1_ioctl(tvfd, VIDIOCGCHAN, &vchannel) < 0)
    {
      perror("v4l: VDIOCGCHAN");
      return false;
    }

  vchannel.norm = m_norm;
  if (v4l1_ioctl(tvfd, VIDIOCSCHAN, &vchannel) < 0)
    {
      perror("v4l: VDIOCSCHAN");
      return false;
    }

  /* get mmap numbers */
  if (v4l1_ioctl(tvfd, VIDIOCGMBUF, &vmbuf) < 0)
    {
      perror("v4l: VIDIOCGMBUF");
      return false;
    }

  if (!(videobuf = (unsigned char *)
        v4l1_mmap(0, vmbuf.size, PROT_READ|PROT_WRITE, MAP_SHARED, tvfd, 0)))
    {
      perror("v4l: mmap");
      return false;
    }

  width = (m_width  > vcap.minwidth ) ? m_width        :  vcap.minwidth;   
  width = (width    > vcap.maxwidth ) ? vcap.maxwidth  : width;
  height =(m_height > vcap.minheight) ? m_height       : vcap.minheight;
  height =(height   > vcap.maxheight) ? vcap.maxheight : height;

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

  if (v4l1_ioctl(tvfd, VIDIOCMCAPTURE, &vmmap[frame]) < 0)    {
    for (i = 0; i < V4L_NBUF; i++)vmmap[i].format = vpicture.palette;
    if (v4l1_ioctl(tvfd, VIDIOCMCAPTURE, &vmmap[frame]) < 0)    {
      if (errno == EAGAIN)
        error("v4l: can't sync (no video source?)");
      else 
        perror("v4l: VIDIOCMCAPTURE");
    }
  }
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

  verbose(1, "v4l::startTransfer opened video connection %X", tvfd);
  return true;
}

/////////////////////////////////////////////////////////
// stopTransfer
//
/////////////////////////////////////////////////////////
bool videoV4L :: stopTransfer()
{
  if(!m_capturing)return false;
  v4l1_munmap(videobuf, vmbuf.size);
  m_capturing=false;
  return true;
}

/////////////////////////////////////////////////////////
// dimenMess
//
/////////////////////////////////////////////////////////
bool videoV4L :: setDimen(int x, int y, int leftmargin, int rightmargin,
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

bool videoV4L :: setNorm(const std::string norm)
{
  const char c=*norm.c_str();
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
  m_norm=i_norm;
  restartTransfer();
  return 0;
}

bool videoV4L :: setChannel(int c, t_float f){
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
      m_channel=c;
      restartTransfer();
    }
  }
  return 0;
}

bool videoV4L :: setDevice(int d)
{
  m_devicename.empty();
  if (d==m_devicenum)return 0;
  m_devicenum=d;
  restartTransfer();
  return 0;
}
bool videoV4L :: setDevice(const std::string name)
{
  m_devicenum=-1;
  m_devicename=name;
  restartTransfer();
  return 0;
}

bool videoV4L :: setColor(int format)
{
  if (format<=0 || format==m_reqFormat)return -1;
  m_reqFormat=format;
  restartTransfer();
  return 0;
}
#else
videoV4L :: videoV4L() : video("")
{ }
videoV4L :: ~videoV4L()
{ }
#endif /* HAVE_VIDEO4LINUX */
