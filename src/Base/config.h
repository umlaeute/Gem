/* configuration-file */
#ifndef HAVE_BASE_CONFIG_H_
#define HAVE_BASE_CONFIG_H_

/* includes system-specific files */

#ifdef __linux__
# include "Base/configLinux.h"
#endif

#ifdef __APPLE__
# include "Base/configDarwin.h"
#endif

#ifdef NT
# include "Base/configNT.h"
#endif


#ifdef NEW_VIDEOFILM
# ifndef FILM_NEW
#  define FILM_NEW
# endif
# ifndef VIDEO_NEW
#  define VIDEO_NEW
# endif
#endif

#ifdef HAVE_LIBFTGL
#  define FTGL
#elif defined HAVE_LIBGLTT
#  define GLTT
#endif

#endif /* HAVE_BASE_CONFIG_H_ */
