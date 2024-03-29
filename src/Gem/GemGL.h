/*
 * GemGL: openGL includes for GEM
 *
 * include this file if you want to include the
 * openGL-headers installed on your system
 *
 * tasks:
 *
 * + this file hides the peculiarities of the various platforms
 *   (like "OpenGL/gl.h" vs "GL/gl.h")
 *
 * + define some pre-processor defines that are missing in the GL-headers
 *
 * + try to exclude parts of the GL-headers based on GemConfig.h
 *
 */


#ifndef _INCLUDE__GEM_GEM_GEMGL_H_
#define _INCLUDE__GEM_GEM_GEMGL_H_

#include "Gem/ExportDef.h"

// I hate Microsoft...I shouldn't have to do this!
#ifdef _WIN32
# include <windows.h>
#endif

#include "Gem/GemGLconfig.h"

#ifdef GEM_MULTICONTEXT
# define GLEW_MX
#endif

#ifndef GEM_HAVE_GLU
# define GLEW_NO_GLU
#else
#endif


#ifdef __EMSCRIPTEN__
# include <GL/glew.h>
# include <SDL/SDL_opengl.h>
# include <GL/gl.h>
# ifdef GEM_HAVE_GLU
#  include <GL/glu.h>
# endif
#else
# include "Gem/glew.h"
#endif

#ifdef __APPLE__
# include <OpenGL/OpenGL.h>
#elif defined(__EMSCRIPTEN__)
#elif defined _WIN32
# include "Gem/wglew.h"
#elif defined(__linux__) || defined(__FreeBSD_kernel__)
# include "Gem/glxew.h"
#endif /* OS */

#ifdef GEM_MULTICONTEXT
GEM_EXTERN GLEWContext*glewGetContext(void);
# ifdef __APPLE__
# elif defined _WIN32
GEM_EXTERN WGLEWContext*wglewGetContext(void);
# elif defined __linux__ || defined HAVE_GL_GLX_H
GEM_EXTERN GLXEWContext*glxewGetContext(void);
# endif

#endif /* GEM_MULTICONTEXT */

#ifndef GL_YUV422_GEM
# define GL_YCBCR_422_GEM GL_YCBCR_422_APPLE
# define GL_YUV422_GEM GL_YCBCR_422_GEM
#endif /* GL_YUV422_GEM */


#ifndef GL_RGB_GEM
# ifdef __APPLE__
#  define GL_RGB_GEM GL_BGR_EXT
# else
#  define GL_RGB_GEM GL_RGB
# endif
#endif /* GL_RGB_GEM */

#ifndef GL_RGBA_GEM
# ifdef __APPLE__
#  define GL_RGBA_GEM GL_BGRA_EXT
# else
#  define GL_RGBA_GEM GL_RGBA
# endif
#endif /* GL_RGBA_GEM */

/* default draw-style */
#ifndef GL_DEFAULT_GEM
# define GL_DEFAULT_GEM 0xFFFF
#endif



/* uäh: in OSX10.3 we only have CGL-1.1 and
 * all the functions are using "long*" rather than "GLint*")
 * only CGL-1.2 got it right
 */
#ifdef CGL_VERSION_1_0
# ifdef CGL_VERSION_1_2
#  define GemCGLint GLint
# else
#  define GemCGLint long
# endif
#endif


// missing from Emscripten's glew implementation
#ifndef GL_TEXTURE_RECTANGLE_EXT
# define GL_TEXTURE_RECTANGLE_EXT 0x84F5
#endif

#endif /* _INCLUDE__GEM_GEM_GEMGL_H_ */
