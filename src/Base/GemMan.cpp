////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute
//    Copyright (c) 2002 tigital
//
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
 
#include "GemMan.h"

#ifdef unix
#include <sys/time.h>
#include <GL/glx.h>
#include <X11/Xlib.h>
#elif MACOSX
#include <stdlib.h>
#include <string.h>
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#include <Carbon/Carbon.h>
#include <Quicktime/Quicktime.h>
#include <time.h>

#elif _WINDOWS
#include <stdlib.h>
// I hate Microsoft...I shouldn't have to do this!
#endif

#include "GemVersion.h"
#include "GemState.h"
#include "GemWinCreate.h"
#include "GemEvent.h"

#include "Controls/gemhead.h"

#ifdef MACOSX
extern bool HaveValidContext (void);
#endif // MACOSX

static WindowInfo gfxInfo;
static WindowInfo constInfo;

// static member data
char* GemMan::m_title = "GEM";
int GemMan::m_xoffset = 0;
int GemMan::m_yoffset = 0;
int GemMan::m_fullscreen = 0;
int GemMan::m_secondscreen = 0;
int GemMan::m_height = 500;
int GemMan::m_width = 500;
int GemMan::m_h = 500;
int GemMan::m_w = 500;
int GemMan::m_border = 1;
int GemMan::m_stereo = 0;
int GemMan::m_buffer = 2;
int GemMan::m_profile = 0;
int GemMan::m_rendering = 0;
GLfloat GemMan::m_clear_color[4];
GLbitfield GemMan::m_clear_mask = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
GLfloat GemMan::m_mat_ambient[4];
GLfloat GemMan::m_mat_specular[4];
GLfloat GemMan::m_mat_shininess;
GLfloat GemMan::m_stereoSep = -15.f;
GLfloat GemMan::m_stereoFocal = 0.f;
bool GemMan::m_stereoLine = true;
int GemMan::m_windowState = 0;
int GemMan::m_windowNumber = 0;
int GemMan::m_windowContext = 0;
int GemMan::m_cursor = 1;
int GemMan::m_topmost = 0;
float GemMan::m_perspect[6];
double GemMan::m_lastRenderTime = 0.;
float GemMan::m_lookat[9];
GemMan::FOG_TYPE GemMan::m_fogMode = GemMan::FOG_OFF;
float GemMan::m_fog;
GLfloat GemMan::m_fogColor[4];
float GemMan::m_fogStart;
float GemMan::m_fogEnd;
float GemMan::m_motionBlur=0.f;
int GemMan::texture_rectangle_supported = 0;	//tigital
int GemMan::client_storage_supported = 0;
float GemMan::fps;


// static data
static const int NUM_LIGHTS = 8;   	// the maximum number of lights
static int s_lightState = 0;        // is lighting on or off
static int s_lights[NUM_LIGHTS];    // the lighting array

static t_clock *s_clock = NULL;
static double s_deltime = 50.;
static int s_hit = 0;

static gemheadLink *s_linkHead = NULL;
static gemheadLink *s_linkHead_2 = NULL;


class gemheadLink
{
public:
  gemheadLink(const gemheadLink &s)
    : base(s.base), next(s.next), priority(s.priority) {}
  gemheadLink(gemhead *base_, int priority_)
    : base(base_), next(NULL), priority(priority_) {}
  gemheadLink(gemhead *base_, int priority_, gemheadLink *link)
    : base(base_), priority(priority_)
  { this->next = link->next; link->next = this; }
    	
  gemhead *base;
  gemheadLink *next;
  const int priority;
private:
  gemheadLink();
};

static int createConstWindow(char *disp);

// static int destroyConstWindow();

GEM_EXTERN void gemAbortRendering()
{
  GemMan::stopRendering();
}

static t_clock *s_windowClock = NULL;
static int s_windowDelTime = 10;

#ifdef _WINDOWS
static int s_windowRun = 0;
static int s_singleContext = 0;

/////////////////////////////////////////////////////////
// dispatchGemWindowMessages
//
/////////////////////////////////////////////////////////
static void dispatchGemWindowMessages()
{
  if (!s_windowRun)
    return;

  MSG msg;
  while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) == TRUE)
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  clock_delay(s_windowClock, s_windowDelTime);
}
#elif unix 
static void dispatchGemWindowMessages()
{
  WindowInfo win; 
  XEvent event; 
  XButtonEvent* eb = (XButtonEvent*)&event; 
  XKeyEvent* kb  = (XKeyEvent*)&event; 
  win = GemMan::getWindowInfo(); 

  while (XCheckWindowEvent(win.dpy,win.win,
			   KeyPressMask | KeyReleaseMask |
			   PointerMotionMask | 
			   ButtonMotionMask |
			   ButtonPressMask | 
			   ButtonReleaseMask,
			   &event))
    {
      switch (event.type)
	{
	case ButtonPress: 
	  triggerButtonEvent(eb->button-1, 1, eb->x, eb->y); 
	  break; 
	case ButtonRelease: 
	  triggerButtonEvent(eb->button-1, 0, eb->x, eb->y); 
	  break; 
	case MotionNotify: 
	  triggerMotionEvent(eb->x, eb->y); 
	  break; 
	case KeyPress:
	  triggerKeyboardEvent(XKeysymToString(XKeycodeToKeysym(win.dpy, kb->keycode, 0)), kb->keycode, 1);
	  break;
	case KeyRelease:
	  triggerKeyboardEvent(XKeysymToString(XKeycodeToKeysym(win.dpy, kb->keycode, 0)), kb->keycode, 0);
	  break;
	default:
	  break; 
	}
    }
  clock_delay(s_windowClock, s_windowDelTime);  
} 
#elif MACOSX
static void dispatchGemWindowMessages(void *)
{
//	::clock_xdelay(s_windowClock, s_windowDelTime);
    clock_delay(s_windowClock, s_windowDelTime);
}
#endif // for Unix

