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
#include "Base/GemConfig.h"
#ifdef _WIN32
# include "gemw32window.h"

# include <stdlib.h>

# ifdef HAVE_QUICKTIME
#  include <QTML.h>
#  include <Movies.h>
# endif /* HAVE_QUICKTIME */

# include "Base/GemEvent.h"
# include "Base/GemGL.h"


static bool initGemWin(void) {
# ifdef HAVE_QUICKTIME
	OSErr		err = noErr;

	// Initialize QuickTime Media Layer
	err = InitializeQTML(0);
	if (err)
    {
      error("GEM Man: Could not initialize quicktime: error %d\n", err);
      return false;
    }	
	// Initialize QuickTime
	EnterMovies();
	if (err)
    {
      error("GEM Man: Could not initialize quicktime: error %d\n", err);
      return false;
    }	
	post("Gem Man: QT init OK");
# endif /* HAVE_QUICKTIME */
  return true;
}


/////////////////////////////////////////////////////////
// bSetupPixelFormat
//
/////////////////////////////////////////////////////////
static BOOL bSetupPixelFormat(HDC hdc, const WindowHints &hints)
{
  PIXELFORMATDESCRIPTOR pfd;
	
	// clean out the descriptor
  memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
    
  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL;
  if (m_buffer == 2)
    pfd.dwFlags = pfd.dwFlags | PFD_DOUBLEBUFFER;
  pfd.dwLayerMask = PFD_MAIN_PLANE;
  pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.cColorBits = 24;
  pfd.cRedBits = 8;
  pfd.cBlueBits = 8;
  pfd.cGreenBits = 8;
  pfd.cDepthBits = 16;
  pfd.cAccumBits = 0;
  pfd.cStencilBits = 8;

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

/////////////////////////////////////////////////////////
// MainWndProc
//
/////////////////////////////////////////////////////////
static LONG WINAPI MainWndProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
    case WM_CREATE:
      {
      }
      break;
      
      // pass all unhandled messages to DefWindowProc
    default:
      lRet = DefWindowProc (hWnd, uMsg, wParam, lParam);
      break;
    }
  return(lRet);
}

HGLRC gemw32window::sharedContext=NULL;

CPPEXTERN_NEW(gemw32window);
gemw32window::gemw32window(void) :
  m_win(NULL), m_dc(NULL), m_context(NULL)
{
  if(!initGemWin())
    throw(GemException("could not initialize window infrastructure"));
}
gemw32window::~gemw32window(void) {
    destroyMess();
}



/////////////////////////////////////////////////////////
// createGemWindow
//
/////////////////////////////////////////////////////////
bool gemw32window:: create(void)
{
  int w = 0;
  int h = 0;

  static bool firstTime = true;
    
  // Register the frame class
  HINSTANCE hInstance = GetModuleHandle(NULL);
  if (!hInstance)
    {
      error("GEM: Unable to get module instance");
      return false;
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
          return false;
        }
      firstTime = false;
    }

  DWORD dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
  DWORD style = WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

  w = m_width;
  h = m_height;

  int x = m_xoffset;
  int y = m_yoffset;

  bool fullscreen=m_fullscreen;
  if (fullscreen){
    DEVMODE dmScreenSettings;								// Device Mode
    
    if (!EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dmScreenSettings)){
      error("GEM: couldn't get screen capabilities!");
    }
    w = dmScreenSettings.dmPelsWidth;
    h = dmScreenSettings.dmPelsHeight;
    
    x=y=0;

    memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
    dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Size Of The Devmode Structure
    dmScreenSettings.dmPelsWidth	= m_width;			// Selected Screen Width
    dmScreenSettings.dmPelsHeight	= m_height;			// Selected Screen Height
    dmScreenSettings.dmBitsPerPel	= 32;					// Selected Bits Per Pixel
    dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;
    // Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
    if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL) {
      dmScreenSettings.dmPelsWidth	= w;
      dmScreenSettings.dmPelsHeight	= h;
      if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL) {
        error("GEM: couldn't switch to fullscreen");
        fullscreen=false;
      }
    }
  }
  if (fullscreen){
    dwExStyle  = WS_EX_APPWINDOW;
    style     |= WS_POPUP;
  } else {
    dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
    if (m_border)
      style |= WS_OVERLAPPEDWINDOW;
    else
      style |= WS_POPUP;
  }

  //  m_fullscreen = fullscreen;

  // Since Windows uses some of the window for the border, etc,
  //		we have to ask how big the window should really be
  RECT newSize;
  newSize.left = x;
  newSize.top  = y;
  newSize.right  = w+x;
  newSize.bottom = h+y;

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
  m_win = CreateWindowEx (
                          dwExStyle,
                          "GEM",
                          m_title->c_str(),
                          style,
                          newSize.left,
                          newSize.top,
                          newSize.right - newSize.left,
                          newSize.bottom - newSize.top,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

  if (!m_win)  {
    error("GEM: Unable to create window");
    return false;
  }

  // create the device context
  m_dc = GetDC(m_win);
  if (!m_dc)  {
    error("GEM: Unable to create device context");
    destroyGemWindow(info);
    return false;
  }

  // set the pixel format for the window
  if (!bSetupPixelFormat(m_dc, hints))  {
    error("GEM: Unable to set window pixel format");
    destroyGemWindow(info);
    return false;
  }

  // create the OpenGL context
  m_context = wglCreateContext(m_dc);
  if (!m_context)  {
    error("GEM: Unable to create OpenGL context");
    destroyGemWindow(info);
    return false;
  }

  // do we share display lists?
  if (hints.shared) wglShareLists(hints.shared, m_context);

  // make the context the current rendering context
  if (!wglMakeCurrent(m_dc, m_context))   {
    error("GEM: Unable to make OpenGL context current");
    destroyGemWindow(info);
    return false;
  }

  if (!m_actuallyDisplay) return true;

  // show and update main window
  if (fullscreen){
    ShowWindow(m_win,SW_SHOW);				// Show The Window
    SetForegroundWindow(m_win);				// Slightly Higher Priority
    SetFocus(m_win);
  } else  ShowWindow(m_win, SW_SHOWNORMAL);

  UpdateWindow(m_win);

  dimensions(h, w);

  return true;
}

