///////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-2000 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
#include "Gem/GemConfig.h"

#include "gemsdlwindow.h"
#include "Gem/GemGL.h"

#include "RTE/MessageCallbacks.h"
#include "Gem/Exception.h"

#define DEBUG ::startpost("%s:%d [%s]:: ", __FILE__, __LINE__, __FUNCTION__), ::post

#include <map>
static std::map<int, gemsdlwindow*>s_windowmap;

CPPEXTERN_NEW(gemsdlwindow);

namespace {
  static unsigned int sdl_count = 0;
}

/////////////////////////////////////////////////////////
//
// gemsdlwindow
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
gemsdlwindow :: gemsdlwindow(void) :
  m_surface(NULL),
  m_videoFlags(0),
  m_bpp(0)
{
  if(!sdl_count) {
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
      throw(GemException("could not initialize SDL window infrastructure"));
  }
  sdl_count++;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
gemsdlwindow :: ~gemsdlwindow()
{
  destroyMess();

  sdl_count--;
  if(!sdl_count) {
    SDL_Quit(); // ????
  }
}


bool gemsdlwindow :: makeCurrent(void){
  if(!m_surface)return false;
  // ????
  return(true);
}

void gemsdlwindow :: swapBuffers(void) {
  if(makeCurrent()) // FIXME: is this needed?
    SDL_GL_SwapBuffers();
}

void gemsdlwindow :: doRender()
{
  // FIXME: ?????
  bang();
}


static std::map<SDLKey, std::string>s_key2symbol;
static std::string key2symbol(SDLKey k) {
  if(0==s_key2symbol.size()) {
    s_key2symbol[SDLK_BACKSPACE]="backspace";
    s_key2symbol[SDLK_TAB]="tab";
    s_key2symbol[SDLK_CLEAR]="clear";
    s_key2symbol[SDLK_RETURN]="return";
    s_key2symbol[SDLK_PAUSE]="pause";
    s_key2symbol[SDLK_ESCAPE]="escape";
    s_key2symbol[SDLK_SPACE]="space";
    s_key2symbol[SDLK_EXCLAIM]="exclamation mark";
    s_key2symbol[SDLK_QUOTEDBL]="double quote";
    s_key2symbol[SDLK_HASH]="hash";
    s_key2symbol[SDLK_DOLLAR]="dollar";
    s_key2symbol[SDLK_AMPERSAND]="ampersand";
    s_key2symbol[SDLK_QUOTE]="single quote";
    s_key2symbol[SDLK_LEFTPAREN]="left parenthesis";
    s_key2symbol[SDLK_RIGHTPAREN]="right parenthesis";
    s_key2symbol[SDLK_ASTERISK]="asterisk";
    s_key2symbol[SDLK_PLUS]="plus sign";
    s_key2symbol[SDLK_COMMA]="comma";
    s_key2symbol[SDLK_MINUS]="minus sign";
    s_key2symbol[SDLK_PERIOD]="period / full stop";
    s_key2symbol[SDLK_SLASH]="forward slash";
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
    s_key2symbol[SDLK_COLON]="colon";
    s_key2symbol[SDLK_SEMICOLON]="semicolon";
    s_key2symbol[SDLK_LESS]="less-than sign";
    s_key2symbol[SDLK_EQUALS]="equals sign";
    s_key2symbol[SDLK_GREATER]="greater-than sign";
    s_key2symbol[SDLK_QUESTION]="question mark";
    s_key2symbol[SDLK_AT]="at";
    s_key2symbol[SDLK_LEFTBRACKET]="left bracket";
    s_key2symbol[SDLK_BACKSLASH]="backslash";
    s_key2symbol[SDLK_RIGHTBRACKET]="right bracket";
    s_key2symbol[SDLK_CARET]="caret";
    s_key2symbol[SDLK_UNDERSCORE]="underscore";
    s_key2symbol[SDLK_BACKQUOTE]="grave";
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
    s_key2symbol[SDLK_DELETE]="delete";
    s_key2symbol[SDLK_WORLD_0]="world 0";
    s_key2symbol[SDLK_WORLD_1]="world 1";
    s_key2symbol[SDLK_WORLD_2]="world 2";
    s_key2symbol[SDLK_WORLD_3]="world 3";
    s_key2symbol[SDLK_WORLD_4]="world 4";
    s_key2symbol[SDLK_WORLD_5]="world 5";
    s_key2symbol[SDLK_WORLD_6]="world 6";
    s_key2symbol[SDLK_WORLD_7]="world 7";
    s_key2symbol[SDLK_WORLD_8]="world 8";
    s_key2symbol[SDLK_WORLD_9]="world 9";
    s_key2symbol[SDLK_WORLD_10]="world 10";
    s_key2symbol[SDLK_WORLD_11]="world 11";
    s_key2symbol[SDLK_WORLD_12]="world 12";
    s_key2symbol[SDLK_WORLD_13]="world 13";
    s_key2symbol[SDLK_WORLD_14]="world 14";
    s_key2symbol[SDLK_WORLD_15]="world 15";
    s_key2symbol[SDLK_WORLD_16]="world 16";
    s_key2symbol[SDLK_WORLD_17]="world 17";
    s_key2symbol[SDLK_WORLD_18]="world 18";
    s_key2symbol[SDLK_WORLD_19]="world 19";
    s_key2symbol[SDLK_WORLD_20]="world 20";
    s_key2symbol[SDLK_WORLD_21]="world 21";
    s_key2symbol[SDLK_WORLD_22]="world 22";
    s_key2symbol[SDLK_WORLD_23]="world 23";
    s_key2symbol[SDLK_WORLD_24]="world 24";
    s_key2symbol[SDLK_WORLD_25]="world 25";
    s_key2symbol[SDLK_WORLD_26]="world 26";
    s_key2symbol[SDLK_WORLD_27]="world 27";
    s_key2symbol[SDLK_WORLD_28]="world 28";
    s_key2symbol[SDLK_WORLD_29]="world 29";
    s_key2symbol[SDLK_WORLD_30]="world 30";
    s_key2symbol[SDLK_WORLD_31]="world 31";
    s_key2symbol[SDLK_WORLD_32]="world 32";
    s_key2symbol[SDLK_WORLD_33]="world 33";
    s_key2symbol[SDLK_WORLD_34]="world 34";
    s_key2symbol[SDLK_WORLD_35]="world 35";
    s_key2symbol[SDLK_WORLD_36]="world 36";
    s_key2symbol[SDLK_WORLD_37]="world 37";
    s_key2symbol[SDLK_WORLD_38]="world 38";
    s_key2symbol[SDLK_WORLD_39]="world 39";
    s_key2symbol[SDLK_WORLD_40]="world 40";
    s_key2symbol[SDLK_WORLD_41]="world 41";
    s_key2symbol[SDLK_WORLD_42]="world 42";
    s_key2symbol[SDLK_WORLD_43]="world 43";
    s_key2symbol[SDLK_WORLD_44]="world 44";
    s_key2symbol[SDLK_WORLD_45]="world 45";
    s_key2symbol[SDLK_WORLD_46]="world 46";
    s_key2symbol[SDLK_WORLD_47]="world 47";
    s_key2symbol[SDLK_WORLD_48]="world 48";
    s_key2symbol[SDLK_WORLD_49]="world 49";
    s_key2symbol[SDLK_WORLD_50]="world 50";
    s_key2symbol[SDLK_WORLD_51]="world 51";
    s_key2symbol[SDLK_WORLD_52]="world 52";
    s_key2symbol[SDLK_WORLD_53]="world 53";
    s_key2symbol[SDLK_WORLD_54]="world 54";
    s_key2symbol[SDLK_WORLD_55]="world 55";
    s_key2symbol[SDLK_WORLD_56]="world 56";
    s_key2symbol[SDLK_WORLD_57]="world 57";
    s_key2symbol[SDLK_WORLD_58]="world 58";
    s_key2symbol[SDLK_WORLD_59]="world 59";
    s_key2symbol[SDLK_WORLD_60]="world 60";
    s_key2symbol[SDLK_WORLD_61]="world 61";
    s_key2symbol[SDLK_WORLD_62]="world 62";
    s_key2symbol[SDLK_WORLD_63]="world 63";
    s_key2symbol[SDLK_WORLD_64]="world 64";
    s_key2symbol[SDLK_WORLD_65]="world 65";
    s_key2symbol[SDLK_WORLD_66]="world 66";
    s_key2symbol[SDLK_WORLD_67]="world 67";
    s_key2symbol[SDLK_WORLD_68]="world 68";
    s_key2symbol[SDLK_WORLD_69]="world 69";
    s_key2symbol[SDLK_WORLD_70]="world 70";
    s_key2symbol[SDLK_WORLD_71]="world 71";
    s_key2symbol[SDLK_WORLD_72]="world 72";
    s_key2symbol[SDLK_WORLD_73]="world 73";
    s_key2symbol[SDLK_WORLD_74]="world 74";
    s_key2symbol[SDLK_WORLD_75]="world 75";
    s_key2symbol[SDLK_WORLD_76]="world 76";
    s_key2symbol[SDLK_WORLD_77]="world 77";
    s_key2symbol[SDLK_WORLD_78]="world 78";
    s_key2symbol[SDLK_WORLD_79]="world 79";
    s_key2symbol[SDLK_WORLD_80]="world 80";
    s_key2symbol[SDLK_WORLD_81]="world 81";
    s_key2symbol[SDLK_WORLD_82]="world 82";
    s_key2symbol[SDLK_WORLD_83]="world 83";
    s_key2symbol[SDLK_WORLD_84]="world 84";
    s_key2symbol[SDLK_WORLD_85]="world 85";
    s_key2symbol[SDLK_WORLD_86]="world 86";
    s_key2symbol[SDLK_WORLD_87]="world 87";
    s_key2symbol[SDLK_WORLD_88]="world 88";
    s_key2symbol[SDLK_WORLD_89]="world 89";
    s_key2symbol[SDLK_WORLD_90]="world 90";
    s_key2symbol[SDLK_WORLD_91]="world 91";
    s_key2symbol[SDLK_WORLD_92]="world 92";
    s_key2symbol[SDLK_WORLD_93]="world 93";
    s_key2symbol[SDLK_WORLD_94]="world 94";
    s_key2symbol[SDLK_WORLD_95]="world 95";
    s_key2symbol[SDLK_KP0]="keypad 0";
    s_key2symbol[SDLK_KP1]="keypad 1";
    s_key2symbol[SDLK_KP2]="keypad 2";
    s_key2symbol[SDLK_KP3]="keypad 3";
    s_key2symbol[SDLK_KP4]="keypad 4";
    s_key2symbol[SDLK_KP5]="keypad 5";
    s_key2symbol[SDLK_KP6]="keypad 6";
    s_key2symbol[SDLK_KP7]="keypad 7";
    s_key2symbol[SDLK_KP8]="keypad 8";
    s_key2symbol[SDLK_KP9]="keypad 9";
    s_key2symbol[SDLK_KP_PERIOD]="keypad period";
    s_key2symbol[SDLK_KP_DIVIDE]="keypad divide";
    s_key2symbol[SDLK_KP_MULTIPLY]="keypad multiply";
    s_key2symbol[SDLK_KP_MINUS]="keypad minus";
    s_key2symbol[SDLK_KP_PLUS]="keypad plus";
    s_key2symbol[SDLK_KP_ENTER]="keypad enter";
    s_key2symbol[SDLK_KP_EQUALS]="keypad equals";
    s_key2symbol[SDLK_UP]="up arrow";
    s_key2symbol[SDLK_DOWN]="down arrow";
    s_key2symbol[SDLK_RIGHT]="right arrow";
    s_key2symbol[SDLK_LEFT]="left arrow";
    s_key2symbol[SDLK_INSERT]="insert";
    s_key2symbol[SDLK_HOME]="home";
    s_key2symbol[SDLK_END]="end";
    s_key2symbol[SDLK_PAGEUP]="page up";
    s_key2symbol[SDLK_PAGEDOWN]="page down";
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
    s_key2symbol[SDLK_NUMLOCK]="numlock";
    s_key2symbol[SDLK_CAPSLOCK]="capslock";
    s_key2symbol[SDLK_SCROLLOCK]="scrollock";
    s_key2symbol[SDLK_RSHIFT]="right shift";
    s_key2symbol[SDLK_LSHIFT]="left shift";
    s_key2symbol[SDLK_RCTRL]="right ctrl";
    s_key2symbol[SDLK_LCTRL]="left ctrl";
    s_key2symbol[SDLK_RALT]="right alt / alt gr";
    s_key2symbol[SDLK_LALT]="left alt";
    s_key2symbol[SDLK_RMETA]="right meta";
    s_key2symbol[SDLK_LMETA]="left meta";
    s_key2symbol[SDLK_LSUPER]="left windows key";
    s_key2symbol[SDLK_RSUPER]="right windows key";
    s_key2symbol[SDLK_MODE]="mode shift";
    s_key2symbol[SDLK_COMPOSE]="compose";
    s_key2symbol[SDLK_HELP]="help";
    s_key2symbol[SDLK_PRINT]="print-screen";
    s_key2symbol[SDLK_SYSREQ]="SysRq";
    s_key2symbol[SDLK_BREAK]="break";
    s_key2symbol[SDLK_MENU]="menu";
    s_key2symbol[SDLK_POWER]="power";
    s_key2symbol[SDLK_EURO]="euro";
    s_key2symbol[SDLK_UNDO]="undo";
  }
  return s_key2symbol[k];
}

void gemsdlwindow :: dispatch()
{
  if(!m_surface)return;

  std::vector<t_atom>al;
  t_atom a;
  int state;

  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch(event.type) {
    default:
      post("event: %d", event.type);
      break;
    case SDL_ACTIVEEVENT: {
      state=event.active.gain;
      if(event.active.state & SDL_APPMOUSEFOCUS) {
        info("entry", state);
      }
      if(event.active.state & SDL_APPINPUTFOCUS) {
        info("inputentry", state);
      }
      if(event.active.state & SDL_APPACTIVE) {
        info("visible", state);
      }
    }
      break;
    case SDL_KEYUP:
    case SDL_KEYDOWN:
      key(key2symbol(event.key.keysym.sym), event.key.keysym.scancode, event.key.state==SDL_PRESSED);
      break;
    case SDL_MOUSEMOTION:
      motion(event.motion.x, event.motion.y);
      break;
    case SDL_MOUSEBUTTONUP:
    case SDL_MOUSEBUTTONDOWN:
      motion(event.button.x, event.button.y);
      button(event.button.button, event.button.state==SDL_PRESSED);
      break;
    case SDL_VIDEORESIZE:
      dimension(event.resize.w, event.resize.h);
      break;
    case SDL_VIDEOEXPOSE:
      info("window", "exposed");
      break;
    case SDL_QUIT:
      info("window", "destroy");
      break;
    }
  }

}


