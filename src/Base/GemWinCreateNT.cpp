////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute
//
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#ifdef _WINDOWS

#include "GemWinCreate.h"

#include <stdlib.h>

#include "wintab.h"
#define PACKETDATA	(PK_X | PK_Y | PK_BUTTONS | PK_NORMAL_PRESSURE | PK_ORIENTATION)
#define PACKETMODE	PK_BUTTONS
#include "pktdef.h"

#include "GemEvent.h"
#include "m_pd.h"

GEM_EXTERN void gemAbortRendering();

/////////////////////////////////////////////////////////
// bSetupPixelFormat
//
/////////////////////////////////////////////////////////
BOOL bSetupPixelFormat(HDC hdc, const WindowHints &hints)
{
    PIXELFORMATDESCRIPTOR pfd;
	
	// clean out the descriptor
    memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
    
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL;
    if (hints.buffer == 2)
        pfd.dwFlags = pfd.dwFlags | PFD_DOUBLEBUFFER;
    pfd.dwLayerMask = PFD_MAIN_PLANE;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
	pfd.cRedBits = 8;
	pfd.cBlueBits = 8;
	pfd.cGreenBits = 8;
    pfd.cDepthBits = 16;
    pfd.cAccumBits = 0;
    pfd.cStencilBits = 0;

    int pixelformat;
    if ( (pixelformat = ChoosePixelFormat(hdc, &pfd)) == 0 )
    {
        post("GEM: ChoosePixelFormat failed");
        return(FALSE);
    }
    if (SetPixelFormat(hdc, pixelformat, &pfd) == FALSE)
    {
        post("GEM: SetPixelFormat failed");
        return(FALSE);
    }
    return(TRUE);
}

//////////
// wintab32 calls
typedef UINT (API *WTINFO_FUNC)(UINT, UINT, LPVOID);
typedef HCTX (API *WTOPEN_FUNC)(HWND, LPLOGCONTEXTA, BOOL);
typedef BOOL (API *WTCLOSE_FUNC)(HCTX);
typedef BOOL (API *WTPACKET_FUNC)(HCTX, UINT, LPVOID);

static WTINFO_FUNC WTInfoPtr = NULL;
static WTOPEN_FUNC WTOpenPtr = NULL;
static WTCLOSE_FUNC WTClosePtr = NULL;
static WTPACKET_FUNC WTPacketPtr = NULL;

static const int tabletXRes = 10000;
static const int tabletYRes = 10000;

static HCTX TabletContext = NULL;
static HINSTANCE s_winTabLib = NULL;

/////////////////////////////////////////////////////////
// TabletInit
//
// Initialize a tablet
//
/////////////////////////////////////////////////////////
static HCTX TabletInit(HWND hWnd)
{
	LOGCONTEXT lcMine;

	// get default region
	if (!(*WTInfoPtr)(WTI_DEFCONTEXT, 0, &lcMine))
	{
		error("GEM: Unable to get tablet info");
		return(NULL);
	}

	// modify the digitizing region
	strcpy(lcMine.lcName, "GEM Tablet");
	lcMine.lcOptions |= CXO_MESSAGES;
	lcMine.lcPktData = PACKETDATA;
	lcMine.lcPktMode = PACKETMODE;
	lcMine.lcMoveMask = PACKETDATA;
	lcMine.lcBtnUpMask = lcMine.lcBtnDnMask;

	// output in XRes x YRes grid
	lcMine.lcOutOrgX = lcMine.lcOutOrgY = 0;
	lcMine.lcOutExtX = tabletXRes;
	lcMine.lcOutExtY = tabletYRes;

	// open the region
	return((*WTOpenPtr)(hWnd, &lcMine, TRUE));
}

