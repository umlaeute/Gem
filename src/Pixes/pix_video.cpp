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

#if defined NT && defined __APPLE__ && defined __linux__
// on this OSs we surely have child-classes for the real function
# define NO_AUTO_REGISTER_CLASS
#endif

#include "pix_video.h"
#include "Base/GemCache.h"

CPPEXTERN_NEW(pix_video)

#define BYTESIN 3

/////////////////////////////////////////////////////////
//
// pix_video
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_video :: pix_video(t_floatarg num)
    	   : m_haveVideo(0), m_swap(1), m_colorSwap(0)
{
  m_pixBlock.image = m_imageStruct;
  m_haveVideo = 0;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_video :: ~pix_video()
{
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void pix_video :: render(GemState *state)
{
    if (!m_haveVideo)
    {
	post("GEM: pix_video: do video for this OS");
	return;
    }
}

/////////////////////////////////////////////////////////
// startRendering
//
/////////////////////////////////////////////////////////
void pix_video :: startRendering()
{
}

/////////////////////////////////////////////////////////
// stopRendering
//
/////////////////////////////////////////////////////////
void pix_video :: stopRendering()
{
    // this is a no-op
}

/////////////////////////////////////////////////////////
// postrender
//
/////////////////////////////////////////////////////////
void pix_video :: postrender(GemState *state)
{
}

/////////////////////////////////////////////////////////
// startTransfer
//
/////////////////////////////////////////////////////////
int pix_video :: startTransfer()
{
     post("no video available for this OS");
    if (!m_haveVideo)
    	return(0);

    return(1);
}

/////////////////////////////////////////////////////////
// stopTransfer
//
/////////////////////////////////////////////////////////
int pix_video :: stopTransfer()
{
    if ( !m_haveVideo )
    	return(0);
    
    return(1);
}

/////////////////////////////////////////////////////////
// offsetMess
//
/////////////////////////////////////////////////////////
void pix_video :: offsetMess(int x, int y)
{
  error("offset not supported on this OS");
}

/////////////////////////////////////////////////////////
// cleanPixBlock -- free the pixel buffer memory
//
/////////////////////////////////////////////////////////
void pix_video :: cleanPixBlock()
{
}

/////////////////////////////////////////////////////////
// swapMess
//
/////////////////////////////////////////////////////////
void pix_video :: swapMess(int state)
{
  error("swap not supported on this OS");
}
/////////////////////////////////////////////////////////
// colorspaceMess
//
/////////////////////////////////////////////////////////
void pix_video :: csMess(int format)
{
  error("colorspace not supported on this OS");
}
/////////////////////////////////////////////////////////
// enumerate devices
//
/////////////////////////////////////////////////////////
void pix_video :: enumerateMess()
{
  error("enumerate not supported on this OS");
}
/////////////////////////////////////////////////////////
// dialog
//
/////////////////////////////////////////////////////////
void pix_video :: dialogMess(int argc, t_atom*argv)
{
  error("dialog not supported on this OS");
}




/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_video :: obj_setupCallback(t_class *classPtr)
{
    class_addmethod(classPtr, (t_method)&pix_video::dimenMessCallback,
    	    gensym("dimen"), A_GIMME, A_NULL);
    class_addmethod(classPtr, (t_method)&pix_video::offsetMessCallback,
    	    gensym("offset"), A_FLOAT, A_FLOAT, A_NULL);
    class_addmethod(classPtr, (t_method)&pix_video::swapMessCallback,
    	    gensym("swap"), A_FLOAT, A_NULL);
    class_addmethod(classPtr, (t_method)&pix_video::enumerateMessCallback,
    	    gensym("enumerate"), A_NULL);
    class_addmethod(classPtr, (t_method)&pix_video::csMessCallback,
    	    gensym("colorspace"), A_DEFSYMBOL, A_NULL);
    class_addmethod(classPtr, (t_method)&pix_video::dialogMessCallback,
    	    gensym("dialog"), A_GIMME, A_NULL);
}
void pix_video :: dimenMessCallback(void *data, t_symbol *s, int ac, t_atom *av)
{
    GetMyClass(data)->dimenMess((int)atom_getfloatarg(0, ac, av),
    	(int)atom_getfloatarg(1, ac, av),
    	(int)atom_getfloatarg(2, ac, av),
    	(int)atom_getfloatarg(3, ac, av),
    	(int)atom_getfloatarg(4, ac, av),
    	(int)atom_getfloatarg(5, ac, av) );
}
void pix_video :: offsetMessCallback(void *data, t_floatarg x, t_floatarg y)
{
    GetMyClass(data)->offsetMess((int)x, (int)y);
}
void pix_video :: swapMessCallback(void *data, t_floatarg state)
{
    GetMyClass(data)->swapMess((int)state);
}
void pix_video :: csMessCallback(void *data, t_symbol*s)
{
  GetMyClass(data)->csMess(getPixFormat(s->s_name));
}
void pix_video :: enumerateMessCallback(void *data)
{
  GetMyClass(data)->enumerateMess();
}
void pix_video :: dialogMessCallback(void *data, t_symbol*s, int argc, t_atom*argv)
{
  GetMyClass(data)->dialogMess(argc, argv);
}
