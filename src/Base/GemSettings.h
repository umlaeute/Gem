/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

	- registers a loader with Pd

    Copyright (c) 2009 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_GEMSETTINGS_H_
#define INCLUDE_GEMSETTINGS_H


#include "m_pd.h"

class GemSettings_data;

class GemSettings {
 public:
  static t_atom*getEnv(t_symbol*name);
  static t_atom*get(t_symbol*name);
  static void set(t_symbol*name, t_atom*value);

  static GemSettings*init(void);

 public:
  GemSettings(void);
  ~GemSettings(void);

 protected:
  virtual t_atom*getValue(t_symbol*name);
  virtual void setValue(t_symbol*name, t_atom*value);

  // dictionary for setting values
  // ??

 private:
  bool open(const char*filename, const char*dirname=NULL);
  void print(void);

  GemSettings_data*m_data;

  static const char* s_configfile[];

  static GemSettings*s_defaultconfig;

};

#endif
