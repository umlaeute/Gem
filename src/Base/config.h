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


#ifdef __NEW__
# ifndef __FILM__NEW
#  define __FILM__NEW
# endif
# ifndef __VIDEO__NEW
#  define __VIDEO__NEW
# endif
#endif

#endif /* HAVE_BASE_CONFIG_H_ */
