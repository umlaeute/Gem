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
#ifdef __linux__
    
#include "pix_videoLinux.h"
#include "Base/GemCache.h"

CPPEXTERN_NEW_WITH_TWO_ARGS(pix_videoLinux,t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

#define BYTESIN 3

/////////////////////////////////////////////////////////
//
// pix_videoLinux
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_videoLinux :: pix_videoLinux(t_floatarg w = 320, t_floatarg h = 240) :
     m_width((int)w),
     m_height((int)h),
     m_channel(COMPOSITEIN),
     m_norm(VIDEO_MODE_AUTO),
     m_devicenum(DEVICENO)
{
  if (!m_width)m_width=64;
  if (!m_height)m_height=64;

  //  post("w = %d, h= %d",m_width, m_height);
  m_pixBlock.image.data = NULL;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_videoLinux :: ~pix_videoLinux()
{
    if (m_haveVideo)
    	stopTransfer();

    cleanPixBlock();
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void pix_videoLinux :: render(GemState *state)
{
    int i, row, column;
    unsigned char *pixp;
    
    if (!m_haveVideo)return;
    
    if (ioctl(tvfd, VIDIOCSYNC, &vmmap[frame].frame) < 0)
    {
	perror("VIDIOCSYNC");
	m_haveVideo = 0;
	stopTransfer();
	return;
    }
    unsigned char *newimage = videobuf + vmbuf.offsets[frame];

    int dataSize =
    	m_pixBlock.image.xsize * m_pixBlock.image.ysize *
    	m_pixBlock.image.csize * sizeof(unsigned char);

    if (skipnext)
    {
    	for (i = 0; i < dataSize; i += 4)
	{
    	    m_pixBlock.image.data[i + 0] = 0;
    	    m_pixBlock.image.data[i + 1] = 0;
    	    m_pixBlock.image.data[i + 2] = 0;
    	    m_pixBlock.image.data[i + 3] = 255;
	}
	skipnext = 0;
    }
    	/* copy the image, converting from RGB to RGBA. */
    else for (row = 0; row < m_pixBlock.image.ysize; row++)
    {
    	unsigned char * inrow = (newimage + BYTESIN *
	    ((m_pixBlock.image.xsize + myleftmargin + myrightmargin) *
	    (row + mytopmargin) + myleftmargin));
    	unsigned char *outrow = m_pixBlock.image.data +
	    4 * m_pixBlock.image.xsize * (m_pixBlock.image.ysize - row - 1);
	for (column = 0; column < m_pixBlock.image.xsize; column++)
	{
    	    outrow[0] = *(inrow+2);
    	    outrow[1] = *(inrow+1);
    	    outrow[2] = *(inrow);
    	    outrow[3] = 255; 	/* opaque */
    	    inrow += 3;
	    outrow += 4;
	}
    }
    m_pixBlock.newimage = 1;
    state->image = &m_pixBlock;
    frame = !frame;

    vmmap[frame].width = m_pixBlock.image.xsize + myleftmargin + myrightmargin;
    vmmap[frame].height = m_pixBlock.image.ysize + mytopmargin + mybottommargin;

    if (ioctl(tvfd, VIDIOCMCAPTURE, &vmmap[frame]) < 0)
    {
    	if (errno == EAGAIN)
	    fprintf(stderr, "can't sync (no video source?)\n");
    	else 
	    perror("VIDIOCMCAPTURE");
	if (ioctl(tvfd, VIDIOCMCAPTURE, &vmmap[frame]) < 0)
	  perror("VIDIOCMCAPTURE2");
      
	post("frame %d %d, format %d, width %d, height %d\n",
	     frame, vmmap[frame].frame, vmmap[frame].format,
	     vmmap[frame].width, vmmap[frame].height);

	stopTransfer();

	m_haveVideo = 0;
	return;
    }
}

/////////////////////////////////////////////////////////
// startRendering
//
/////////////////////////////////////////////////////////
void pix_videoLinux :: startRendering()
{
     startTransfer();
     m_pixBlock.newimage = 1;
}

/////////////////////////////////////////////////////////
// stopRendering
//
/////////////////////////////////////////////////////////
void pix_videoLinux :: stopRendering()
{
     stopTransfer();
}

/////////////////////////////////////////////////////////
// postrender
//
/////////////////////////////////////////////////////////
void pix_videoLinux :: postrender(GemState *state)
{
    m_pixBlock.newimage = 0;
    state->image = NULL;
}

/////////////////////////////////////////////////////////
// startTransfer
//
/////////////////////////////////////////////////////////
int pix_videoLinux :: startTransfer()
{
    char buf[256];
    int i, dataSize;
    frame = 0;
    int width, height;
    
    skipnext = 0;
    sprintf(buf, "/dev/video%d", m_devicenum);
    if ((tvfd = open(buf, O_RDWR)) < 0)
    {
	perror(buf);
	goto closit;
    }
    if (ioctl(tvfd, VIDIOCGCAP, &vcap) < 0)
    {
	perror("get capabilities");
	goto closit;
    }
    post("cap: name %s type %d channels %d maxw %d maxh %d minw %d minh %d\n",
    	vcap.name, vcap.type,  vcap.channels,  vcap.maxwidth,  vcap.maxheight,
	    vcap.minwidth,  vcap.minheight);
    if (ioctl(tvfd, VIDIOCGPICT, &vpicture) < 0)
    {
	perror("VIDIOCGCAP");
	goto closit;
    }
    
    post("picture: brightness %d depth %d palette %d\n",
	    vpicture.brightness, vpicture.depth, vpicture.palette);

    for (i = 0; i < vcap.channels; i++)
    {
	vchannel.channel = i;
	if (ioctl(tvfd, VIDIOCGCHAN, &vchannel) < 0)
	{
	    perror("VDIOCGCHAN");
	    goto closit;
	}
    	printf("channel %d name %s type %d flags %d\n",
    	    vchannel.channel, vchannel.name, 
	    vchannel.type, vchannel.flags);
    }
    vchannel.channel = m_channel;
    if (ioctl(tvfd, VIDIOCGCHAN, &vchannel) < 0)
    {
	perror("VDIOCGCHAN");
	goto closit;
    }

    vchannel.norm = m_norm;
    if (ioctl(tvfd, VIDIOCSCHAN, &vchannel) < 0)
    {
	perror("VDIOCSCHAN");
	goto closit;
    }


    	/* get mmap numbers */
    if (ioctl(tvfd, VIDIOCGMBUF, &vmbuf) < 0)
    {
	perror("VIDIOCGMBUF");
	goto closit;
    }
    post("buffer size %d, frames %d, offset %d %d\n", vmbuf.size,
    	vmbuf.frames, vmbuf.offsets[0], vmbuf.offsets[1]);
    if (!(videobuf = (unsigned char *)
    	mmap(0, vmbuf.size, PROT_READ|PROT_WRITE, MAP_SHARED, tvfd, 0)))
    {
	perror("mmap");
	goto closit;
    }

    width = (m_width > vcap.minwidth) ? m_width :  vcap.minwidth;   
    width = (width > vcap.maxwidth) ? vcap.maxwidth : width;
    height = m_height > vcap.minheight ? m_height : vcap.minheight;
    height = (height > vcap.maxheight) ? vcap.maxheight : height;

    for (i = 0; i < NBUF; i++)
    {
    	vmmap[i].format = VIDEO_PALETTE_RGB24;
    	vmmap[i].width = width;
    	vmmap[i].height = height;
	vmmap[i].frame  = i;
    }
    if (ioctl(tvfd, VIDIOCMCAPTURE, &vmmap[frame]) < 0)
    {
    	if (errno == EAGAIN)
	    fprintf(stderr, "can't sync (no video source?)\n");
    	else 
	    perror("VIDIOCMCAPTURE");
	goto closit;
    }
    post("frame %d %d, format %d, width %d, height %d\n",
    	frame, vmmap[frame].frame, vmmap[frame].format,
    	vmmap[frame].width, vmmap[frame].height);

    	/* fill in image specifics for Gem pixel object.  Could we have
	just used RGB, I wonder? */
    m_pixBlock.image.xsize = width;
    m_pixBlock.image.ysize = height;
    m_pixBlock.image.csize = 4;
    m_pixBlock.image.format = GL_RGBA;
    m_pixBlock.image.type = GL_UNSIGNED_BYTE;
    myleftmargin = 0;
    myrightmargin = 0;
    mytopmargin = 0;
    mybottommargin = 0;
    
    dataSize = m_pixBlock.image.xsize * m_pixBlock.image.ysize
    	    	     * 4 * sizeof(unsigned char);
    m_pixBlock.image.data = new unsigned char[dataSize];

    m_haveVideo = 1;
    post("GEM: pix_video: Opened video connection");
    return(1);

closit:
    if (tvfd >= 0)
    {
    	close(tvfd);
	tvfd = -1;
    }
    m_haveVideo = 0;
    return(0);
}

/////////////////////////////////////////////////////////
// stopTransfer
//
/////////////////////////////////////////////////////////
int pix_videoLinux :: stopTransfer()
{
     munmap(videobuf, vmbuf.size);
     if (tvfd) close(tvfd);
     tvfd = 0;
     m_haveVideo = 0;
     return(1);
}

/////////////////////////////////////////////////////////
// dimenMess
//
/////////////////////////////////////////////////////////
void pix_videoLinux :: dimenMess(int x, int y, int leftmargin, int rightmargin,
    int topmargin, int bottommargin)
{

    int xtotal = x + leftmargin + rightmargin;
    int ytotal = y + topmargin + bottommargin;
    if (xtotal > 844)
    	post("x dimensions too great");
    else if (xtotal < vcap.minwidth || x < 1 ||
    	leftmargin < 0 || rightmargin < 0)
    	    post("x dimensions too small");
    if (ytotal > vcap.maxheight)
    	post("y dimensions too great");
    else if (ytotal < vcap.minheight || y < 1 ||
	     topmargin < 0 || bottommargin < 0)
      post("y dimensions too small");

    myleftmargin = leftmargin;
    myrightmargin = rightmargin;
    mytopmargin = topmargin;
    mybottommargin = bottommargin;

    m_width=x;
    m_height=y;
    m_pixBlock.image.xsize = x;
    m_pixBlock.image.ysize = y;

    cleanPixBlock();
    int dataSize = m_pixBlock.image.xsize * m_pixBlock.image.ysize
    	    	    * 4 * sizeof(unsigned char);
    m_pixBlock.image.data = new unsigned char[dataSize];
    skipnext = 1;
}

/////////////////////////////////////////////////////////
// cleanPixBlock -- free the pixel buffer memory
//
/////////////////////////////////////////////////////////
void pix_videoLinux :: cleanPixBlock()
{
    if (m_pixBlock.image.data) delete [] m_pixBlock.image.data;
    m_pixBlock.image.data = NULL;
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_videoLinux :: obj_setupCallback(t_class *classPtr)
{
  class_addcreator((t_newmethod)_classpix_videoLinux,gensym("pix_video"),A_DEFFLOAT,A_DEFFLOAT,A_NULL);
  pix_video::real_obj_setupCallback(classPtr);
  class_addmethod(classPtr, (t_method)&pix_videoLinux::freqMessCallback,
		  gensym("freq"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_videoLinux::modeMessCallback,
		  gensym("mode"), A_GIMME, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_videoLinux::channelMessCallback,
		  gensym("channel"), A_FLOAT, A_NULL);
}


void pix_videoLinux :: modeMess(int argc, t_atom *argv)
{
  int mode=m_channel;
  char c=0;
  if (argc==1){
    if (argv->a_type==A_FLOAT)mode=atom_getint(argv);
    else if (argv->a_type==A_SYMBOL)c=toupper(*argv->a_w.w_symbol->s_name);
    else return;
  } else if (argc==2){
    if (argv->a_type==A_FLOAT && (argv+1)->a_type==A_SYMBOL){
      mode=atom_getint(argv);
      c=toupper(*(argv+1)->a_w.w_symbol->s_name);
    } else if ((argv+1)->a_type==A_FLOAT && argv->a_type==A_SYMBOL){
      mode=atom_getint(argv+1);
      c=toupper(*argv->a_w.w_symbol->s_name);
    } else return;
  }

  stopTransfer();

  switch (c){
  case 'P':
    m_norm = VIDEO_MODE_PAL;
    break;
  case 'N':
    m_norm = VIDEO_MODE_NTSC;
    break;
  default:
    error("pix_video: unknown norm");
    break;
  }
  m_channel = (int) mode;
  vchannel.channel = m_channel;

  if(gem_amRendering)startTransfer();
}

void pix_videoLinux :: channelMess(int c)
{
  if(gem_amRendering)stopTransfer();

  m_channel = c;
  vchannel.channel = m_channel;

  if(gem_amRendering)startTransfer();
}

void pix_videoLinux :: deviceMess(int d)
{
  if(gem_amRendering)stopTransfer();
  m_devicenum=d;
  if(gem_amRendering)startTransfer();
}

void pix_videoLinux :: freqMess(t_floatarg c)
{
     int freq = (int) c;
     vtuner.tuner = m_channel;
     if (ioctl(tvfd,VIDIOCGTUNER,&vtuner) < 0) {
	  post("Error setting frequency -- no tuner");
	  return;
     }
     
     if (ioctl(tvfd,VIDIOCSFREQ,&freq) < 0) {
	  post("Error setting frequency");
	  return;
     }
}

void pix_videoLinux :: modeMessCallback(void *data, t_symbol* norm, int argc, t_atom *argv)
{
  if (argc==1 || argc==2)GetMyClass(data)->modeMess(argc, argv);
}


void pix_videoLinux :: freqMessCallback(void *data, t_floatarg f)
{
    GetMyClass(data)->freqMess(f);
}

void pix_videoLinux :: channelMessCallback(void *data, t_floatarg f)
{
    GetMyClass(data)->channelMess((int)f);
}

void pix_videoLinux :: deviceMessCallback(void *data, t_floatarg f)
{
    GetMyClass(data)->deviceMess((int)f);
}
#endif
