/*
 *  pix_videoDarwin.cpp
 *  gem_darwin
 *
 *  Created by James Tittle on Fri Jul 12 2002.
 *  Copyright (c) 2002 James Tittle & Chris Clepper
 *
 */
#ifdef __APPLE__

#include "pix_videoDarwin.h"
#include "GemCache.h"
#include <OpenGL/glext.h>
#include <Carbon/Carbon.h>

CPPEXTERN_NEW_WITH_TWO_ARGS(pix_videoDarwin, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)
 
#define DEFAULT_WIDTH		320
#define DEFAULT_HEIGHT		240
#define DEFAULT_FRAMES		5		// 5 fps
#define MAX_RECORDING_TIME	100 * 60	// n * 60 ticks  (n : seconds)
#define DEFAULT_INTERVAL	5		// 5 milliseconds for calling SGIdle()

pix_videoDarwin :: pix_videoDarwin( t_floatarg w, t_floatarg h )
{

  if (w > 0 ){
    m_vidXSize = (int)w;
  }else{
    m_vidXSize = 320;
  }
  if (h > 0){
    m_vidYSize = (int)h;
  }else{
    m_vidYSize = 240;
  }
    
  m_haveVideo = 0;
  post("pix_videoDarwin: height %d width %d",m_vidXSize,m_vidYSize);  
  m_pixBlock.image.xsize = m_vidXSize;
  m_pixBlock.image.ysize = m_vidYSize;
  m_pixBlock.image.csize = 4;
  m_pixBlock.image.format = GL_BGRA_EXT;
  m_pixBlock.image.type = GL_UNSIGNED_INT_8_8_8_8_REV;
  int dataSize = m_pixBlock.image.xsize * m_pixBlock.image.ysize
					* 4 * sizeof(unsigned char);
  m_pixBlock.image.data = new unsigned char[dataSize];
  m_quality = 0; //normal quality gives non-interlaced images from DV cams
  m_colorspace = GL_YCBCR_422_GEM; //default to YUV
  InitSeqGrabber();
  
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_videoDarwin :: ~pix_videoDarwin()
{
   if (m_vc) {
		if (::SGDisposeChannel(m_sg, m_vc)) {
			error ("GEM: pix_video: Unable to dispose a video channel");
		}
		m_vc = NULL;
	}
	if (m_sg) {
		if (::CloseComponent(m_sg)) {
			error("GEM: pix_video: Unable to dispose a sequence grabber component");
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
void pix_videoDarwin :: startRendering()
{
     m_haveVideo = 1;
     m_pixBlock.newimage = 1;
}
/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void pix_videoDarwin :: render(GemState *state)
{
    OSErr	err;
 
    err = SGIdle(m_sg);
    if (err != noErr){
            post("pix_videoDarwin: SGIdle failed\n");
            m_haveVideo = 0;
        } else {
        m_haveVideo = 1;
        m_newFrame = 1;
        } 
    if (!m_haveVideo)
    {
	post("GEM: pix_videoDarwin: no video yet");
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
void pix_videoDarwin :: postrender(GemState *state)
{
  m_pixBlock.newimage = 0;
state->image = NULL;
  
}

/////////////////////////////////////////////////////////
// startTransfer
//
/////////////////////////////////////////////////////////
int pix_videoDarwin :: startTransfer()
{
     m_haveVideo = 1;
     m_pixBlock.newimage = 1;
     return 1;
}

/////////////////////////////////////////////////////////
// startTransfer
//
/////////////////////////////////////////////////////////
int pix_videoDarwin :: stopTransfer()
{
     
     return 1;
}

/*---------------------------------------------------------------------*/
//	video settings dialog to setup camera				//
/*---------------------------------------------------------------------*/
pascal Boolean pix_videoDarwin :: SeqGrabberModalFilterProc (DialogPtr theDialog, const EventRecord *theEvent, short *itemHit, long refCon){
//#pragma unused(theDialog, itemHit)
    Boolean	handled = false;

    if ((theEvent->what == updateEvt) &&
        ((WindowPtr) theEvent->message == (WindowPtr) refCon))
    {
        BeginUpdate ((WindowPtr) refCon);
        EndUpdate ((WindowPtr) refCon);
        handled = true;
    } 
    
     WindowRef  awin = GetDialogWindow(theDialog);
    ShowWindow (awin);
    SetWindowClass(awin,kUtilityWindowClass);
    //ChangeWindowAttributes(awin,kWindowStandardHandlerAttribute,0);     	SGPanelEvent(m_sg,m_vc,theDialog,0,theEvent,itemHit,&handled);
  //  AEProcessAppleEvent (theEvent);
    
    return (handled);
}

void pix_videoDarwin :: DoVideoSettings(){
    Rect	newActiveVideoRect;
    Rect	curBounds, curVideoRect, newVideoRect;
    ComponentResult	err;
    SGModalFilterUPP	seqGragModalFilterUPP;

    // Get our current state - do i need this???
    err = SGGetChannelBounds (m_vc, &curBounds);
    err = SGGetVideoRect (m_vc, &curVideoRect);

    // Pause
    err = SGPause (m_sg, true);

    // Do the dialog thang
    seqGragModalFilterUPP = (SGModalFilterUPP)NewSGModalFilterUPP(SeqGrabberModalFilterProc);
    err = SGSettingsDialog(m_sg, m_vc, 0,
                           NULL, seqGrabSettingsPreviewOnly, seqGragModalFilterUPP, (long)m_srcGWorld);
    DisposeSGModalFilterUPP(seqGragModalFilterUPP);

    // What happened?
    err = SGGetVideoRect (m_vc, &newVideoRect);
    err = SGGetSrcVideoBounds (m_vc, &newActiveVideoRect);

    err = SGPause (m_sg, false);
}


void pix_videoDarwin :: InitSeqGrabber()
{
    OSErr anErr;
    Rect m_srcRect = {0,0, m_vidYSize, m_vidXSize};
    int num_components = 0;
    Component c = 0;
     ComponentDescription cd;
     
     cd.componentType = SeqGrabComponentType;
     cd.componentSubType = 0;
     cd.componentManufacturer = 0;
     cd.componentFlags = 0;
     cd.componentFlagsMask = 0;
     
     while((c = FindNextComponent(c, &cd)) != 0) {
       num_components++;  }                 // add component c to the list.
  //   post("pix_videoDarwin: number of SGcomponents: %d",num_components);
    m_sg = OpenDefaultComponent(SeqGrabComponentType, 0);
    if(m_sg==NULL){
        post("pix_videoDarwin: could not open defalut component");
        return;
    }    
	anErr = SGInitialize(m_sg);
    if(anErr!=noErr){
        post("pix_videoDarwin: could not initialize SG");
    }
    
    anErr = SGSetDataRef(m_sg, 0, 0, seqGrabDontMakeMovie);
        if (anErr != noErr){
            post("dataref failed\n");
        }
        
    anErr = SGNewChannel(m_sg, VideoMediaType, &m_vc);		
    if(anErr!=noErr){
        post("pix_videoDarwin: could not make new SG channnel");
    }
    
    anErr = SGSetChannelBounds(m_vc, &m_srcRect);
    if(anErr!=noErr){
        post("pix_videoDarwin: could not set SG ChannelBounds ");
    }
        
    anErr = SGSetChannelUsage(m_vc, seqGrabPreview);
    if(anErr!=noErr){
        post("pix_videoDarwin: could not set SG ChannelUsage ");
    }
    m_rowBytes = m_vidXSize*4;
    
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
    if (m_colorspace==GL_BGRA_EXT){
        m_pixBlock.image.xsize = m_vidXSize;
        m_pixBlock.image.ysize = m_vidYSize;
        m_pixBlock.image.csize = 4;
        m_pixBlock.image.format = GL_BGRA_EXT;
        m_pixBlock.image.type = GL_UNSIGNED_INT_8_8_8_8_REV;
        int dataSize = m_pixBlock.image.xsize * m_pixBlock.image.ysize
                                            * 4 * sizeof(unsigned char);
        m_pixBlock.image.data = new unsigned char[dataSize]; 
         m_rowBytes = m_vidXSize*4;
        anErr = QTNewGWorldFromPtr (&m_srcGWorld,
                                    k32ARGBPixelFormat,
                                    &m_srcRect, 
                                    NULL, 
                                    NULL, 
                                    0, 
                                    m_pixBlock.image.data, 
                                    m_rowBytes);
                                    
        post ("pix_videoDarwin: Using RGB");                         
        }else{
            m_pixBlock.image.xsize = m_vidXSize;
            m_pixBlock.image.ysize = m_vidYSize;
            m_pixBlock.image.csize = 2;
            m_pixBlock.image.format = GL_YCBCR_422_APPLE;
            m_pixBlock.image.type = GL_UNSIGNED_SHORT_8_8_REV_APPLE;
            int dataSize = m_pixBlock.image.xsize * m_pixBlock.image.ysize
                                                * 2 * sizeof(unsigned char);
            m_pixBlock.image.data = new unsigned char[dataSize]; 
             m_rowBytes = m_vidXSize*2;
            anErr = QTNewGWorldFromPtr (&m_srcGWorld,
                                    k422YpCbCr8CodecType,
                                    &m_srcRect, 
                                    NULL, 
                                    NULL, 
                                    0, 
                                    m_pixBlock.image.data, 
                                    m_rowBytes);
            
        post ("pix_videoDarwin: Using YUV");
        }
        
	if (anErr!= noErr)
  	{
		post ("pix_videoDarwin: %d error at QTNewGWorldFromPtr", anErr);
		return;
	}  
    if (NULL == m_srcGWorld)
	{
		post ("Could not allocate off screen");
		return;
	}
    SGSetGWorld(m_sg,(CGrafPtr)m_srcGWorld, NULL);
    SGStartPreview(m_sg);
    m_haveVideo = 1;
    
}
 
void pix_videoDarwin :: destroySeqGrabber()
{
    if (m_vc) {
		if (::SGDisposeChannel(m_sg, m_vc)) {
			error ("GEM: pix_video: Unable to dispose a video channel");
		}
		m_vc = NULL;
	}
	if (m_sg) {
		if (::CloseComponent(m_sg)) {
			error("GEM: pix_video: Unable to dispose a sequence grabber component");
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

void pix_videoDarwin :: resetSeqGrabber()
{
    OSErr anErr;
    post ("pix_videoDarwin: starting reset");

    destroySeqGrabber();
    InitSeqGrabber();

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

/////////////////////////////////////////////////////////
// dimenMess
//
/////////////////////////////////////////////////////////
void pix_videoDarwin :: dimenMess(int x, int y, int leftmargin, int rightmargin,
    int topmargin, int bottommargin)
{
    if (x > 0 ){
        m_vidXSize = (int)x;
    }else{
        m_vidXSize = 320;
    }
    if (y > 0){
        m_vidYSize = (int)y;
    }else{
        m_vidYSize = 240;
    }
    
  post("pix_videoDarwin: height %d width %d",m_vidXSize,m_vidYSize);  
//  m_pixBlock.image.xsize = m_vidXSize;
//  m_pixBlock.image.ysize = m_vidYSize;
    
}

/////////////////////////////////////////////////////////
// colorspaceMess
//
/////////////////////////////////////////////////////////
void pix_videoDarwin :: csMess(int format)
{
    m_colorspace = format;
    if (format == GL_RGBA) post("pix_videoDarwin: colorspace is GL_RGBA %d",m_colorspace);
    else
        if (format == GL_BGRA_EXT) post("pix_videoDarwin: colorspace is GL_RGBA %d",m_colorspace);
    else
        if (format == GL_YCBCR_422_GEM) post("pix_videoDarwin: colorspace is YUV %d",m_colorspace);
    else post("pix_videoDarwin: colorspace is unknown %d",m_colorspace);
}

/////////////////////////////////////////////////////////
// dialog
//
/////////////////////////////////////////////////////////
//void pix_videoDarwin :: dialogMess(int argc, t_atom*argv)
void pix_videoDarwin :: dialogMess(int argc, t_atom*argv)
{
    DoVideoSettings();
}

void pix_videoDarwin :: obj_setupCallback(t_class *classPtr)
{
class_addcreator((t_newmethod)_classpix_videoDarwin,gensym("pix_video"),A_DEFFLOAT,A_DEFFLOAT,A_NULL);
pix_video::real_obj_setupCallback(classPtr);
    class_addmethod(classPtr, (t_method)&pix_videoDarwin::qualityCallback,
		  gensym("quality"), A_DEFFLOAT, A_NULL);
    class_addmethod(classPtr, (t_method)&pix_videoDarwin::resetCallback,
		  gensym("reset"), A_NULL);
   // class_addmethod(classPtr, (t_method)&pix_videoDarwin::dialogCallback,
//		  gensym("dialog"), A_NULL);
    class_addmethod(classPtr, (t_method)&pix_videoDarwin::dialogMess,
                    gensym("dialog"), A_GIMME, A_NULL);
    class_addmethod(classPtr, (t_method)&pix_videoDarwin::colorspaceCallback,
		  gensym("colorspace"), A_SYMBOL, A_NULL);
//    class_addmethod(classPtr, (t_method)&pix_videoDarwin::csMessCallback,
 //                   		  gensym("colorspace"), A_SYMBOL, A_NULL);
}

void pix_videoDarwin :: qualityCallback(void *data, t_floatarg X)
{
  GetMyClass(data)->m_quality=((int)X);
  
} 

void pix_videoDarwin :: resetCallback(void *data)
{
GetMyClass(data)->resetSeqGrabber();
  
}

void pix_videoDarwin ::dialogCallback(void *data)
{
    
//GetMyClass(data)->DoVideoSettings();
  
}
 
void pix_videoDarwin :: csMessCallback(void *data, t_symbol*s)
{
   // GetMyClass(data)->csMess(getPixFormat(s->s_name));
    post("pix_video: Am I even being called??????????????");
    int format=0;
    char c =*s->s_name;
    switch (c){
        case 'g': case 'G': format=GL_LUMINANCE; break;
        case 'y': case 'Y': format=GL_YCBCR_422_GEM; break;
        case 'r': case 'R': format=GL_RGBA; break;
        default:
            post("pix_video: colorspace must be 'RGBA', 'YUV' or 'Gray'");
    }

    if(format==GL_LUMINANCE){
        post("pix_video: 'Gray' not yet supported...using YUV");
        format=GL_YCBCR_422_GEM;
    }
    if(format)GetMyClass(data)->csMess(format);
    if(format)GetMyClass(data)->m_colorspace = format;
}


void pix_videoDarwin :: colorspaceCallback(void *data, t_symbol *state)
{
  int format=0;
  char c =*state->s_name;
  switch (c){
  case 'g': case 'G': format=GL_LUMINANCE; break;
  case 'y': case 'Y': format=GL_YCBCR_422_GEM; break;
  case 'r': case 'R': format=GL_RGBA; break;
  default:
    post("pix_video: colorspace must be 'RGBA', 'YUV' or 'Gray'");
  }

  if(format==GL_LUMINANCE){
    post("pix_video: 'Gray' not yet supported...using YUV");
    format=GL_YCBCR_422_GEM;
  }
  if(format)GetMyClass(data)->m_colorspace = format;
}

#endif // __APPLE__
