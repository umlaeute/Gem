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
#include "Base/GemConfig.h"
#if defined GEM_VIDEOBACKEND && GEM_VIDEOBACKEND == GEM_VIDEOBACKEND_SGI

//#ifdef __sgi

#include "pix_videoSGI.h"

#include <unistd.h>
#include <dmedia/vl_vino.h>

#include "Base/GemCache.h"

CPPEXTERN_NEW(pix_video)

/////////////////////////////////////////////////////////
//
// video
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_video :: pix_video()
    	   : m_haveVideo(0), m_swap(1), m_colorSwap(0),
    	     m_svr(NULL), m_drn(NULL), m_src(NULL), m_path(NULL)
{
    m_pixBlock.image.data = NULL;
    
    // Connect to the daemon
    if ( !(m_svr = vlOpenVideo("")) )
    {
    	error("GEM: pix_video: Unable to open video");
    	return;
    }
    // Set up a drain node in memory
    m_drn = vlGetNode(m_svr, VL_DRN, VL_MEM, VL_ANY);
    
    // Set up a source node on any video source
    m_src = vlGetNode(m_svr, VL_SRC, VL_VIDEO, VL_ANY);

    // Create a path using the first device that will support it
    m_path = vlCreatePath(m_svr, VL_ANY, m_src, m_drn); 

    // Set up the hardware for and define the usage of the path
    if ( (vlSetupPaths(m_svr, (VLPathList)&m_path, 1, VL_SHARE, VL_SHARE)) < 0 )
    {
    	error("GEM: pix_video: Unable to setup video path");
    	return;
    }

    // Set the packing to RGBA
    VLControlValue val;

    // first try to see if it can handle RGBA format

    // according to vl.h, this is really RGBA (OpenGL format)
    val.intVal = VL_PACKING_ABGR_8;
    if ( vlSetControl(m_svr, m_path, m_drn, VL_PACKING, &val) )
    {
    	// nope, so try ABGR, and set the color swapping
    	// according to vl.h, this is really ABGR (IrisGL format)
        val.intVal = VL_PACKING_RGBA_8;
        if ( vlSetControl(m_svr, m_path, m_drn, VL_PACKING, &val) )
    	{
    	    post("GEM: pix_video: Unable to set the video packing");
    	    return;
    	}
    	post("GEM: pix_video: Video has to color swap (ABGR to RGBA)");
    	m_colorSwap = 1;
    }
    
    // Get the video size
    vlGetControl(m_svr, m_path, m_drn, VL_SIZE, &val);

    m_pixBlock.image.xsize = val.xyVal.x;
    m_pixBlock.image.ysize = val.xyVal.y;
    m_pixBlock.image.csize = 4;
    m_pixBlock.image.format = GL_RGBA;
    m_pixBlock.image.type = GL_UNSIGNED_BYTE;
        
    int dataSize = m_pixBlock.image.xsize * m_pixBlock.image.ysize
    	    	     * 4 * sizeof(unsigned char);
    m_pixBlock.image.allocate(dataSize);

    m_haveVideo = 1;
    if ( !startTransfer() )
    {
    	m_haveVideo = 0;
    	return;
    }
    
    post("GEM: pix_video: Opened video connection");
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_video :: ~pix_video()
{
    stopTransfer();

    // Clean up the video
    if (m_haveVideo)
    {
    	vlDestroyPath(m_svr, m_path);
    	vlCloseVideo(m_svr);
    	post("GEM: pix_video: Closed video connection");
    }
    cleanPixBlock();
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void pix_video :: render(GemState *state)
{
    if (!m_haveVideo)
    {
		post("GEM: pix_video: do not have a video stream");
		return;
    }

    VLInfoPtr info = vlGetLatestValid(m_svr, m_buffer);
    while (!info)
    {
		sginap(1);
		info = vlGetLatestValid(m_svr, m_buffer);
    }

    // Get a pointer to the frame
    unsigned char *dataPtr = (unsigned char *)(vlGetActiveRegion(m_svr, m_buffer, info));
    int dataSize =
    	m_pixBlock.image.xsize * m_pixBlock.image.ysize *
    	m_pixBlock.image.csize * sizeof(unsigned char);
    
    int holdVal = dataSize - 4;
    if (m_swap && m_colorSwap)
    {
		// copy the data over - the buffer is upside down though and ABGR...
		for (int i = 0; i < dataSize; i += 4)
		{
    		m_pixBlock.image.data[i + 3] = dataPtr[holdVal - i];
    		m_pixBlock.image.data[i + 2] = dataPtr[holdVal - i + 1];
    		m_pixBlock.image.data[i + 1] = dataPtr[holdVal - i + 2];
    		m_pixBlock.image.data[i + 0] = dataPtr[holdVal - i + 3];
		}
    }
    else if (m_swap && !m_colorSwap)
    {
		// copy the data over - the buffer is upside down though...
		for (int i = 0; i < dataSize; i += 4)
		{
    		memcpy(&(m_pixBlock.image.data[i]), &(dataPtr[holdVal - i]), 4);
		}
    }
    else if (m_colorSwap && !m_swap)
    {
		// copy the data over - the buffer is ABGR though...
		for (int i = 0; i < dataSize; i += 4)
		{
    		m_pixBlock.image.data[i + 3] = dataPtr[i];
    		m_pixBlock.image.data[i + 2] = dataPtr[i + 1];
    		m_pixBlock.image.data[i + 1] = dataPtr[i + 2];
    		m_pixBlock.image.data[i + 0] = dataPtr[i + 3];
		}
    }
    // don't do anything except copy it over
    else memcpy(m_pixBlock.image.data, dataPtr, dataSize);
    
    // free the frame
    vlPutFree(m_svr, m_buffer);
    
    m_pixBlock.newimage = 1;
    state->image = &m_pixBlock;
}

/////////////////////////////////////////////////////////
// startRendering
//
/////////////////////////////////////////////////////////
void pix_video :: startRendering()
{
    m_pixBlock.newimage = 1;
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
    m_pixBlock.newimage = 0;
    state->image = NULL;
}

/////////////////////////////////////////////////////////
// startTransfer
//
/////////////////////////////////////////////////////////
int pix_video :: startTransfer()
{
    if (!m_haveVideo) return(0);
    
    // Create and register a buffer for 1 frame
    m_buffer = vlCreateBuffer(m_svr, m_path, m_drn, 1);
    if ( !m_buffer )
    {
    	error("GEM: pix_video: Unable to allocate buffer");	
    	return(0);
    }
    vlRegisterBuffer(m_svr, m_path, m_drn, m_buffer);
    
    // Begin the data transfer
    if ( vlBeginTransfer(m_svr, m_path, 0, NULL) )
    {
    	error("GEM: pix_video: Unable to start video transfer");
    	return(0);
    }
    return(1);
}

/////////////////////////////////////////////////////////
// stopTransfer
//
/////////////////////////////////////////////////////////
int pix_video :: stopTransfer()
{
    if ( !m_haveVideo ) return(0);

    // Clean up the buffer
    vlEndTransfer(m_svr, m_path);
    vlDeregisterBuffer(m_svr, m_path, m_drn, m_buffer);
    vlDestroyBuffer(m_svr, m_buffer);
    
    return(1);
}

/////////////////////////////////////////////////////////
// offsetMess
//
/////////////////////////////////////////////////////////
void pix_video :: offsetMess(int x, int y)
{
    if (!m_haveVideo)
    {
    	post("GEM: pix_video: Connect to video first");
    	return;
    }
    
    // stop the transfer and destroy the buffer
    if ( !stopTransfer() ) 
    {
    	post("GEM: pix_video: error stopping transfer");
    	return;
    }

    VLControlValue value;
    value.xyVal.x = x;
    value.xyVal.y = y;
    if ( vlSetControl(m_svr, m_path, m_drn, VL_OFFSET, &value) )
    {
    	post("GEM: pix_video: offset error");
    	startTransfer();
    	return;
    }

    // start the transfer and rebuild the buffer
    if ( !startTransfer() ) 
    {
    	post("GEM: pix_video: error starting transfer");
    	return;
    }
}

/////////////////////////////////////////////////////////
// dimenMess
//
/////////////////////////////////////////////////////////
void pix_video :: dimenMess(int x, int y)
{
    if (!m_haveVideo)
    {
    	post("GEM: pix_video: Connect to video first");
    	return;
    }
    
    // stop the transfer and destroy the buffer
    if ( !stopTransfer() ) 
    {
    	post("GEM: pix_video: error stopping transfer");
    	return;
    }

    VLControlValue value;
    value.xyVal.x = x;
    value.xyVal.y = y;
    if ( vlSetControl(m_svr, m_path, m_drn, VL_SIZE, &value) )
    {
    	post("GEM: pix_video: dimen error");
    	startTransfer();
    	return;
    }
    m_pixBlock.image.xsize = x;
    m_pixBlock.image.ysize = y;;

    cleanPixBlock();
    int dataSize = m_pixBlock.image.xsize * m_pixBlock.image.ysize
    	    	    * 4 * sizeof(unsigned char);
    m_pixBlock.image.allocate(dataSize);

    // start the transfer and rebuild the buffer
    if ( !startTransfer() ) 
    {
    	post("GEM: pix_video: error starting transfer");
    	return;
    }
}

/////////////////////////////////////////////////////////
// cleanPixBlock
//
/////////////////////////////////////////////////////////
void pix_video :: cleanPixBlock()
{
    m_pixBlock.image.clear();
}

/////////////////////////////////////////////////////////
// swapMess
//
/////////////////////////////////////////////////////////
void pix_video :: swapMess(int state)
{
    if (state) m_swap = 1;
    else m_swap = 0;
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_video :: obj_setupCallback(t_class *classPtr)
{
    class_addmethod(classPtr, (t_method)&pix_video::dimenMessCallback,
    	    gensym("dimen"), A_FLOAT, A_FLOAT, A_NULL);
    class_addmethod(classPtr, (t_method)&pix_video::offsetMessCallback,
    	    gensym("offset"), A_FLOAT, A_FLOAT, A_NULL);
    class_addmethod(classPtr, (t_method)&pix_video::swapMessCallback,
    	    gensym("swap"), A_FLOAT, A_NULL);
}
void pix_video :: dimenMessCallback(void *data, t_floatarg x, t_floatarg y)
{
    GetMyClass(data)->dimenMess(x, y);
}
void pix_video :: offsetMessCallback(void *data, t_floatarg x, t_floatarg y)
{
    GetMyClass(data)->offsetMess(x, y);
}
void pix_video :: swapMessCallback(void *data, t_floatarg state)
{
    GetMyClass(data)->swapMess(state);
}
#endif
