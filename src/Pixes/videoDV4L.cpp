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

#ifdef __linux__
    
#include "videoDV4L.h"

#define N_BUF 2 /*DV1394_MAX_FRAMES/4*/
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
#ifdef HAVE_DV
  post("ieee1394 for linux");
  m_channel = 0;//0x63;
  m_devicenum  = 0;
  m_norm = PAL;
  m_decoder=NULL;
  m_frame_ready=false;
  decodedbuf = new unsigned char[725*576*40];
#endif
  m_haveVideo=false;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
videoDV4L :: ~videoDV4L(){
#ifdef HAVE_DV
  if(m_haveVideo)stopTransfer();
  if(decodedbuf)delete[]decodedbuf;
  if(m_decoder!=NULL)dv_decoder_free(m_decoder);
#endif
}
#ifdef HAVE_DV
/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void *videoDV4L :: capturing(void*you)
{
  videoDV4L *me=(videoDV4L *)you;
  dv_decoder_t *decoder = me->m_decoder;
  int fd=me->dvfd;
  int framesize = me->m_framesize;
  struct dv1394_status dvst;
  int n_frames = N_BUF;
  unsigned char* mmapbuf = me->m_mmapbuf;
  unsigned char* dvdata = NULL;       // encoded data (somewhere in the mmap'ed region)
  unsigned char** data = &me->videobuf; // decoded data

  /* this will hang if no ieee1394-device is present, what to do about it ??? */
  me->m_haveVideo=false;
  if(ioctl(fd, DV1394_WAIT_FRAMES, 1)) {
    perror("error: ioctl WAIT_FRAMES");
    me->m_capturing=false; return NULL;
  }
  if (ioctl(fd, DV1394_GET_STATUS, &dvst))   {
    perror("ioctl GET_STATUS");
    me->m_capturing=false; return NULL;
  }
  me->m_haveVideo=true;
  me->m_capturing=true;

  while(me->m_continue_thread){
    if(ioctl(fd, DV1394_WAIT_FRAMES, n_frames - 1)) {
      perror("error: ioctl WAIT_FRAMES");
      me->m_capturing=false; return NULL;
    }
    if (ioctl(fd, DV1394_GET_STATUS, &dvst))   {
      perror("ioctl GET_STATUS");
      me->m_capturing=false; return NULL;
    }
    /*
      dvst.init
      dvst.active_frame
      dvst.first_clear_frame
      dvst.n_clear_frames
      dvst.dropped_frames
    */	
    if (dvst.dropped_frames > 0) {
      post("dv1394: dropped at least %d frames.", dvst.dropped_frames);
    }
    /*
      memcpy( g_current_frame->data, 
      (g_dv1394_map + (dvst.first_clear_frame * DV1394_PAL_FRAME_SIZE)),
      DV1394_PAL_FRAME_SIZE );
    */
    me->videobuf = mmapbuf + (dvst.first_clear_frame * framesize);

    //post("thread %d\t%x %x", me->frame, me->tvfd, me->vmmap);
    if (ioctl(fd, DV1394_RECEIVE_FRAMES, 1) < 0)    {
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
  if (!m_decoder)return NULL;
  if (!m_frame_ready) m_image.newimage = 0;
  else {
    dv_parse_header(m_decoder, videobuf);
    //dv_parse_packs (m_decoder, videobuf);
    if(dv_frame_changed(m_decoder)) {
      int pitches[3] = {0,0,0};
            //      pitches[0]=m_decoder->width*3; // rgb
      pitches[0]=m_decoder->width*((m_reqFormat==GL_RGBA)?3:2);
      m_image.image.ysize=m_decoder->height;
      m_image.image.xsize=m_decoder->width;
      m_image.image.setCsizeByFormat(m_reqFormat);
      
      /* decode the DV-data to something we can handle and that is similar to the wanted format */
      //      dv_report_video_error(m_decoder, videobuf);  // do we need this ?
      dv_decode_full_frame(m_decoder, videobuf, ((m_reqFormat==GL_RGBA)?e_dv_color_rgb:e_dv_color_yuv), &decodedbuf, pitches); // gosh, this(e_dv_color_rgb) is expansive:: the decoding is done in software only...
      //     post("sampling %d", m_decoder->sampling);

      /* convert the colour-space to the one we want */
      // btw. shouldn't this be done in [pix_video] rather than here ?
      if (m_reqFormat==GL_RGBA)m_image.image.fromRGB(decodedbuf);
      else m_image.image.fromYVYU(decodedbuf);
    }

    m_image.newimage=1;
    m_frame_ready = false;
  }
  return &m_image;
}

/////////////////////////////////////////////////////////
// openDevice
//
/////////////////////////////////////////////////////////
int videoDV4L :: openDevice(int devnum, int format){
  int fd = -1;
  char buf[255];
  struct dv1394_init init = {
    DV1394_API_VERSION, // api version 
    0x63,              // isochronous transmission channel
    N_BUF,             // number of frames in ringbuffer
    (m_norm==NTSC)?DV1394_NTSC:DV1394_PAL,         // PAL or NTSC
    0, 0 , 0                // default packet rate
  };
  m_framesize=(m_norm==NTSC)?DV1394_NTSC_FRAME_SIZE:DV1394_PAL_FRAME_SIZE;

  if (devnum<0)devnum=0;
  sprintf(buf, "/dev/ieee1394/dv/host%d/%s/in", devnum, (m_norm==NTSC)?"NTSC":"PAL");
  if ((fd = open(buf, O_RDWR)) < 0)    {
    perror(buf);
    return -1;
  }

  if (ioctl(fd, DV1394_INIT, &init) < 0)    {
    perror("initializing");
    close(fd);
    return -1;
  }
  
  m_mmapbuf = (unsigned char *) mmap( NULL, N_BUF*m_framesize,
				       PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
  if(m_mmapbuf == MAP_FAILED) {
    perror("mmap frame buffers");
    close(fd);
    return -1;
  }
  
  if(ioctl(fd, DV1394_START_RECEIVE, NULL)) {
    perror("dv1394 START_RECEIVE ioctl");
    close(fd);
    return -1;
  }
  return(fd);
}
/////////////////////////////////////////////////////////
// resetDevice
//
/////////////////////////////////////////////////////////
int videoDV4L :: resetDevice(void){

}
/////////////////////////////////////////////////////////
// closeDevice
//
/////////////////////////////////////////////////////////
void videoDV4L :: closeDevice(void){
  if(m_mmapbuf!=NULL)munmap(m_mmapbuf, N_BUF*m_framesize);
  if(dvfd>=0)close(dvfd);
  m_haveVideo=false;
}

/////////////////////////////////////////////////////////
// startTransfer
//
/////////////////////////////////////////////////////////
int videoDV4L :: startTransfer(int format)
{
  if ((dvfd=openDevice(m_devicenum, format))<0){
    post("closed");
    return(0);
  }
  m_image.newimage=0;
  m_image.image.data=0;
  m_image.image.xsize=720;
  m_image.image.ysize=576;
  m_image.image.setCsizeByFormat(m_reqFormat);
  m_image.image.reallocate();
  videobuf=NULL;
  

  if(m_decoder!=NULL)dv_decoder_free(m_decoder);
  if (!(m_decoder=dv_decoder_new(true, true, true))){
    post("unable to create DV-decoder...closing");
    closeDevice();
    return(0);
  }  
  m_decoder->quality=DV_QUALITY_BEST;

  m_continue_thread = true;
  pthread_create(&m_thread_id, 0, capturing, this);
  return 1;
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
  int i=0;
  if(m_haveVideo){
    while(m_capturing){usleep(10);i++;}
    post("shutting down dv1394 after %d usec", i*10);
    ioctl(dvfd, DV1394_SHUTDOWN);
  }
  closeDevice();
  return(1);
}

/////////////////////////////////////////////////////////
// dimenMess
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
  if(m_haveVideo){
    stopTransfer();
    startTransfer();
  }
  return 0;
}

int videoDV4L :: setDevice(int d){
  if (d==m_devicenum)return 0;
  m_devicenum=d;
#if 0
  // need this commented out for testing
  if(m_haveVideo){
    stopTransfer();
    startTransfer();
  }
#endif
  return 0;
}

int videoDV4L :: setColor(int format){
  if (format<=0)return -1;
  m_reqFormat=format;
  return 0;
}

#endif // HAVE_DV
#endif // linux
