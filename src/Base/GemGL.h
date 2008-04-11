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
 * + try to exclude parts of the GL-headers based on config.h
 *
 */


#ifndef INCLUDE_GEMGL_H_
#define INCLUDE_GEMGL_H_

#include "config.h"

// I hate Microsoft...I shouldn't have to do this!
#ifdef __WIN32__
# include <windows.h>
#endif

#define GLEW_STATIC
#include "Base/glew.h"

#ifdef __APPLE__
# include <OpenGL/glext.h>
# include <OpenGL/OpenGL.h>
#elif defined __WIN32__
# include "Base/wglew.h"
#elif defined __linux__

#endif /* OS */

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

#endif /* INCLUDE_GEMGL_H_ */
