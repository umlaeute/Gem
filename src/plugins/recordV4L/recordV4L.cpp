////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file 
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "recordV4L.h"
using namespace gem;
#include "Base/GemBase.h"
#include "Base/GemMan.h"

#include "Base/GemException.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdio.h>

#include <stdlib.h>


#ifdef  HAVE_VIDEO4LINUX
REGISTER_RECORDFACTORY("V4L", recordV4L);
/////////////////////////////////////////////////////////
//
// recordV4L
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////

recordV4L :: recordV4L(): 
  record(),
  m_fd(-1)
{
  m_image.xsize=720;
  m_image.xsize=576;
  m_image.setCsizeByFormat(GL_YUV422_GEM);
  m_image.reallocate();



  switch(m_image.format) {
  case GL_YUV422_GEM: m_palette = VIDEO_PALETTE_YUV422; break;
  case GL_LUMINANCE:  m_palette = VIDEO_PALETTE_GREY; break;
  case GL_RGBA:       m_palette = VIDEO_PALETTE_RGB32; break;
  default: throw(new GemException("invalid colorspace"));
  }
  

}

////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
recordV4L :: ~recordV4L()
{
  close();
}

void recordV4L :: close(void)
{
  if(m_fd>=0)
    ::close(m_fd);
  m_fd=-1;

}

bool recordV4L :: open(const char *filename)
{
  close();

  m_fd=::open(filename, O_RDWR);
  if(m_fd<0)return false;

  struct video_picture vid_pic;
  if (ioctl(m_fd, VIDIOCGPICT, &vid_pic) == -1) {
    perror("VIDIOCGPICT");
    close(); return false;
  }
  vid_pic.palette = m_palette;
  if (ioctl(m_fd, VIDIOCSPICT, &vid_pic) == -1) {
    perror("VIDIOCSPICT");
    close(); return false;
  }


  struct video_window vid_win;
  if (ioctl(m_fd, VIDIOCGWIN, &vid_win) == -1) {
    perror("(VIDIOCGWIN)");
    close(); return false;
  }

  m_init=false;
  return true;
}

bool recordV4L::init(const imageStruct* dummyImage, const int framedur) {
  if(m_init)return true;
  if(m_fd<0)return false;

  struct video_picture vid_pic;
  struct video_window vid_win;

  if (ioctl(m_fd, VIDIOCGPICT, &vid_pic) == -1) {
    perror("VIDIOCGPICT");
    close(); return false;
  }

  vid_pic.palette = m_palette;
  
  if (ioctl(m_fd, VIDIOCSPICT, &vid_pic) == -1) {
    perror("VIDIOCSPICT");
    close(); return false;
  }

  if (ioctl(m_fd, VIDIOCGWIN, &vid_win) == -1) {
    perror("ioctl (VIDIOCGWIN)");
    close(); return false;
  }
  
  vid_win.width  = dummyImage->xsize;
  vid_win.height = dummyImage->ysize;
  if (ioctl(m_fd, VIDIOCSWIN, &vid_win) == -1) {
    perror("ioctl (VIDIOCSWIN)");
    close(); return false;
  }

  m_image.xsize=dummyImage->xsize;
  m_image.ysize=dummyImage->ysize;
  m_image.reallocate();

  m_currentFrame=0;
  m_init=true;
  return true;
}



/////////////////////////////////////////////////////////
// do the actual encoding and writing to file
//
/////////////////////////////////////////////////////////
int recordV4L :: putFrame(imageStruct*img)
{
  if(!m_init){
    if(!init(img, 0))
      return 0;
  }
  m_image.convertFrom(img);

  m_image.upsidedown=!m_image.upsidedown;
  m_image.fixUpDown();

  int size=m_image.xsize*m_image.ysize*m_image.csize;

  ::write(m_fd, m_image.data, size);

  m_currentFrame++;
  return m_currentFrame;
}



/////////////////////////////////////////////////////////
// get number of codecs
//
/////////////////////////////////////////////////////////
int recordV4L :: getNumCodecs()
{
  return 1;
}
const char*recordV4L :: getCodecName(int i)
{
  if(i==0)
    return gensym("v4l")->s_name;
  return NULL;
}
const char*recordV4L :: getCodecDescription(int i)
{
  if(i==0)
    return gensym("v4l loopback")->s_name;
  return NULL;
}

/////////////////////////////////////////////////////////
// set codec by name
//
/////////////////////////////////////////////////////////
bool recordV4L :: setCodec(const char*name)
{
  return true;
}
#else
recordV4L :: recordV4L()
{
}
////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
recordV4L :: ~recordV4L()
{
}

#endif
