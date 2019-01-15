///////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.at
//
// Implementation file
//
//    Copyright (c) 1997-2000 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2012 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
#include "Gem/GemConfig.h"

#include "gemsdl2window.h"
#include "Gem/GemGL.h"

#include "RTE/MessageCallbacks.h"
#include "Gem/Exception.h"

#define DEBUG ::startpost("%s:%d [%s]:: ", __FILE__, __LINE__, __FUNCTION__), ::post

#include <map>

CPPEXTERN_NEW(gemsdl2window);

namespace
{
  static unsigned int sdl_count = 0;
  static std::map<Uint32, gemsdl2window*>s_windowmap;
  static SDL_GLContext s_context = 0;
};

#ifdef __APPLE__
#include <dlfcn.h>
//This must be called before playing with SDL, else it won't work on osx.

namespace
{
static void pre_init()
{
  void* cocoa_lib;

  cocoa_lib = dlopen( "/System/Library/Frameworks/Cocoa.framework/Cocoa",
                      RTLD_LAZY );
  if(!cocoa_lib) {
    return;
  }
  void (*nsappload)(void);
  nsappload = (void(*)()) dlsym( cocoa_lib, "NSApplicationLoad");
  if(!nsappload) {
    return;
  }
  nsappload();
}
};
#else /* __APPLE__ */
namespace
{
void pre_init() {;}
};
#endif /* __APPLE__ */

/////////////////////////////////////////////////////////
//
// gemsdl2window
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
gemsdl2window :: gemsdl2window(void)
  : m_window(0)
  , m_context(0)
  , m_videoFlags(0)
  , m_mousegrab(false)
{
  if(!sdl_count) {
    pre_init();
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
      throw(GemException("could not initialize SDL window infrastructure"));
    }
  }
  sdl_count++;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
gemsdl2window :: ~gemsdl2window()
{
  destroyMess();

  sdl_count--;
  if(!sdl_count) {
    SDL_Quit(); // ????
  }
}


bool gemsdl2window :: makeCurrent(void)
{
  if(!m_window) {
    return false;
  }
  // ????
  return(true);
}

void gemsdl2window :: swapBuffers(void)
{
  if(makeCurrent()) { // FIXME: is this needed?
    SDL_GL_SwapWindow(m_window);
  }
}

void gemsdl2window :: doRender()
{
  // FIXME: ?????
  bang();
}


