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
#elif __APPLE__
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
#include "Controls/gemcontrol.h"

#include "Base/config.h"
#ifdef INCLUDE_GLEXT
# include <GL/glext.h>
#endif

#ifdef __APPLE__
extern bool HaveValidContext (void);
#endif // __APPLE__

// static member data
int GemMan::m_profile = 0;
int GemMan::m_rendering = 0;
int GemMan::m_windowNumber = 0;
int GemMan::m_windowContext = 0;
double GemMan::m_lastRenderTime = 0.;
int GemMan::texture_rectangle_supported = 0;	//tigital
int GemMan::client_storage_supported = 0;
int GemMan::texture_range_supported = 0;
int GemMan::texture_yuv_supported = 0;
float GemMan::fps;

// static data
static const int NUM_LIGHTS = 8;   	// the maximum number of lights
static int s_lights[NUM_LIGHTS];    // the lighting array

static t_clock *s_clock = NULL;
static double s_deltime = 50.;
static int s_hit = 0;

static gemheadLink *s_linkHead = NULL;
static gemheadLink *s_linkHead_2 = NULL;

static gemctrlLink *s_ctrlHead = NULL;


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

class gemctrlLink
{
public:
  gemctrlLink(const gemctrlLink &s)
    : base(s.base), next(s.next) {}
  gemctrlLink(gemcontrol *base_)
    : base(base_), next(NULL) {}
  gemctrlLink(gemcontrol *base_, gemctrlLink *link)
    : base(base_)
  { this->next = link->next; link->next = this; }
    	
  gemcontrol *base;
  gemctrlLink *next;
private:
  gemctrlLink();
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
#endif

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

#ifdef __APPLE__
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
    //  s_windowClock = clock_new(NULL, (t_method)dispatchGemWindowMessages);
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
    
  s_clock = clock_new(NULL, (t_method)&GemMan::render);

  post("GEM: Graphics Environment for Multimedia");
  post("GEM: ver: %s", GEM_VERSION);
  post("GEM: compiled: " __DATE__);
  post("GEM: maintained by IOhannes m zmoelnig");
  post("GEM: Authors :\tMark Danks (original version on irix/windows)");
  post("GEM: \t\tChris Clepper (macOS-X)");
#ifdef INCLUDE_GEIGER
  post("GEM: \t\tGuenter Geiger (linux)");
#endif
  post("GEM: \t\tDaniel Heckenberg (windows)");
  post("GEM: \t\tJames Tittle (macOS-X)");
  post("GEM: \t\tIOhannes m zmoelnig (linux/windows)");
}



/////////////////////////////////////////////////////////
// addCtrl
//
/////////////////////////////////////////////////////////
void GemMan :: addCtrl(gemcontrol *obj)
{
  gemctrlLink *linkPtr = s_ctrlHead;
  if (!linkPtr) {
    s_ctrlHead = new gemctrlLink(obj);
    return;
  }

  while (linkPtr->next)linkPtr = linkPtr->next;

  linkPtr = new gemctrlLink(obj, linkPtr);
}

/////////////////////////////////////////////////////////
// removeCtrl
//
/////////////////////////////////////////////////////////
void GemMan :: removeCtrl(gemcontrol *obj)
{
  gemctrlLink *linkPtr = s_ctrlHead;
  if (!linkPtr){
    stopRendering();
    return;
  }
    
  // unique case if the object is the s_ctrlHead
  if (linkPtr->base == obj) {
    gemctrlLink *nextPtr = linkPtr->next;
    delete s_ctrlHead;
    s_ctrlHead = nextPtr;
    if(s_ctrlHead==NULL)stopRendering();
    return;
  }
    
  while (linkPtr->next && linkPtr->next->base != obj)
    linkPtr = linkPtr->next;
    
  // didn't find anything
  if ( !linkPtr->next ) return;
    
  gemctrlLink *removePtr = linkPtr->next;
  linkPtr->next = removePtr->next;
  delete [] removePtr;
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
// fillGemState
//
/////////////////////////////////////////////////////////
void GemMan :: fillGemState(GemState &state)
{

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

void GemMan :: render1(GemState currentState){
  renderChain(s_linkHead, &currentState);
}
void GemMan :: render2(GemState currentState){
  renderChain(s_linkHead_2, &currentState);
}

void GemMan :: render(void *)
{
#ifdef _WINDOWS
  static int firstTime = 1;
  static float countFreq = 0;
#endif

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
#elif __APPLE__
    UnsignedWide startTime;
    ::Microseconds(&startTime);
#else
#error Define OS specific profiling
#endif
    
  s_hit = 0;

  GemState currentState;

  currentState.tickTime = (float)(clock_gettimesince(m_lastRenderTime));
  m_lastRenderTime = clock_getsystime();


  gemctrlLink* head = s_ctrlHead;
  while(head){
    head->base->render();
    head = head->next;
  }

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
    GemMan::fps = (1. / (seconds * 1000.)) * 1000.;
  }
#elif __APPLE__
    {
        UnsignedWide endTime;
        ::Microseconds(&endTime);
        float seconds = (float)(endTime.lo - startTime.lo) / 1000000.f;
        GemMan::fps = (1. / (seconds * 1000.)) * 1000.;
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
  if (m_rendering)
    return;
#ifdef DEBUG
  post("GEM: Start rendering");
#endif

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
  //  if (GemMan::m_buffer == 1)    return;

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
#ifdef DEBUG
  post("GEM: Stop rendering");
#endif
}

void GemMan :: checkExtensions() {
  /*
   *    Check for the presence of a couple of useful OpenGL extensions
   *    we can use to speed up the movie rendering.
   *
   *    GL_EXT_texture_rectangle allows for non-power-of-two sized
   *    textures.  Texture coordinates for these textures run from
   *    0..width, 0..height instead of 0..1, 0..1 as for normal
   *    power-of-two textures.  GL_EXT_texture_rectangle is available
   *    on the NVidia GeForce2MX and above, or the ATI Radeon and above.
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
     *  GL_APPLE_client_storage allows better performance when modifying
     *  a texture image extensively:  under normal circumstances, a
     *  texture image is passed from the application to the driver and
     *  then to the graphics card.  GL_APPLE_client_storage allows you
     *  to avoid the application -> driver copy as long as you agree to
     *  keep your copy of the texture image around for when the driver
     *  needs it.  GL_APPLE_client_storage is supported on all video
     *  cards under __APPLE__ 10.1 and above.
     */
  client_storage_supported
#ifdef GL_UNPACK_CLIENT_STORAGE_APPLE
    = OpenGLExtensionIsSupported("GL_APPLE_client_storage");
#else
    = 0;
#endif

  texture_range_supported
#ifdef GL_APPLE_texture_range
    = OpenGLExtensionIsSupported("GL_APPLE_texture_range");
#else
    = 0;
#endif


    /*
     * GL_APPLE_ycbcr_422 allows for direct texturing of YUV-textures
     * we want to check this at runtime, since modern implementations 
     * of Mesa support this feature while nvidia's drivers still don't.
     * checks at pre-processer-stage will eventually lead to no texturing
     * as the header files support YUV while the drivers don't
     */
  texture_yuv_supported
#ifdef GL_YCBCR_422_APPLE
    = OpenGLExtensionIsSupported("GL_APPLE_ycbcr_422");
#else
  = 0;
#endif
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
  GemMan::fps = framespersecond;
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
  /*
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
  post("profile: %d", m_profile);
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
  post("direct yuv texturing: %d", texture_yuv_supported);

  post("");
  */
}
