/*
 *  yuv_videoDarwin.cpp
 *  gem_darwin
 *
 *  Created by James Tittle on Mon Dec 9 2002.
 *  Copyright (c) 2002 James Tittle & Chris Clepper
 *
 */
#ifdef MACOSX

#include "yuv_videoDarwin.h"
#include "GemCache.h"
#include <OpenGL/glext.h>
#include <Carbon/Carbon.h>

static inline int powerOfTwo( int value )
{
    int x = 1;
    while ( x<value ) x<<= 1;
    return( x );
}

CPPEXTERN_NEW_WITH_TWO_ARGS(yuv_videoDarwin, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)
 
#define DEFAULT_WIDTH		320
#define DEFAULT_HEIGHT		240
#define DEFAULT_FRAMES		5		// 5 fps
#define MAX_RECORDING_TIME	100 * 60	// n * 60 ticks  (n : seconds)
#define DEFAULT_INTERVAL	5		// 5 milliseconds for calling SGIdle()

yuv_videoDarwin :: yuv_videoDarwin( t_floatarg w, t_floatarg h )
{
   if (w > 0){
    m_vidXSize = (int)w;
  }else{
    m_vidXSize = 320;
  }
  if (h > 0){
    m_vidYSize = (int)h;
  }else{
    m_vidYSize = 240;
  }
    
  post("yuv_videoDarwin: height %d width %d",m_vidXSize,m_vidYSize); 
    m_pixBlock.image.xsize = m_vidXSize;
    m_pixBlock.image.ysize = m_vidYSize;

    m_pixBlock.image.csize = 2;
    m_pixBlock.image.format = GL_YCBCR_422_APPLE;
    m_pixBlock.image.type = GL_UNSIGNED_SHORT_8_8_REV_APPLE;
    int dataSize = m_pixBlock.image.xsize * m_pixBlock.image.ysize
					* 2 * sizeof(unsigned char);
    m_pixBlock.image.data = new unsigned char[dataSize];
    m_quality = 1; //high quality for DV. why not?
    InitSeqGrabber();
    m_haveVideo = 1;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
yuv_videoDarwin :: ~yuv_videoDarwin()
{
    if (m_vc) {
        if (::SGDisposeChannel(m_sg, m_vc)) {
            error ("GEM: yuv_videoDarwin: Unable to dispose a video channel");
        }
        m_vc = NULL;
    }
    if (m_sg) {
        if (::CloseComponent(m_sg)) {
            error("GEM: yuv_videoDarwin: Unable to dispose a sequence grabber component");
        }
        m_sg = NULL;
        if (m_srcGWorld) {
            ::DisposeGWorld(m_srcGWorld);
            m_pixMap = NULL;
            m_srcGWorld = NULL;
            m_baseAddr = NULL;
	}
    }
}
/////////////////////////////////////////////////////////
// startrender
//
/////////////////////////////////////////////////////////
void yuv_videoDarwin :: startRendering()
{
     m_haveVideo = 1;
     m_pixBlock.newimage = 1;
}
/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void yuv_videoDarwin :: render(GemState *state)
{
    OSErr	err;
 
    err = SGIdle(m_sg);
    if (err != noErr){
            post("yuv_videoDarwin: SGIdle failed");
            m_haveVideo = 0;
        } else {
        m_haveVideo = 1;
        m_newFrame = 1;
        } 
    if (!m_haveVideo)
    {
	post("GEM: yuv_videoDarwin: no video yet");
	return;
    }
    m_pixBlock.newimage = m_newFrame;
  state->image = &m_pixBlock;
  m_newFrame = 0;
}

/////////////////////////////////////////////////////////
// postrender
//
/////////////////////////////////////////////////////////
void yuv_videoDarwin :: postrender(GemState *state)
{
	m_pixBlock.newimage = 0;
	state->image = NULL;
}

void yuv_videoDarwin :: InitSeqGrabber()
{
    OSErr anErr;
   Rect tempRect = {0,0,m_vidYSize,m_vidXSize} ;
    
    m_srcRect = tempRect ;
   // SetRect (&m_srcRect, 0, 0,m_vidYSize,m_vidXSize); // l,t, r, b
  // OffsetRect(&m_srcRect,  -m_srcRect.left,  -m_srcRect.top);
    m_sg = OpenDefaultComponent(SeqGrabComponentType, 0);
    if(m_sg==NULL){
        post("yuv_videoDarwin: could not open default component");
        return;
    }    else{
            post("yuv_videoDarwin: opened default component");
        }

	anErr = SGInitialize(m_sg);
    if(anErr!=noErr){
        post("yuv_videoDarwin: could not initialize SG");
    }else{
            post("yuv_videoDarwin: initialized SG");
        }
    
    anErr = SGSetDataRef(m_sg, 0, 0, seqGrabDontMakeMovie);
        if (anErr != noErr){
            post("yuv_videoDarwin: dataref failed");
        }else{
            post("yuv_videoDarwin: dataref ok\n");
        }
    
    anErr = SGNewChannel(m_sg, VideoMediaType, &m_vc);		
    if(anErr!=noErr){
        post("yuv_videoDarwin: could not make new SG channnel");
    }else{
            post("yuv_videoDarwin: made new SG channnel");
        }
    anErr = SGSetChannelBounds(m_vc, &m_srcRect);
    if(anErr!=noErr){
        post("yuv_videoDarwin: could not set SG ChannelBounds ");
    }else{
            post("yuv_videoDarwin: set SG ChannelBounds");
        }
    anErr = SGSetChannelUsage(m_vc, seqGrabPreview);
    if(anErr!=noErr){
        post("yuv_videoDarwin: could not set SG ChannelUsage ");
    }else{
            post("yuv_videoDarwin: set SG ChannelUsage");
        }
    
    switch (m_quality){
    case 0:
        anErr = SGSetChannelPlayFlags(m_vc, channelPlayNormal);
        post("yuv_videoDarwin: set SG NormalQuality");
        break;
    case 1:
        anErr = SGSetChannelPlayFlags(m_vc, channelPlayHighQuality);
        post("yuv_videoDarwin: set SG HighQuality");
        break;
    case 2:
        anErr = SGSetChannelPlayFlags(m_vc, channelPlayFast);
        post("yuv_videoDarwin: set SG FastQuality");
        break;
    case 3:
        anErr = SGSetChannelPlayFlags(m_vc, channelPlayAllData);
        post("yuv_videoDarwin: set SG PlayAlldata");
        break;
    
    }
        
    m_rowBytes = m_vidXSize*2;
    anErr = QTNewGWorldFromPtr (&m_srcGWorld,
                                 k422YpCbCr8CodecType,
                                 &m_srcRect, 
                                 NULL, 
                                 NULL, 
                                 0, 
                                 m_pixBlock.image.data, 
                                 m_rowBytes);
	if (anErr!= noErr)
  	{
		post ("yuv_videoDarwin: %d error at QTNewGWorldFromPtr",anErr);
		return;
	}  
    if (NULL == m_srcGWorld)
	{
		post ("yuv_videoDarwin: Could not allocate off screen");
		return;
	}
    SGSetGWorld(m_sg,(CGrafPtr)m_srcGWorld, NULL);
    SGStartPreview(m_sg);
}
 
void yuv_videoDarwin :: resetSeqGrabber()
{
OSErr anErr;
post ("pix_videoDarwin: starting reset");

    post("pix_videoDarwin: quality %d",m_quality);
    switch (m_quality){
    case 0:
        anErr = SGSetChannelPlayFlags(m_vc, channelPlayNormal);
        post("pix_videoDarwin: set SG NormalQuality");
        break;
    case 1:
        anErr = SGSetChannelPlayFlags(m_vc, channelPlayHighQuality);
        post("pix_videoDarwin: set SG HighQuality");
        break;
    case 2:
        anErr = SGSetChannelPlayFlags(m_vc, channelPlayFast);
        post("pix_videoDarwin: set SG FastQuality");
        break;
    case 3:
        anErr = SGSetChannelPlayFlags(m_vc, channelPlayAllData);
        post("pix_videoDarwin: set SG PlayAlldata");
        break;
    
    }
    
} 
 
void yuv_videoDarwin :: obj_setupCallback(t_class *classPtr)
{
    class_addmethod(classPtr, (t_method)&yuv_videoDarwin::qualityCallback,
		  gensym("quality"), A_DEFFLOAT, A_NULL);
    class_addmethod(classPtr, (t_method)&yuv_videoDarwin::resetCallback,
		  gensym("reset"), A_NULL);
}

void yuv_videoDarwin :: qualityCallback(void *data, t_floatarg X)
{
  GetMyClass(data)->m_quality=((int)X);
  
}

void yuv_videoDarwin :: resetCallback(void *data)
{
GetMyClass(data)->resetSeqGrabber();
  
}

#endif // MACOSX
