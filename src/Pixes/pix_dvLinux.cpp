#ifdef __linux__
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
#include "../Base/config.h"
#ifdef HAVE_DV

#include "pix_dvLinux.h"

/* let's hope RGB(24bit) is 0;
 * i have not found a #define anywhere, though it is mentioned in quicktime's firewire-docu
 * quicktime/colormodels.h defines BC_RGB888 with 9
 */
#define DV_RGB888 0


CPPEXTERN_NEW(pix_dvLinux)

/////////////////////////////////////////////////////////
//
// pix_dvLinux
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_dvLinux :: pix_dvLinux(void)
{
  m_norm = DV_PAL;
  m_pixBlock.image.data  = NULL;
  m_pixBlock.image.xsize = m_pixBlock.image.ysize=0;
  m_pixBlock.image.csize = 3;
  m_pixBlock.image.format= GL_RGB; /* we won't get RGBA, so we do not even try...*/


  m_port   =0;
  m_channel=0;

  m_grabber = dv_grabber_new();
  m_dv      = dv_new();

  post("DV=%d", m_haveVideo);
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_dvLinux :: ~pix_dvLinux()
{
  if (m_haveVideo){
    stopTransfer();
    dv_grabber_delete(m_grabber);
    dv_delete(m_dv);
  }
  cleanPixBlock();
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void pix_dvLinux :: render(GemState *state)
{
  int width=m_pixBlock.image.xsize;
  int height=m_pixBlock.image.ysize;
  long int size=0;
  unsigned char *data=NULL;
  unsigned char *rows[height];
  int i=height;

  if (!m_haveVideo)return;

  dv_grab_frame(m_grabber, &data, &size);
  if (size==DV_PAL_SIZE)m_norm=DV_PAL;
  else if (size==DV_NTSC_SIZE)m_norm=DV_NTSC;
  else error("GEM: pix_dvLinux: got stupid frame-size of %d", size);

  i=height;
  while(i--)rows[i]=m_pixBlock.image.data+i*width;

  dv_read_video(m_dv, rows, data, size, DV_RGB888);

  dv_unlock_frame(m_grabber);
}

/////////////////////////////////////////////////////////
// startTransfer
//
/////////////////////////////////////////////////////////
int pix_dvLinux :: startTransfer()
{
  /* hope, that dv_start_grabbing returns non-zero on failure */
  if (dv_start_grabbing(m_grabber, m_port, m_channel, 20))
    m_haveVideo=0;
}

/////////////////////////////////////////////////////////
// stopTransfer
//
/////////////////////////////////////////////////////////
int pix_dvLinux :: stopTransfer()
{
  if (!m_haveVideo)return -1;
  if (dv_grabber_crashed(m_grabber)){
    dv_interrupt_grabber(m_grabber);
    dv_stop_grabbing(m_grabber);
    dv_grabber_delete(m_grabber);
    m_haveVideo=0;
  } else dv_stop_grabbing(m_grabber);
}


/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_dvLinux :: obj_setupCallback(t_class *classPtr)
{
  class_addcreator((t_newmethod)_classpix_dvLinux,gensym("pix_DV"),A_NULL);
  pix_dv::real_obj_setupCallback(classPtr);
  class_addmethod(classPtr, (t_method)&pix_dvLinux::channelMessCallback,
		  gensym("channel"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_dvLinux::portMessCallback,
		  gensym("port"), A_FLOAT, A_NULL);
}

void pix_dvLinux :: portMessCallback(void *data, t_floatarg f)
{
  GetMyClass(data)->m_port=(f<0)?0:(int)f;
}


void pix_dvLinux :: channelMessCallback(void *data, t_floatarg f)
{
  GetMyClass(data)->m_channel=(f<0)?0:(int)f;
}
#endif /* HAVE_DV */
#endif
