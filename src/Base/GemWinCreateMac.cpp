////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// tigital@mac.com
//
// Implementation file
//
//    Copyright (c) 1997-2000 Mark Danks.
//    Copyright (c) 2002 Jamie Tittle
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#ifdef __APPLE__

#include <Carbon/Carbon.h>
#include "GemWinCreate.h"
#include "GemMan.h"
#include "GemEvent.h"
#include "m_pd.h"

#define PIXEL_SIZE	32		// 16 or 32
#define DEPTH_SIZE	16

struct structGLInfo // storage for setup info
{
    SInt16 width;		// input: width of drawable (screen width in full screen mode), return: actual width allocated
    SInt16 height;		// input: height of drawable (screen height in full screen mode), return: actual height allocated
    UInt32 pixelDepth;		// input: requested pixel depth
    Boolean fDepthMust;		// input: pixel depth must be set (if false then current depth will be used if able)
    Boolean fFullscreen;	// input: use DSp to get fullscreen? (or find full screen renderer)
                                // if fFullscreen, will search for full screen renderers first then use DSp for others
                                //  unless a device is specified, in which case we will try there first
    Boolean fAcceleratedMust; 	// input: must renderer be accelerated?
    GLint aglAttributes[64]; 	// input: pixel format attributes always required (reset to what was actually allocated)
    SInt32 VRAM;		// input: minimum VRAM; output: actual (if successful otherwise input)
    SInt32 textureRAM;		// input: amount of texture RAM required on card; output: same (used in allcoation to ensure enough texture
    AGLPixelFormat	fmt;	// input: none; output pixel format...
    SInt32 freq;		// input: frequency request for display; output: actual
};
typedef struct structGLInfo structGLInfo;
typedef struct structGLInfo * pstructGLInfo;

// structure for creating a context from a window
struct structGLWindowInfo // storage for setup info
{
    Boolean fAcceleratedMust; 	// input: must renderer be accelerated?
    GLint aglAttributes[64]; 	// input: pixel format attributes always required (reset to what was actually allocated)
    SInt32 VRAM;		// input: minimum VRAM; output: actual (if successful otherwise input)
    SInt32 textureRAM;		// input: amount of texture RAM required on card; output: same (used in allcoation to ensure enough texture
    AGLPixelFormat	fmt;	// input: none; output pixel format...
    Boolean fDraggable;		// input: is window going to be dragable, 
                                //        if so renderer check (accel, VRAM, textureRAM) will look at all renderers vice just the current one
                                //        if window is not dragable renderer check will either check the single device or short 
                                //            circuit to software if window spans multiple devices 
                                // software renderer is consider to have unlimited VRAM, unlimited textureRAM and to not be accelerated
};
typedef struct structGLWindowInfo structGLWindowInfo;
typedef struct structGLWindowInfo * pstructGLWindowInfo;

// globals (internal/private) -----------------------------------------------

const RGBColor		rgbBlack = { 0x0000, 0x0000, 0x0000 };

const short kWindowType = kWindowDocumentProc;

// GL stuff
Boolean gfHasPackedPixels = false;
//structGLInfo		glInfo;
structGLWindowInfo	glWInfo;
AGLDrawable 		gaglDraw = NULL;
//Rect 			grectWin;
AGLContext 		gaglContext = 0;
GLuint 			gFontList = 0;
char 			gInfoString [512] = "";
static bool 		haveContext = false;

EventHandlerUPP		gEvtHandler;
EventHandlerRef		ref;
ProcessSerialNumber	psn = {0, kCurrentProcess };
static pascal OSStatus evtHandler(EventHandlerCallRef myHandler, EventRef event, void* userData);

void SetContextValid (bool val);
bool HaveValidContext (void);
AGLContext SetupAGLFullScreen (GDHandle display, short * pWidth, short * pHeight);

// Prototypes
// -------------------------------------------------------------------------------------------------------------
// Takes device # and geometry request and tries to build best context and drawable
// 	If requested device does not work, will start at first device and walk down devices 
//	 looking for first one that satisfies requirments
//  Devices are numbered in order that DMGetFirstScreenDevice/DMGetNextScreenDevice returns, 
//	 fullscreen devices are numbered after this, but they will be searched first if fFullscreen == true,
//	 they will not be searched in the non-fullscreen case

// Inputs: 	*pnumDevice: -1: main device, 0: any device, other #: attempt that device first, then any device
//			*pcontextInfo: request and requirements for cotext and drawable

// Outputs: *paglDraw, *paglContext and *pdspContext as allocated
//			*pnumDevice to device number in list that was used 
//			*pcontextInfo:  allocated parameters

// If fail to build context: paglDraw, paglContext and pdspContext will be NULL
// If fatal error: will return error and paglDraw, paglContext and pdspContext will be NULL
// Note: Errors can be generated internally when a specific device fails, this is normal and these
//		  will not be returned is a subsequent device succeeds

OSStatus BuildGL (AGLDrawable* paglDraw, AGLContext* paglContext, //DSpContextReference* pdspContext, 
				  short* pnumDevice, pstructGLInfo pcontextInfo, AGLContext aglShareContext);
                                  
static OSStatus BuildGLonDevice (AGLDrawable* paglDraw, AGLContext* paglContext, //DSpContextReference* pdspContext, 
				  		  GDHandle hGD, pstructGLInfo pcontextInfo, AGLContext aglShareContext);

static OSStatus BuildGLonWindow (WindowPtr pWindow, AGLContext* paglContext, pstructGLWindowInfo pcontextInfo, AGLContext aglShareContext);
static OSStatus BuildDrawable (AGLDrawable* paglDraw, GDHandle hGD, pstructGLInfo pcontextInfo);

static OSStatus BuildGLContext (AGLDrawable* paglDraw, AGLContext* paglContext, /*DSpContextReference* pdspContext,*/ GDHandle hGD, 
								pstructGLInfo pcontextInfo, AGLContext aglShareContext);
static void DumpCurrent (AGLDrawable* paglDraw, AGLContext* paglContext, /*DSpContextReference* pdspContext,*/ pstructGLInfo pcontextInfo);
static Boolean CheckRenderer (GDHandle hGD, long *VRAM, long *textureRAM, GLint* pDepthSizeSupport, Boolean fAccelMust);
static Boolean CheckAllDeviceRenderers (long* pVRAM, long* pTextureRAM, GLint* pDepthSizeSupport, Boolean fAccelMust);
static Boolean CheckWindowExtents (GDHandle hGD, short width, short height);

// Destroys drawable and context
// Ouputs: *paglDraw, *paglContext and *pdspContext should be 0 on exit

OSStatus DestroyGL (AGLDrawable* paglDraw, AGLContext* paglContext, /*DSpContextReference* pdspContext,*/ pstructGLInfo pcontextInfo);

// same as above except that it takes a window as input and attempts to build requested conext on that
OSStatus BuildGLFromWindow (WindowPtr pWindow, AGLContext* paglContext, pstructGLWindowInfo pcontextInfo, AGLContext aglShareContext);

// same as above but destorys a context that was associated with an existing window, window is left intacted
OSStatus DestroyGLFromWindow (AGLContext* paglContext, pstructGLWindowInfo pcontextInfo);