/////////////////////////////////////////////////////////
// Cleanup all of the tablet functions and free the library
//
/////////////////////////////////////////////////////////
static void TabletCleanup()
{
	if (s_winTabLib)
		FreeLibrary(s_winTabLib);
	WTInfoPtr = NULL;
	WTOpenPtr = NULL;
	WTClosePtr = NULL;
	WTPacketPtr = NULL;
	s_winTabLib = NULL;                            
	TabletContext = NULL;
}
static int TabletFunctionResolve(HWND hWnd)
{
	// see if the user has the wintab library installed on their machine
	s_winTabLib = LoadLibrary("wintab32.dll");
	if (!s_winTabLib)
		return(0);

	// dynamically load all of the functions that we care about
	WTInfoPtr = (WTINFO_FUNC)GetProcAddress(s_winTabLib, (const char *)ORD_WTInfoA);
	if (!WTInfoPtr)
	{
		error("GEM: Unable to find WinTab32 WTInfo()");
		TabletCleanup();
		return(0);
	}
	WTOpenPtr = (WTOPEN_FUNC)GetProcAddress(s_winTabLib, (const char *)ORD_WTOpen);
	if (!WTOpenPtr)
	{
		error("GEM: Unable to find WinTab32 WTOpen()");
		TabletCleanup();
		return(0);
	}
	WTClosePtr = (WTCLOSE_FUNC)GetProcAddress(s_winTabLib, (const char *)ORD_WTClose);
	if (!WTOpenPtr)
	{
		error("GEM: Unable to find WinTab32 WTClose()");
		TabletCleanup();
		return(0);
	}
	WTPacketPtr = (WTPACKET_FUNC)GetProcAddress(s_winTabLib, (const char *)ORD_WTPacket);
	if (!WTPacketPtr)
	{
		error("GEM: Unable to find WinTab32 WTPacket()");
		TabletCleanup();
		return(0);
	}

	// initialize the tablet
	TabletContext = TabletInit(hWnd);
	if (TabletContext)
		post("GEM: connected to tablet");
	else
	{
		error("GEM: unable to connect to tablet");
		TabletCleanup();
		return(0);
	}
	return(1);
}

/////////////////////////////////////////////////////////
// Class to clean up the tablet
//
/////////////////////////////////////////////////////////
class dummyTabletKill
{
    public:
        ~dummyTabletKill();
};
dummyTabletKill:: ~dummyTabletKill()
{
	if (s_winTabLib)
	{
		WTClosePtr(TabletContext);
		FreeLibrary(s_winTabLib);
	}
}
static dummyTabletKill myDumb;

/////////////////////////////////////////////////////////
// MainWndProc
//
/////////////////////////////////////////////////////////
LONG WINAPI MainWndProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static RECT rcClient;
	static int ctrlKeyDown = 0;

	// assume that we handle the message
    long lRet = 0;

    switch (uMsg)
    {
        // mouse motion
        case WM_MOUSEMOVE:
            triggerMotionEvent(LOWORD(lParam), HIWORD(lParam));
            break;

        // left button up
        case WM_LBUTTONUP:
            triggerButtonEvent(0, 0, LOWORD(lParam), HIWORD(lParam));
            break;

        // left button down
        case WM_LBUTTONDOWN:
            triggerButtonEvent(0, 1, LOWORD(lParam), HIWORD(lParam));
            break;

        // middle button up
        case WM_MBUTTONUP:
            triggerButtonEvent(1, 0, LOWORD(lParam), HIWORD(lParam));
            break;

        // middle button down
        case WM_MBUTTONDOWN:
            triggerButtonEvent(1, 1, LOWORD(lParam), HIWORD(lParam));
            break;

        // right button up
        case WM_RBUTTONUP:
            triggerButtonEvent(2, 0, LOWORD(lParam), HIWORD(lParam));
            break;

        // right button down
        case WM_RBUTTONDOWN:
            triggerButtonEvent(2, 1, LOWORD(lParam), HIWORD(lParam));
            break;

        // tablet
		case WT_PACKET:
            {
                static POINT ptNew, ptOld;
                static float prsNew, prsOld;
                static UINT oldAz, oldAlt, oldTw, newAz, newAlt, newTw;
                PACKET pkt;

                // make sure the dll was actually loaded in
                if (!WTPacketPtr)
                    return(lRet);

			    if ((*WTPacketPtr)((HCTX)lParam, wParam, &pkt))
                {
                    ptOld = ptNew;
				    prsOld = prsNew;
                    oldAz = newAz;
                    oldAlt = newAlt;
                    oldTw = newTw;
	    			ptNew.x = MulDiv((UINT)pkt.pkX, rcClient.right, tabletXRes);
    				ptNew.y = MulDiv((UINT)pkt.pkY, rcClient.bottom, tabletYRes);
                    // get to same orientation as mouse
                    ptNew.y = rcClient.bottom - ptNew.y;
				    prsNew = pkt.pkNormalPressure / 128.f;
			    	if (ptNew.x != ptOld.x ||
		    			ptNew.y != ptOld.y ||
	    				prsNew != prsOld)
                    {
                        triggerTabletMotionEvent(ptNew.x, ptNew.y, prsNew);
				    }
                    newAz = pkt.pkOrientation.orAzimuth;
                    newAlt = pkt.pkOrientation.orAltitude;
                    newTw = pkt.pkOrientation.orTwist;
                    if (newAz != oldAz ||
                        newAlt != oldAlt ||
                        newTw != oldTw)
                    {
                        triggerTabletRotationEvent(newAz, newAlt, newTw);
                    }
	    			if (HIWORD(pkt.pkButtons) == TBN_DOWN)
					{
						WORD which = LOWORD(pkt.pkButtons);
                        triggerTabletButtonEvent(which, 1, ptNew.x, ptNew.y);
                    }
					else if (HIWORD(pkt.pkButtons) == TBN_UP)
					{
						WORD which = LOWORD(pkt.pkButtons);
                        triggerTabletButtonEvent(which, 0, ptNew.x, ptNew.y);
					}
			    }
            }
            break;

        // keyboard action
        case WM_KEYUP:
			if ((int)wParam == VK_CONTROL)
				ctrlKeyDown = 0;

            triggerKeyboardEvent((char*)&wParam, (int)wParam, 1);
            break;

        // keyboard action
        case WM_KEYDOWN:
			if ((int)wParam == VK_CONTROL)
				ctrlKeyDown = 1;
			else if (ctrlKeyDown && (int)wParam == 'R')
					gemAbortRendering();
			else
				triggerKeyboardEvent((char*)&wParam, (int)wParam, 0);
            break;

        // resize event
        case WM_SIZE:
            triggerResizeEvent(LOWORD(lParam), HIWORD(lParam));
            GetClientRect(hWnd, &rcClient);
            break;

		// we want to override these messages
        // and not do anything
   	    case WM_DESTROY:
       	case WM_CLOSE:
            break;

        // try to initialize the tablet driver
	    case WM_CREATE:
            {
                static int s_wintabLoaded = 0;
                static int s_tabletCreated = 0;
                if (!s_wintabLoaded)
                {
                    s_wintabLoaded = 1;
					// is the no tablet environment variable set?
					if (getenv("GEM_NO_TABLET") &&
						!strcmp("1", getenv("GEM_NO_TABLET")))
					{
						post("GEM: NO_TABLET environment variable set");
					}
					else
						TabletFunctionResolve(hWnd);					
                }
		    }
            break;

        // pass all unhandled messages to DefWindowProc
        default:
            lRet = DefWindowProc (hWnd, uMsg, wParam, lParam);
            break;
    }
    return(lRet);
}

