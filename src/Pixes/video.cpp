////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-1998 Mark Danks.
//    Copyright (c) G�nther Geiger.
//    Copyright (c) 2001-2003 IOhannes m zmoelnig. forum::f�r::uml�ute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
  
#include "video.h"

/////////////////////////////////////////////////////////
//
// pix_videoLinux
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
video :: video(int format) {
  m_reqFormat=format;
  m_width=64;
  m_height=64;
  m_capturing=false;
#if 0
  m_channel=COMPOSITEIN;
  m_norm=VIDEO_MODE_AUTO;
  m_devicenum=DEVICENO;
#endif
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
video :: ~video()
{
  if (m_haveVideo)closeDevice();

}
/////////////////////////////////////////////////////////
// openDevice
//
/////////////////////////////////////////////////////////
int video :: openDevice(int i, int o)
{
  return 0;
}

/////////////////////////////////////////////////////////
// closeDevice
//
/////////////////////////////////////////////////////////
void video :: closeDevice()
{
}

/////////////////////////////////////////////////////////
// resetDevice
//
/////////////////////////////////////////////////////////
int video :: resetDevice()
{
  return(0);
}
/////////////////////////////////////////////////////////
// startTransfer
//
/////////////////////////////////////////////////////////
int video :: startTransfer(int format)
{
  if (format>0)m_reqFormat=format;
  return 0;
}

/////////////////////////////////////////////////////////
// stopTransfer
//
/////////////////////////////////////////////////////////
int video :: stopTransfer()
{
  return(0);
}


/////////////////////////////////////////////////////////
// set dimension
int video :: setDimen(int x, int y, int leftmargin, int rightmargin, int topmargin, int bottommargin){
  post("setting the dimension for video is not supported by this OS/device");
  return -1;
}
/////////////////////////////////////////////////////////
// set the displacment
int video :: setOffset(int x, int y){
  post("setting the channel is not supported by this OS/device");
  return -1;
}
/////////////////////////////////////////////////////////
// should the image be swapped ?
int video :: setSwap(int state){
  post("swapping the image is not supported by this OS/device");
  return -1;
}
/////////////////////////////////////////////////////////
// set the tv-norm
int video :: setNorm(char *n){
  post("setting the video-norm is not supported by this OS/device");
  return -1;
}
/////////////////////////////////////////////////////////
// set the channel of the current device
int video :: setChannel(int chan, t_float freq){
  post("setting the channel is not supported by this OS/device");
  return -1;
}
/////////////////////////////////////////////////////////
// set the device
int video :: setDevice(int d){
  post("setting the video-device is not supported by this OS/device");
  return -1;
}
/////////////////////////////////////////////////////////
// set the color-space
int video :: setColor(int d){
  post("setting the color-space is not supported by this OS/device");
  return -1;
}
/////////////////////////////////////////////////////////
// set the quality for DV decoding
int video :: setQuality(int d){
  post("setting the quality is not supported by this OS/device");
  return -1;
}