// Handle reporting of agl errors, error code is passed through
OSStatus aglReportError (void);

// Handle reporting of OpenGL errors, error code is passed through
OSStatus glReportError (void);

// Runtime check to see if we are running on Mac OS X
// Inputs:  None
// Returns: 0 if < Mac OS X or version number of Mac OS X (10.0 for GM)
UInt32 CheckMacOSX (void);

short FindGDHandleFromRect (Rect * pRect, GDHandle * phgdOnThisDevice);

short FindGDHandleFromWindow (WindowPtr pWindow, GDHandle * phgdOnThisDevice);



/////////////////////////////////////////////////////////

GEM_EXTERN void gemAbortRendering();

#pragma mark -----functions-----
/////////////////////////////////////////////////////////
// createGemWindow
//
// this takes the parameters passed from the gemwin and decides which AGL paths to take
//
/////////////////////////////////////////////////////////
GEM_EXTERN int createGemWindow(WindowInfo &info, WindowHints &hints)
{
    OSStatus	err;
    GDHandle hGD, hTargetDevice = NULL;
    short numDevices = 0;
    short whichDevice = 1; // number of device to try (0 = 1st device, 1 = 2nd/external device)
#ifdef DEBUG
    post("MAC: createGemWindow()");
#endif
    EventTypeSpec	list[] = {  //{ kEventClassApplication, kEventAppActivated },
                                    //{ kEventClassWindow, kEventWindowCursorChange },
                                    //{ kEventClassWindow,  kEventWindowCollapsing },
                                    //{ kEventClassWindow, kEventWindowCollapsed },
                                    //{ kEventClassWindow, kEventWindowShown },
                                    { kEventClassWindow, kEventWindowActivated },
                                    //{ kEventClassWindow, kEventWindowDeactivated },
                                    { kEventClassWindow, kEventWindowClose },
                                    //{ kEventClassWindow, kEventWindowDrawContent },
                                    //{ kEventClassWindow, kEventWindowBoundsChanged },
                                    //{ kEventClassWindow, kEventWindowZoomed },
                                    { kEventClassMouse, kEventMouseDown },
                                    { kEventClassMouse, kEventMouseUp },
                                    { kEventClassMouse, kEventMouseMoved },
                                    { kEventClassMouse, kEventMouseDragged },
                                    { kEventClassMouse, kEventMouseWheelMoved },
                                    { kEventClassKeyboard, kEventRawKeyDown },
                                    { kEventClassKeyboard, kEventRawKeyUp } };

    short i =0;
	
    // Build GL context and window or fullscreen

    if (!hints.actuallyDisplay){
        return(1);
    }
    // look for connected graphics devices
    hGD = DMGetFirstScreenDevice (true); // check number of screens
    hTargetDevice = hGD; // default to first device

    
    
    //////////////////////////////////////////////////////////////////
    //check for a fullscreen request and then do the 10.3 workaround
    //
    // NOTE - this does NOT set the hints.height and width so the viewport is still the default
    // maybe it's better to have a standard viewport so screen resizing doesn't change geometry settings?
    //
    if (hints.fullscreen) {
        ////////////////////////////////////////////
        // a new CGL method of determining the number of attached displays and their coords
        //
        
        CGDisplayCount maxDisplays = 32;
        CGDirectDisplayID activeDspys[32];
        CGDisplayErr error;
        CGDisplayCount newDspyCnt = 0;
        CGRect displayRect;
        
        error = CGGetActiveDisplayList(maxDisplays, activeDspys, &newDspyCnt);
        if (error) {
            post("GemWinCreateMac: CGGetActiveDisplayList returned error %d", error);
        }

        post("GemWinCreateMac: newDspyCnt %d", newDspyCnt);

        for (i=0; i < newDspyCnt; i++){
            CGRect displayRect = CGDisplayBounds (activeDspys[i]);
            post("GemWinCreateMac: display %d width %d height %d origin.x %d origin.y %d", i, (long)displayRect.size.width, (long)displayRect.size.height, (long)displayRect.origin.x, (long)displayRect.origin.y);
        }
        
        post("GemWinCreateMac: attempting fullscreen on display %d",hints.fullscreen-1);
        if (hints.fullscreen-1 > newDspyCnt){
            post("GemWinCreateMac: display %d does not exist",hints.fullscreen-1);
            return 0;
        }

        //the device should be the first one so this will find the next one until it gets the user requested device
        //i think this should work
        whichDevice = hints.fullscreen;
        do
        {
            if (numDevices == whichDevice)
                hTargetDevice = hGD; // if device number matches
            numDevices++;
            hGD = DMGetNextScreenDevice (hGD, true);
        }
        while (hGD);

        //grab the coords of the requested display
        displayRect = CGDisplayBounds (activeDspys[hints.fullscreen-1]);
        //hTargetDevice = hGD;

        //set a rect to cover the entire selected display
        SetRect(&info.r,
                (long)displayRect.origin.x,
                (long)displayRect.origin.y,
                ((long)displayRect.size.width + (long)displayRect.origin.x),
                ((long)displayRect.size.height + (long)displayRect.origin.y)
                );

        //this winodw has no attributes like a title bar etc
        err = CreateNewWindow ( kDocumentWindowClass,
                                kWindowNoAttributes,
                                &info.r,
                                &info.pWind );
        
        if (err)
        {
            post("GemWinCreateMac: Fullscreen CreateNewWindow err = %d",err);
            return 0;
        }
      //  gaglDraw = GetWindowPort( info.pWind );
    }
    else{ //go the usual windowed way
    
    ///////////////////////////////////////////
    //find the right device
    //might be redundant now with the new fullscreen stuff??
    do
    {
        if (numDevices == whichDevice)
            hTargetDevice = hGD; // if device number matches						
        numDevices++;
        hGD = DMGetNextScreenDevice (hGD, true);
    }
    while (hGD);
    post("GemwinMac: width - %d height - %d",hints.width,hints.height);

    /* //old fullscreen method that doesn't work so hot on 10.3
    //change this to use the fullscreen message
    //the device number will be (fullscreen -1) so 'fullscreen 1' is main, 'fullscreen 2' is second, etc 
    //
    //if (( numDevices > 1 ) && (hints.fullscreen))
    if (( numDevices > 1 ) && (hints.secondscreen)) // Try AGL full screen on 2nd device if more than one device found
    {
        short width = 640, height = 480; 
        
        if ((hints.width < 640) || (hints.height < 480)){
            width = 640; height = 480;
        }
        else{
            height = hints.height;
            width = hints.width;
        }
        info.context = SetupAGLFullScreen (hTargetDevice, &width, &height); // Setup the OpenGL context
        ::SetRect(&info.r, (short)hints.x_offset, (short)hints.y_offset, 
                                (short)(hints.width + hints.x_offset), (short)(hints.height + hints.y_offset)); // l, t, r, b
        post( "AGL Full Screen: %d x %d", width, height);
        
    }else{	// end of fullscreen creation on 2nd/external device
        */

    // show and update main window

    // this should put the title bar below the menu bar
        if (hints.y_offset < 50){
            hints.y_offset+=50; 
        }
        
        SetRect(&info.r, (short)hints.x_offset, (short)hints.y_offset,
                (short)(hints.width + hints.x_offset),
                (short)(hints.height + hints.y_offset));

        err = CreateNewWindow ( kDocumentWindowClass,
                                //kMovableModalWindowClass,
                                //kFloatingWindowClass,
                                kWindowStandardHandlerAttribute | kWindowCloseBoxAttribute | kWindowCollapseBoxAttribute,
                                &info.r,
                                &info.pWind );
        if (err)
        {
            post("err = %d",err);
            return 0;
        }

        //this should take whatever input the user sets with the gemwin 'title' message?
        SetWindowTitleWithCFString ( info.pWind, CFSTR("GEM") );

        gaglDraw = GetWindowPort( info.pWind );
        
    }//end of conditional for fullscreen vs windowed
    
        gEvtHandler = NewEventHandlerUPP( evtHandler );
        InstallEventHandler( GetApplicationEventTarget(), gEvtHandler,
                                        GetEventTypeCount( list ), list,
                                        info.pWind, &ref );
        //InstallWindowEventHandler ( info.pWind, gEvtHandler, GetEventTypeCount (list),
        //                            list, info.pWind, &ref );

        glWInfo.fAcceleratedMust = true; 		// must renderer be accelerated?
        glWInfo.VRAM = 0 * 1048576;			// minimum VRAM (if not zero this is always required)
        glWInfo.textureRAM = 0 * 1048576;		// minimum texture RAM (if not zero this is always required)
        glWInfo.fDraggable = false; 		// desired vertical refresh frquency in Hz (0 = any)
        glWInfo.fmt = 0;				// output pixel format
		
        i = 0;
        glWInfo.aglAttributes [i++] = AGL_RGBA;
        glWInfo.aglAttributes [i++] = AGL_PIXEL_SIZE;
        glWInfo.aglAttributes [i++] = 32;
        glWInfo.aglAttributes [i++] = AGL_DEPTH_SIZE;
        glWInfo.aglAttributes [i++] = 16;
        if (hints.buffer == 2){
            glWInfo.aglAttributes [i++] = AGL_DOUBLEBUFFER;
        }
        //going to try for some FSAA here
        if (hints.fsaa){
            glWInfo.aglAttributes [i++] = AGL_SAMPLE_BUFFERS_ARB;
            glWInfo.aglAttributes [i++] = 1;
            glWInfo.aglAttributes [i++] = AGL_SAMPLES_ARB;
            glWInfo.aglAttributes [i++] = hints.fsaa;
        }
    
        glWInfo.aglAttributes [i++] = AGL_ACCELERATED;
        glWInfo.aglAttributes [i++] = AGL_NO_RECOVERY; 	// should be used whenever packed pixels is used to 
                                                        //	disable software back up textures
        glWInfo.aglAttributes [i++] = AGL_NONE;
/*        
        info.pWind = ::GetWindowFromPort(gaglDraw);
        BuildGLFromWindow (GetWindowFromPort(gaglDraw), &info.context, &glWInfo, hints.shared);
*/        
        BuildGLFromWindow ( info.pWind, &info.context, &glWInfo, hints.shared);
   // }// end of window creation on main device - this is the old fullscreen code
    
    if (!info.context){
        post("MAC:  no info.context");
        return (0);
    }
    SetContextValid (true);
    hints.real_w = hints.width;
    hints.real_h = hints.height;
    info.fs = 0;//hints.fullscreen;
    
    SetFrontProcess( &psn );
    
    ShowWindow ( info.pWind );
    // um, this may be overkill?
    SelectWindow( info.pWind );
    err = ActivateWindow( info.pWind, true );
    post("GemWindow Activate err = %d", err );

#ifdef DEBUG
    post("createGemWindow() finished");
    post("hints: actuallyDisplay = %d",hints.actuallyDisplay);
    post("hints: border = %d",hints.border);
    post("hints: width = %d",hints.width);
    post("hints: height = %d",hints.height);
    post("hints: real_w = %d",hints.real_w);
    post("hints: real_h = %d",hints.real_h);
    post("hints: x_offset = %d",hints.x_offset);
    post("hints: y_offset = %d", hints.y_offset);
    post("hints: fullscreen = %d", hints.fullscreen);
    post("hints: border = %d", hints.border);
    post("hints: display = %s",hints.display);
    post("hints: title = %s",hints.title);
    post("hints: shared = %d",hints.shared);
    post("hints: fsaa = %d",hints.fsaa);
#endif
    hGD = NULL;
    return(1);
}