static void resizeCallback(int xSize, int ySize, void *)
{
  float xDivy = (float)xSize / (float)ySize;
  GemMan::m_h = ySize;
  GemMan::m_w = xSize;

  // setup the viewpoint
  glViewport(0, 0, xSize, ySize);
  // setup the matrices
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(GemMan::m_perspect[0] * xDivy, GemMan::m_perspect[1] * xDivy,	// left, right
	    GemMan::m_perspect[2], GemMan::m_perspect[3],			// bottom, top
	    GemMan::m_perspect[4], GemMan::m_perspect[5]);			// front, back
 
  glMatrixMode(GL_MODELVIEW);
}
/*
 This is SGI sample code taken directly from OpenGL.org:
 http://www.opengl.org/developers/code/features/OGLextensions/OGLextensions.html
 */
int OpenGLExtensionIsSupported(const char* extension) {
  const GLubyte *extensions = NULL;
  const GLubyte *start;
  GLubyte *where, *terminator;

  /* Extension names should not have spaces. */
  where = (GLubyte *) strchr(extension, ' ');
  if (where || *extension == '\0')
    return 0;
  extensions = glGetString(GL_EXTENSIONS);
  /* It takes a bit of care to be fool-proof about parsing the
     OpenGL extensions string. Don't be fooled by sub-strings,
     etc. */
  start = extensions;
  for (;;) {
    where = (GLubyte *) strstr((const char *) start, extension);
    if (!where)
      break;
    terminator = where + strlen(extension);
    if (where == start || *(where - 1) == ' ')
      if (*terminator == ' ' || *terminator == '\0')
        return 1;
    start = terminator;
  }
  return 0;
}

void GemMan :: createContext(char* disp)
{
#ifdef unix
  Display *dummyDpy;
  if ( (dummyDpy = XOpenDisplay(disp)) == NULL)
    { 
      error("GEM: could not open X display %s",disp);
      return;
    }
    
  int dummy, dummy2;
  if ( !glXQueryExtension(dummyDpy, &dummy, &dummy2) )
    {
      error("GEM: X server has no OpenGL GLX extension");
      XCloseDisplay(dummyDpy);
      return;
    }
#elif _WINDOWS
  // can we only have one context?
  if (getenv("GEM_SINGLE_CONTEXT") &&
      !strcmp("1", getenv("GEM_SINGLE_CONTEXT")))
    {
      post("GEM: using GEM_SINGLE_CONTEXT");
      s_singleContext = 1;
      m_width = 640;
      m_height = 480;
    }
#elif MACOSX
    // Check QuickTime installed
    long	QDfeature;
    if (OSErr err = ::Gestalt(gestaltQuickTime, &QDfeature)) {
            error ("GEM: QuickTime is not installed : %d", err);
            return;
    } else {
            if (OSErr err = ::EnterMovies()) {
                error("GEM: Couldn't initialize QuickTime : %d", err);
                return;
            }
    }
    // check existence of OpenGL libraries
    if ((Ptr)kUnresolvedCFragSymbolAddress == (Ptr)aglChoosePixelFormat) {
            error ("GEM : OpenGL is not installed");
            return;
    }
#endif
  s_windowClock = clock_new(NULL, (t_method)dispatchGemWindowMessages);

  if (!createConstWindow(disp))
    {
      error("GEM: A serious error occured creating const Context");
      error("GEM: Do not continue!");
    }
  m_windowContext = 1;
  setResizeCallback(resizeCallback, NULL);
}


/////////////////////////////////////////////////////////
//
// GemMan
//
/////////////////////////////////////////////////////////
// initGem
//
/////////////////////////////////////////////////////////
void GemMan :: initGem()
{
  static int alreadyInit = 0;
  if (alreadyInit)
    return;
  alreadyInit = 1;
    
  // clear the light array
  for (int i = 0; i < NUM_LIGHTS; i++)
    s_lights[i] = 0;
    
  m_clear_color[0] = 0.0;
  m_clear_color[1] = 0.0;
  m_clear_color[2] = 0.0;
  m_clear_color[3] = 0.0;
  m_mat_ambient[0] = 0.0f;
  m_mat_ambient[1] = 0.0f;
  m_mat_ambient[2] = 0.0f;
  m_mat_ambient[3] = 1.0f;
  m_mat_specular[0] = 1.0;
  m_mat_specular[1] = 1.0;
  m_mat_specular[2] = 1.0;
  m_mat_specular[3] = 1.0;
  m_mat_shininess = 100.0;

  s_clock = clock_new(NULL, (t_method)&GemMan::render);

  post("GEM: Graphics Environment for Multimedia");
  post("GEM: ver: %s", GEM_VERSION);
  post("GEM: compiled: " __DATE__);
  post("GEM: maintained by IOhannes m zmoelnig");
  post("GEM: Authors :\tMark Danks (original version on irix/windows)");
  post("GEM: \t\tChris Clepper (macOS-X)");
  post("GEM: \t\tDaniel Heckenberg (windows)");
  post("GEM: \t\tGuenter Geiger (linux)");
  post("GEM: \t\tJames Tittle (macOS-X)");
  post("GEM: \t\tIOhannes m zmoelnig (linux/windows)");

  //#ifdef MACOSX
  //	post("GEM: Mac OS X port by James Tittle & Chris Clepper");
  //#endif

  // setup the perspective values
  m_perspect[0] = -1.f;	// left
  m_perspect[1] =  1.f;	// right
  m_perspect[2] = -1.f;	// bottom
  m_perspect[3] =  1.f;	// top
  m_perspect[4] =  1.f;	// front
  m_perspect[5] = 20.f;	// back

  // setup the lookat values
  m_lookat[0] = 0.f;	// eye:		x
  m_lookat[1] = 0.f;	//		y
  m_lookat[2] = 4.f;	//		z
  m_lookat[3] = 0.f;	// center :	x
  m_lookat[4] = 0.f;	//		y
  m_lookat[5] = 0.f;	//		z
  m_lookat[6] = 0.f;	// up::		x
  m_lookat[7] = 1.f;	//		y
  m_lookat[8] = 0.f;	//		z

  // setup the fog
  m_fogMode	= FOG_OFF;
  m_fogStart	= 1.f;
  m_fogEnd	= 20.f;
  m_fog		= 0.5f;
  m_fogColor[0] = m_fogColor[1] = m_fogColor[2] = m_fogColor[3] = 1.f;

  m_motionBlur = 0.f;
}

