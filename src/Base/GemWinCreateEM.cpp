////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// claude@mathr.co.uk
//
// Implementation file
//
//    Copyright (c) 1997-2000 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2012 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    Copyright (c) 2019 Claude Heiland-Allen
//
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
#include "Gem/GemConfig.h"
#ifdef __EMSCRIPTEN__

#include "GemWinCreate.h"
#include "Gem/Event.h"
#include "Gem/GemGL.h"
#include "Gem/RTE.h"

#include <map>
#include <string>

/////////////////////////////////////////////////////////
// createGemWindow
//
/////////////////////////////////////////////////////////
GEM_EXTERN int createGemWindow(WindowInfo &info, WindowHints &hints)
{
  static int firstTime = 1;
  if (firstTime) {
    SDL_Init(SDL_INIT_VIDEO);
    firstTime = 0;
  }

  if (! hints.actuallyDisplay) {
    return(1);
  }

  int w = hints.width;
  int h = hints.height;
  int x = hints.x_offset;
  int y = hints.y_offset;
  bool fullscreen = (hints.fullscreen != 0);
  bool border = (hints.border != 0);

  info.win = SDL_CreateWindow(hints.title, x, y, w, h
    , SDL_WINDOW_OPENGL
    | SDL_WINDOW_RESIZABLE
    | (fullscreen ? SDL_WINDOW_FULLSCREEN : 0)
    | (border ? 0 : SDL_WINDOW_BORDERLESS)
    );
  info.fs = fullscreen;

  if (! info.win)  {
    error("GEM: Unable to create window");
    return(0);
  }

// Emscripten doesn't support shared contexts yet
#if 0
  if (hints.shared) {
    if (SDL_GL_MakeCurrent(info.win, hints.shared)) {
      error("GEM: Unable to make shared OpenGL context current: %s", SDL_GetError());
      destroyGemWindow(info);
      return(0);
    }
    SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);
  }
#endif
  info.context = SDL_GL_CreateContext(info.win);
#if 0
  if (! info.context) {
    error("GEM: Unable to create OpenGL context: %s", SDL_GetError());
    SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 0);
    destroyGemWindow(info);
    return(0);
  }
  SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 0);
#endif

  if (SDL_GL_MakeCurrent(info.win, info.context)) {
    error("GEM: Unable to make OpenGL context current: %s", SDL_GetError());
    destroyGemWindow(info);
    return(0);
  }

  if (fullscreen) {
    SDL_RaiseWindow(info.win);
  }

  return(1);
}

/////////////////////////////////////////////////////////
// destroyGemWindow
//
/////////////////////////////////////////////////////////
GEM_EXTERN void destroyGemWindow(WindowInfo &info)
{
  if (info.win) {
    if (info.context) {
      SDL_GL_DeleteContext(info.context);
    }
    SDL_DestroyWindow(info.win);
  }
  info.win = NULL;
  info.context = 0;
}

