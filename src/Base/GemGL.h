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


#ifndef INCLUDE_GEMGL_H_
#define INCLUDE_GEMGL_H_

#include "Base/GemExportDef.h"

// I hate Microsoft...I shouldn't have to do this!
#ifdef _WIN32
# include <windows.h>
#endif

#ifdef GLEW_MX
# define GEM_MULTICONTEXT
#endif

#define GLEW_STATIC
#include "Base/glew.h"

#ifdef __APPLE__
# include <OpenGL/OpenGL.h>
#elif defined _WIN32
# include "Base/wglew.h"
#elif defined __linux__
#  include "Base/glxew.h"
#endif /* OS */

#ifdef GEM_MULTICONTEXT
GEM_EXTERN GLEWContext*glewGetContext(void);
# ifdef __APPLE__
# elif defined _WIN32
GEM_EXTERN WGLEWContext*wglewGetContext(void);
# elif defined __linux__
GEM_EXTERN GLXEWContext*glxewGetContext(void);
# endif

#endif /* GEM_MULTICONTEXT */

#ifndef GL_YUV422_GEM
# define GL_YCBCR_422_GEM GL_YCBCR_422_APPLE
# define GL_YUV422_GEM GL_YCBCR_422_GEM
#endif /* GL_YUV422_GEM */


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

#endif /* INCLUDE_GEMGL_H_ */