/////////////////////////////////////////////////////////
// addObj
//
/////////////////////////////////////////////////////////
void GemMan :: addObj(gemhead *obj, int priority)
{
  gemheadLink *linkPtr = s_linkHead;
  if (priority<0){
    priority*=-1;
    linkPtr = s_linkHead_2;
    if (!linkPtr) {
      s_linkHead_2 = new gemheadLink(obj, priority);
      return;
    }
    
    // unique case if the s_linkHead has a worse priority number
    if (linkPtr->priority > priority) {
      s_linkHead_2 = new gemheadLink(obj, priority);
      s_linkHead_2->next = linkPtr;
      return;
    }
  } else {
    if (!linkPtr) {
      s_linkHead = new gemheadLink(obj, priority);
      return;
    }
    // unique case if the s_linkHead has a worse priority number
    if (linkPtr->priority > priority)  {
      s_linkHead = new gemheadLink(obj, priority);
      s_linkHead->next = linkPtr;
      return;
    }
  }

  while (linkPtr->next && linkPtr->next->priority <= priority)
    linkPtr = linkPtr->next;
       
  linkPtr = new gemheadLink(obj, priority, linkPtr);
}

/////////////////////////////////////////////////////////
// removeObj
//
/////////////////////////////////////////////////////////
void GemMan :: removeObj(gemhead *obj, int priority=50)
{
  gemheadLink *linkPtr = s_linkHead;
  if (priority<0)linkPtr = s_linkHead_2;
  if (!linkPtr) return;
    
  // unique case if the object is the s_linkHead
  if (linkPtr->base == obj)
    {
      gemheadLink *nextPtr = linkPtr->next;
      if (priority<0){
	delete s_linkHead_2;
	s_linkHead_2 = nextPtr;
      } else {
	delete s_linkHead;
	s_linkHead = nextPtr;
      }
      return;
    }
    
  while (linkPtr->next && linkPtr->next->base != obj)
    linkPtr = linkPtr->next;
    
  // didn't find anything
  if ( !linkPtr->next ) return;
    
  gemheadLink *removePtr = linkPtr->next;
  linkPtr->next = removePtr->next;
  delete [] removePtr;
}

/////////////////////////////////////////////////////////
// resetValues
//
/////////////////////////////////////////////////////////
void GemMan :: resetValues()
{
  if (s_lightState)
    {
      glEnable(GL_LIGHTING);
      glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
      glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
      glEnable(GL_COLOR_MATERIAL);
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, GemMan::m_mat_ambient);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, GemMan::m_mat_specular);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &GemMan::m_mat_shininess);
      glEnable(GL_AUTO_NORMAL);
      glEnable(GL_NORMALIZE);
      glShadeModel(GL_SMOOTH);
    }
  else
    {
      glDisable(GL_LIGHTING);
      glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
      glDisable(GL_COLOR_MATERIAL);
      glDisable(GL_AUTO_NORMAL);
      glDisable(GL_NORMALIZE);
      glShadeModel(GL_FLAT);
    }

  // setup the transformation matrices
  float xDivy = (float)m_w / (float)m_h;

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(m_perspect[0] * xDivy, m_perspect[1] * xDivy,	// left, right
	    m_perspect[2], m_perspect[3],				// bottom, top
	    m_perspect[4], m_perspect[5]);				// front, back
    
  glMatrixMode(GL_MODELVIEW);
  gluLookAt(m_lookat[0], m_lookat[1], m_lookat[2], m_lookat[3], m_lookat[4],
	    m_lookat[5], m_lookat[6], m_lookat[7], m_lookat[8]);

  if (m_fogMode == FOG_OFF) {
    glDisable(GL_FOG);
  } else {
    glEnable(GL_FOG);
    switch (m_fogMode)  {
    case (FOG_LINEAR):
      glFogf(GL_FOG_MODE, GL_LINEAR);
      break;
    case (FOG_EXP):
      glFogf(GL_FOG_MODE, GL_EXP);
      break;
    case (FOG_EXP2):
      glFogf(GL_FOG_MODE, GL_EXP2);
      break;
    case (FOG_OFF):
        glDisable(GL_FOG);
        break;
    }
    glFogf(GL_FOG_DENSITY, GemMan::m_fog);
    glFogf(GL_FOG_START, GemMan::m_fogStart);
    glFogf(GL_FOG_END, GemMan::m_fogEnd);
    glFogfv(GL_FOG_COLOR, GemMan::m_fogColor);
  }
}