/////////////////////////////////////////////////////////
// switch cursor on/off
//
/////////////////////////////////////////////////////////
int cursorGemWindow(WindowInfo &info, int state)
{
  static int cursor_state = 1;
  state=!(!state);
  if (cursor_state != state) {
    cursor_state=SDL_ShowCursor(state);
    if (cursor_state != (cursor_state & 1))
    {
      fprintf(stderr, "cursor problem: %s\n", SDL_GetError());
    }
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
  if (state)
  {
    SDL_RaiseWindow(info.win); // FIXME not permanent
  }
  topmost_state = state;
  return topmost_state;
}

void gemWinSwapBuffers(WindowInfo &nfo)
{
  SDL_GL_SwapWindow(nfo.win);
}

void gemWinMakeCurrent(WindowInfo&nfo)
{
  if (!nfo.win || !nfo.context) {
    return;  // do not crash ??
  }
  SDL_GL_MakeCurrent(nfo.win, nfo.context);
}

bool initGemWin(void)
{
  return 1;
}

GEM_EXTERN void initWin_sharedContext(WindowInfo &info, WindowHints &hints)
{
  //  myHints.shared = constInfo.context;
  hints.shared = NULL;
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

#ifndef __EMSCRIPTEN__
    s_worldkey2symbol[SDLK_WORLD_0]="World_0";
    s_worldkey2symbol[SDLK_WORLD_1]="World_1";
    s_worldkey2symbol[SDLK_WORLD_2]="World_2";
    s_worldkey2symbol[SDLK_WORLD_3]="World_3";
    s_worldkey2symbol[SDLK_WORLD_4]="World_4";
    s_worldkey2symbol[SDLK_WORLD_5]="World_5";
    s_worldkey2symbol[SDLK_WORLD_6]="World_6";
    s_worldkey2symbol[SDLK_WORLD_7]="World_7";
    s_worldkey2symbol[SDLK_WORLD_8]="World_8";
    s_worldkey2symbol[SDLK_WORLD_9]="World_9";
    s_worldkey2symbol[SDLK_WORLD_10]="World_10";
    s_worldkey2symbol[SDLK_WORLD_11]="World_11";
    s_worldkey2symbol[SDLK_WORLD_12]="World_12";
    s_worldkey2symbol[SDLK_WORLD_13]="World_13";
    s_worldkey2symbol[SDLK_WORLD_14]="World_14";
    s_worldkey2symbol[SDLK_WORLD_15]="World_15";
    s_worldkey2symbol[SDLK_WORLD_16]="World_16";
    s_worldkey2symbol[SDLK_WORLD_17]="World_17";
    s_worldkey2symbol[SDLK_WORLD_18]="World_18";
    s_worldkey2symbol[SDLK_WORLD_19]="World_19";
    s_worldkey2symbol[SDLK_WORLD_20]="World_20";
    s_worldkey2symbol[SDLK_WORLD_21]="World_21";
    s_worldkey2symbol[SDLK_WORLD_22]="World_22";
    s_worldkey2symbol[SDLK_WORLD_23]="World_23";
    s_worldkey2symbol[SDLK_WORLD_24]="World_24";
    s_worldkey2symbol[SDLK_WORLD_25]="World_25";
    s_worldkey2symbol[SDLK_WORLD_26]="World_26";
    s_worldkey2symbol[SDLK_WORLD_27]="World_27";
    s_worldkey2symbol[SDLK_WORLD_28]="World_28";
    s_worldkey2symbol[SDLK_WORLD_29]="World_29";
    s_worldkey2symbol[SDLK_WORLD_30]="World_30";
    s_worldkey2symbol[SDLK_WORLD_31]="World_31";
    s_worldkey2symbol[SDLK_WORLD_32]="World_32";
    s_worldkey2symbol[SDLK_WORLD_33]="World_33";
    s_worldkey2symbol[SDLK_WORLD_34]="World_34";
    s_worldkey2symbol[SDLK_WORLD_35]="World_35";
    s_worldkey2symbol[SDLK_WORLD_36]="World_36";
    s_worldkey2symbol[SDLK_WORLD_37]="World_37";
    s_worldkey2symbol[SDLK_WORLD_38]="World_38";
    s_worldkey2symbol[SDLK_WORLD_39]="World_39";
    s_worldkey2symbol[SDLK_WORLD_40]="World_40";
    s_worldkey2symbol[SDLK_WORLD_41]="World_41";
    s_worldkey2symbol[SDLK_WORLD_42]="World_42";
    s_worldkey2symbol[SDLK_WORLD_43]="World_43";
    s_worldkey2symbol[SDLK_WORLD_44]="World_44";
    s_worldkey2symbol[SDLK_WORLD_45]="World_45";
    s_worldkey2symbol[SDLK_WORLD_46]="World_46";
    s_worldkey2symbol[SDLK_WORLD_47]="World_47";
    s_worldkey2symbol[SDLK_WORLD_48]="World_48";
    s_worldkey2symbol[SDLK_WORLD_49]="World_49";
    s_worldkey2symbol[SDLK_WORLD_50]="World_50";
    s_worldkey2symbol[SDLK_WORLD_51]="World_51";
    s_worldkey2symbol[SDLK_WORLD_52]="World_52";
    s_worldkey2symbol[SDLK_WORLD_53]="World_53";
    s_worldkey2symbol[SDLK_WORLD_54]="World_54";
    s_worldkey2symbol[SDLK_WORLD_55]="World_55";
    s_worldkey2symbol[SDLK_WORLD_56]="World_56";
    s_worldkey2symbol[SDLK_WORLD_57]="World_57";
    s_worldkey2symbol[SDLK_WORLD_58]="World_58";
    s_worldkey2symbol[SDLK_WORLD_59]="World_59";
    s_worldkey2symbol[SDLK_WORLD_60]="World_60";
    s_worldkey2symbol[SDLK_WORLD_61]="World_61";
    s_worldkey2symbol[SDLK_WORLD_62]="World_62";
    s_worldkey2symbol[SDLK_WORLD_63]="World_63";
    s_worldkey2symbol[SDLK_WORLD_64]="World_64";
    s_worldkey2symbol[SDLK_WORLD_65]="World_65";
    s_worldkey2symbol[SDLK_WORLD_66]="World_66";
    s_worldkey2symbol[SDLK_WORLD_67]="World_67";
    s_worldkey2symbol[SDLK_WORLD_68]="World_68";
    s_worldkey2symbol[SDLK_WORLD_69]="World_69";
    s_worldkey2symbol[SDLK_WORLD_70]="World_70";
    s_worldkey2symbol[SDLK_WORLD_71]="World_71";
    s_worldkey2symbol[SDLK_WORLD_72]="World_72";
    s_worldkey2symbol[SDLK_WORLD_73]="World_73";
    s_worldkey2symbol[SDLK_WORLD_74]="World_74";
    s_worldkey2symbol[SDLK_WORLD_75]="World_75";
    s_worldkey2symbol[SDLK_WORLD_76]="World_76";
    s_worldkey2symbol[SDLK_WORLD_77]="World_77";
    s_worldkey2symbol[SDLK_WORLD_78]="World_78";
    s_worldkey2symbol[SDLK_WORLD_79]="World_79";
    s_worldkey2symbol[SDLK_WORLD_80]="World_80";
    s_worldkey2symbol[SDLK_WORLD_81]="World_81";
    s_worldkey2symbol[SDLK_WORLD_82]="World_82";
    s_worldkey2symbol[SDLK_WORLD_83]="World_83";
    s_worldkey2symbol[SDLK_WORLD_84]="World_84";
    s_worldkey2symbol[SDLK_WORLD_85]="World_85";
    s_worldkey2symbol[SDLK_WORLD_86]="World_86";
    s_worldkey2symbol[SDLK_WORLD_87]="World_87";
    s_worldkey2symbol[SDLK_WORLD_88]="World_88";
    s_worldkey2symbol[SDLK_WORLD_89]="World_89";
    s_worldkey2symbol[SDLK_WORLD_90]="World_90";
    s_worldkey2symbol[SDLK_WORLD_91]="World_91";
    s_worldkey2symbol[SDLK_WORLD_92]="World_92";
    s_worldkey2symbol[SDLK_WORLD_93]="World_93";
    s_worldkey2symbol[SDLK_WORLD_94]="World_94";
    s_worldkey2symbol[SDLK_WORLD_95]="World_95";
#endif

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
#ifndef __EMSCRIPTEN__
    s_key2symbol[SDLK_NUMLOCK]="Num_Lock";
#endif
    s_key2symbol[SDLK_CAPSLOCK]="Caps_Lock";
    s_key2symbol[SDLK_SCROLLLOCK]="Scroll_Lock";
    s_key2symbol[SDLK_RSHIFT]="Shift_R";
    s_key2symbol[SDLK_LSHIFT]="Shift_L";
    s_key2symbol[SDLK_RCTRL]="Control_R";
    s_key2symbol[SDLK_LCTRL]="Control_L";
    s_key2symbol[SDLK_RALT]="AltGr";
    s_key2symbol[SDLK_LALT]="Alt_L";
#ifndef __EMSCRIPTEN__
    s_key2symbol[SDLK_RMETA]="Meta_R";
    s_key2symbol[SDLK_LMETA]="Meta_L";
    s_key2symbol[SDLK_LSUPER]="Super_L";
    s_key2symbol[SDLK_RSUPER]="Super_R";
#endif
    s_key2symbol[SDLK_MODE]="Mode";
#ifndef __EMSCRIPTEN__
    s_key2symbol[SDLK_COMPOSE]="Compose";
#endif
    s_key2symbol[SDLK_HELP]="Help";
#ifndef __EMSCRIPTEN__
    s_key2symbol[SDLK_PRINT]="Print";
#endif
    s_key2symbol[SDLK_SYSREQ]="SysRq";
#ifndef __EMSCRIPTEN__
    s_key2symbol[SDLK_BREAK]="Break";
#endif
    s_key2symbol[SDLK_MENU]="Menu";
    s_key2symbol[SDLK_POWER]="Power";
#ifndef __EMSCRIPTEN__
    s_key2symbol[SDLK_EURO]="€";
#endif
    s_key2symbol[SDLK_UNDO]="Undo";
  }
  std::string s = s_key2symbol[k];
  if(s.empty()) {
    s=s_worldkey2symbol[k];
  }
  if(s.empty()) {
    s="<unknown>";
  }

  return s;
}