/////////////////////////////////////////////////////////
// bufferMess
//
/////////////////////////////////////////////////////////
void gemsdlwindow :: bufferMess(int buf)
{
  switch(buf) {
  case 1: case 2:
    m_buffer=buf;
    if(m_surface) {
      post("changing buffer type will only effect newly created windows");
    }
    break;
  default:
    error("buffer can only be '1' (single) or '2' (double) buffered");
    break;
  }
}

/////////////////////////////////////////////////////////
// fsaaMess
//
/////////////////////////////////////////////////////////
void gemsdlwindow :: fsaaMess(int value)
{
}

/////////////////////////////////////////////////////////
// titleMess
//
/////////////////////////////////////////////////////////
void gemsdlwindow :: titleMess(std::string s)
{
  m_title = s;
  if(m_surface) {
    SDL_WM_SetCaption(m_title.c_str(), m_title.c_str());
  }
}
/////////////////////////////////////////////////////////
// dimensionsMess
//
/////////////////////////////////////////////////////////
void gemsdlwindow :: dimensionsMess(unsigned int width, unsigned int height)
{
  if (width <= 0) {
    error("width must be greater than 0");
    return;
  }

  if (height <= 0 ) {
    error ("height must be greater than 0");
    return;
  }
  m_width = width;
  m_height = height;
  if(makeCurrent()){
    m_surface = SDL_SetVideoMode( m_width,
                                  m_height,
                                  m_bpp,
                                  m_videoFlags );
  }
}
/////////////////////////////////////////////////////////
// fullscreenMess
//
/////////////////////////////////////////////////////////
void gemsdlwindow :: fullscreenMess(int on)
{
  bool toggle=false;
  m_fullscreen = on;
  if(m_surface) {
    if(( m_fullscreen && !(m_surface->flags & SDL_FULLSCREEN)) ||
       (!m_fullscreen &&  (m_surface->flags & SDL_FULLSCREEN)))
        toggle=true;
  }
  if(toggle && makeCurrent()){
    SDL_WM_ToggleFullScreen( m_surface );
  }
}

