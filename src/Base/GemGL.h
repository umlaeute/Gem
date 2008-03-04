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

#include "Base/glew.h"

#ifdef __APPLE__
# include <OpenGL/glext.h>
# include <OpenGL/OpenGL.h>
#else
/* linux, w32,... */
# define GL_GLEXT_PROTOTYPES   1

# if (!defined DONT_INCLUDE_GLEXT)
/* windos is (again) a bit difficult:
 * by default, there are no GL/glext.h headers
 * but of course you can install nvidia's headers to get them.
 * since i don't know, whether the system has this headers,
 * we define DONT_INCLUDE_GLEXT in Base/configNT.h on demand
 * so, if your system lacks GL/glext.h,
 * just undefine the appropriate line in Base/configNT.h
 */
#  if  (!defined GL_GLEXT_VERSION)
#   include <GL/glext.h>
#  endif /* GL_GLEXT_VERSION */

#  ifdef __WIN32__
#   include <GL/wglext.h>
#  endif

# endif /* GLEXT */ 

#endif // __APPLE__

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