/////////////////////////////////////////////////////////
// fillGemState
//
/////////////////////////////////////////////////////////
void GemMan :: fillGemState(GemState &state)
{
  if (s_lightState) {
    state.lighting = 1;
    state.smooth = 1;
  }
}

/////////////////////////////////////////////////////////
// resetState
//
/////////////////////////////////////////////////////////
void GemMan :: resetState()
{
#ifdef DEBUG
  post("MAN::resetState entered");
#endif
  m_clear_color[0] = 0.0;
  m_clear_color[1] = 0.0;
  m_clear_color[2] = 0.0;
  m_clear_color[3] = 0.0;
#ifdef MACOSX
  if (HaveValidContext ())
    glClearColor(m_clear_color[0], m_clear_color[1], m_clear_color[2], m_clear_color[3]);
#else
  glClearColor(m_clear_color[0], m_clear_color[1], m_clear_color[2], m_clear_color[3]);
#endif //MACOSX
  m_clear_mask = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
  m_mat_ambient[0] = 0.1f;
  m_mat_ambient[1] = 0.1f;
  m_mat_ambient[2] = 0.1f;
  m_mat_ambient[3] = 1.0f;
  m_mat_specular[0] = 1.0;
  m_mat_specular[1] = 1.0;
  m_mat_specular[2] = 1.0;
  m_mat_specular[3] = 1.0;
  m_mat_shininess = 100.0;

  s_lightState = 0;

  // window hints
  m_height = 500;
  m_width = 500;
  m_w=m_width;
  m_h=m_height;
  m_xoffset = 0;
  m_yoffset = 0;
  m_fullscreen = 0;
  m_title = "GEM";

  m_buffer = 2;

  m_stereo=0;
  m_stereoSep = -15.f;
  m_stereoFocal = 0.f;
  m_stereoLine = true; 

  // setup the perspective values
  m_perspect[0] = -1.f;	// left
  m_perspect[1] =  1.f;	// right
  m_perspect[2] = -1.f;	// bottom
  m_perspect[3] =  1.f;	// top
  m_perspect[4] =  1.f;	// front
  m_perspect[5] = 20.f;	// back

  // setup the lookat values
  m_lookat[0] = 0.f;	// eye:	x
  m_lookat[1] = 0.f;	//		y
  m_lookat[2] = 4.f;	//		z
  m_lookat[3] = 0.f;	// center :	x
  m_lookat[4] = 0.f;	//		y
  m_lookat[5] = 0.f;	//		z
  m_lookat[6] = 0.f;	// up::	        x
  m_lookat[7] = 1.f;	//		y
  m_lookat[8] = 0.f;	//		z

  // setup the fog
  m_fogMode	= FOG_OFF;
  m_fogStart	= 1.f;
  m_fogEnd	= 20.f;
  m_fog		= 0.5f;
  m_fogColor[0] = m_fogColor[1] = m_fogColor[2] = m_fogColor[3] = 1.f;

  // turn on the cursor
  m_cursor = 1;
  fps = 0;
  m_topmost = 0;

}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void GemMan :: renderChain(gemheadLink *head, GemState *state){
  while (head) {
    head->base->renderGL(state);
    head = head->next;
  }
}

