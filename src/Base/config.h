/* configuration-file */
#ifndef HAVE_BASE_CONFIG_H_
#define HAVE_BASE_CONFIG_H_

#ifdef _MSC_VER
# ifndef __WIN32__
#  define __WIN32__
# endif
#endif

#ifdef __WIN32__
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

# ifdef __WIN32__
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

#ifdef HAVE_FFMPEG
# define HAVE_LIBAVCODEC
# define HAVE_LIBAVFORMAT
#elif defined (HAVE_LIBAVCODEC) && defined (HAVE_LIBAVFORMAT)
# define HAVE_FFMPEG
#endif

#endif /* HAVE_BASE_CONFIG_H_ */