/////////////////////////////////////////////////////////
// createGemWindow
//
/////////////////////////////////////////////////////////
GEM_EXTERN int createGemWindow(WindowInfo &info, WindowHints &hints)
{
  static int firstTime = 1;
    
  // Register the frame class
  HINSTANCE hInstance = GetModuleHandle(NULL);
  if (!hInstance)
    {
      error("GEM: Unable to get module instance");
      return(0);
    }
  if (firstTime)
    {
      WNDCLASS wndclass;
      wndclass.style         = 0;
      wndclass.lpfnWndProc   = (WNDPROC)MainWndProc;
      wndclass.cbClsExtra    = 0;
      wndclass.cbWndExtra    = 0;
      wndclass.hInstance     = hInstance;
      wndclass.hCursor       = LoadCursor(NULL, IDC_CROSS);
      wndclass.hIcon         = LoadIcon(NULL, IDI_WINLOGO);
      wndclass.hbrBackground = NULL;
      wndclass.lpszMenuName  = NULL;
      wndclass.lpszClassName = "GEM";

      if (!RegisterClass(&wndclass) )
        {
	  error("GEM: Unable to register window class");
	  return(0);
        }
      firstTime = 0;
    }

  DWORD dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
  DWORD style = WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

  hints.real_w = hints.width;
  hints.real_h = hints.height;

  int x = hints.x_offset;
  int y = hints.y_offset;

  bool fullscreen=(hints.fullscreen!=0);
  if (fullscreen){
    DEVMODE dmScreenSettings;								// Device Mode
    
    if (!EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dmScreenSettings)){
      error("GEM: couldn't get screen capabilities!");
    }
    int w = dmScreenSettings.dmPelsWidth;
    int h = dmScreenSettings.dmPelsHeight;
    
    x=y=0;

    memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
    dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Size Of The Devmode Structure
    dmScreenSettings.dmPelsWidth	= hints.width;			// Selected Screen Width
    dmScreenSettings.dmPelsHeight	= hints.height;			// Selected Screen Height
    dmScreenSettings.dmBitsPerPel	= 32;					// Selected Bits Per Pixel
    dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;
    // Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
    if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL) {
      dmScreenSettings.dmPelsWidth	= w;
      dmScreenSettings.dmPelsHeight	= h;
      if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL) {
	error("GEM: couldn't switch to fullscreen");
	fullscreen=false;
      } else {
	hints.real_h=h;
	hints.real_w=w;
      }
    }
  }
  if (fullscreen){
    dwExStyle  = WS_EX_APPWINDOW;
    style     |= WS_POPUP;
  } else {
    dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
    if (hints.border)
      style |= WS_OVERLAPPEDWINDOW;
    else
      style |= WS_POPUP;
  }

  info.fs = fullscreen;//hints.fullscreen;

  // Since Windows uses some of the window for the border, etc,
  //		we have to ask how big the window should really be
  RECT newSize;
  newSize.left = x;
  newSize.top = y;
  newSize.right = hints.real_w+x;
  newSize.bottom = hints.real_h+y;

  AdjustWindowRectEx(&newSize, style, FALSE, dwExStyle); // no menu

  if (newSize.left<0 && x>=0){
	  newSize.right-=newSize.left;
	  newSize.left=0;
  }
  if (newSize.top<0 && y>=0){
	  newSize.bottom-=newSize.top;
	  newSize.top=0;
  }

  // Create the window
  info.win = CreateWindowEx (
			     dwExStyle,
			     "GEM",
			     hints.title,
			     style,
			     newSize.left,
			     newSize.top,
			     newSize.right - newSize.left,
			     newSize.bottom - newSize.top,
			     NULL,
			     NULL,
			     hInstance,
			     NULL);

  if (!info.win)  {
      error("GEM: Unable to create window");
      return(0);
    }

  // create the device context
  info.dc = GetDC(info.win);
  if (!info.dc)  {
      error("GEM: Unable to create device context");
      destroyGemWindow(info);
      return(0);
    }

  // set the pixel format for the window
  if (!bSetupPixelFormat(info.dc, hints))  {
      error("GEM: Unable to set window pixel format");
      destroyGemWindow(info);
      return(0);
    }

  // create the OpenGL context
  info.context = wglCreateContext(info.dc);
  if (!info.context)  {
      error("GEM: Unable to create OpenGL context");
      destroyGemWindow(info);
      return(0);
    }

  // do we share display lists?
  if (hints.shared) wglShareLists(hints.shared, info.context);

  // make the context the current rendering context
  if (!wglMakeCurrent(info.dc, info.context))   {
      error("GEM: Unable to make OpenGL context current");
      destroyGemWindow(info);
      return(0);
    }

  if (!hints.actuallyDisplay) return(1);

  // show and update main window
  if (fullscreen){
    ShowWindow(info.win,SW_SHOW);				// Show The Window
    SetForegroundWindow(info.win);				// Slightly Higher Priority
    SetFocus(info.win);
  } else  ShowWindow(info.win, SW_SHOWNORMAL);

  UpdateWindow(info.win);

  return(1);
}