void GemMan :: render(void *)
{
#ifdef _WINDOWS
  static int firstTime = 1;
  static float countFreq = 0;
#endif

  if (!m_windowState)
    return;

  // are we profiling?
#ifdef _WINDOWS
  if (firstTime)
    {
      LARGE_INTEGER freq;
      if (!QueryPerformanceFrequency(&freq))
	countFreq = 0;
      else
	countFreq = (float)(freq.QuadPart);
      firstTime = 0;
    }
  LARGE_INTEGER startTime;
  //	if (m_profile == 1 || m_profile == 2)
  QueryPerformanceCounter(&startTime);
#elif unix
  timeval startTime;
  //	if (m_profile == 1 || m_profile == 2)
  {
    gettimeofday(&startTime, 0);
  }
#elif MACOSX
    UnsignedWide startTime;
    ::Microseconds(&startTime);
#else
#error Define OS specific profiling
#endif
    
  s_hit = 0;
  resetValues();

  GemState currentState;

  // fill in the elapsed time
  if (m_buffer == 1)
    currentState.tickTime = 50.f;
  else
    currentState.tickTime = (float)(clock_gettimesince(m_lastRenderTime));
  m_lastRenderTime = clock_getsystime();

  // if stereoscopic rendering
  switch (m_stereo) {
  case 1: // 2-screen stereo
    {
      int xSize = m_w / 2;
      int ySize = m_h;
      float xDivy = (float)xSize / (float)ySize;

      // setup the left viewpoint
      glViewport(0, 0, xSize, ySize);
      // setup the matrices
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      glFrustum(GemMan::m_perspect[0] * xDivy, GemMan::m_perspect[1] * xDivy,	// left, right
		GemMan::m_perspect[2], GemMan::m_perspect[3],			// bottom, top
		GemMan::m_perspect[4], GemMan::m_perspect[5]);			// front, back
 
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      gluLookAt(m_lookat[0] - m_stereoSep / 100.f, m_lookat[1], m_lookat[2], m_lookat[3], m_lookat[4],
		m_lookat[5] + m_stereoFocal, m_lookat[6], m_lookat[7], m_lookat[8]);

      // render left view
      fillGemState(currentState);
      currentState.stereo = 1;

      renderChain(s_linkHead, &currentState);

      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      gluLookAt(0 - m_stereoSep / 100.f, 0, 4, 0, 0, 0 + m_stereoFocal, 0, 1, 0);
      renderChain(s_linkHead_2, &currentState);

      // setup the right viewpoint
      glViewport(xSize, 0, xSize, ySize);
      // setup the matrices
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      glFrustum(GemMan::m_perspect[0] * xDivy, GemMan::m_perspect[1] * xDivy,	// left, right
		GemMan::m_perspect[2], GemMan::m_perspect[3],			// bottom, top
		GemMan::m_perspect[4], GemMan::m_perspect[5]);			// front, back
 
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      gluLookAt(m_lookat[0] + m_stereoSep / 100.f, m_lookat[1], m_lookat[2], m_lookat[3], m_lookat[4],
		m_lookat[5] + m_stereoFocal, m_lookat[6], m_lookat[7], m_lookat[8]);

      // render right view
      fillGemState(currentState);
      currentState.stereo = 2;
      renderChain(s_linkHead, &currentState);

      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      gluLookAt(0 + m_stereoSep / 100.f, 0, 4, 0, 0, 0 + m_stereoFocal, 0, 1, 0);
      renderChain(s_linkHead_2, &currentState);


      if (GemMan::m_stereoLine){
	// draw a line between the views
	glDisable(GL_LIGHTING);

	glViewport(0, 0, m_w, m_h);
	xDivy = (float)m_w / (float)ySize;
	// setup the matrices
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1, 1, -1, 1, 1, 20);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 4, 0, 0, 0, 0, 1, 0);

	glLineWidth(2.f);
	glColor3f(1.f, 1.f, 1.f);
	glBegin(GL_LINES);
	glVertex2f(0.f, -6.f);
	glVertex2f(0.f, 6.f);
	glEnd();
	glLineWidth(1.0f);
      }
    }
    break;
  case 2: // color-stereo
    {
      int xSize = m_w;
      int ySize = m_h;
      float xDivy = (float)xSize / (float)ySize;

      int left_color=0;  // RED
      int right_color=1; // GREEN

      glClear(GL_COLOR_BUFFER_BIT & m_clear_mask);
      glClear(GL_DEPTH_BUFFER_BIT & m_clear_mask);

      // setup the left viewpoint
      switch (left_color){
      case 1:
	glColorMask(GL_FALSE,GL_TRUE,GL_FALSE,GL_TRUE);
	break;
      case 2:
	glColorMask(GL_FALSE,GL_FALSE,GL_TRUE,GL_TRUE);
	break;
      case 0:
      default:
	glColorMask(GL_TRUE,GL_FALSE,GL_FALSE,GL_TRUE);
      }

      // setup the matrices
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      glFrustum(GemMan::m_perspect[0] * xDivy, GemMan::m_perspect[1] * xDivy,	// left, right
		GemMan::m_perspect[2], GemMan::m_perspect[3],			// bottom, top
		GemMan::m_perspect[4], GemMan::m_perspect[5]);			// front, back
 
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      gluLookAt(m_lookat[0] - m_stereoSep / 100.f, m_lookat[1], m_lookat[2], m_lookat[3], m_lookat[4],
		m_lookat[5] + m_stereoFocal, m_lookat[6], m_lookat[7], m_lookat[8]);

      // render left view
      fillGemState(currentState);
      currentState.stereo = 1;
      renderChain(s_linkHead, &currentState);
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      gluLookAt(0 - m_stereoSep / 100.f, 0, 4, 0, 0, 0 + m_stereoFocal, 0, 1, 0);
      renderChain(s_linkHead_2, &currentState);

      // setup the right viewpoint
  glClear(GL_DEPTH_BUFFER_BIT & m_clear_mask);
      switch (right_color){
      case 0:
	glColorMask(GL_TRUE,GL_FALSE,GL_FALSE,GL_TRUE);
	break;
      case 1:
      default:
	glColorMask(GL_FALSE,GL_TRUE,GL_FALSE, GL_TRUE);
	break;
      case 2:
	glColorMask(GL_FALSE,GL_FALSE,GL_TRUE,GL_TRUE);
      }

      // setup the matrices
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      glFrustum(GemMan::m_perspect[0] * xDivy, GemMan::m_perspect[1] * xDivy,	// left, right
		GemMan::m_perspect[2], GemMan::m_perspect[3],			// bottom, top
		GemMan::m_perspect[4], GemMan::m_perspect[5]);			// front, back
 
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      gluLookAt(m_lookat[0] + m_stereoSep / 100.f, m_lookat[1], m_lookat[2], m_lookat[3], m_lookat[4],
		m_lookat[5] + m_stereoFocal, m_lookat[6], m_lookat[7], m_lookat[8]);

      // render right view
      fillGemState(currentState);
      currentState.stereo = 2;
      renderChain(s_linkHead, &currentState);

      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      gluLookAt(0 + m_stereoSep / 100.f, 0, 4, 0, 0, 0 + m_stereoFocal, 0, 1, 0);
      renderChain(s_linkHead_2, &currentState);
    
      glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
    }
    break;
  default: // normal rendering
    {
      fillGemState(currentState);
      renderChain(s_linkHead, &currentState);

      // setup the matrices
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      gluLookAt(0, 0, 4, 0, 0, 0, 0, 1, 0);
      renderChain(s_linkHead_2, &currentState);
    }
  }

  // only want to swap if we are in double buffer mode
  if (GemMan::m_buffer == 2)
    swapBuffers();

  // are we profiling?
  if (m_profile == 1 || m_profile == 2)