/////////////////////////////////////////////////////////
// destroyGemWindow
//
/////////////////////////////////////////////////////////
GEM_EXTERN void destroyGemWindow(WindowInfo &info)
{
#ifdef DEBUG
    post("destroyGemWindow()");
#endif
    if (info.offscreen) {
        if (info.context) {
            ::aglSetCurrentContext(NULL);
            ::aglSetDrawable(info.context, NULL);
            ::aglDestroyContext(info.context);
            info.context  = NULL;
        }
        RemoveEventHandler( ref );
        ::UnlockPixels(info.pixMap);
        ::DisposeGWorld(info.offscreen);
        info.offscreen = NULL;
        ::DisposeWindow( ::GetWindowFromPort(gaglDraw) );
        return;
    }
    if (info.context)
    {
        ::aglSetCurrentContext(NULL);
        ::aglSetDrawable(info.context, NULL);
        ::aglDestroyContext(info.context);
        info.context  = NULL;
#ifdef DEBUG
        post("destroy context done");
#endif
    }

    if (info.pWind){
      post("destroyGemWindow() DisposeWindow");  
    ::DisposeWindow( info.pWind );
#ifdef DEBUG
    post("destroyGemWindow() finished");
#endif
    }else post("no info.pWind to destroy!!");
}
int cursorGemWindow(WindowInfo &info, int state)
{
    return(0); 
}
//-----------------------------------------------------------------------------------------------------------------------
// SetupAGLFullScreen, for 2nd monitor
//-----------------------------------------------------------------------------------------------------------------------
AGLContext SetupAGLFullScreen (GDHandle display, short * pWidth, short * pHeight)
{
	GLint		attrib[64];
	AGLPixelFormat 	fmt;
	AGLContext     	ctx;
	
// different possible pixel format choices for different renderers 
// basics requirements are RGBA and double buffer
// OpenGL will select acclerated context if available

	short i = 0;
	attrib [i++] = AGL_RGBA; // red green blue and alpha
	attrib [i++] = AGL_DOUBLEBUFFER; // double buffered
	attrib [i++] = AGL_ACCELERATED; // HWA pixel format only
	attrib [i++] = AGL_FULLSCREEN;
	attrib [i++] = AGL_PIXEL_SIZE;
	attrib [i++] = 32;
        attrib [i++] = AGL_DEPTH_SIZE;
        attrib [i++] = 16;
	attrib [i++] = AGL_NONE;	

	fmt = aglChoosePixelFormat(&display, 1, attrib); // this may fail if looking for acclerated across multiple monitors
	if (NULL == fmt) 
	{
		post("Could not find valid pixel format");
		return NULL;
	}

	ctx = aglCreateContext (fmt, NULL); // Create an AGL context
	if (NULL == ctx)
	{
		post ("Could not create context");
		return NULL;
	}
        
        ::aglEnable( ctx, AGL_FS_CAPTURE_SINGLE );

	//if (!aglSetFullScreen (ctx, *pWidth, *pHeight, 60, 0))
        if (!aglSetFullScreen (ctx, *pWidth, *pHeight, 0, 0))
	{
		post ("SetFullScreen failed");
		return NULL;
	}

	if (!aglSetCurrentContext (ctx)) // make the context the current context
	{
		post ("SetCurrentContext failed");
		aglSetDrawable (ctx, NULL); // turn off full screen
		return NULL;
	}

	aglDestroyPixelFormat(fmt); // pixel format is no longer needed

	return ctx;
}

