#ifndef GEM_VERSION_H
#define GEM_VERSION_H

#define GEM_VERSION_MAJOR 0
#define GEM_VERSION_MINOR 91
#define GEM_VERSION_BUGFIX CVS
#define GEM_VERSION_CODENAME 

class GEM_EXTERN GemVersion {
  public:
	static char* versionString(void);
	static bool versionCheck(int major, int minor);
};

#endif

