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

    
#include "yuv_video.h"

#include "Base/GemCache.h"

CPPEXTERN_NEW(yuv_video)

#define BYTESIN 3

/////////////////////////////////////////////////////////
//
// yuv_video
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
yuv_video :: yuv_video(t_floatarg num)
    	   : m_haveVideo(0), m_swap(1), m_colorSwap(0)
{
    m_haveVideo = 0;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
yuv_video :: ~yuv_video()
{
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void yuv_video :: render(GemState *state)
{
    //int i, row, column;		//tigital: unused
    //unsigned char *pixp;		//tigital: unused
    
    if (!m_haveVideo)
    {
	post("GEM: yuv_video: do video for this OS");
	return;
    }
}

/////////////////////////////////////////////////////////
// startRendering
//
/////////////////////////////////////////////////////////
void yuv_video :: startRendering()
{
}

/////////////////////////////////////////////////////////
// stopRendering
//
/////////////////////////////////////////////////////////
void yuv_video :: stopRendering()
{
    // this is a no-op
}

/////////////////////////////////////////////////////////
// postrender
//
/////////////////////////////////////////////////////////
void yuv_video :: postrender(GemState *state)
{
}

/////////////////////////////////////////////////////////
// startTransfer
//
/////////////////////////////////////////////////////////
int yuv_video :: startTransfer()
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
int yuv_video :: stopTransfer()
{
    if ( !m_haveVideo )
    	return(0);
    
    return(1);
}

/////////////////////////////////////////////////////////
// offsetMess
//
/////////////////////////////////////////////////////////
void yuv_video :: offsetMess(int x, int y)
{
    post("warning: yuv_video_offset does nothing in Linux");
}

/////////////////////////////////////////////////////////
// dimenMess
//
/////////////////////////////////////////////////////////
void yuv_video :: dimenMess(int x, int y, int leftmargin, int rightmargin,
    int topmargin, int bottommargin)
{
}

/////////////////////////////////////////////////////////
// cleanPixBlock -- free the pixel buffer memory
//
/////////////////////////////////////////////////////////
void yuv_video :: cleanPixBlock()
{
}

/////////////////////////////////////////////////////////
// swapMess
//
/////////////////////////////////////////////////////////
void yuv_video :: swapMess(int state)
{
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void yuv_video :: obj_setupCallback(t_class *classPtr)
{
    class_addmethod(classPtr, (t_method)&yuv_video::dimenMessCallback,
    	    gensym("dimen"), A_GIMME, A_NULL);
    class_addmethod(classPtr, (t_method)&yuv_video::offsetMessCallback,
    	    gensym("offset"), A_FLOAT, A_FLOAT, A_NULL);
    class_addmethod(classPtr, (t_method)&yuv_video::swapMessCallback,
    	    gensym("swap"), A_FLOAT, A_NULL);
}
void yuv_video :: dimenMessCallback(void *data, t_symbol *s, int ac, t_atom *av)
{
    GetMyClass(data)->dimenMess(atom_getfloatarg(0, ac, av),
    	atom_getfloatarg(1, ac, av),
    	atom_getfloatarg(2, ac, av),
    	atom_getfloatarg(3, ac, av),
    	atom_getfloatarg(4, ac, av),
    	atom_getfloatarg(5, ac, av) );
}
void yuv_video :: offsetMessCallback(void *data, t_floatarg x, t_floatarg y)
{
    GetMyClass(data)->offsetMess(x, y);
}
void yuv_video :: swapMessCallback(void *data, t_floatarg state)
{
    GetMyClass(data)->swapMess(state);
}

