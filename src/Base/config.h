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

#endif /* HAVE_BASE_CONFIG_H_ */