//-----------------------------------------------------------------------------------------------------------------------

// BuildGLFromWindow

// Takes window in the form of an AGLDrawable and geometry request and tries to build best context

// Inputs: 	aglDraw: a valid AGLDrawable (i.e., a WindowPtr)
//			*pcontextInfo: request and requirements for cotext and drawable

// Outputs: *paglContext as allocated
//			*pcontextInfo:  allocated parameters

// if fail to allocate: paglContext will be NULL
// if error: will return error and paglContext will be NULL

OSStatus BuildGLFromWindow (WindowPtr pWindow, AGLContext* paglContext, pstructGLWindowInfo pcontextInfo, AGLContext aglShareContext)
{
    if (!pWindow){
        return paramErr;
    }
    return BuildGLonWindow (pWindow, paglContext, pcontextInfo, aglShareContext);
}
// --------------------------------------------------------------------------

// BuildGLonDrawable

// Takes a drawable and tries to build on it

// Inputs: 	aglDraw: a valid AGLDrawable
//			*pcontextInfo: request and requirements for cotext and drawable

// Outputs: *paglContext as allocated
//			*pcontextInfo:  allocated parameters

// if fail to allocate: paglContext will be NULL
// if error: will return error and paglContext will be NULL

static OSStatus BuildGLonWindow (WindowPtr pWindow, AGLContext* paglContext, pstructGLWindowInfo pcontextInfo, AGLContext aglShareContext)
{
    GDHandle hGD = NULL;
    GrafPtr cgrafSave = NULL;
    short numDevices;
    GLint depthSizeSupport;
    OSStatus err = noErr;
#ifdef DEBUG
    post("MAC: BuildGLonWindow entered");
#endif
    if (!pWindow || !pcontextInfo)
    {
#ifdef DEBUG
        post("MAC: BuildGLonWindow: no drawable");
#endif
        return paramErr;
    }
	
    GetPort (&cgrafSave);
    SetPortWindowPort(pWindow);

    // check renderere VRAM and acceleration
    numDevices = FindGDHandleFromWindow (pWindow, &hGD);
    if (!pcontextInfo->fDraggable) 	// if numDevices > 1 then we will only be using the software renderer otherwise check only window device
    {
#ifdef DEBUG
        post("MAC: BuildGLonWindow: fDraggable= false");
#endif
        if ((numDevices > 1) || (numDevices == 0)) // this window spans mulitple devices thus will be software only
        {
#ifdef DEBUG
            post("MAC: BuildGLonWindow: numDevices>1 || numDevices ==0");
#endif
            // software renderer
            // infinite VRAM, infinite textureRAM, not accelerated
            if (pcontextInfo->fAcceleratedMust)
            {
#ifdef DEBUG
                post("MAC: BuildGLonWindow: Unable to accelerate window that spans multiple devices");
#endif
                return err;
            }
        }
        else // not draggable on single device
        {
#ifdef DEBUG
        post("MAC: BuildGLonWindow: not draggable on single device");
#endif
        if (!CheckRenderer (hGD, &(pcontextInfo->VRAM), &(pcontextInfo->textureRAM), &depthSizeSupport, pcontextInfo->fAcceleratedMust))
        {
#ifdef DEBUG
            post("MAC: BuildGLonWindow: Renderer check failed 1");
#endif
            return err;
        }
    }
}
	// else draggable so must check all for support (each device should have at least one renderer that meets the requirements)
	else if (!CheckAllDeviceRenderers (&(pcontextInfo->VRAM), &(pcontextInfo->textureRAM), &depthSizeSupport, pcontextInfo->fAcceleratedMust))
	{
#ifdef DEBUG
            post("MAC: BuildGLonWindow: Renderer check failed 2");
#endif
            return err;
	}
	
	// do agl
	if ((Ptr) kUnresolvedCFragSymbolAddress == (Ptr) aglChoosePixelFormat) // check for existance of OpenGL
	{
#ifdef DEBUG
                post("MAC: BuildGLonWindow: OpenGL not installed");
#endif
		return NULL;
	}	
	// we successfully passed the renderer check

	if ((!pcontextInfo->fDraggable && (numDevices == 1))){  // not draggable on a single device
		pcontextInfo->fmt = aglChoosePixelFormat (&hGD, 1, pcontextInfo->aglAttributes); // get an appropriate pixel format
#ifdef DEBUG
                post("MAC: BuildGLonWindow (!pcontextInfo->fDraggable && (numDevices == 1))");
#endif
        } else {
		pcontextInfo->fmt = aglChoosePixelFormat (NULL, 0, pcontextInfo->aglAttributes); // get an appropriate pixel format
#ifdef DEBUG
                post("MAC: BuildGLonWindow else");
#endif
        }
	aglReportError ();
	if (NULL == pcontextInfo->fmt) 
	{
#ifdef DEBUG
                post("MAC: BuildGLonWindow: Could not find valid pixel format");
#endif
		return NULL;
	}

        *paglContext = aglCreateContext (pcontextInfo->fmt, aglShareContext); // Create an AGL context
         
	if (AGL_BAD_MATCH == aglGetError()){
#ifdef DEBUG
            post("MAC: BuildGLonWindow: AGL_BAD_MATCH");
#endif
            *paglContext = aglCreateContext (pcontextInfo->fmt, 0); // unable to sahre context, create without sharing
        }
	aglReportError ();
	if (NULL == *paglContext) 
	{
#ifdef DEBUG
            post("MAC: BuildGLonWindow: Unable to create AGL context");
#endif
            return NULL;
	}
	
	if (!aglSetDrawable (*paglContext, GetWindowPort (pWindow))){ // attach the CGrafPtr to the context
            post("MAC: BuildGLonWindow: Unable to attach the CGrafPtr to the context");
            return aglReportError ();
        }
	
	if(!aglSetCurrentContext (*paglContext)){ // make the context the current context
            post("MAC: BuildGLonWindow: Unable to make the context the current context");
            return aglReportError ();
        }
#ifdef DEBUG
        post("MAC: BuildGLonWindow exit");
#endif
	return err;
}

// --------------------------------------------------------------------------

// BuildGL

// Takes device and geometry request and tries to build best context and drawable
// if device does not work will walk down devices looking for first one that satisfies requirments

// Inputs: 	*pnumDevice: 0 any device, # attempt that device first, then any device
//			*pcontextInfo: request and requirements for cotext and drawable

// Outputs: *paglDraw, *paglContext and *pdspContext as allocated
//			*pnumDevice to device number in list that was used 
//			*pcontextInfo:  allocated parameters

