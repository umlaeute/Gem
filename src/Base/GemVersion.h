#ifndef GEM_VERSION_H
#define GEM_VERSION_H

#define GEM_VERSION_MAJOR 0
#define GEM_VERSION_MINOR 91
#define GEM_VERSION_BUGFIX 1
#define GEM_VERSION_CODENAME "'tigital'"


#define STRINGIFY2(x) #x
#define STRINGIFY(x) STRINGIFY2(x)

static const char *GEM_VERSION = \
  "" STRINGIFY(GEM_VERSION_MAJOR)  \
  "." STRINGIFY(GEM_VERSION_MINOR) \
  "." STRINGIFY(GEM_VERSION_BUGFIX) \
  " " GEM_VERSION_CODENAME;


static const char *GEM_MAINTAINER = "IOhannes m zmoelnig";

static const char *GEM_AUTHORS[] = {
  "Chris Clepper",
  "James Tittle",
  "IOhannes m zmoelnig"};

static const char* GEM_OTHERAUTHORS =
  "Guenter Geiger, Daniel Heckenberg, Cyrille Henry, et al.";
#endif

