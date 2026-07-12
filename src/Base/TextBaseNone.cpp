////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// Implementation file
//
// SPDX-FileCopyrightText: © 2014, IOhannes m zmölnig and the GEM contributors
// SPDX-License-Identifier: GPL-2.0-or-later
//
////////////////////////////////////////////////////////

/*
 * FIXXME: check how font handling behaves with multiple contexts
 */

#include "TextBase.h"
#ifndef FTGL

TextBase :: TextBase(int argc, t_atom *argv) :
  m_dist(1), m_valid(0), m_fontSize(20), m_fontDepth(20), m_precision(3.f),
  m_widthJus(CENTER), m_heightJus(MIDDLE), m_depthJus(HALFWAY),
  m_inlet(NULL),
  m_infoOut(gem::RTE::Outlet(this))
{
  static bool first_time=true;
  if (first_time) {
    post("Gem has been compiled without FONT-support !");
    first_time=false;
  }
  m_inlet = inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"),
                      gensym("ft1"));
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void TextBase :: render(GemState*)
{/* a no-op */ }

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
TextBase :: ~TextBase()
{
  /* textbase deletion */
  if(m_inlet) {
    inlet_free(m_inlet);
  }
}

void TextBase :: textMess(int argc, t_atom *argv)
{ }
void TextBase :: stringMess(int argc, t_atom *argv)
{ }
void TextBase :: fontNameMess(const std::string&s)
{ }
void TextBase :: setPrecision(float prec)
{ }
void TextBase :: linedistMess(float dist)
{ }
void TextBase :: justifyMessCallback(void *data, t_symbol* s, int argc,
                                     t_atom*argv)
{ }


void TextBase::breakLine(wstring)
{ }
void TextBase::setFontSize(float)
{ }
void TextBase::setFontSize(void)
{ }
void TextBase::setJustification(JustifyWidth)
{ }
void TextBase::setJustification(JustifyWidth, JustifyHeight)
{ }
void TextBase::setJustification(JustifyWidth, JustifyHeight, JustifyDepth)
{ }
TextBase::Justification TextBase::justifyFont(float x1, float, float,
    float, float, float, float yo)
{
  TextBase::Justification result;
  result.scale =1;
  result.width =x1;
  result.height=yo;
  result.depth =0;
  return result;
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void TextBase :: obj_setupCallback(t_class *classPtr)
{
  CPPEXTERN_MSG(classPtr, "list", textMess);
  CPPEXTERN_MSG(classPtr, "text", textMess);

  //-- moocow
  CPPEXTERN_MSG(classPtr, "string", stringMess);
  //-- /moocow

  CPPEXTERN_MSG1(classPtr, "font", fontNameMess, std::string);
  CPPEXTERN_MSG1(classPtr, "ft1", setFontSize, float);
  CPPEXTERN_MSG1(classPtr, "precision", setPrecision, float);
  CPPEXTERN_MSG1(classPtr, "linedist", linedistMess, float);

  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&TextBase::justifyMessCallback),
                  gensym("justify"), A_GIMME, A_NULL);
}
#endif
