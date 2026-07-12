////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// Implementation file
//
// SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
// SPDX-License-Identifier: GPL-2.0-or-later
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
                               const gem::RTE::Array&array)
{
  linearFunc(x, ret, numDimen, array);
}

void linear_path :: obj_setupCallback(t_class *)
{ }
