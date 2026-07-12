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

#include "alpha.h"

CPPEXTERN_NEW_WITH_ONE_ARG(alpha, t_floatarg, A_DEFFLOAT);

/////////////////////////////////////////////////////////
//
// alpha
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
alpha :: alpha(t_floatarg fun)
  : m_alphaState(1),
    m_alphaTest(1),
    m_depthtest(1)
{
  funMess(static_cast<int>(fun));
  m_inlet =  inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"),
                       gensym("function"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
alpha :: ~alpha()
{
  inlet_free(m_inlet);
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void alpha :: render(GemState *)
{
  if (m_alphaState)    {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, m_function);
    if (!m_depthtest) {
      glDepthMask(GL_FALSE);  // turn off depth test for transparent objects
    }

    if (m_alphaTest)            {
      glEnable(GL_ALPHA_TEST);
      glAlphaFunc(GL_GREATER, 0.f);
    }
  }
}

/////////////////////////////////////////////////////////
// postrender
//
/////////////////////////////////////////////////////////
void alpha :: postrender(GemState *)
{
  if (m_alphaState) {
    glDisable(GL_BLEND);
    if (!m_depthtest) {
      glDepthMask(GL_TRUE);
    }

    if (m_alphaTest) {
      glDisable(GL_ALPHA_TEST);
    }
  }

}
/////////////////////////////////////////////////////////
// alphaMess
//
/////////////////////////////////////////////////////////
void alpha :: alphaMess(int alphaState)
{
  m_alphaState = alphaState;
  setModified();
}
/////////////////////////////////////////////////////////
// funMess
//
/////////////////////////////////////////////////////////
void alpha :: funMess(int fun)
{
  switch(fun) {
  case 1:
    m_function=GL_ONE;
    break;
  case 2:
    m_function=GL_ZERO;
    break;
  case 3:
    m_function=GL_SRC_COLOR;
    break;
  case 4:
    m_function=GL_ONE_MINUS_SRC_COLOR;
    break;
  case 5:
    m_function=GL_DST_COLOR;
    break;
  case 6:
    m_function=GL_ONE_MINUS_DST_COLOR;
    break;
  case 7:
    m_function=GL_SRC_ALPHA;
    break;
  case 8:
    m_function=GL_ONE_MINUS_SRC_ALPHA;
    break;
  case 9:
    m_function=GL_DST_ALPHA;
    break;
  case 10:
    m_function=GL_ONE_MINUS_DST_ALPHA;
    break;
  case 11:
    m_function=GL_CONSTANT_COLOR;
    break;
  case 12:
    m_function=GL_ONE_MINUS_CONSTANT_COLOR;
    break;
  case 13:
    m_function=GL_CONSTANT_ALPHA;
    break;
  case 14:
    m_function=GL_ONE_MINUS_CONSTANT_ALPHA;
    break;
  case 15:
    m_function=GL_SRC_ALPHA_SATURATE;
    break;
  case 16:
    m_function=GL_SRC1_COLOR;
    break;
  case 17:
    m_function=GL_ONE_MINUS_SRC1_COLOR;
    break;
  case 18:
    m_function=GL_SRC1_ALPHA;
    break;
  case 19:
    m_function=GL_ONE_MINUS_SRC1_ALPHA;
    break;
  default:
    m_function=GL_ONE_MINUS_SRC_ALPHA;
  }
  setModified();
}
/////////////////////////////////////////////////////////
// testMess
//
/////////////////////////////////////////////////////////
void alpha :: testMess(int alphaTest)
{
  m_alphaTest = alphaTest;
  setModified();
}
/////////////////////////////////////////////////////////
// depthtestMess
//
/////////////////////////////////////////////////////////
void alpha :: depthtestMess(int i)
{
  m_depthtest = i;
  setModified();
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void alpha :: obj_setupCallback(t_class *classPtr)
{
  CPPEXTERN_MSG1(classPtr, "float", alphaMess, int);
  CPPEXTERN_MSG1(classPtr, "test", testMess, int);
  CPPEXTERN_MSG1(classPtr, "function", funMess, int);
  CPPEXTERN_MSG1(classPtr, "auto", depthtestMess, int);
}