static std::map<SDL_Keycode, std::string>s_key2symbol;
static std::map<SDL_Keycode, std::string>s_worldkey2symbol;
static std::string key2symbol(SDL_Keycode k)
{
  if(0==s_key2symbol.size()) {
    s_key2symbol[SDLK_BACKSPACE]="BackSpace";
    s_key2symbol[SDLK_TAB]="Tab";
    s_key2symbol[SDLK_CLEAR]="Clear";
    s_key2symbol[SDLK_RETURN]="Return";
    s_key2symbol[SDLK_PAUSE]="Pause";
    s_key2symbol[SDLK_ESCAPE]="Escape";
    s_key2symbol[SDLK_SPACE]="Space";
    s_key2symbol[SDLK_EXCLAIM]="!";
    s_key2symbol[SDLK_QUOTEDBL]="\"";
    s_key2symbol[SDLK_HASH]="#";
    s_key2symbol[SDLK_DOLLAR]="$";
    s_key2symbol[SDLK_AMPERSAND]="&";
    s_key2symbol[SDLK_QUOTE]="'";
    s_key2symbol[SDLK_LEFTPAREN]="(";
    s_key2symbol[SDLK_RIGHTPAREN]=")";
    s_key2symbol[SDLK_ASTERISK]="*";
    s_key2symbol[SDLK_PLUS]="+";
    s_key2symbol[SDLK_COMMA]=",";
    s_key2symbol[SDLK_MINUS]="-";
    s_key2symbol[SDLK_PERIOD]=".";
    s_key2symbol[SDLK_SLASH]="/";
    s_key2symbol[SDLK_0]="0";
    s_key2symbol[SDLK_1]="1";
    s_key2symbol[SDLK_2]="2";
    s_key2symbol[SDLK_3]="3";
    s_key2symbol[SDLK_4]="4";
    s_key2symbol[SDLK_5]="5";
    s_key2symbol[SDLK_6]="6";
    s_key2symbol[SDLK_7]="7";
    s_key2symbol[SDLK_8]="8";
    s_key2symbol[SDLK_9]="9";
    s_key2symbol[SDLK_COLON]=":";
    s_key2symbol[SDLK_SEMICOLON]=";";
    s_key2symbol[SDLK_LESS]="<";
    s_key2symbol[SDLK_EQUALS]="=";
    s_key2symbol[SDLK_GREATER]=">";
    s_key2symbol[SDLK_QUESTION]="?";
    s_key2symbol[SDLK_AT]="@";
    s_key2symbol[SDLK_LEFTBRACKET]="]";
    s_key2symbol[SDLK_BACKSLASH]="\\";
    s_key2symbol[SDLK_RIGHTBRACKET]="]";
    s_key2symbol[SDLK_CARET]="^";
    s_key2symbol[SDLK_UNDERSCORE]="_";
    s_key2symbol[SDLK_BACKQUOTE]="`";
    s_key2symbol[SDLK_a]="a";
    s_key2symbol[SDLK_b]="b";
    s_key2symbol[SDLK_c]="c";
    s_key2symbol[SDLK_d]="d";
    s_key2symbol[SDLK_e]="e";
    s_key2symbol[SDLK_f]="f";
    s_key2symbol[SDLK_g]="g";
    s_key2symbol[SDLK_h]="h";
    s_key2symbol[SDLK_i]="i";
    s_key2symbol[SDLK_j]="j";
    s_key2symbol[SDLK_k]="k";
    s_key2symbol[SDLK_l]="l";
    s_key2symbol[SDLK_m]="m";
    s_key2symbol[SDLK_n]="n";
    s_key2symbol[SDLK_o]="o";
    s_key2symbol[SDLK_p]="p";
    s_key2symbol[SDLK_q]="q";
    s_key2symbol[SDLK_r]="r";
    s_key2symbol[SDLK_s]="s";
    s_key2symbol[SDLK_t]="t";
    s_key2symbol[SDLK_u]="u";
    s_key2symbol[SDLK_v]="v";
    s_key2symbol[SDLK_w]="w";
    s_key2symbol[SDLK_x]="x";
    s_key2symbol[SDLK_y]="y";
    s_key2symbol[SDLK_z]="z";
    s_key2symbol[SDLK_DELETE]="Delete";


    s_key2symbol[SDLK_KP_0]="KeyPad_0";
    s_key2symbol[SDLK_KP_1]="KeyPad_1";
    s_key2symbol[SDLK_KP_2]="KeyPad_2";
    s_key2symbol[SDLK_KP_3]="KeyPad_3";
    s_key2symbol[SDLK_KP_4]="KeyPad_4";
    s_key2symbol[SDLK_KP_5]="KeyPad_5";
    s_key2symbol[SDLK_KP_6]="KeyPad_6";
    s_key2symbol[SDLK_KP_7]="KeyPad_7";
    s_key2symbol[SDLK_KP_8]="KeyPad_8";
    s_key2symbol[SDLK_KP_9]="KeyPad_9";
    s_key2symbol[SDLK_KP_PERIOD]="KeyPad_.";
    s_key2symbol[SDLK_KP_DIVIDE]="KeyPad_/";
    s_key2symbol[SDLK_KP_MULTIPLY]="KeyPad_*";
    s_key2symbol[SDLK_KP_MINUS]="KeyPad_-";
    s_key2symbol[SDLK_KP_PLUS]="KeyPad_+";
    s_key2symbol[SDLK_KP_ENTER]="KeyPad_Enter";
    s_key2symbol[SDLK_KP_EQUALS]="KeyPad_=";
    s_key2symbol[SDLK_UP]="Up";
    s_key2symbol[SDLK_DOWN]="Down";
    s_key2symbol[SDLK_RIGHT]="Right";
    s_key2symbol[SDLK_LEFT]="Left";
    s_key2symbol[SDLK_INSERT]="Insert";
    s_key2symbol[SDLK_HOME]="Home";
    s_key2symbol[SDLK_END]="End";
    s_key2symbol[SDLK_PAGEUP]="Prior";
    s_key2symbol[SDLK_PAGEDOWN]="Next";
    s_key2symbol[SDLK_F1]="F1";
    s_key2symbol[SDLK_F2]="F2";
    s_key2symbol[SDLK_F3]="F3";
    s_key2symbol[SDLK_F4]="F4";
    s_key2symbol[SDLK_F5]="F5";
    s_key2symbol[SDLK_F6]="F6";
    s_key2symbol[SDLK_F7]="F7";
    s_key2symbol[SDLK_F8]="F8";
    s_key2symbol[SDLK_F9]="F9";
    s_key2symbol[SDLK_F10]="F10";
    s_key2symbol[SDLK_F11]="F11";
    s_key2symbol[SDLK_F12]="F12";
    s_key2symbol[SDLK_F13]="F13";
    s_key2symbol[SDLK_F14]="F14";
    s_key2symbol[SDLK_F15]="F15";
    s_key2symbol[SDLK_NUMLOCKCLEAR]="Num_Lock";
    s_key2symbol[SDLK_CAPSLOCK]="Caps_Lock";
    s_key2symbol[SDLK_SCROLLLOCK]="Scroll_Lock";
    s_key2symbol[SDLK_RSHIFT]="Shift_R";
    s_key2symbol[SDLK_LSHIFT]="Shift_L";
    s_key2symbol[SDLK_RCTRL]="Control_R";
    s_key2symbol[SDLK_LCTRL]="Control_L";
    s_key2symbol[SDLK_RALT]="AltGr";
    s_key2symbol[SDLK_LALT]="Alt_L";
    s_key2symbol[SDLK_RGUI]="Meta_R";
    s_key2symbol[SDLK_LGUI]="Meta_L";
    s_key2symbol[SDLK_MODE]="Mode";
    s_key2symbol[SDLK_APPLICATION]="Compose";
    s_key2symbol[SDLK_HELP]="Help";
    s_key2symbol[SDLK_PRINTSCREEN]="Print";
    s_key2symbol[SDLK_SYSREQ]="SysRq";
    s_key2symbol[SDLK_PAUSE]="Break";
    s_key2symbol[SDLK_MENU]="Menu";
    s_key2symbol[SDLK_POWER]="Power";
#if 0
    s_key2symbol[SDLK_LSUPER]="Super_L";
    s_key2symbol[SDLK_RSUPER]="Super_R";
    s_key2symbol[SDLK_EURO]="€";
#endif
    s_key2symbol[SDLK_UNDO]="Undo";
  }
  std::string s = s_key2symbol[k];
  if(s.empty()) {
    const char*keyname = SDL_GetKeyName(k);
    s = keyname?keyname:"";
  }
  if(s.empty()) {
    s="<unknown>";
  }

  return s;
}

