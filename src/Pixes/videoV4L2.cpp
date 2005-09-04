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
#include "videoV4L2.h"

/////////////////////////////////////////////////////////
//
// videoV4L2
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
videoV4L2 :: videoV4L2(int format) : video(format)
#ifdef HAVE_VIDEO4LINUX2
                                   , m_gotFormat(0), m_colorConvert(0),
                                     m_tvfd(0), m_deviceName(NULL),
                                     m_buffers(NULL), m_nbuffers(0), 
                                     m_currentBuffer(NULL),
                                     m_frame(0), m_last_frame(0),
                                     m_maxwidth(844), m_minwidth(32),
                                     m_maxheight(650), m_minheight(32),
                                     m_thread_id(0), m_continue_thread(false), m_frame_ready(false),
                                     m_rendering(false)
{
  if (!m_width)m_width=320;
  if (!m_height)m_height=240;
  m_capturing=false;
  m_devicenum=DEVICENO;
  post("video4linux2");
#else
{
#endif /* HAVE_VIDEO4LINUX2 */
}
  
/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
videoV4L2 :: ~videoV4L2()
{
#ifdef HAVE_VIDEO4LINUX2
    if (m_haveVideo)stopTransfer();
#endif /* HAVE_VIDEO4LINUX2 */
}

#ifdef HAVE_VIDEO4LINUX2
static int xioctl(int                    fd,
                  int                    request,
                  void *                 arg)
{
  int r;

  do r = ioctl (fd, request, arg);
  while (-1 == r && EINTR == errno);

  return r;
}

int videoV4L2::init_mmap (void)
{
  struct v4l2_requestbuffers req;

  memset (&(req), 0, sizeof (req));

  req.count               = NBUF;
  req.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  req.memory              = V4L2_MEMORY_MMAP;

  if (-1 == xioctl (m_tvfd, VIDIOC_REQBUFS, &req)) {
    if (EINVAL == errno) {
      error("%s does not support "
               "memory mapping\n", m_deviceName);
      return 0;
    } else {
      error ("VIDIOC_REQBUFS");
      return 0;
    }
  }

  if (req.count < NBUF) {
    error("Insufficient buffer memory on %s", m_deviceName);
    return(0);
  }

  m_buffers = (t_v4l2_buffer*)calloc (req.count, sizeof (*m_buffers));

  if (!m_buffers) {
    error("Out of memory");
    return(0);
  }

  for (m_nbuffers = 0; m_nbuffers < req.count; ++m_nbuffers) {
    struct v4l2_buffer buf;

    memset (&(buf), 0, sizeof (buf));

    buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory      = V4L2_MEMORY_MMAP;
    buf.index       = m_nbuffers;

    if (-1 == xioctl (m_tvfd, VIDIOC_QUERYBUF, &buf)){
      error ("VIDIOC_QUERYBUF");
      return(0);
    }

    m_buffers[m_nbuffers].length = buf.length;
    m_buffers[m_nbuffers].start =
      mmap (NULL /* start anywhere */,
            buf.length,
            PROT_READ | PROT_WRITE /* required */,
            MAP_SHARED /* recommended */,
            m_tvfd, buf.m.offset);

    if (MAP_FAILED == m_buffers[m_nbuffers].start){
      error ("mmap");
      return 0;
    }
  }
  return 1;
}

/////////////////////////////////////////////////////////
// this is the work-horse
// a thread that does the capturing
//
/////////////////////////////////////////////////////////
#define depost
void *videoV4L2 :: capturing(void*you)
{
  videoV4L2 *me=(videoV4L2 *)you;
  t_v4l2_buffer*buffers=me->m_buffers;

  struct v4l2_buffer buf;
  unsigned int i;
    
  fd_set fds;
  struct timeval tv;
  int r;

  int m_tvfd=me->m_tvfd;
  me->m_capturing=true;

  FD_ZERO (&fds);
  FD_SET (m_tvfd, &fds);

  depost("memset");
  memset(&(buf), 0, sizeof (buf));
  
  buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  buf.memory = V4L2_MEMORY_MMAP;

  while(me->m_continue_thread){
    depost("grab");

    me->m_frame++;
    me->m_frame%=NBUF;

    
#if 1
    /* Timeout. */
    tv.tv_sec = 2;
    tv.tv_usec = 0;

    tv.tv_sec = 2;
    tv.tv_usec = 0;

    depost("select");
    //    r = select (m_tvfd + 1, &fds, NULL, NULL, &tv);
    r = select (m_tvfd + 1, &fds, NULL, NULL, &tv);

    if (-1 == r) {
      if (EINTR == errno)
        continue;
      error ("select");//exit
    }
    if (0 == r) {
      error("select timeout");
      me->m_continue_thread=false;
    }
#endif

    depost("dqbuf");
    if (-1 == xioctl (m_tvfd, VIDIOC_DQBUF, &buf)) {
      switch (errno) {
      case EAGAIN:
        return 0;
      case EIO:
        /* Could ignore EIO, see spec. */
        /* fall through */
      default:
        error ("VIDIOC_DQBUF");
      }
    }

    //error("grabbed %d", buf.index);

    me->m_currentBuffer=buffers[buf.index].start;
    //process_image (m_buffers[buf.index].start);

    depost("qbuf");
    if (-1 == xioctl (m_tvfd, VIDIOC_QBUF, &buf)){
      error ("VIDIOC_QBUF");
    }
    
    me->m_frame_ready = 1;
    me->m_last_frame=me->m_frame;
  }

  // stop capturing
  if(me->m_tvfd){
    enum v4l2_buf_type type;
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (-1 == xioctl (me->m_tvfd, VIDIOC_STREAMOFF, &type)){
      error ("VIDIOC_STREAMOFF");
    }
  }
  me->m_capturing=false;
  return NULL;
}

//////////////////
// this reads the data that was captured by capturing() and returns it within a pixBlock
pixBlock *videoV4L2 :: getFrame(){
  if(!m_haveVideo)return NULL;
  //post("getting frame %d", m_frame_ready);
  m_image.newfilm=0;
  if (!m_frame_ready) m_image.newimage = 0;
  else {
    int i=0; // FIXME
    unsigned char*data=(unsigned char*)m_currentBuffer;
    if (m_colorConvert){
      m_image.image.notowned = false;
      switch(m_gotFormat){
      case V4L2_PIX_FMT_RGB24: m_image.image.fromRGB   (data); break;
      case V4L2_PIX_FMT_RGB32: m_image.image.fromRGBA  (data); break;
      case V4L2_PIX_FMT_GREY : m_image.image.fromGray  (data); break;
      case V4L2_PIX_FMT_UYVY : m_image.image.fromYUV422(data); break;

      default: // ? what should we do ?
	m_image.image.data=data;
	m_image.image.notowned = true;
      }
    } else {
      m_image.image.data=data;
      m_image.image.notowned = true;
    }
    m_image.image.upsidedown=true;
    
    m_image.newimage = 1;
    m_frame_ready = false;
  }
  return &m_image;
}
/////////////////////////////////////////////////////////
// restartTransfer
//
/////////////////////////////////////////////////////////
void videoV4L2 :: restartTransfer()
{
  bool rendering=m_rendering;
  if(m_capturing)stopTransfer();
  if (rendering)startTransfer();
}


/////////////////////////////////////////////////////////
// startTransfer
//
/////////////////////////////////////////////////////////
int videoV4L2 :: startTransfer(int format)
{
  m_rendering=true;
  if (format>1)m_reqFormat=format;
  //  verbose(1, "starting transfer");
  char buf[256];
  char*dev_name=m_deviceName;
  int i;

  struct stat st; 
  struct v4l2_capability cap;
  struct v4l2_cropcap cropcap;
  struct v4l2_crop crop;
  struct v4l2_format fmt;
  unsigned int min;

  enum v4l2_buf_type type;

  m_frame = 0;
  m_last_frame = 0;


  /* check the device */

  // if we don't have a devicename, create one
  if(!dev_name){
    if (m_devicenum<0){
      sprintf(buf, "/dev/video");
    } else {
      sprintf(buf, "/dev/video%d", m_devicenum);
    }
    dev_name=buf;
  }
  
  // try to open the device
  post("device: %s", dev_name);
  if (-1 == stat (dev_name, &st)) {
    error("Cannot identify '%s': %d, %s\n", dev_name, errno, strerror (errno));
    goto closit;
  }

  if (!S_ISCHR (st.st_mode)) {
    error("%s is no device\n", dev_name);
    goto closit;
  }

  m_tvfd = open (dev_name, O_RDWR /* required */ | O_NONBLOCK, 0);

  if (-1 == m_tvfd) {
    error("Cannot open '%s': %d, %s\n",
             dev_name, errno, strerror (errno));
    goto closit;
  }

  // by now, we have an open file-descriptor
  // check whether this is really a v4l2-device
  if (-1 == xioctl (m_tvfd, VIDIOC_QUERYCAP, &cap)) {
    if (EINVAL == errno) {
      error("%s is no V4L2 device",  dev_name);
      goto closit;
    } else {
      error ("VIDIOC_QUERYCAP");//exit
      goto closit;
    }
  }

  if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
    error("%s is no video capture device", dev_name);
    goto closit;
  }

  if (!(cap.capabilities & V4L2_CAP_STREAMING)) {
    error("%s does not support streaming i/o", dev_name);
    goto closit;
  }

  /* Select video input, video standard and tune here. */

  cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

  if (-1 == xioctl (m_tvfd, VIDIOC_CROPCAP, &cropcap)) {
    /* Errors ignored. */
  }

  crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  crop.c = cropcap.defrect; /* reset to default */

  if (-1 == xioctl (m_tvfd, VIDIOC_S_CROP, &crop)) {
    switch (errno) {
    case EINVAL:
      /* Cropping not supported. */
      break;
    default:
      /* Errors ignored. */
      break;
    }
  }

  memset (&(fmt), 0, sizeof (fmt));

  fmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  fmt.fmt.pix.width       = m_width;
  fmt.fmt.pix.height      = m_height;
  switch(m_reqFormat){
  case GL_YCBCR_422_GEM: fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_UYVY; break;
  case GL_LUMINANCE: fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_GREY; break;
  case GL_RGB: fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_RGB24; break;
  default: fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_RGB32; m_reqFormat=GL_RGBA;
  }
  fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;
  
  startpost("wanted %d (%d)\tgot ", m_reqFormat, fmt.fmt.pix.pixelformat);

  if (-1 == xioctl (m_tvfd, VIDIOC_S_FMT, &fmt)){
    error ("VIDIOC_S_FMT");//exit
  }
  
  // query back what we have set
  if (-1 == xioctl (m_tvfd, VIDIOC_G_FMT, &fmt)){
    error ("VIDIOC_G_FMT");//exit
  }

  m_gotFormat=fmt.fmt.pix.pixelformat;
  switch(m_gotFormat){
  case V4L2_PIX_FMT_RGB32: post("RGBA");break;
  case V4L2_PIX_FMT_UYVY: post("YUV ");break;
  case V4L2_PIX_FMT_GREY: post("gray");break;
  default: post("unknown %d",fmt.fmt.pix.pixelformat );
  }

  /* Note VIDIOC_S_FMT may change width and height. */

  /* Buggy driver paranoia. */
  min = fmt.fmt.pix.width * 2;
  if (fmt.fmt.pix.bytesperline < min)
    fmt.fmt.pix.bytesperline = min;
  min = fmt.fmt.pix.bytesperline * fmt.fmt.pix.height;
  if (fmt.fmt.pix.sizeimage < min)
    fmt.fmt.pix.sizeimage = min;

  if(!init_mmap ())goto closit;

  for (i = 0; i < m_nbuffers; ++i) {
    struct v4l2_buffer buf;
    
    memset (&(buf), 0, sizeof (buf));
    
    buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory      = V4L2_MEMORY_MMAP;
    buf.index       = i;
    
    if (-1 == xioctl (m_tvfd, VIDIOC_QBUF, &buf)){
      error ("VIDIOC_QBUF");//exit
    }
  }
		
  type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

  if (-1 == xioctl (m_tvfd, VIDIOC_STREAMON, &type)){
    error ("VIDIOC_STREAMON");//exit
  }
  
  /* fill in image specifics for Gem pixel object.  Could we have
     just used RGB, I wonder? */
  m_image.image.xsize = m_width;
  m_image.image.ysize = m_height;
  m_image.image.setCsizeByFormat(m_reqFormat);
  m_image.image.reallocate();
  
  post("format: %d -> %d", m_gotFormat, m_reqFormat);
  switch(m_gotFormat){
  case V4L2_PIX_FMT_GREY  : m_colorConvert=(m_reqFormat!=GL_LUMINANCE); break;
  case V4L2_PIX_FMT_RGB24 : m_colorConvert=(m_reqFormat!=GL_RGB); break;
  case V4L2_PIX_FMT_RGB32 : m_colorConvert=(m_reqFormat!=GL_RGBA); break;//RGB32!=RGBA; is it ARGB or ABGR?
  case V4L2_PIX_FMT_UYVY  : m_colorConvert=(m_reqFormat!=GL_YCBCR_422_GEM); break;
  default: m_colorConvert=true;
  }
  
  post("colorconvert=%d", m_colorConvert);
  
  m_haveVideo = 1;
  
  /* create thread */
  m_continue_thread = 1;
  m_frame_ready = 0;
  pthread_create(&m_thread_id, 0, capturing, this);
  
  post("GEM: pix_video: Opened video connection 0x%X", m_tvfd);
  
  return(1);
  
 closit:
  stopTransfer();
  return(0);
}