/////////////////////////////////////////////////////////
// offsetMess
//
/////////////////////////////////////////////////////////
void gemsdlwindow :: offsetMess(int x, int y)
{
}

/////////////////////////////////////////////////////////
// createMess
//
/////////////////////////////////////////////////////////
bool gemsdlwindow :: create(void)
{
  if(m_surface) {
    error("window already made!");
    return false;
  }

  if ( SDL_InitSubSystem( SDL_INIT_VIDEO ) < 0 ) {
    error("could not (re)initialize SDL window infrastructure");
    return false;
  }

  /* Fetch the video info */
  const SDL_VideoInfo *videoInfo = SDL_GetVideoInfo( );

  m_videoFlags  = SDL_OPENGL;          /* Enable OpenGL in SDL */
  if(2==m_buffer) {
    m_videoFlags |= SDL_GL_DOUBLEBUFFER; /* Enable double buffering */

    /* Sets up OpenGL double buffering */
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
  } else {
    /* Sets up OpenGL double buffering */
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 0 );
  }
  m_videoFlags |= SDL_HWPALETTE;       /* Store the palette in hardware */
  m_videoFlags |= SDL_RESIZABLE;       /* Enable window resizing */


  if(videoInfo) {
    /* This checks to see if surfaces can be stored in memory */
    if ( videoInfo->hw_available )
      m_videoFlags |= SDL_HWSURFACE;
    else
      m_videoFlags |= SDL_SWSURFACE;

    /* This checks if hardware blits can be done */
    if ( videoInfo->blit_hw )
      m_videoFlags |= SDL_HWACCEL;
  }

  /* get a SDL surface */
  m_surface = SDL_SetVideoMode( m_width, m_height,
                                m_bpp,
                                m_videoFlags );

  if(!m_surface)
    return false;


  if(!createGemWindow()) {
    destroyMess();
    return false;
  }
  titleMess(m_title);
  fullscreenMess(m_fullscreen);

  dispatch();
  return true;
}
void gemsdlwindow :: createMess(std::string) {
  create();
}


/////////////////////////////////////////////////////////
// destroy window
//
/////////////////////////////////////////////////////////
void gemsdlwindow :: destroy(void)
{
  destroyGemWindow();
  m_surface=NULL;
  info("window", "closed");
}
void gemsdlwindow :: destroyMess(void)
{
  if(makeCurrent()) {
    SDL_QuitSubSystem( SDL_INIT_VIDEO );
  }
  destroy();
}

/////////////////////////////////////////////////////////
// cursorMess
//
/////////////////////////////////////////////////////////
void gemsdlwindow :: cursorMess(bool setting)
{
  m_cursor=setting;
}


/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void gemsdlwindow :: obj_setupCallback(t_class *classPtr)
{}