#ifdef _WINDOWS
    {
      LARGE_INTEGER endTime;
      QueryPerformanceCounter(&endTime);
      if (countFreq)
	post("GEM: time: %f",
	     (float)(endTime.QuadPart - startTime.QuadPart)/countFreq * 1000.f);
      else
	error("GEM: unable to profile");
    }
#elif unix
  {
    timeval endTime;
    gettimeofday(&endTime, 0);
    float seconds = (endTime.tv_sec - startTime.tv_sec) +
      (endTime.tv_usec - startTime.tv_usec) * 0.000001;
    post("GEM: time: %f", seconds);
  }
#elif MACOSX
    {
        UnsignedWide endTime;
        ::Microseconds(&endTime);
        float seconds = (float)(endTime.lo - startTime.lo) / 1000000.f;
        GemMan::fps = (1 / (seconds * 1000)) * 1000;
    //   m_fps = (1 / (seconds * 1000)) * 1000;
      //  post("GEM: time: %f", seconds);
    }
#else
#error Define OS specific profiling
#endif

  // only keep going if no one set the s_hit (could be hit if scheduler gets
  //	    ahold of a stopRendering command)
  if (!s_hit && (0.0 != s_deltime))
    clock_delay(s_clock, s_deltime);
}

/////////////////////////////////////////////////////////
// startRendering
//
/////////////////////////////////////////////////////////
void GemMan :: startRendering()
{
  if (!m_windowState)
    {
      error("GEM: Create window first!");
      return;
    }
    
  if (m_rendering)
    return;
    
  post("GEM: Start rendering");
    
  // set up all of the gemheads
  gemheadLink *head = s_linkHead_2;
  while(head) {
    head->base->startRendering();
    head = head->next;
  }
  head = s_linkHead;
  while(head)
    {
      head->base->startRendering();
      head = head->next;
    }

  m_rendering = 1;
    
  // if only single buffering then just return
  if (GemMan::m_buffer == 1)
    return;

  m_lastRenderTime = clock_getsystime();
  render(NULL);
}

/////////////////////////////////////////////////////////
// stopRendering
//
/////////////////////////////////////////////////////////
void GemMan :: stopRendering()
{
  if (!m_rendering) return;

  m_rendering = 0;
  clock_unset(s_clock);
  s_hit = 1;

  // clean out all of the gemheads
  gemheadLink *head = s_linkHead;
  while(head)
    {
      head->base->stopRendering();
      head = head->next;
    }

  post("GEM: Stop rendering");
}

/////////////////////////////////////////////////////////
// windowInit
//
/////////////////////////////////////////////////////////
void GemMan :: windowInit()
{
  glDisable(GL_ALPHA_TEST);
  glDisable(GL_BLEND);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glClearDepth(1.0);    
  glClearColor(m_clear_color[0], m_clear_color[1], m_clear_color[2], m_clear_color[3]);
 
  #ifdef MACOSX
  GLint swapInt = 1;
  aglSetInteger ( gfxInfo.context, AGL_SWAP_INTERVAL, &swapInt);
  #endif
 
  resetValues();
}

/////////////////////////////////////////////////////////
// windowCleanup
//
/////////////////////////////////////////////////////////
void GemMan :: windowCleanup()
{ }

/////////////////////////////////////////////////////////
// createWindow
//
/////////////////////////////////////////////////////////
int GemMan :: createWindow(char* disp)
{
  if ( m_windowState ) return(0);
#ifdef DEBUG
  post("GemMan: create window");
#endif

  WindowHints myHints;
  myHints.border = m_border;
  myHints.buffer = m_buffer;
  myHints.width = m_width;
  myHints.height = m_height;
  myHints.fullscreen = m_fullscreen;
  myHints.secondscreen = m_secondscreen;
  myHints.x_offset = m_xoffset;
  myHints.y_offset = m_yoffset;
  myHints.shared = constInfo.context;
  myHints.actuallyDisplay = 1;
  myHints.display = disp;
  myHints.title = m_title;
  
  if (disp) post("Creating on display %s",disp);

  if (!createGemWindow(gfxInfo, myHints) )
    {
      error("GEM: Unable to create window");
      return(0);
    }
    /*
        Check for the presence of a couple of useful OpenGL extensions
        we can use to speed up the movie rendering.
   
        GL_EXT_texture_rectangle allows for non-power-of-two sized
        textures.  Texture coordinates for these textures run from
        0..width, 0..height instead of 0..1, 0..1 as for normal
        power-of-two textures.  GL_EXT_texture_rectangle is available
        on the NVidia GeForce2MX and above, or the ATI Radeon and above.
    */
  
  /* of course, nvidia choose their own extension... (jmz) */
  /* i have ifdef'ed these, because some code in [pix_texture]
   * depends on texture_rectangle_supported, without checking
   * whether it was compiled with rectangle support or not (depending on the GL-headers)
   * so, if Gem was compiled with the original (non-nvidia) GL-headers,
   * there was no rectangle-support from within Gem.
   * however, when it was run with nvidia-drivers, the openGL-extension was present
   * resulting in doing a lot of stupid things (like texturing black!)
   */
#ifdef GL_EXT_texture_rectangle
  if (texture_rectangle_supported
      = OpenGLExtensionIsSupported("GL_EXT_texture_rectangle")){}
  else
#endif
#ifdef GL_TEXTURE_RECTANGLE_NV
    if (texture_rectangle_supported
	=  OpenGLExtensionIsSupported("GL_NV_texture_rectangle")){}
    else
#endif
      texture_rectangle_supported = 0;
  
    /*
        GL_APPLE_client_storage allows better performance when modifying
        a texture image extensively:  under normal circumstances, a
        texture image is passed from the application to the driver and
        then to the graphics card.  GL_APPLE_client_storage allows you
        to avoid the application -> driver copy as long as you agree to
        keep your copy of the texture image around for when the driver
        needs it.  GL_APPLE_client_storage is supported on all video
        cards under MacOSX 10.1 and above.
    */

  client_storage_supported
#ifdef GL_UNPACK_CLIENT_STORAGE_APPLE
    = OpenGLExtensionIsSupported("GL_APPLE_client_storage");
#else
    = 0;
#endif

  m_w=myHints.real_w;
  m_h=myHints.real_h;

  m_windowState = 1;
  cursorOnOff(m_cursor);
  topmostOnOff(m_topmost);
  m_windowNumber++;

  windowInit();
    
  clock_delay(s_windowClock, s_windowDelTime);

#ifdef _WINDOWS
  s_windowRun = 1;
#endif
  return(1);
}

