/* ../Base/config.h.  Generated automatically by configure.  */
/* Define if you have the <mpeg.h> header file.  */
#define HAVE_MPEG_H 1

/* Define if you have the <libmpeg3.h> header file.  */
#define HAVE_LIBMPEG3_H 1

/* Define if you have the <quicktime/quicktime.h> header file.  */
#define HAVE_QUICKTIME_QUICKTIME_H 1

/* Define if you have the <quicktime/libdv/libdv.h> header file. */
#define HAVE_QUICKTIME_LIBDV_LIBDV_H 0




/* --------------------------- */
/* no editing beyond this line */

#if HAVE_LIBMPEG3_H
#define HAVE_MPEG3
#else
#if HAVE_MPEG_H
#define HAVE_MPEG
#endif
#endif

#if HAVE_QUICKTIME_QUICKTIME_H
#define HAVE_QUICKTIME
#if HAVE_QUICKTIME_LIBDV_LIBDV_H
#define HAVE_DV
#endif
#endif
