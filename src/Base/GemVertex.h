
/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    include file for VertexArrays

    Copyright (c) 2004 IOhannes m zmoelnig. forum::f�r::uml�ute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/


#ifndef INCLUDE_GEMVERTEX_H_
#define INCLUDE_GEMVERTEX_H_

#include "Base/GemBase.h"

class GEM_EXTERN GemVertex : public GemBase {
 protected:

    	
  //////////
  // Constructor
  GemVertex();

  ~GemVertex();

  //////////
  // If anything in the object has changed
  // especially, if the vertex-array has changed
  virtual void  	setModified();
};


#endif /* INCLUDE_GEMVERTEX_H_ */
