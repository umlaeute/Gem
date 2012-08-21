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
  m_bpp(16)
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
void gemsdlwindow :: dispatch()
{
  if(!m_surface)return;
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    post("event: %d", event.type);

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
