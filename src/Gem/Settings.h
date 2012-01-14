/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

	- Runtime Configuration of Gem

    Copyright (c) 2009-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef _INCLUDE__GEM_GEM_SETTINGS_H_
#define _INCLUDE__GEM_GEM_SETTINGS_H_

#include <string>
#include <vector>

typedef struct _atom t_atom;
namespace gem { namespace Settings {
  void init(void);
  void print(void);
  void save(void);

  t_atom*get(const std::string key);
  void set(const std::string key, t_atom*value=NULL);

  void get(const std::string key, int&value);
  void get(const std::string key, float&value);
  void get(const std::string key, double&value);
  void get(const std::string key, std::string&value);

  std::vector<std::string> keys();
}; };
#endif