// if fail to allocate: paglDraw, paglContext and pdspContext will be NULL
// if error: will return error and paglDraw, paglContext and pdspContext will be NULL

OSStatus BuildGL (AGLDrawable* paglDraw, AGLContext* paglContext,
                    short* pnumDevice, pstructGLInfo pcontextInfo, AGLContext aglShareContext)
{
	OSStatus err = noErr;
	GDHandle hGD = NULL;
	structGLInfo contextInfoSave;
	
	// clear
	*paglDraw = NULL;
	*paglContext = 0;
	contextInfoSave = *pcontextInfo; // save info to reset on failures
	
	//find main device
	if (*pnumDevice == -1)
	{
		GDHandle hDevice; // check number of screens
		hGD = GetMainDevice ();
		if (NULL != hGD)
		{
			err = BuildGLonDevice (paglDraw, paglContext, hGD, pcontextInfo, aglShareContext);
			// find device number
			*pnumDevice = 0;
			hDevice = DMGetFirstScreenDevice (true);
			do
			{
				if (hDevice == hGD)
					break;
				hDevice = DMGetNextScreenDevice (hDevice, true);
				(*pnumDevice)++;
			}
			while (hDevice);
			if (!hDevice)
				post("WinCreateMac: main device match not found");
		}
	}

	if ((err != noErr) || (*paglContext == 0))
	{
		err = noErr;
		DumpCurrent (paglDraw, paglContext, pcontextInfo); // dump what ever partial solution we might have
		*pcontextInfo = contextInfoSave; // restore info
		//find target device and check this first is one exists
		if (*pnumDevice)
		{
			short i;
			hGD = DMGetFirstScreenDevice (true);
			for (i = 0; i < *pnumDevice; i++)
			{
				GDHandle hGDNext = DMGetNextScreenDevice (hGD, true);
				if (NULL == hGDNext) // ensure we did not run out of devices
					break; // if no more devices drop out
				else
					hGD = hGDNext; // otherwise continue
			}
			*pnumDevice = i; // record device we actually got
			err = BuildGLonDevice (paglDraw, paglContext, /*pdspContext,*/ hGD, pcontextInfo, aglShareContext);
		}
	}
	
	// while we have not allocated a context or there were errors
	if ((err != noErr) || (*paglContext == 0))
	{
		err = noErr;
		DumpCurrent (paglDraw, paglContext, pcontextInfo); // dump what ever partial solution we might have
		*pcontextInfo = contextInfoSave; // restore info
		// now look through the devices in order
		hGD = DMGetFirstScreenDevice (true);	
		*pnumDevice = -1;
		do 
		{
			(*pnumDevice)++;
			err = BuildGLonDevice (paglDraw, paglContext, hGD, pcontextInfo, aglShareContext);
			if ((err != noErr) || (*paglDraw == NULL) || (*paglContext == 0))	// reset hGD only if we are not done
			{
				hGD = DMGetNextScreenDevice (hGD, true);
				DumpCurrent (paglDraw, paglContext, pcontextInfo); // dump what ever partial solution we might have
				*pcontextInfo = contextInfoSave; // restore info
			}
		}
		while (((err != noErr) || (*paglContext == 0)) && hGD);
	}
	return err;
}

// --------------------------------------------------------------------------

// DestroyGL

// Destroys drawable and context
// Ouputs: *paglDraw, *paglContext and *pdspContext should be 0 on exit

OSStatus DestroyGL (AGLDrawable* paglDraw, AGLContext* paglContext, pstructGLInfo pcontextInfo)
{
	if ((!paglContext) || (!*paglContext))
		return paramErr; // not a valid context
	glFinish ();
	DumpCurrent (paglDraw, paglContext, pcontextInfo);
	return noErr;
}

// --------------------------------------------------------------------------

// DestroyGLFromWindow

// Destroys context that waas allocated with BuildGLFromWindow
// Ouputs: *paglContext should be NULL on exit

OSStatus DestroyGLFromWindow (AGLContext* paglContext, pstructGLWindowInfo pcontextInfo)
{
	OSStatus err;
	
	if ((!paglContext) || (!*paglContext))
		return paramErr; // not a valid context
	glFinish ();
	aglSetCurrentContext (NULL);
	err = aglReportError ();
	aglSetDrawable (*paglContext, NULL);
	err = aglReportError ();
	aglDestroyContext (*paglContext);
	err = aglReportError ();
	*paglContext = NULL;

	if (pcontextInfo->fmt)
	{
		aglDestroyPixelFormat (pcontextInfo->fmt); // pixel format is no longer valid
		err = aglReportError ();
	}
	pcontextInfo->fmt = 0;
	
	return err;
}

//---------------------------------------------------------------------------
// BuildGLonDevice

// Takes device single device and tries to build on it

// Inputs: 	hGD: GDHandle to device to look at
//			*pcontextInfo: request and requirements for cotext and drawable

// Outputs: *paglDraw, *paglContext and *pdspContext as allocated
//			*pcontextInfo:  allocated parameters

// if fail to allocate: paglDraw, paglContext and pdspContext will be NULL
// if error: will return error and paglDraw, paglContext and pdspContext will be NULL
// Note: *paglDraw and *pdspContext can be null is aglFullScreen is used

static OSStatus BuildGLonDevice (AGLDrawable* paglDraw, AGLContext* paglContext, 
                                    GDHandle hGD, pstructGLInfo pcontextInfo, AGLContext aglShareContext)
{
	GLint depthSizeSupport;
	OSStatus err = noErr;
	Boolean fCheckRenderer = false;
    post("WinCreateMac:BuildGLonDevice: no fullscreen");
	{
		if (pcontextInfo->pixelDepth == 0)	// default
		{
			pcontextInfo->pixelDepth = (**(**hGD).gdPMap).pixelSize;
			if (16 > pcontextInfo->pixelDepth)
				pcontextInfo->pixelDepth = 16;
		}
		if (pcontextInfo->fDepthMust && (pcontextInfo->pixelDepth != (**(**hGD).gdPMap).pixelSize))	// device depth must match and does not
		{
			post("Pixel Depth does not match device in windowed mode.");
			return err;
		}
		// copy back the curretn depth
		pcontextInfo->pixelDepth = (**(**hGD).gdPMap).pixelSize;
		if (!CheckWindowExtents (hGD, pcontextInfo->width, pcontextInfo->height))
		{
			post("Window will not fit on device in windowed mode.");
			return err;
		}
	}
	
	// if we have not already checked the renderer, check for VRAM and accelerated
	if (!fCheckRenderer)
		if (!CheckRenderer (hGD, &(pcontextInfo->VRAM), &(pcontextInfo->textureRAM), &depthSizeSupport, pcontextInfo->fAcceleratedMust))
		{
			post("Renderer check failed");
			return err;
		}
	
	// do agl
	// need to send device #'s through this
	err = BuildGLContext (paglDraw, paglContext, hGD, pcontextInfo, aglShareContext);

	return err;
}
// --------------------------------------------------------------------------

// DumpCurrent

// Kills currently allocated context
// does not care about being pretty (assumes display is likely faded)

// Inputs: 	paglDraw, paglContext, pdspContext: things to be destroyed

