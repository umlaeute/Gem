///////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 2009 IOhannes m zmoelnig. forum::für::umläute. IEM @ KUG
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//



#include "GemContextData.h"
#include "GemContext.h"

/* LATER, when we have multiple contexts, this should really be "-1" or such
 */
const int GemContextDataBase::INVALID_CONTEXT=0;

int GemContextDataBase::getCurContext(void) {
  /* this should get an integer-index of the current context from GemContext */
  int id=0;
  id=GemContext::getContextId();
  return id;
}
