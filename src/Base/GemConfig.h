/* configuration-file */
#ifndef HAVE_BASE_GEMCONFIG_H_
#define HAVE_BASE_GEMCONFIG_H_

#ifdef _MSC_VER
# ifndef _WIN32
#  define _WIN32
# endif
#endif

#ifdef _WIN32
# ifndef NT
#  define NT
# endif
# ifndef MSW
#  define MSW
# endif
# ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
# endif
#endif



#ifdef HAVE_CONFIG_GENERIC_H

# include "Base/configGeneric.h"

#else /* includes system-specific files */

# ifdef __linux__
#  include "Base/configLinux.h"
# endif

# ifdef __APPLE__
#  include "Base/configDarwin.h"
#  define HAVE_QUICKTIME
# endif

# ifdef _WIN32
#  include "Base/configNT.h"
# endif
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
#endif

#endif /* HAVE_BASE_GEMCONFIG_H_ */