/////////////////////////////////////////////////////////
// destroyGemWindow
//
/////////////////////////////////////////////////////////
GEM_EXTERN void destroyGemWindow(WindowInfo &info)
{
  if (info.fs) ChangeDisplaySettings(NULL,0);	// Switch Back To The Desktop

  if (info.win) {
    if (info.dc) {
      if (info.context) {
	wglDeleteContext(info.context);
      }
      ReleaseDC(info.win, info.dc);
    }
    DestroyWindow(info.win);
  }
  info.dc  = NULL;
  info.win = NULL;
  info.dc  = NULL;
}

/////////////////////////////////////////////////////////
// switch cursor on/off
//
/////////////////////////////////////////////////////////
int cursorGemWindow(WindowInfo &info, int state)
{
  static int cursor_state = 1;
  state=!(!state);
  if (cursor_state != state){
    cursor_state=ShowCursor(state)+1;
  }

  return cursor_state;
}

/////////////////////////////////////////////////////////
// set topmost position on/off
//
/////////////////////////////////////////////////////////
int topmostGemWindow(WindowInfo &info, int state)
{
  static int topmost_state = 0;
  state=!(!state);
  if (topmost_state != state){
	if (state)
		SetWindowPos(info.win, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE); 
	else
		SetWindowPos(info.win, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE); 
	topmost_state = state;
  }

  return topmost_state;
}

#endif