void DumpCurrent (AGLDrawable* paglDraw, AGLContext* paglContext, pstructGLInfo pcontextInfo)
{
	if (*paglContext)
	{
		aglSetCurrentContext (NULL);
		aglReportError ();
		aglSetDrawable (*paglContext, NULL);
		aglReportError ();
		aglDestroyContext (*paglContext);
		aglReportError ();
		*paglContext = NULL;
	}
	
	if (pcontextInfo->fmt)
	{
		aglDestroyPixelFormat (pcontextInfo->fmt); // pixel format is no longer needed
		aglReportError ();
	}
	pcontextInfo->fmt = 0;

	if (*paglDraw && !(pcontextInfo->fFullscreen && CheckMacOSX ())) // do not destory a window on DSp if in Mac OS X
                                                                        // since there is no window built in X
		DisposeWindow (GetWindowFromPort (*paglDraw));

	*paglDraw = NULL;
}
// CheckRenderer

// looks at renderer attributes it has at least the VRAM is accelerated

// Inputs: 	hGD: GDHandle to device to look at
//			pVRAM: pointer to VRAM in bytes required; out is actual VRAM if a renderer was found, otherwise it is the input parameter
//			pTextureRAM:  pointer to texture RAM in bytes required; out is same (implementation assume VRAM returned by card is total so we add texture and VRAM)
//			fAccelMust: do we check for acceleration

// Returns: true if renderer for the requested device complies, false otherwise

static Boolean CheckRenderer (GDHandle hGD, long* pVRAM, long* pTextureRAM, GLint* pDepthSizeSupport, Boolean fAccelMust)
{
	AGLRendererInfo info, head_info;
	GLint inum;
	GLint dAccel = 0;
	GLint dVRAM = 0, dMaxVRAM = 0;
	Boolean canAccel = false, found = false;
	head_info = aglQueryRendererInfo(&hGD, 1);
	aglReportError ();
	if(!head_info)
	{
		post("aglQueryRendererInfo error");
		return false;
	}
	else
	{
		info = head_info;
		inum = 0;
		// see if we have an accelerated renderer, if so ignore non-accelerated ones
		// this prevents returning info on software renderer when actually we'll get the hardware one
		while (info)
		{	
			aglDescribeRenderer(info, AGL_ACCELERATED, &dAccel);
			aglReportError ();
			if (dAccel)
				canAccel = true;
			info = aglNextRendererInfo(info);
			aglReportError ();
			inum++;
		}
			
		info = head_info;
		inum = 0;
		while (info)
		{
			aglDescribeRenderer (info, AGL_ACCELERATED, &dAccel);
			aglReportError ();
			// if we can accel then we will choose the accelerated renderer 
			// how about compliant renderers???
			if ((canAccel && dAccel) || (!canAccel && (!fAccelMust || dAccel)))
			{
				aglDescribeRenderer (info, AGL_VIDEO_MEMORY, &dVRAM);	// we assume that VRAM returned is total thus add texture and VRAM required
				aglReportError ();
				if (dVRAM >= (*pVRAM + *pTextureRAM))
				{
					if (dVRAM >= dMaxVRAM) // find card with max VRAM
					{
						aglDescribeRenderer (info, AGL_DEPTH_MODES, pDepthSizeSupport);	// which depth buffer modes are supported
						aglReportError ();
						dMaxVRAM = dVRAM; // store max
						found = true;
					}
				}
			}
			info = aglNextRendererInfo(info);
			aglReportError ();
			inum++;
		}
	}
	aglDestroyRendererInfo(head_info);
	if (found) // if we found a card that has enough VRAM and meets the accel criteria
	{
		*pVRAM = dMaxVRAM; // return VRAM
		return true;
	}
	// VRAM will remain to same as it did when sent in
	return false;
}

//-----------------------------------------------------------------------------------------------------------------------

// CheckAllDeviceRenderers 

// looks at renderer attributes and each device must have at least one renderer that fits the profile

// Inputs: 	pVRAM: pointer to VRAM in bytes required; out is actual min VRAM of all renderers found, otherwise it is the input parameter
//			pTextureRAM:  pointer to texture RAM in bytes required; out is same (implementation assume VRAM returned by card is total so we add texture and VRAM)
//			fAccelMust: do we check fro acceleration

// Returns: true if any renderer for on each device complies (not necessarily the same renderer), false otherwise

static Boolean CheckAllDeviceRenderers (long* pVRAM, long* pTextureRAM, GLint* pDepthSizeSupport, Boolean fAccelMust)
{
    AGLRendererInfo info, head_info;
    GLint inum;
    GLint dAccel = 0;
    GLint dVRAM = 0, dMaxVRAM = 0;
    Boolean canAccel = false, found = false, goodCheck = true; // can the renderer accelerate, did we find a valid renderer for the device, are we still successfully on all the devices looked at
    long MinVRAM = 0x8FFFFFFF; // max long
    GDHandle hGD = GetDeviceList (); // get the first screen
    while (hGD && goodCheck)
    {
        head_info = aglQueryRendererInfo(&hGD, 1);
        aglReportError ();
        if(!head_info)
        {
            post("aglQueryRendererInfo error");
            return false;
        }
        else
        {
            info = head_info;
            inum = 0;
            // see if we have an accelerated renderer, if so ignore non-accelerated ones
            // this prevents returning info on software renderer when actually we'll get the hardware one
            while (info)
            {
                aglDescribeRenderer(info, AGL_ACCELERATED, &dAccel);
                aglReportError ();
                if (dAccel)
                    canAccel = true;
                info = aglNextRendererInfo(info);
                aglReportError ();
                inum++;
            }
                            
            info = head_info;
            inum = 0;
            while (info)
            {	
                aglDescribeRenderer(info, AGL_ACCELERATED, &dAccel);
                aglReportError ();
                // if we can accel then we will choose the accelerated renderer 
                // how about compliant renderers???
                if ((canAccel && dAccel) || (!canAccel && (!fAccelMust || dAccel)))
                {
                    aglDescribeRenderer(info, AGL_VIDEO_MEMORY, &dVRAM);	// we assume that VRAM returned is total thus add texture and VRAM required
                    aglReportError ();
                    if (dVRAM >= (*pVRAM + *pTextureRAM))
                    {
                        if (dVRAM >= dMaxVRAM) // find card with max VRAM
                        {
                            aglDescribeRenderer(info, AGL_DEPTH_MODES, pDepthSizeSupport);	// which depth buffer modes are supported
                            aglReportError ();
                            dMaxVRAM = dVRAM; // store max
                            found = true;
                        }
                    }
                }
                info = aglNextRendererInfo(info);
                aglReportError ();
                inum++;
            }
        }
        aglDestroyRendererInfo(head_info);
        if (found) // if we found a card that has enough VRAM and meets the accel criteria
        {
            if (MinVRAM > dMaxVRAM)
                MinVRAM = dMaxVRAM; // return VRAM
        }
        else
            goodCheck = false; // one device failed thus entire requirement fails
            hGD = GetNextDevice (hGD); // get next device
    } // while
    if (goodCheck) // we check all devices and each was good
    {
        *pVRAM = MinVRAM; // return VRAM
        return true;
    }
    return false; //at least one device failed to have mins
}
//-----------------------------------------------------------------------------------------------------------------------

// CheckWindowExtents

