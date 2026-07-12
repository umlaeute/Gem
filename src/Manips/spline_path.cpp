////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// Implementation file
//
// SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
// SPDX-License-Identifier: GPL-2.0-or-later
//
////////////////////////////////////////////////////////

#include "spline_path.h"

#include "Utils/Functions.h"

CPPEXTERN_NEW_WITH_GIMME(spline_path);

spline_path :: spline_path(int argc, t_atom *argv)
  : GemPathBase(argc, argv)
{ }
spline_path :: ~spline_path()
{ }

/////////////////////////////////////////////////////////
// floatMess
//
/////////////////////////////////////////////////////////
void spline_path :: lookupFunc(t_float x, t_float *ret, int numDimen,
                               const gem::RTE::Array&array)
{
  splineFunc(x, ret, numDimen, array);
}

void spline_path :: obj_setupCallback(t_class *)
{ }
