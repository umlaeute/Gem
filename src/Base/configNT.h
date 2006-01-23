/* configuration for Windows M$VC
 *
 * this file should get included if _MSC_VER is defined
 * for default-defines on _MSC_VER see
 * see http://msdn.microsoft.com/library/default.asp?url=/library/en-us/vclang/html/_predir_predefined_macros.asp for available macros
 * 
 * most of these settings require special libraries to be linked against
 * so you might have to edit Project->Settings too
 *
 * NOTE: LATER think about loading the libs from within this file on M$VC
 *       #pragma comment(lib, "library")
 *       see http://msdn.microsoft.com/library/default.asp?url=/library/en-us/vclang/html/_predir_comment.asp
 */

/* use FTGL instead of GLTT for font-rendering */
#define FTGL

/* use the "new" film-objects, to allow the use of multiple APIs */
#define FILM_NEW

/* quicktime-support for film-loading */
#define HAVE_QUICKTIME

/* use direct-show for video-in (e.g. for firewire,...) */
#define HAVE_DIRECTSHOW


/*
 * if we use nvidia's headers, we will most likely want to include gl/glext.h
 * if you don't have gl/glext.h, enable the following...
 */
#define DONT_INCLUDE_GLEXT


/*
 * we want libjpeg and libtiff for reading/writing images
 */
#define HAVE_LIBTIFF
#define HAVE_LIBJPEG



/*
 * seems like the glBlendEquation is non-existing on my w32-machine
 */
#define DONT_HAVE_GLBLENDEQUATION









/* **********************************************************************
 * now do a bit of magic based on the information given above
 * probably you don't want to edit this
 * (but who knows)
 * ******************************************************************** */


#ifdef FTGL
# define FTGL_LIBRARY_STATIC
#endif