// checks to see window fits on screen completely

// Inputs: 	hGD: GDHandle to device to look at
//			width/height: requested width and height of window

// Returns: true if window and borders fit, false otherwise

static Boolean CheckWindowExtents (GDHandle hGD, short width, short height)
{
	Rect strucRect, rectWin = {0, 0, 1, 1};
	short deviceHeight = (short) ((**hGD).gdRect.bottom - (**hGD).gdRect.top - GetMBarHeight ());	
	short deviceWidth = (short) ((**hGD).gdRect.right - (**hGD).gdRect.left);
	short windowWidthExtra, windowHeightExtra;
	// build window (not visible)
	WindowPtr pWindow = NewCWindow (NULL, &rectWin, "\p", true, kWindowType, (WindowPtr)-1, 0, 0);
	
	GetWindowBounds (pWindow, kWindowStructureRgn, &strucRect);

	windowWidthExtra = (short) ((strucRect.right - strucRect.left) - 1);
	windowHeightExtra = (short) ((strucRect.bottom - strucRect.top) - 1);
	DisposeWindow (pWindow);
	if ((width + windowWidthExtra <= deviceWidth) &&
		(height + windowHeightExtra <= deviceHeight))
		return true;
	return false;
}
// --------------------------------------------------------------------------

// BuildGLContext

// Builds OpenGL context

// Inputs: 	hGD: GDHandle to device to look at
//			pcontextInfo: request and requirements for cotext and drawable

// Outputs: paglContext as allocated
//			pcontextInfo:  allocated parameters

// if fail to allocate: paglContext will be NULL
// if error: will return error paglContext will be NULL

static OSStatus BuildGLContext (AGLDrawable* paglDraw, AGLContext* paglContext, //DSpContextReference* pdspContext,
							  GDHandle hGD, pstructGLInfo pcontextInfo, AGLContext aglShareContext)
{
	OSStatus err = noErr;
	//NumVersion versionDSp = GetDSpVersion ();

	if ((Ptr) kUnresolvedCFragSymbolAddress == (Ptr) aglChoosePixelFormat) // check for existance of OpenGL
	{
		post("OpenGL not installed");
		return noErr;
	}	
	
	// DSp has problems on Mac OS X with DSp version less than 1.99 so use agl full screen
	if ((pcontextInfo->fFullscreen) && (CheckMacOSX ()) /*&& ((versionDSp.majorRev == 0x01) && (versionDSp.minorAndBugRev < 0x99))*/) // need to set pixel format for full screen
	{
		short i = 0;
		while (pcontextInfo->aglAttributes[i++] != AGL_NONE) {}
		i--; // point to AGL_NONE
		pcontextInfo->aglAttributes [i++] = AGL_FULLSCREEN;
		pcontextInfo->aglAttributes [i++] = AGL_PIXEL_SIZE;
		pcontextInfo->aglAttributes [i++] = (SInt32) pcontextInfo->pixelDepth;
		pcontextInfo->aglAttributes [i++] = AGL_NONE;
	}

	pcontextInfo->fmt = aglChoosePixelFormat (&hGD, 1, pcontextInfo->aglAttributes); // get an appropriate pixel format
	aglReportError ();
	if (NULL == pcontextInfo->fmt) 
	{
		post("Could not find valid pixel format");
		return noErr;
	}

	// using a default method of sharing all the contexts enables texture sharing across these contexts by default
	*paglContext = aglCreateContext (pcontextInfo->fmt, aglShareContext);		
        // Create an AGL context
 	if (AGL_BAD_MATCH == aglGetError())
		*paglContext = aglCreateContext (pcontextInfo->fmt, 0); // unable to sahre context, create without sharing
	aglReportError ();
	if (NULL == *paglContext) 
	{
		post("Could not create context");
		return paramErr;
	}
	if (aglShareContext == NULL)
		aglShareContext = *paglContext;
	
	// set our drawable
        // not Mac OS X fullscreen:  this is for three cases 1) Mac OS 9 windowed 2) Mac OS X windowed 3) Mac OS 9 fullscreen (as you need to build a window on top of DSp for GL to work correctly
	{
		// build window as late as possible
		err = BuildDrawable (paglDraw, hGD, pcontextInfo);
		if (err != noErr)
		{
			post("Could not build drawable");
			return err;
		}
		if (!aglSetDrawable (*paglContext, *paglDraw))		// attach the CGrafPtr to the context
			return aglReportError ();
	}
	if(!aglSetCurrentContext (*paglContext))			// make the context the current context
		return aglReportError ();
	
	return err;
}
/////////////////////////////////////////////////////////////////////////////
// CheckMacOSX

// Runtime check to see if we are running on Mac OS X

// Inputs:  None

// Returns: 0 if < Mac OS X or version number of Mac OS X (10.0 for GM)

UInt32 CheckMacOSX (void)
{
	UInt32 response;
    
	if ((Gestalt(gestaltSystemVersion, (SInt32 *) &response) == noErr) && (response >= 0x01000))
		return response;
	else
		return 0;
}
// --------------------------------------------------------------------------

// GetWindowDevice

// Inputs:	a valid WindowPtr

// Outputs:	the GDHandle that that window is mostly on

// returns the number of devices that the windows content touches

short FindGDHandleFromWindow (WindowPtr pWindow, GDHandle * phgdOnThisDevice)
{
	GrafPtr pgpSave;
	Rect rectWind, rectSect;
	long greatestArea, sectArea;
	short numDevices = 0;
	GDHandle hgdNthDevice;
	
	if (!pWindow || !phgdOnThisDevice)
		return NULL;
		
	*phgdOnThisDevice = NULL;
	
	GetPort (&pgpSave);
	SetPortWindowPort (pWindow);

	GetWindowPortBounds (pWindow, &rectWind);

	LocalToGlobal ((Point*)& rectWind.top);	// convert to global coordinates
	LocalToGlobal ((Point*)& rectWind.bottom);
	hgdNthDevice = GetDeviceList ();
	greatestArea = 0;
	// check window against all gdRects in gDevice list and remember 
	//  which gdRect contains largest area of window}
	while (hgdNthDevice)
	{
		if (TestDeviceAttribute (hgdNthDevice, screenDevice))
			if (TestDeviceAttribute (hgdNthDevice, screenActive))
			{
				// The SectRect routine calculates the intersection 
				//  of the window rectangle and this gDevice 
				//  rectangle and returns TRUE if the rectangles intersect, 
				//  FALSE if they don't.
				SectRect (&rectWind, &(**hgdNthDevice).gdRect, &rectSect);
				// determine which screen holds greatest window area
				//  first, calculate area of rectangle on current device
				sectArea = (long) (rectSect.right - rectSect.left) * (rectSect.bottom - rectSect.top);
				if (sectArea > 0)
					numDevices++;
				if (sectArea > greatestArea)
				{
					greatestArea = sectArea; // set greatest area so far
					*phgdOnThisDevice = hgdNthDevice; // set zoom device
				}
				hgdNthDevice = GetNextDevice(hgdNthDevice);
			}
	}
	
	SetPort (pgpSave);
	return numDevices;
}
// --------------------------------------------------------------------------

// BuildDrawable

// Builds window to be used as drawable

