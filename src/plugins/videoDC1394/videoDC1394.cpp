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
/////////////////////////////////////////////////////////
//
// videoDC1394
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
videoDC1394 :: videoDC1394() : video(),
                               m_dccamera(NULL),
                               m_dcframe(NULL),
                               m_dc(NULL)
{
  m_dc = dc1394_new(); /* Initialize libdc1394 */
  if(!m_dc) return;

  m_frame.xsize=1600;
  m_frame.ysize=1200;
  m_frame.setCsizeByFormat(GL_RGBA);
  m_frame.allocate();

  provide("dc1394");
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
videoDC1394 :: ~videoDC1394(){
  close();

  if(m_dccamera)dc1394_camera_free (m_dccamera);m_dccamera=NULL;
  if(m_dc)dc1394_free(m_dc);m_dc=NULL;
}
/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
bool videoDC1394 :: grabFrame()
{
  dc1394video_frame_t*frame, *colframe;
  dc1394error_t err=dc1394_capture_dequeue(m_dccamera, DC1394_CAPTURE_POLICY_WAIT, &frame);/* Capture */
  if(DC1394_SUCCESS!=err) {
    return false;
  }

  /* do something with the frame */
  colframe=( dc1394video_frame_t*)calloc(1,sizeof(dc1394video_frame_t));
  colframe->color_coding=DC1394_COLOR_CODING_RGB8;
  dc1394_convert_frames(frame, colframe);

  m_frame.xsize=frame->size[0];
  m_frame.ysize=frame->size[1];
  m_frame.setCsizeByFormat(GL_RGBA);
  m_frame.fromRGB(colframe->image);

  lock();
  m_image.image.convertFrom(&m_frame, m_reqFormat); 
  unlock();

  free(colframe->image);
  free(colframe);

  /* Release the buffer */
  err=dc1394_capture_enqueue(m_dccamera, frame);
  if(DC1394_SUCCESS!=err) {
    return false;
  }

  return true;
}

/////////////////////////////////////////////////////////
// openDevice
//
/////////////////////////////////////////////////////////
bool videoDC1394 :: openDevice(){
  dc1394error_t err;
  dc1394camera_list_t *list=NULL;

  err=dc1394_camera_enumerate (m_dc, &list); /* Find cameras */
  if(DC1394_SUCCESS!=err) {
    error("videoDC1394: %s: failed to enumerate", dc1394_error_get_string(err));
    return false;
  }
  if (list->num < 1) {
    error("videoDC1394: no cameras found");
    dc1394_camera_free_list (list);
    return false;
  }
  if (list->num < m_devicenum) {
    error("videoDC1394: only found %d cameras but requested #%d!", list->num, m_devicenum);
    dc1394_camera_free_list (list);
    return false;
  }

  /* Work with first camera */
#warning shouldnt this be channel ?
  m_dccamera = dc1394_camera_new (m_dc, list->ids[m_devicenum].guid); 
  if(!m_dccamera) {
    error("videoDC1394: only found %d cameras but requested #%d!", list->num, m_devicenum);
    dc1394_camera_free_list (list);
    return false;
  }
  dc1394_camera_free_list (list);

  err=dc1394_capture_setup(m_dccamera, 
                           4,  /* 4 DMA buffers */
                           DC1394_CAPTURE_FLAGS_DEFAULT);     /* Setup capture */
  if(DC1394_SUCCESS!=err) {
    error("videoDC1394: %s: failed to enumerate", dc1394_error_get_string(err));
    return false;
  }

  verbose(1, "DC1394: Successfully opened...");
  return true;
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
bool videoDC1394 :: startTransfer()
{
  /* Start transmission */
  dc1394error_t err=dc1394_video_set_transmission(m_dccamera, DC1394_ON); 
  if(DC1394_SUCCESS!=err) {
    return false;
  }

  return true;
}

/////////////////////////////////////////////////////////
// stopTransfer
//
/////////////////////////////////////////////////////////
bool videoDC1394 :: stopTransfer()
{
  /* Stop transmission */
  dc1394error_t err=dc1394_video_set_transmission(m_dccamera, DC1394_OFF); 
  if(DC1394_SUCCESS!=err) {
    error("unable to stop transmission");
  }
  return true;
}


int videoDC1394 :: setColor(int format){
  if (format<=0)return -1;
  m_reqFormat=format;
  return 0;
}

#else
videoDC1394 :: videoDC1394() : video()
{}
videoDC1394 :: ~videoDC1394()
{}

#endif // HAVE_LIBDC1394
