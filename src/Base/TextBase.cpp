////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-2000 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "TextBase.h"

#include <stdio.h>
#include <string.h>

char *TextBase::DEFAULT_FONT = "arial.ttf";

/////////////////////////////////////////////////////////
//
// TextBase
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
TextBase :: TextBase(int argc, t_atom *argv)
  : m_valid(0), m_theString(NULL), m_theMaxStringSize(0),
    m_fontSize(20), m_precision(1.f),
    m_widthJus(CENTER), m_heightJus(MIDDLE)
{
  // at least allocate something
  m_theString = new char[16];
  m_theMaxStringSize = 16;
  strcpy(m_theString, "gem");

  // do we have a text message?
  if (argc)
    textMess(argc, argv);

  m_inlet = inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"), gensym("ft1"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
TextBase :: ~TextBase()
{
  inlet_free(m_inlet);
  delete [] m_theString;
}

/////////////////////////////////////////////////////////
// setJustification
//
/////////////////////////////////////////////////////////
void TextBase :: setJustification(JustifyWidth wType, JustifyHeight hType)
{
  m_widthJus = wType;
  m_heightJus = hType;
}

/////////////////////////////////////////////////////////
// textMess
//
/////////////////////////////////////////////////////////
void TextBase :: textMess(int argc, t_atom *argv)
{
  if (argv[0].a_type != A_SYMBOL)
    {
      error("GEM: TextBase: wrong type");
      return;
    }
    
  if ( argc < 1 )
    {
      // we are guarenteed to have some memory here...
      m_theString[0] = 0;
      return;
    }
    
  // do we actually need to reallocate memory?
  int charCount = 0;
  char newtext[MAXPDSTRING];
    	
  int i;
  for (i = 0; i < argc; ++i)
    {
      atom_string(&argv[i], newtext, MAXPDSTRING);
      charCount += strlen(newtext);
      // we need a space between each item
      charCount++;
    }
	
  // yep, need to allocate
  if (m_theMaxStringSize < charCount + 1)
    {
      delete [] m_theString;
      // allocate plus the terminator
      m_theMaxStringSize = charCount + 1;
      m_theString = new char[m_theMaxStringSize];
    }

  // okay, go through the arguments for real and
  //		strcat them together
  int curCount = 0;
  for (i = 0; i < argc; i++)
    {
      atom_string(&argv[i], &m_theString[curCount], m_theMaxStringSize - curCount);
      // add a space
      strcat(m_theString, " ");

      // where is the end of the string?
      curCount = strlen(m_theString);
    }
  setModified();
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void TextBase :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr, (t_method)&TextBase::textMessCallback,
		  gensym("text"), A_GIMME, A_NULL);
  class_addmethod(classPtr, (t_method)&TextBase::precisionMessCallback,
		  gensym("precision"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&TextBase::fontNameMessCallback,
		  gensym("font"), A_SYMBOL, A_NULL);
  class_addmethod(classPtr, (t_method)&TextBase::justifyMessCallback,
		  gensym("justify"), A_SYMBOL, A_SYMBOL, A_NULL);
  class_addmethod(classPtr, (t_method)&TextBase::fontSizeMessCallback,
		  gensym("ft1"), A_FLOAT, A_NULL);
  class_addlist(classPtr, (t_method)&TextBase::textMessCallback);
}
void TextBase :: textMessCallback(void *data, t_symbol *, int argc, t_atom *argv)
{
  GetMyClass(data)->textMess(argc, argv);
}
void TextBase :: fontNameMessCallback(void *data, t_symbol *s)
{
  GetMyClass(data)->fontNameMess(s->s_name);
}
void TextBase :: justifyMessCallback(void *data, t_symbol *width, t_symbol *height)
{
  JustifyWidth wType;
  if ( !strcmp(width->s_name, "left") )
    wType = LEFT;
  else if ( !strcmp(width->s_name, "right") )
    wType = RIGHT;
  else if ( !strcmp(width->s_name, "center") )
    wType = CENTER;
  else
    {
      error("GEM: TextBase: invalid width justification: %s", width->s_name);
      return;
    }

  JustifyHeight hType;
  if ( !strcmp(height->s_name, "bottom") )
    hType = BOTTOM;
  else if ( !strcmp(height->s_name, "top") )
    hType = TOP;
  else if ( !strcmp(height->s_name, "center") )
    hType = MIDDLE;
  else
    {
      error("GEM: TextBase: invalid height justification: %s", height->s_name);
      return;
    }

  GetMyClass(data)->setJustification(wType, hType);
}
void TextBase :: fontSizeMessCallback(void *data, t_floatarg size)
{
  GetMyClass(data)->setFontSize((int)size);
}
void TextBase :: precisionMessCallback(void *data, t_floatarg prec)
{
  GetMyClass(data)->setPrecision((float)prec);
}
