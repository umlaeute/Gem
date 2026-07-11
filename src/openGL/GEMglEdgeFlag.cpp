////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// Implementation file
//
// SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
// SPDX-License-Identifier: GPL-2.0-or-later
//
// this file has been generated...
////////////////////////////////////////////////////////

#include "GEMglEdgeFlag.h"

CPPEXTERN_NEW_WITH_ONE_ARG ( GEMglEdgeFlag, t_floatarg, A_DEFFLOAT);

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglEdgeFlag :: GEMglEdgeFlag  (t_floatarg arg0) :
  flag(static_cast<GLboolean>(arg0))
{
  m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"),
                         gensym("flag"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglEdgeFlag :: ~GEMglEdgeFlag ()
{
  inlet_free(m_inlet[0]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglEdgeFlag :: render(GemState *state)
{
  glEdgeFlag (flag);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglEdgeFlag :: flagMess (t_float arg1)   // FUN
{
  flag = static_cast<GLboolean>(arg1);
  setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglEdgeFlag :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&GEMglEdgeFlag::flagMessCallback),
                  gensym("flag"), A_DEFFLOAT, A_NULL);
};

void GEMglEdgeFlag :: flagMessCallback (void* data, t_float arg0)
{
  GetMyClass(data)->flagMess ( static_cast<t_float>(arg0));
}