/////////////////////////////////////////////////////////
// destroyWindow
//
/////////////////////////////////////////////////////////
void GemMan :: destroyWindow()
{
#ifdef _WINDOWS
  // don't want to get rid of this
  if (s_singleContext)
    return;
#endif

  if (!m_windowState) return;

  stopRendering();
  clock_unset(s_windowClock);
  s_windowClock = NULL;

  glFlush();
  glFinish();

  destroyGemWindow(gfxInfo);

  m_windowState = 0;
    
  windowCleanup();

  // reestablish the const glxContext
#ifdef unix                 // for Unix
  //post("dpy=%x\twin=%x\tcontext=%x", constInfo.dpy, constInfo.win, constInfo.context);

  if (!constInfo.dpy && !constInfo.win && !constInfo.context)return; // do not crash

  glXMakeCurrent(constInfo.dpy, constInfo.win, constInfo.context);   
#elif _WINDOWS              // for Windows

  if (!constInfo.dc && !constInfo.context)return; // do not crash ??

  wglMakeCurrent(constInfo.dc, constInfo.context);
  s_windowRun = 0;
#elif MACOSX		// for PPC Macintosh
    ::aglSetDrawable( constInfo.context, GetWindowPort(constInfo.pWind) );
    ::aglSetCurrentContext(constInfo.context);
#else
#error Define OS specific OpenGL context make current
#endif
}

/////////////////////////////////////////////////////////
// createConstWindow
//
/////////////////////////////////////////////////////////
int createConstWindow(char* disp)
{
#ifdef _WINDOWS
  // can we only have one context?
  if (s_singleContext)
    {
      return(GemMan::createWindow(disp));		
    }
#endif

  WindowHints myHints;
  myHints.title = GemMan::m_title;
  myHints.border = 1;
  myHints.buffer = 1;
  myHints.x_offset = 0;
  myHints.y_offset = 0;
  myHints.width = GemMan::m_width;
  myHints.height = GemMan::m_height;
#ifndef MACOSX
  myHints.shared = NULL;
#else
  myHints.shared = constInfo.context;
#endif
  myHints.actuallyDisplay = 0;
  myHints.fullscreen = 0;
  myHints.display = disp;

  if (!createGemWindow(constInfo, myHints) )
    {
      error("GEM: Error creating const context");
      return(0);
    }

  return(1);
}

/////////////////////////////////////////////////////////
// destroyConstWindow
//
/////////////////////////////////////////////////////////
void destroyConstWindow()
{
#ifdef _WINDOWS
  if (s_singleContext)
    {
    }
  else
#endif
    destroyGemWindow(constInfo);
}

/////////////////////////////////////////////////////////
// swapBuffers
//
/////////////////////////////////////////////////////////
void GemMan :: swapBuffers()
{
  if (!m_windowState) return;
  if (GemMan::m_buffer == 2)
#ifdef unix             // for Unix
    glXSwapBuffers(gfxInfo.dpy, gfxInfo.win);
#elif _WINDOWS          // for WinNT
  SwapBuffers(gfxInfo.dc);
#elif MACOSX		// for Macintosh
  ::aglSwapBuffers(gfxInfo.context);
#else                   // everyone else
#error Define OS specific swap buffer
#endif
  else glFlush();

  glClear(m_clear_mask);
  glColor3f(1.0, 1.0, 1.0);
  glLoadIdentity();

  if (GemMan::m_buffer == 1)
    {
      glFlush();
      // setup the transformation matrices
      float xDivy = (float)m_w / (float)m_h;

      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      glFrustum(m_perspect[0] * xDivy, m_perspect[1] * xDivy,	// left, right
		m_perspect[2], m_perspect[3],			// bottom, top
		m_perspect[4], m_perspect[5]);			// front, back
    
      glMatrixMode(GL_MODELVIEW);
      gluLookAt(m_lookat[0], m_lookat[1], m_lookat[2], m_lookat[3], m_lookat[4],
		m_lookat[5], m_lookat[6], m_lookat[7], m_lookat[8]);
    }
}

/////////////////////////////////////////////////////////
// lightingOnOff
//
/////////////////////////////////////////////////////////
void GemMan :: lightingOnOff(int state)
{
  if (state) s_lightState = 1;
  else s_lightState = 0;
}

/////////////////////////////////////////////////////////
// cursorOnOff
//
/////////////////////////////////////////////////////////
void GemMan :: cursorOnOff(int state)
{
  if (m_windowState)
    cursorGemWindow(gfxInfo,state);
  m_cursor = state;
}

