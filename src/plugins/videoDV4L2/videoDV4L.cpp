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

#ifdef HAVE_DV
#define N_BUF 2 /*DV1394_MAX_FRAMES/4*/


REGISTER_VIDEOFACTORY("dv4l2", videoDV4L);

/////////////////////////////////////////////////////////
//
// videoDV4L
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////

videoDV4L :: videoDV4L() : video(false)
{
  m_channel = 0;//0x63;
  m_devicenum  = 0;
  m_decoder=NULL;

  provide("dv4l2");
  provide("ieee1394");
  provide("dv4l");
  provide("dv");

  dv_init(1, 1); // singleton?
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
videoDV4L :: ~videoDV4L(){
  if(m_haveVideo)stopTransfer();
  if(m_decoder!=NULL)dv_decoder_free(m_decoder);

  dv_cleanup();
}


bool videoDV4L :: grabFrame(){
  /* this actually only transports the raw1394 stream
   * libiec will issue a callback when a frame is ready
   */
  fd_set rfds;
  FD_ZERO(&rfds);
  FD_SET(m_dvfd, &rfds);
  int rv = select(m_dvfd + 1, &rfds, NULL, NULL, NULL);
  if(rv > 0) {
    if(FD_ISSET(m_dvfd, &rfds)) {
      raw1394_loop_iterate(m_raw);
    }
  } else {
    perror("select");
  }
  return true;
}

int videoDV4L::decodeFrame(unsigned char*data, int len) {
  dv_decode_full_frame(m_decoder, data,
                       e_dv_color_rgb,
                       m_frame,
                       m_pitches);

  lock();
  m_image.image.fromRGB(m_frame[0]);
  unlock();

  return 0;
}


int videoDV4L::iec_frame(
                          unsigned char *data,
                          int len,
                          int complete,
                          void *arg
                          )
{
  if(complete) {
    videoDV4L*dv4l=(videoDV4L*)arg;
    return dv4l->decodeFrame(data, len);
  }
  return 0;
}

/////////////////////////////////////////////////////////
// openDevice
//
/////////////////////////////////////////////////////////
bool videoDV4L :: openDevice(){
  m_raw = raw1394_new_handle_on_port(0);
  if(NULL==m_raw)return false;
  int fd = raw1394_get_fd(m_raw);
  if(fd<0) {
    closeDevice();
    return false;
  }
  return true;
}

/////////////////////////////////////////////////////////
// closeDevice
//
/////////////////////////////////////////////////////////
void videoDV4L :: closeDevice(void){
  if(m_dvfd>=0)      close(m_dvfd);m_dvfd=-1;
  if(m_raw)          raw1394_destroy_handle(m_raw);m_raw=NULL;
}

/////////////////////////////////////////////////////////
// startTransfer
//
/////////////////////////////////////////////////////////
bool videoDV4L :: startTransfer()
{
  m_image.newimage=0;
  m_image.image.data=0;
  m_image.image.xsize=720;
  m_image.image.ysize=576;
  m_image.image.setCsizeByFormat(m_reqFormat);
  m_image.image.reallocate();

  if(m_decoder!=NULL)dv_decoder_free(m_decoder);m_decoder=NULL;

  if (!(m_decoder=dv_decoder_new(true, true, true))){
    error("DV4L: unable to create DV-decoder...closing");
    return false;
  }

  m_decoder->quality=m_quality;
  verbose(1, "DV4L: DV decoding quality %d ", m_decoder->quality);


  m_iec = iec61883_dv_fb_init(m_raw, iec_frame, this);
  if(NULL==m_iec) {
    stopTransfer();
    return false;
  }

  return true;
}

/////////////////////////////////////////////////////////
// stopTransfer
//
/////////////////////////////////////////////////////////
bool videoDV4L :: stopTransfer()
{
  /* close the dv4l device and dealloc buffer */
  /* terminate thread if there is one */
  stopThread(100);

  return(1);
}

/////////////////////////////////////////////////////////
// normMess
//
/////////////////////////////////////////////////////////
int videoDV4L :: setNorm(char*norm){
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