void gemsdl2window :: dispatch()
{
  if(!m_window) {
    return;
  }

  std::vector<t_atom>al;
  t_atom a;
  int state;
  unsigned long devID=0;
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
#warning CHECK event->window.windowID;
  switch(event.type) {
  default:
    post("event: %d", event.type);
    break;
  case SDL_WINDOWEVENT:
    post("windowevent for %d @%p", event.window.windowID, m_window);
    switch (event.window.event) {
    case SDL_WINDOWEVENT_SHOWN:
      info("visible", 1);
      break;
    case SDL_WINDOWEVENT_HIDDEN:
      info("visible", 0);
      break;
    case SDL_WINDOWEVENT_EXPOSED:
      info("window", "exposed");
      break;
    case SDL_WINDOWEVENT_MOVED:
      position(event.window.data1, event.window.data2);
      break;
    case SDL_WINDOWEVENT_RESIZED:
      /* nop: this event always follows the SIZE_CHANGED event */
      break;
    case SDL_WINDOWEVENT_SIZE_CHANGED:
      dimension(event.window.data1, event.window.data2);
      break;
    case SDL_WINDOWEVENT_MINIMIZED:
#warning minimized,maximized,restored events
      SDL_Log("minimized %p", event.window.windowID);
      info("visible", 0);
      break;
    case SDL_WINDOWEVENT_MAXIMIZED:
      SDL_Log("maximized %p", event.window.windowID);
      info("visible", 1);
      break;
    case SDL_WINDOWEVENT_RESTORED:
      SDL_Log("restored %p", event.window.windowID);
      info("visible", 1);
      break;
    case SDL_WINDOWEVENT_ENTER:
      entry(devID, 1); // "mouse" <devID> "entry" 1
      break;
    case SDL_WINDOWEVENT_LEAVE:
      entry(devID, 0); // "mouse" <devID> "entry" 0
      break;
    case SDL_WINDOWEVENT_FOCUS_GAINED:
      info("inputentry", 1);
      break;
    case SDL_WINDOWEVENT_FOCUS_LOST:
      info("inputentry", 0);
      break;
    case SDL_WINDOWEVENT_CLOSE:
      info("window", "close");
      break;
#if SDL_VERSION_ATLEAST(2, 0, 5)
    case SDL_WINDOWEVENT_TAKE_FOCUS:
      //SDL_Log("Window %d is offered a focus", event.window.windowID);
      break;
    case SDL_WINDOWEVENT_HIT_TEST:
      //SDL_Log("Window %d has a special hit test", event.window.windowID);
      break;
#endif
      break;
    }
    break;
  case SDL_KEYUP:
  case SDL_KEYDOWN:
    /* FIXME: event.key.repeat */
    key
      (
        /* SDL2 dropped event.key.which */
        devID
        , key2symbol(event.key.keysym.sym)
        , event.key.keysym.scancode
        , event.key.state==SDL_PRESSED
        );
    break;
  case SDL_MOUSEMOTION:
    motion(event.motion.which, event.motion.x, event.motion.y);
    break;
  case SDL_MOUSEBUTTONUP:
  case SDL_MOUSEBUTTONDOWN:
    motion(event.button.which, event.button.x, event.button.y);
    button(event.button.which, event.button.button-SDL_BUTTON_LEFT,
           event.button.state==SDL_PRESSED);
    break;
  case SDL_QUIT:
    info("window", "destroy");
    break;
  }
}