/////////////////////////////////////////////////////////
// destroyGemWindow
//
/////////////////////////////////////////////////////////
void gemw32window:: destroy(void)
{
  if (m_fullscreen)
    ChangeDisplaySettings(NULL,0);	// Switch Back To The Desktop

  if (m_win) {
    if (m_dc) {
      if (m_context) {
        wglDeleteContext(m_context);
      }
      ReleaseDC(m_win, m_dc);
    }
    DestroyWindow(m_win);
  }
  m_dc  = NULL;
  m_win = NULL;
}

/////////////////////////////////////////////////////////
// switch cursor on/off
//
/////////////////////////////////////////////////////////
void gemw32window::cursorMess(bool state)
{
  cursor_state=ShowCursor(state);
}

/////////////////////////////////////////////////////////
// set topmost position on/off
//
/////////////////////////////////////////////////////////
void gemw32window::topmostMess(bool state)
{
  static int topmost_state = 0;
  if (state)
    SetWindowPos(m_win, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE); 
  else
    SetWindowPos(m_win, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE); 
}


void gemw32window::swapBuffers(void)
{
  SwapBuffers(m_dc);
}

bool gemw32window::makeCurrent(void)
{
  if (!m_dc && !m_context)return false;
  wglMakeCurrent(m_dc, m_context); 
  return true;
}

void gemw32window::dispatch(void)
{
  MSG msg;
  while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) == TRUE)
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
}






/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void gemw32window :: obj_setupCallback(t_class *classPtr)
{

  CPPEXTERN_MSG0(classPtr, "bang", render);
  CPPEXTERN_MSG1(classPtr, "create", createMess, t_symbol*);
  CPPEXTERN_MSG0(classPtr, "destroy", destroyMess);


  CPPEXTERN_MSG1(classPtr, "buffer", bufferMess, int);
  CPPEXTERN_MSG1(classPtr, "FSAA", fsaaMess, int);
  CPPEXTERN_MSG1(classPtr, "title", titleMess, t_symbol*);
  CPPEXTERN_MSG2(classPtr, "dimen", dimensionsMess, unsigned int, unsigned int);
  CPPEXTERN_MSG2(classPtr, "offset", offsetMess, int, int);
  CPPEXTERN_MSG1(classPtr, "fullscreen", fullscreenMess, int);
  CPPEXTERN_MSG1(classPtr, "border", borderMess, bool);
  CPPEXTERN_MSG1(classPtr, "cursor", cursorMess, bool);

  //  CPPEXTERN_MSG0(classPtr, "print", printMess);

}












#endif /* WIN32 */
