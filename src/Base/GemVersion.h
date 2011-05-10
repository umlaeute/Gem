#ifndef GEM_VERSION_H
#define GEM_VERSION_H

#include "Base/GemExportDef.h"

#define GEM_VERSION_MAJOR 0
#define GEM_VERSION_MINOR 92
#define GEM_VERSION_BUGFIX 4
#define GEM_VERSION_CODENAME 

class GEM_EXTERN GemVersion {
  public:
	const static char* const versionString(void);
	static bool versionCheck(int major, int minor);
};

#endif

