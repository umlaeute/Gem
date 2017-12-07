////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2014 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "linear_path.h"

#include "Utils/Functions.h"

CPPEXTERN_NEW_WITH_GIMME(linear_path);

linear_path :: linear_path(int argc, t_atom *argv)
  : GemPathBase(argc, argv)
{ }
linear_path :: ~linear_path()
{ }

/////////////////////////////////////////////////////////
// floatMess
//
/////////////////////////////////////////////////////////
void linear_path :: lookupFunc(t_float x, t_float *ret, int numDimen,
                               int npnts, t_float *pnts)
{
  linearFunc(x, ret, numDimen, npnts, pnts);
}

void linear_path :: obj_setupCallback(t_class *)
{ }