// Inputs: 	hGD: GDHandle to device to look at
//			pcontextInfo: request and requirements for cotext and drawable

// Outputs: paglDraw as allocated
//			pcontextInfo:  allocated parameters

// if fail to allocate: paglDraw will be NULL
// if error: will return error paglDraw will be NULL

static OSStatus BuildDrawable (AGLDrawable* paglDraw, GDHandle hGD, pstructGLInfo pcontextInfo)
{
	Rect rectWin;
	RGBColor rgbSave;
	GrafPtr pGrafSave;
	OSStatus err = noErr;
	
	// center window in our context's gdevice
	rectWin.top  = (short) ((**hGD).gdRect.top + ((**hGD).gdRect.bottom - (**hGD).gdRect.top) / 2); // v center
	rectWin.top  -= pcontextInfo->height / 2;
	rectWin.left  = (short) ((**hGD).gdRect.left + ((**hGD).gdRect.right - (**hGD).gdRect.left) / 2);	// h center
	rectWin.left  -= pcontextInfo->width / 2;
	rectWin.right = (short) (rectWin.left + pcontextInfo->width);
	rectWin.bottom = (short) (rectWin.top + pcontextInfo->height);
	
	if (pcontextInfo->fFullscreen)
		*paglDraw = GetWindowPort (NewCWindow (NULL, &rectWin, "\p", 0, plainDBox, (WindowPtr)-1, 0, 0));
	else
		*paglDraw = GetWindowPort (NewCWindow (NULL, &rectWin, "\p", 0, kWindowType, (WindowPtr)-1, 0, 0));
		ShowWindow (GetWindowFromPort (*paglDraw));

	GetPort (&pGrafSave);
	SetPort ((GrafPtr)*paglDraw);
	GetForeColor (&rgbSave);
	RGBForeColor (&rgbBlack);

	GetWindowBounds (GetWindowFromPort (*paglDraw), kWindowContentRgn, &rectWin);

	PaintRect (&rectWin);
	RGBForeColor (&rgbSave); // ensure color is reset for proper blitting
	SetPort (pGrafSave);
	return err;
}
//---------------------------------------------------------------------------

// if error dump agl errors to debugger string, return error

OSStatus aglReportError (void)
{
	GLenum err = aglGetError();
	if (AGL_NO_ERROR != err)
		post((char *)aglErrorString(err));
	// ensure we are returning an OSStatus noErr if no error condition
	if (err == AGL_NO_ERROR)
		return noErr;
	else
		return (OSStatus) err;
}

void SetContextValid (bool val)
{
	haveContext = val;
}

bool HaveValidContext (void)
{
	if (haveContext)
		return true;
	else
		return false;
}

static pascal OSStatus evtHandler (EventHandlerCallRef myHandler, EventRef event, void* userData)
{
//#pragma unused (userData)
    OSStatus err = 0;
    OSStatus result = eventNotHandledErr;
    UInt32 evtClass = GetEventClass (event);
    UInt32 kind = GetEventKind (event);
    WindowRef	winRef;
    UInt32	keyCode;
    char	macKeyCode;

    if (eventNotHandledErr == result)
    {
        switch (evtClass) {
            case kEventClassApplication:
                switch (kind)
                {
                    case kEventAppActivated:
                        GetEventParameter( event, kEventParamWindowRef, typeWindowRef, NULL, sizeof(WindowRef), NULL, &winRef);
                        SelectWindow(winRef);
                        result = noErr;
                        break;
                }
                break;
            case kEventClassWindow:
                switch (kind)
                {
                    case kEventWindowActivated:
                        GetEventParameter( event, kEventParamDirectObject, typeWindowRef, NULL, sizeof(WindowRef), NULL, &winRef);
                        SelectWindow(winRef);
                        result = noErr;
                        break;
                    case kEventWindowClosed:
                        //destroyGemWindow(
                        break;
                }
                break;
            case kEventClassKeyboard:
                switch (kind)
                {
                    case kEventRawKeyDown:
                        GetEventParameter( event, kEventParamKeyCode, typeUInt32, NULL, sizeof(UInt32), NULL, &keyCode);
                        GetEventParameter( event, kEventParamKeyMacCharCodes, typeUInt32, NULL, sizeof(char), NULL, &macKeyCode);
                        triggerKeyboardEvent( &macKeyCode, keyCode, 1);
                        result = noErr;
                        break;
                    case kEventRawKeyUp:
                        GetEventParameter( event, kEventParamKeyCode, typeUInt32, NULL, sizeof(UInt32), NULL, &keyCode);
                        GetEventParameter( event, kEventParamKeyMacCharCodes, typeChar, NULL, sizeof(char), NULL, &macKeyCode);
                        triggerKeyboardEvent( &macKeyCode, keyCode, 0);
                        result = noErr;
                        break;
                }
                break;
            case kEventClassMouse:
                HIPoint			location = {0.0f, 0.0f};
                EventMouseButton	button = 0;
                //MouseWheelAxis		axis = 0;
                UInt32		modifiers = 0;
                long		wheelDelta = 0;
                switch (kind)
                {
                    case kEventMouseMoved:
                        GetEventParameter(event, kEventParamMouseLocation, typeHIPoint, NULL, sizeof(HIPoint), NULL, &location);
                        triggerMotionEvent( (int)location.x, (int)location.y );
                        result = noErr;
                        break;
                        
                    case kEventMouseDragged:
                        GetEventParameter(event, kEventParamWindowMouseLocation, typeHIPoint, 
                                                NULL, sizeof(HIPoint), NULL, &location);
                        triggerMotionEvent( (int)location.x, (int)location.y );
                        result = noErr;
                        break;
                        
                    case kEventMouseDown:
                        GetEventParameter(event, kEventParamMouseButton, typeMouseButton, 
                                                NULL, sizeof(EventMouseButton), NULL, &button);                        
                        GetEventParameter(event, kEventParamWindowMouseLocation, typeHIPoint, 
                                                NULL, sizeof(HIPoint), NULL, &location);
                        triggerButtonEvent( button, 1, (int)location.x, (int)location.y );
                        GetEventParameter(event, kEventParamKeyModifiers, typeUInt32, 
                                                NULL, sizeof(UInt32), NULL, &modifiers);
                        result = noErr;
                        break;
                        
                    case kEventMouseUp:
                        GetEventParameter(event, kEventParamMouseButton, typeMouseButton, 
                                                NULL, sizeof(EventMouseButton), NULL, &button);
                        GetEventParameter(event, kEventParamWindowMouseLocation, typeHIPoint, 
                                                NULL, sizeof(HIPoint), NULL, &location);
                        triggerButtonEvent( button, 0, (int)location.x, (int)location.y );
                        result = noErr;
                        break;
                        
                    case kEventMouseWheelMoved:
                        GetEventParameter(event, kEventParamMouseWheelDelta, typeLongInteger, 
                                                NULL, sizeof(long), NULL, &wheelDelta);
                        //GetEventParameter(event, kEventParamMouseWheelAxis, typeMouseWheelAxis, 
                        //                        NULL, sizeof(long), NULL, &axis);
                        //triggerWheelEvent( axis, wheelDelta );
                        result = noErr;
                        break;
                }
                break;
        }
    }
    return result;
}
#endif