GEM_EXTERN void dispatchGemWindowMessages(WindowInfo &win)
{
  SDL_Event event;
  while (SDL_PollEvent(&event))
  {
    switch (event.type)
    {
      case SDL_QUIT:
        // ignore
        break;
      case SDL_WINDOWEVENT:
        switch (event.window.event)
        {
          case SDL_WINDOWEVENT_CLOSE:
            // ignore
            break;
          case SDL_WINDOWEVENT_RESIZED:
          case SDL_WINDOWEVENT_SIZE_CHANGED:
            triggerResizeEvent(event.window.data1, event.window.data2);
            break;
        }
        break;
      case SDL_KEYDOWN:
      case SDL_KEYUP:
        triggerKeyboardEvent
          ( key2symbol(event.key.keysym.sym).c_str()
          , event.key.keysym.scancode
          , event.type == SDL_KEYDOWN
          );
        break;
      case SDL_MOUSEBUTTONDOWN:
      case SDL_MOUSEBUTTONUP:
        triggerButtonEvent
          ( event.button.button == SDL_BUTTON_LEFT ? 0
          : event.button.button == SDL_BUTTON_MIDDLE ? 1
          : event.button.button == SDL_BUTTON_RIGHT ? 2
          : event.button.button // FIXME
          , event.type == SDL_MOUSEBUTTONDOWN
          , event.button.x
          , event.button.y
          );
        break;
      case SDL_MOUSEMOTION:
        triggerMotionEvent(event.motion.x, event.motion.y);
        break;
      case SDL_MOUSEWHEEL: // FIXME
        if (event.wheel.y != 0) triggerWheelEvent(0, event.wheel.y);
        if (event.wheel.x != 0) triggerWheelEvent(1, event.wheel.x);
        break;
    }
  }
}

#endif /* Emscripten */
