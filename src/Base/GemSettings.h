/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

	- Runtime Configuration of Gem

    Copyright (c) 2009 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_GEMSETTINGS_H_
#define INCLUDE_GEMSETTINGS_H

#include <string>

typedef struct _atom t_atom;
class GemSettings {
 public:
  static void init(void);
  static void print(void);
  static void save(void);

  static t_atom*get(const std::string key);
  static void set(const std::string key, t_atom*value=NULL);

  static void get(const std::string key, int&value);
  static void get(const std::string key, float&value);
  static void get(const std::string key, std::string&value);

 private:
  class PIMPL;
  static PIMPL*settings;
};
#endif
