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

/* use FTGL for font-rendering */
#define FTGL

/* use the "new" film-objects, to allow the use of multiple APIs */
//#define GEM_VIDEOBACKEND GEM_VIDEOBACKEND_DS

/* quicktime-support for film-loading */
#define HAVE_QUICKTIME

/* **********************************************************************
 * now do a bit of magic based on the information given above
 * probably you don't want to edit this
 * (but who knows)
 * ******************************************************************** */
#ifdef FTGL
# define FTGL_LIBRARY_STATIC
#endif