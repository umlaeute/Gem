/* configuration for Windows 
 * 
 * most of these settings require special libraries to be linked against
 * so you might have to edit Project->Settings too
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
//#define DONT_INCLUDE_GLEXT




/*
 * here comes some generic configuration based on the information above.
 * probably you don't want to edit this
 */

#ifdef FTGL
# define FTGL_LIBRARY_STATIC
#endif
