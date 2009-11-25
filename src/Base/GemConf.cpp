#if 0
////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.at
//
// Implementation file
//
//    Copyright (c) 2009 IOhannes m zmoelnig. forum::für::umläute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
// a wrapper for calling Pd's sys_register_loader()
//
/////////////////////////////////////////////////////////

#include "GemConfig.h"



GemConfig::s_configfile = { 
#ifdef __linux__
/*
  $(pwd)/gem.conf
  ~/pd/gem.conf
  /etc/pd/gem.conf
*/
  "/etc/pd/gem.conf",
  "~/pd/gem.conf", 
  "gem.conf",
#elif defined __APPLE__
/*

*/
#elif defined  _WIN32
/*

*/
#endif
  0};




GemConfig::GemConfig(void) :
  m_intialized(false) 
{
  int i=0;

  while(m_configfile[i]) {
    load(m_configfile[i]);
  }
  
  while

}

GemConfig::~GemConfig(void) {}

bool GemConfig::load(char*filename) {
 

}
#endif
