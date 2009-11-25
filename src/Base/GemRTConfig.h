/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

	- registers a loader with Pd

    Copyright (c) 2009 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_GEMRTCONFIG_H_
#define INCLUDE_GEMRTCONFIG_H


#include "m_pd.h"

class GemRTConfig_data;

class GemRTConfig {
 public:
  static t_atom*getEnv(t_symbol*name);
  static t_atom*get(t_symbol*name);
  static void set(t_symbol*name, t_atom*value);

  static GemRTConfig*init(void);

 public:
  GemRTConfig(void);
  ~GemRTConfig(void);

 protected:
  virtual t_atom*getValue(t_symbol*name);
  virtual void setValue(t_symbol*name, t_atom*value);

  // dictionary for setting values
  // ??

 private:
  bool open(const char*filename, const char*dirname=NULL);
  void print(void);

  GemRTConfig_data*m_data;

  static const char* s_configfile[];

  static GemRTConfig*s_defaultconfig;

};

#endif
