/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

	- registers a loader with Pd

    Copyright (c) 2009 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_GEMCONFIG_H_
#define INCLUDE_GEMCONFIG_H

#include "m_pd.h"

class GemConfig {
 public:
  static void*getValue(t_symbol*name);
  static void setValue(t_symbol*name, void*value);

 public:
  GemSetup(void);
  ~GemSetup(void);

 protected:
  virtual void*getValue(t_symbol*name);
  virtual void setValue(t_symbol*name, void*value);

  // dictionary for setting values
  // ??

 private:
  void open(const char*);
  static char** s_configfile

}

#endif