/////////////////////////////////////////////////////////
// bufferMess
//
/////////////////////////////////////////////////////////
void gemsdl2window :: bufferMess(int buf)
{
  switch(buf) {
  case 1:
  case 2:
    m_buffer=buf;
    if(m_window) {
      post("changing buffer type will only effect newly created windows");
    }
    break;
  default:
    error("buffer can only be '1' (single) or '2' (double) buffered");
    break;
  }
}

/////////////////////////////////////////////////////////
// titleMess
//
/////////////////////////////////////////////////////////
void gemsdl2window :: titleMess(const std::string&s)
{
  m_title = s;
  if(m_window) {
    SDL_SetWindowTitle(m_window, m_title.c_str());
  }
}
/////////////////////////////////////////////////////////
// dimensionsMess
//
/////////////////////////////////////////////////////////
void gemsdl2window :: dimensionsMess(unsigned int width,
                                    unsigned int height)
{
  if (width < 1) {
    error("width must be greater than 0");
    return;
  }

  if (height < 1) {
    error ("height must be greater than 0");
    return;
  }
  m_width = width;
  m_height = height;
  if(m_window && makeCurrent()) {
    SDL_SetWindowSize(m_window, m_width, m_height);
  }
}
/////////////////////////////////////////////////////////
// fullscreenMess
//
/////////////////////////////////////////////////////////
void gemsdl2window :: fullscreenMess(int on)
{
  m_fullscreen = on;
  if(!m_window)
    return;
  if(!SDL_SetWindowFullscreen(m_window, m_fullscreen?SDL_WINDOW_FULLSCREEN:0)) {
    error("fullscreen failed: %s", SDL_GetError());
  }
}
void gemsdl2window :: borderMess(bool on)
{
  m_border = on;
  if(m_window)
    SDL_SetWindowBordered(m_window, m_border?SDL_TRUE:SDL_FALSE);
}
void gemsdl2window :: cursorMess(bool state)
{
  m_cursor=state;
  SDL_ShowCursor(state?SDL_ENABLE:SDL_DISABLE);
}
void gemsdl2window :: topmostMess(bool state)
{
  if(m_window)
    SDL_RaiseWindow(m_window);
}
void gemsdl2window :: mousegrabMess(bool state)
{
  m_mousegrab = state;
  if(m_window)
    SDL_SetWindowGrab(m_window, m_mousegrab?SDL_TRUE:SDL_FALSE);
}