/////////////////////////////////////////////////////////
// topmostOnOff
//
/////////////////////////////////////////////////////////
void GemMan :: topmostOnOff(int state)
{
#ifdef _WINDOWS
  if (m_windowState)
    topmostGemWindow(gfxInfo,state);
#else
  /* we don't care for this warning, since only windows has problems with
   * the Gem-window being not automatically on top (?) (jmz)
   */
  //  post("gemwin: \"topmost\" message not supported on this platform");
#endif
  m_topmost = state;
}

/////////////////////////////////////////////////////////
// frameRate
//
/////////////////////////////////////////////////////////
void GemMan :: frameRate(float framespersecond)
{
  if (framespersecond == 0.)
  {
	  s_deltime = 0.;
	  return;
  }
  if (framespersecond < 0.)
    {
      error("GEM: Invalid frame rate: %f", framespersecond);
      framespersecond = 20;
    }
  s_deltime = 1000. / framespersecond;
}

/////////////////////////////////////////////////////////
// requestLight
//
/////////////////////////////////////////////////////////
GLenum GemMan :: requestLight(int specific)
{
  int i = 0;
  if (specific > 0)
    i = specific - 1;
  else
    {
      while(s_lights[i])
	{
	  i++;
	  if (i >= NUM_LIGHTS)
	    {
	      error("GEM: Unable to allocate light");
	      return((GLenum)0);
	    }
	}
    }
  s_lights[i]++;
  GLenum retLight;
  switch(i)
    {
    case (0) :
      retLight = GL_LIGHT0;
      break;
    case (1) :
      retLight = GL_LIGHT1;
      break;
    case (2) :
      retLight = GL_LIGHT2;
      break;
    case (3) :
      retLight = GL_LIGHT3;
      break;
    case (4) :
      retLight = GL_LIGHT4;
      break;
    case (5) :
      retLight = GL_LIGHT5;
      break;
    case (6) :
      retLight = GL_LIGHT6;
      break;
    case (7) :
      retLight = GL_LIGHT7;
      break;
    default :
      error("GEM: Unable to allocate world_light");
      return((GLenum)0);
      // break;
    }
  return(retLight);
}

/////////////////////////////////////////////////////////
// freeLight
//
/////////////////////////////////////////////////////////
void GemMan :: freeLight(GLenum lightNum)
{
  int i = 0;
  switch(lightNum)
    {
    case(GL_LIGHT0):
      i = 0;
      break;
    case(GL_LIGHT1):
      i = 1;
      break;
    case(GL_LIGHT2):
      i = 2;
      break;
    case(GL_LIGHT3):
      i = 3;
      break;
    case(GL_LIGHT4):
      i = 4;
      break;
    case(GL_LIGHT5):
      i = 5;
      break;
    case(GL_LIGHT6):
      i = 6;
      break;	
    case(GL_LIGHT7):
      i = 7;
      break;
    default:
      error("GEM: Error freeing a light - bad number");
      return;
      // break;
    }
  s_lights[i]--;
  if (s_lights[i] < 0)
    {
      error("GEM: light ref count below zero: %d", i);
      s_lights[i] = 0;
    }
}

/////////////////////////////////////////////////////////
// printInfo
//
/////////////////////////////////////////////////////////
void GemMan :: printInfo()
{
  post("GEM information");
  post("---------------");
  post("OpenGL info");
  post("Vendor: %s", glGetString(GL_VENDOR));
  post("Renderer: %s", glGetString(GL_RENDERER));
  post("Version: %s", glGetString(GL_VERSION));

  if (glGetString(GL_EXTENSIONS)){
    char *text = new char [strlen((char *)glGetString(GL_EXTENSIONS)) + 1];
    strcpy(text,(char *)glGetString(GL_EXTENSIONS));
    char *token = strtok(text, " ");	// Parse 'text' For Words, Seperated By " " (spaces)
    while(token != NULL) {		// While The Token Isn't NULL
      post("Extensions: %s", token);	// Print extension string
      token = strtok(NULL, " ");
    }
    delete [] text;
  }

  post("---------------");
  post("window state: %d", m_windowState);
  post("topmost: %d", m_topmost);
  post("profile: %d", m_profile);
  post("buffer: %d", m_buffer);
  post("stereo: %d", m_stereo);
  post("full screen: %d", m_fullscreen);
  post("width: %d, height %d", m_width, m_height);
  post("offset: %d+%d", m_xoffset, m_yoffset);
  post("frame rate: %f", (0.0 != s_deltime) ? 1000. / s_deltime : 0.0);

  GLint bitnum = 0;
  glGetIntegerv(GL_RED_BITS, &bitnum);
  post("red: %d", bitnum);
  glGetIntegerv(GL_GREEN_BITS, &bitnum);
  post("green: %d", bitnum);
  glGetIntegerv(GL_BLUE_BITS, &bitnum);
  post("blue: %d", bitnum);
  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &bitnum);
  post("max texture: %d", bitnum);
  
  post("lighting %d", s_lightState);
  for (int i = 0; i < NUM_LIGHTS; i++) {
    if (s_lights[i])
      post("light%d: on", i);
  }

  post("rectangle texturing: %d", texture_rectangle_supported);
  post("");
}

/////////////////////////////////////////////////////////
// getWindowInfo
//
/////////////////////////////////////////////////////////
WindowInfo &GemMan :: getWindowInfo()
{
  return(gfxInfo);
}

/////////////////////////////////////////////////////////
// getConstWindowInfo
//
/////////////////////////////////////////////////////////
WindowInfo &GemMan :: getConstWindowInfo()
{
  return(constInfo);
}
