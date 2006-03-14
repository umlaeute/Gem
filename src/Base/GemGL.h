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

#ifdef __APPLE__
# include <OpenGL/gl.h>
# include <OpenGL/glu.h>
# include <OpenGL/glext.h>
# include <OpenGL/OpenGL.h>
#else

// on mesa, GL_GLEXT_LEGACY automatically includes glext.h from within gl.h
# define GL_GLEXT_LEGACY
# define GL_GLEXT_PROTOTYPES   1

# include <GL/gl.h>
# include <GL/glu.h>

# if  (!defined GL_GLEXT_VERSION) && (!defined DONT_INCLUDE_GLEXT)
/* windos is (again) a bit difficult:
 * by default, there are no GL/glext.h headers
 * but of course you can install nvidia's headers to get them.
 * since i don't know, whether the system has this headers,
 * we define DONT_INCLUDE_GLEXT in Base/configNT.h on demand
 * so, if your system lacks GL/glext.h,
 * just undefine the appropriate line in Base/configNT.h
 */

#  include <GL/glext.h>

# endif /* GLEXT */ 

/* some people have problems with the ARB-extensions for vertex shaders */
# ifdef DONT_USE_ARB
#  ifdef GL_ARB_vertex_program
#   undef GL_ARB_vertex_program
#  endif
#  ifdef GL_ARB_fragment_program
#   undef GL_ARB_fragment_program
#  endif

# ifdef GL_ARB_shader_objects
#  undef GL_ARB_shader_objects
# endif
# ifdef GL_ARB_shading_language_100
#  undef GL_ARB_shading_language_100
# endif

# endif /* DONT_USE_ARB */

/* using ATI's GL-libraries you don't have nvidia's extensions... */
# ifdef DONT_USE_NV
#  ifdef GL_NV_vertex_program
#   undef GL_NV_vertex_program
#  endif
#  ifdef GL_NV_fragment_program
#   undef GL_NV_fragment_program
#  endif
# endif /* DONT_USE_NV */
#endif // __APPLE__

#ifndef DONT_USE_ARB
# ifndef GL_ARB_fragment_program
#  define GL_ARB_fragment_program
#  define GL_FRAGMENT_PROGRAM_ARB           0x8804
#  define GL_PROGRAM_ALU_INSTRUCTIONS_ARB   0x8805
#  define GL_PROGRAM_TEX_INSTRUCTIONS_ARB   0x8806
#  define GL_PROGRAM_TEX_INDIRECTIONS_ARB   0x8807
#  define GL_PROGRAM_NATIVE_ALU_INSTRUCTIONS_ARB 0x8808
#  define GL_PROGRAM_NATIVE_TEX_INSTRUCTIONS_ARB 0x8809
#  define GL_PROGRAM_NATIVE_TEX_INDIRECTIONS_ARB 0x880A
#  define GL_MAX_PROGRAM_ALU_INSTRUCTIONS_ARB 0x880B
#  define GL_MAX_PROGRAM_TEX_INSTRUCTIONS_ARB 0x880C
#  define GL_MAX_PROGRAM_TEX_INDIRECTIONS_ARB 0x880D
#  define GL_MAX_PROGRAM_NATIVE_ALU_INSTRUCTIONS_ARB 0x880E
#  define GL_MAX_PROGRAM_NATIVE_TEX_INSTRUCTIONS_ARB 0x880F
#  define GL_MAX_PROGRAM_NATIVE_TEX_INDIRECTIONS_ARB 0x8810
#  define GL_MAX_TEXTURE_COORDS_ARB         0x8871
#  define GL_MAX_TEXTURE_IMAGE_UNITS_ARB    0x8872
# endif /* GL_ARB_fragment_program */
#endif /* DONT_USE_ARB */



// packed pixel defines for textures
#ifndef GL_EXT_packed_pixels
# define GL_UNSIGNED_BYTE_3_3_2_EXT          0x8032
# define GL_UNSIGNED_SHORT_4_4_4_4_EXT       0x8033
# define GL_UNSIGNED_SHORT_5_5_5_1_EXT       0x8034
# define GL_UNSIGNED_INT_8_8_8_8_EXT         0x8035
# define GL_UNSIGNED_INT_10_10_10_2_EXT      0x8036
#endif


// windows has this oh so old openGL installed...
#if !defined GL_BGRA && defined GL_BGRA_EXT
# define GL_BGRA GL_BGRA_EXT
#endif
#if !defined GL_BGRA_EXT && defined GL_BGRA
# define GL_BGRA_EXT GL_BGRA
#endif
#if !defined GL_BGRA && !defined GL_BGRA_EXT
# define GL_BGRA 0x80E1
# define GL_BGRA_EXT 0x80E1
#endif

#if !defined GL_BGR && defined GL_BGR_EXT
# define GL_BGR GL_BGR_EXT
#endif
#if !defined GL_BGR_EXT && defined GL_BGR
# define GL_BGR_EXT GL_BGR
#endif
#if !defined GL_BGR && !defined GL_BGR_EXT
# define GL_BGR 0x80E0
# define GL_BGR_EXT 0x80E0
#endif

#ifndef GL_YUV422_GEM

# ifdef GL_YCBCR_422_APPLE
#  define GL_YCBCR_422_GEM GL_YCBCR_422_APPLE
# elif defined GL_CRYCBY_422_NVX
#  define GL_YCBCR_422_GEM GL_CRYCBY_422_NVX
//#  define GL_YCBCR_422_GEM GL_YCRYCB_422_NVX
# elif defined GL_YCRCB_422_SGIX
#  define GL_YCBCR_422_GEM GL_YCRCB_422_SGIX
# endif

# ifndef GL_YCBCR_422_GEM
#  define GL_YCBCR_422_GEM 0x85B9
# endif

# define GL_YUV422_GEM GL_YCBCR_422_GEM
#endif /* GL_YUV422_GEM */

#ifndef GL_TEXTURE_RECTANGLE_EXT
# define GL_TEXTURE_RECTANGLE_EXT 0x84F5
#endif


/* default draw-style */
#ifndef GL_DEFAULT_GEM
# define GL_DEFAULT_GEM 0xFFFF
#endif

#endif /* INCLUDE_GEMGL_H_ */