/////////////////////////////////////////////////////////
// createMess
//
/////////////////////////////////////////////////////////
bool gemsdl2window :: create(void)
{
  if(m_window) {
    error("window already made!");
    return false;
  }

  if ( SDL_InitSubSystem( SDL_INIT_VIDEO ) < 0 ) {
    error("could not (re)initialize SDL window infrastructure");
    return false;
  }

  Uint32 flags = SDL_WINDOW_OPENGL;
  flags |= SDL_WINDOW_RESIZABLE; /* Enable window resizing */
  flags |= SDL_WINDOW_INPUT_GRABBED;
  if(m_fullscreen)
    flags |= SDL_WINDOW_FULLSCREEN;
  if(!m_border)
    flags |= SDL_WINDOW_BORDERLESS;

  /* get a SDL surface */
  m_window = SDL_CreateWindow(
    m_title.c_str(),
    m_xoffset, m_yoffset,
    m_width, m_height,
    flags );

  if(!m_window) {
    return false;
  }
  if(s_context) {
    if (SDL_GL_MakeCurrent(m_window, s_context)) {
      error("unable to make shared openGL context currect: %s", SDL_GetError());
      destroyMess();
      return false;
    }
    SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);
  }

  m_context = SDL_GL_CreateContext(m_window);
  SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 0);


  if(!m_context) {
    error("unable to create OpenGL context: %s", SDL_GetError());
    destroyMess();
    return false;
  }

  if (SDL_GL_MakeCurrent(m_window, m_context)) {
    error("unable to make OpenGL context current: %s", SDL_GetError());
    destroyMess();
    return false;
  }

  if(!createGemWindow()) {
    destroyMess();
    return false;
  }

  m_videoFlags = flags;

  SDL_DisableScreenSaver();
  SDL_RaiseWindow(m_window);
  SDL_CaptureMouse(SDL_TRUE);
  mousegrabMess(m_mousegrab);
  dispatch();
  return true;
}
void gemsdl2window :: createMess(const std::string&)
{
  create();
}


/////////////////////////////////////////////////////////
// destroy window
//
/////////////////////////////////////////////////////////
void gemsdl2window :: destroy(void)
{
  destroyGemWindow();
  m_window=NULL;
  info("window", "closed");
}
void gemsdl2window :: destroyMess(void)
{
  if(makeCurrent()) {
    SDL_QuitSubSystem( SDL_INIT_VIDEO );
  }
  destroy();
}


/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void gemsdl2window :: obj_setupCallback(t_class *classPtr)
{
  CPPEXTERN_MSG1(classPtr, "topmost", topmostMess, bool);
  CPPEXTERN_MSG1(classPtr, "mousegrab", mousegrabMess, bool);
}
