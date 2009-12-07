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


#include "m_pd.h"

class GemSettingsData;

class GemSettings {
 public:
  static void init(void);
  static void print(void);
  static void save(void);

  static t_atom*get(t_symbol*key);
  static void set(t_symbol*key, t_atom*value=NULL);

  /* shorthands without symbols */
  static t_atom*get(const char*key);
  static void set(const char*key, t_atom*value=NULL);


  static void get(const char*key, int&value);
  static void get(const char*key, t_float&value);
  static void get(const char*key, t_symbol*&value);
  static void get(const char*key, char*&value);


 private:
  static GemSettingsData*settings;
};
#endif
