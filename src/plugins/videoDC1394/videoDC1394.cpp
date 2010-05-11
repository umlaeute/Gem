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

#include "videoDC1394.h"
using namespace gem;

#include "Gem/RTE.h"

#ifdef HAVE_LIBDC1394
REGISTER_VIDEOFACTORY("dc1394", videoDC1394);
#endif
/////////////////////////////////////////////////////////
//
// videoDC1394
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
#ifndef HAVE_LIBDC1394
videoDC1394 :: videoDC1394() : video()
{}
videoDC1394 :: ~videoDC1394() {}

#else
videoDC1394 :: videoDC1394() : video(),
                               m_continue_thread(false),

                               m_dccamera(NULL),
                               m_dcframe(NULL),
                               m_dc(NULL)
{
  m_dc = dc1394_new(); /* Initialize libdc1394 */
  if(m_dc) 
    provide("dc1394");
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
videoDC1394 :: ~videoDC1394(){
  if(m_dccamera)dc1394_camera_free (m_dccamera);m_dccamera=NULL;
  if(m_dc)dc1394_free(m_dc);m_dc=NULL;
}
/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void *videoDC1394 :: capturing(void*you)
{
  videoDC1394 *me=(videoDC1394 *)you;
  me->captureThread();
  return NULL;
}

void videoDC1394 :: captureThread()
{
  dc1394error_t err;
  dc1394video_frame_t*frame;

  if(!m_dccamera) {
    m_haveVideo=false;
    return;
  }
  m_haveVideo=true;

  err=dc1394_video_set_transmission(m_dccamera, DC1394_ON); /* Start transmission */
  if(DC1394_SUCCESS!=err) {
    m_haveVideo=false;
    return;
  }

  m_capturing=true;
  while(m_continue_thread){
    err=dc1394_capture_dequeue(m_dccamera, DC1394_CAPTURE_POLICY_WAIT, &frame);/* Capture */

    err=dc1394_capture_enqueue(m_dccamera, frame);                             /* Release the buffer */
  }
  m_capturing=false;

  err=dc1394_video_set_transmission(m_dccamera, DC1394_OFF); /* Start transmission */
  if(DC1394_SUCCESS!=err) {
    error("unable to stop transmission");
  }
  return;
}


pixBlock *videoDC1394 :: getFrame(){
  return NULL;
  //return &m_image;
}

/////////////////////////////////////////////////////////
// openDevice
//
/////////////////////////////////////////////////////////
int videoDC1394 :: openDevice(int format){
  dc1394error_t err;
  dc1394camera_list_t *list=NULL;

  err=dc1394_camera_enumerate (m_dc, &list); /* Find cameras */
  if(DC1394_SUCCESS!=err) {
    error("videoDC1394: %s: failed to enumerate", dc1394_error_get_string(err));
    return -1;
  }
  if (list->num < 1) {
    error("videoDC1394: no cameras found");
    dc1394_camera_free_list (list);

    return -1;
  }
  if (list->num < m_devicenum) {
    error("videoDC1394: only found %d cameras but requested #%d!", list->num, m_devicenum);
    dc1394_camera_free_list (list);
    return -1;
  }

  m_dccamera = dc1394_camera_new (m_dc, list->ids[m_devicenum].guid); /* Work with first camera */
  if(!m_dccamera) {
    error("videoDC1394: only found %d cameras but requested #%d!", list->num, m_devicenum);
    dc1394_camera_free_list (list);
    return -1;
  }
  dc1394_camera_free_list (list);

  err=dc1394_capture_setup(m_dccamera, 
                           4,  /* 4 DMA buffers */
                           DC1394_CAPTURE_FLAGS_DEFAULT);     /* Setup capture */
  if(DC1394_SUCCESS!=err) {
    error("videoDC1394: %s: failed to enumerate", dc1394_error_get_string(err));
    return -1;
  }

  return -1;

  verbose(1, "DC1394: Successfully opened...");
  //  return(fd);
}
/////////////////////////////////////////////////////////
// resetDevice
//
/////////////////////////////////////////////////////////
int videoDC1394 :: resetDevice(void){
 return 0;
}
/////////////////////////////////////////////////////////
// closeDevice
//
/////////////////////////////////////////////////////////
void videoDC1394 :: closeDevice(void){

  if(m_dccamera) {
    dc1394error_t err=dc1394_capture_stop(m_dccamera);  /* Stop capture */
  }


}

/////////////////////////////////////////////////////////
// startTransfer
//
/////////////////////////////////////////////////////////
int videoDC1394 :: startTransfer(int format)
{
  pthread_create(&m_thread_id, 0, capturing, this);
  return 1;
}

/////////////////////////////////////////////////////////
// stopTransfer
//
/////////////////////////////////////////////////////////
int videoDC1394 :: stopTransfer()
{
  /* close the dv4l device and dealloc buffer */
  /* terminate thread if there is one */
  m_continue_thread=false;
  int i=0;
  if(m_haveVideo){
    while(m_capturing){
      struct timeval sleep;
      sleep.tv_sec=0;  sleep.tv_usec=10; /* 10us */
      select(0,0,0,0,&sleep);
      i++;
    }
    verbose(1, "DC1394: shutting down after %d usec", i*10);
    //  ioctl(dvfd, DV1394_SHUTDOWN);
  }
  closeDevice();
  return(1);

  return(1);
}

/////////////////////////////////////////////////////////
// normMess
//
/////////////////////////////////////////////////////////
int videoDC1394 :: setNorm(char*norm){
  return 0;
}

int videoDC1394 :: setDevice(int d){
  return 0;
}
int videoDC1394 :: setDevice(char*name){
  return 0;
}

int videoDC1394 :: setColor(int format){
  if (format<=0)return -1;
  m_reqFormat=format;
  return 0;
}

/////////////////////////////////////////
//
// Set the quality for DV decoding
//
/////////////////////////////////////////
int videoDC1394 :: setQuality(int quality){
  return 0;
}

#endif // HAVE_LIBDC1394