/////////////////////////////////////////////////////////
// stopTransfer
//
/////////////////////////////////////////////////////////
int videoV4L2 :: stopTransfer()
{
  int i=0;
  /* close the v4l2 device and dealloc buffer */
  /* terminate thread if there is one */
  if(m_continue_thread){
    void *dummy;
    m_continue_thread = 0;
    pthread_join (m_thread_id, &dummy);
  }
  while(m_capturing){post("waiting for thread");}

  // unmap the mmap
  for (i = 0; i < m_nbuffers; ++i)
    if (-1 == munmap (m_buffers[i].start, m_buffers[i].length)){
      // oops: couldn't unmap the memory
    }
  free (m_buffers);

  // close the file-descriptor
  if (m_tvfd) close(m_tvfd);

  m_tvfd = 0;
  m_haveVideo = 0;
  m_frame_ready = 0;
  m_rendering=false;
  return(1);
}

/////////////////////////////////////////////////////////
// dimenMess
//
/////////////////////////////////////////////////////////
int videoV4L2 :: setDimen(int x, int y, int leftmargin, int rightmargin,
    int topmargin, int bottommargin)
{
    int xtotal = x + leftmargin + rightmargin;
    int ytotal = y + topmargin + bottommargin;
    if (xtotal > m_maxwidth) /* 844 */
      error("x dimensions too great");
    else if (xtotal < m_minwidth || x < 1 || leftmargin < 0 || rightmargin < 0)
      error("x dimensions too small");
    if (ytotal > m_maxheight)
      error("y dimensions too great");
    else if (ytotal < m_minheight || y < 1 ||
	     topmargin < 0 || bottommargin < 0)
      error("y dimensions too small");

    m_width=x;
    m_height=y;
    m_image.image.xsize = x;
    m_image.image.ysize = y;

    m_image.image.reallocate();
    restartTransfer();
    return 0;
}

int videoV4L2 :: setNorm(char*norm)
{
  char c=*norm;
  int i_norm=-1;

  switch (c){
  case 'p':
  case 'P':
    i_norm = V4L2_STD_PAL;
    break;
  case 'n':
  case 'N':
    i_norm = V4L2_STD_NTSC;
    break;
  default:
    error("pix_video: unknown norm");
    return -1;
    break;
  }
  //  if (i_norm==m_norm)return 0;
  m_norm=i_norm;
  restartTransfer();
  return 0;
}

int videoV4L2 :: setChannel(int c, t_float f){
  return 0;
}

int videoV4L2 :: setDevice(int d)
{
  if (d==m_devicenum)return 0;
  m_devicenum=d;
  restartTransfer();
  //  verbose(1, "new device set %d", m_devicenum);
  return 0;
}


int videoV4L2 :: setColor(int format)
{
  if (format<=0 || format==m_reqFormat)return -1;
  m_reqFormat=format;
  restartTransfer();
  return 0;
}
#endif /* HAVE_VIDEO4LINUX2 */
