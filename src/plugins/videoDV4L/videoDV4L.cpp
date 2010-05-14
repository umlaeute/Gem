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
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "videoDV4L.h"
using namespace gem;

#include "Gem/RTE.h"

#define N_BUF 2 /*DV1394_MAX_FRAMES/4*/
#define PAL 0
#define NTSC 1

#ifdef HAVE_DV
REGISTER_VIDEOFACTORY("dv4l", videoDV4L);

/////////////////////////////////////////////////////////
//
// videoDV4L
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////

videoDV4L :: videoDV4L() : video()
{
  m_channel = 0;//0x63;
  m_devicenum  = 0;
  m_norm = PAL;
  m_decoder=NULL;
  m_frame_ready=false;
  m_quality = DV_QUALITY_BEST;
  decodedbuf = new unsigned char[720*576*3];

  provide("ieee1394");
  provide("dv4l");
  provide("dv");
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
videoDV4L :: ~videoDV4L(){
  if(m_haveVideo)stopTransfer();
  if(decodedbuf)delete[]decodedbuf;
  if(m_decoder!=NULL)dv_decoder_free(m_decoder);
}
/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
bool videoDV4L :: grabFrame() {
  struct dv1394_status dvst;
  int n_frames = N_BUF;

  if(ioctl(dvfd, DV1394_WAIT_FRAMES, n_frames - 1)) {
    perror("error: ioctl WAIT_FRAMES");
    return false;
  }
  if (ioctl(dvfd, DV1394_GET_STATUS, &dvst))   {
    perror("ioctl GET_STATUS");
    return false;
  }
  if (dvst.dropped_frames > 0) {
    verbose(1,"dv1394: dropped at least %d frames", dvst.dropped_frames);
  }

  videobuf = m_mmapbuf + (dvst.first_clear_frame * m_framesize);
  if (ioctl(dvfd, DV1394_RECEIVE_FRAMES, 1) < 0)    {
    perror("receiving...");
  }
  dv_parse_header(m_decoder, videobuf);
  if(dv_frame_changed(m_decoder)) {
    int pitches[3] = {0,0,0};
    //      pitches[0]=m_decoder->width*3; // rgb
    //      pitches[0]=m_decoder->width*((m_reqFormat==GL_RGBA)?3:2);
    pitches[0]=m_decoder->width*2;
    
    /* decode the DV-data to something we can handle and that is similar to the wanted format */
    //      dv_report_video_error(m_decoder, videobuf);  // do we need this ?
    // gosh, this(e_dv_color_rgb) is expansive:: the decoding is done in software only...
    //      dv_decode_full_frame(m_decoder, videobuf, ((m_reqFormat==GL_RGBA)?e_dv_color_rgb:e_dv_color_yuv), &decodedbuf, pitches);
    dv_decode_full_frame(m_decoder, videobuf, e_dv_color_yuv, &decodedbuf, pitches);

    lock();
    m_image.newimage=true;

    m_image.image.ysize=m_decoder->height;
    m_image.image.xsize=m_decoder->width;
    m_image.image.setCsizeByFormat(m_reqFormat);

    m_image.image.fromYVYU(decodedbuf);
    unlock();
  }

  return true;
}

/////////////////////////////////////////////////////////
// openDevice
//
/////////////////////////////////////////////////////////
bool videoDV4L :: openDevice(){
  /*
  If video feed is already open and "device <something>" message is passed
  this will segfault Pd. Hence, we need to check if the device is already open
  before trying to open it again.

  Ico Bukvic ico@vt.edu 2-18-07
  */
  if(m_haveVideo){
    verbose(1, "Stream already going on. Doing some clean-up...");
    stopTransfer();
  }

  /*
  All of the errors in this method return -1 anyhow, so fd should be 0 to allow
  successful open if everything goes ok.

  Ico Bukvic ico@vt.edu 2-18-07
  */
  int fd = 0; 
  struct dv1394_init init = {
    DV1394_API_VERSION, // api version 
    0x63,              // isochronous transmission channel
    N_BUF,             // number of frames in ringbuffer
    (m_norm==NTSC)?DV1394_NTSC:DV1394_PAL,         // PAL or NTSC
    0, 0 , 0                // default packet rate
  };

  m_framesize=(m_norm==NTSC)?DV1394_NTSC_FRAME_SIZE:DV1394_PAL_FRAME_SIZE;

  if(m_devicename){
    if ((fd = ::open(m_devicename, O_RDWR)) < 0) {
        perror(m_devicename);
        return -1;
    }
  } else {
    signed char devnum=(m_devicenum<0)?0:(signed char)m_devicenum;
    char buf[256];
    buf[255]=0;buf[32]=0;buf[33]=0;
    if (devnum<0)devnum=0;
    snprintf(buf, 32, "/dev/ieee1394/dv/host%d/%s/in", devnum, (m_norm==NTSC)?"NTSC":"PAL");
    if ((fd = ::open(buf, O_RDWR)) < 0)    {
      snprintf(buf, 32, "/dev/dv1394/%d", devnum);
      if ((fd = ::open(buf, O_RDWR)) < 0) {
	if ((fd=::open("/dev/dv1394", O_RDWR)) < 0)    {
	  perror(buf);
	  return -1;
	}
      }
    }
  }
  if (ioctl(fd, DV1394_INIT, &init) < 0)    {
    perror("initializing");
    ::close(fd);
    return -1;
  }
  
  m_mmapbuf = (unsigned char *) mmap( NULL, N_BUF*m_framesize,
				       PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
  if(m_mmapbuf == MAP_FAILED) {
    perror("mmap frame buffers");
    ::close(fd);
    return -1;
  }
  
  if(ioctl(fd, DV1394_START_RECEIVE, NULL)) {
    perror("dv1394 START_RECEIVE ioctl");
    ::close(fd);
    return -1;
  }
  /*Extra verbosity never hurt anyone...

  Ico Bukvic ico@vt.edu 2-18-07
  */
  verbose(1, "DV4L: Successfully opened...");
  return(fd);
}

/////////////////////////////////////////////////////////
// closeDevice
//
/////////////////////////////////////////////////////////
void videoDV4L :: closeDevice(void){
  if(dvfd) {
    verbose(1, "DV4L: shutting down dv1394");
    ioctl(dvfd, DV1394_SHUTDOWN);
  }
  if(m_mmapbuf!=NULL)munmap(m_mmapbuf, N_BUF*m_framesize);
  if(dvfd>=0)::close(dvfd);
  m_haveVideo=false;
}

/////////////////////////////////////////////////////////
// startTransfer
//
/////////////////////////////////////////////////////////
bool videoDV4L :: startTransfer()
{
  if(dvfd<0) {
    // forgot to open?
    dvfd=openDevice();
  }
  if (dvfd<0){
    verbose(1, "DV4L: closed");
    return false;
  }
      
  m_image.newimage=0;
  m_image.image.data=0;
  m_image.image.xsize=720;
  m_image.image.ysize=576;
  m_image.image.setCsizeByFormat(m_reqFormat);
  m_image.image.reallocate();
  videobuf=NULL;

  m_frame_ready = false; 

  if(m_decoder!=NULL)dv_decoder_free(m_decoder);m_decoder=NULL;

  if (!(m_decoder=dv_decoder_new(true, true, true))){
    error("DV4L: unable to create DV-decoder...closing");
    closeDevice();
    return false;
  }

  m_decoder->quality=m_quality;
  verbose(1, "DV4L: DV decoding quality %d ", m_decoder->quality);

  return true;
}

/////////////////////////////////////////////////////////
// stopTransfer
//
/////////////////////////////////////////////////////////
bool videoDV4L :: stopTransfer()
{
  return true;
}

/////////////////////////////////////////////////////////
// normMess
//
/////////////////////////////////////////////////////////
int videoDV4L :: setNorm(char*norm){
  int inorm = m_norm;
  switch(norm[0]){
  case 'N': case 'n':
    inorm=NTSC;
    break;
  case 'P': case 'p':
    inorm=PAL;
    break;
  }
  if (inorm==m_norm)return 0;
  m_norm=inorm;
  return 0;
}

int videoDV4L :: setDevice(int d){
  m_devicename=NULL;
  if (d==m_devicenum)return 0;
  m_devicenum=d;

  if(m_haveVideo){
    stopTransfer();
    startTransfer();
  }
  return 0;
}
int videoDV4L :: setDevice(char*name){
  m_devicenum=-1;
  m_devicename=name;

  if(m_haveVideo){
    stopTransfer();
    startTransfer();
  }
  return 0;
}

int videoDV4L :: setColor(int format){
  if (format<=0)return -1;
  m_reqFormat=format;
  return 0;
}

/////////////////////////////////////////
//
// Set the quality for DV decoding
//
/////////////////////////////////////////
int videoDV4L :: setQuality(int quality){
  if (quality<0)return -1;
  if (quality>5)return -1;
  m_quality=quality;

  if(m_haveVideo){
    stopTransfer();
    startTransfer();
  }  
  return 0;
}

#else // ! HAVE_DV
/////////////////////////////////////////////////////////
//
// videoDV4L without DV-support
//
videoDV4L :: videoDV4L() : video()
{ }
videoDV4L :: ~videoDV4L()
{ }
#endif // HAVE_DV
