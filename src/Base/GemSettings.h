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

  static t_atom*get(t_symbol*name);
  static void set(t_symbol*name, t_atom*value);

  /* shorthands without symbols */
  static t_atom*get(const char*name);
  static void set(const char*name, t_atom*value);

 private:
  static GemSettingsData*settings;
};
#endif
