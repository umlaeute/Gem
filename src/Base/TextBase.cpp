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
    m_fontSize(20), m_fontDepth(20), m_precision(1.f),
  //  m_precision= 72;
#ifdef FTGL
    m_font(NULL),
#endif
#if defined GLTT || defined FTGL
    m_face(NULL),
#endif    
    m_widthJus(CENTER), m_heightJus(MIDDLE), m_depthJus(HALFWAY)
{
  static bool first_time=true;
  if (first_time){
#ifdef GLTT
    post("Gem has been compiled with GLTT !");
#elif defined FTGL
    post("Gem has been compiled with FTGL !");
#else
    post("Gem has been compiled without FONT-support !");
#endif
    first_time=false;
  }

  // at least allocate something
  m_theString = new char[16];
  m_theMaxStringSize = 16;
  strcpy(m_theString, "gem");
#ifndef MACOSX
  // do we have a text message?
  if (argc)
    textMess(argc, argv);
#endif
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
// setFontSize
//
/////////////////////////////////////////////////////////
void TextBase :: setFontSize(int size)
{
  m_fontSize = size;
#ifdef FTGL
  if (!m_font)return;
  if( ! m_font->FaceSize(m_fontSize) ) {
    error("GEMtext: unable to create font!");
    m_valid=0;
  } else m_valid=1;
  m_font->Depth(m_fontDepth);
#elif defined GLTT
  m_valid = makeFontFromFace();
#endif
  setModified();
}
/////////////////////////////////////////////////////////
// setPrecision
//
/////////////////////////////////////////////////////////
void TextBase :: setPrecision(float prec)
{
  m_precision = prec;
#ifdef FTGL
#elif defined GLTT
  m_valid = makeFontFromFace();
#endif
  setModified();
}

/////////////////////////////////////////////////////////
// fontNameMess
//
/////////////////////////////////////////////////////////
void TextBase :: fontNameMess(const char *filename)
{
  m_valid = 0;
  char buf[MAXPDSTRING];
  canvas_makefilename(getCanvas(), (char *)filename, buf, MAXPDSTRING);
#ifdef FTGL
  if (!m_font)return;
  if( ! m_font->Open(buf, false) ) {
    error("GEMtext: unable to open font: %s", buf);
    return;
  }
  m_valid = 1;

  setFontSize(m_fontSize);
#elif defined GLTT
  delete m_face;
  m_face = new FTFace;

  if( ! m_face->open(buf) ) {
    error("GEMtext: unable to open font: %s", buf);
    return;
  }
  m_valid = makeFontFromFace();
#endif
  setModified();
}
/////////////////////////////////////////////////////////
// setJustification
//
/////////////////////////////////////////////////////////
void TextBase :: setJustification(JustifyWidth wType, JustifyHeight hType, JustifyDepth dType)
{
  m_widthJus = wType;
  m_heightJus = hType;
  m_depthJus = dType;
}
void TextBase :: setJustification(JustifyWidth wType, JustifyHeight hType)
{
  m_widthJus = wType;
  m_heightJus = hType;
}
void TextBase :: setJustification(JustifyWidth wType)
{
  m_widthJus = wType;
}

void TextBase :: justifyFont(float x1, float y1, float z1, float x2, float y2, float z2){
  float width  = 0.f;
  float height = 0.f;
  float depth  = 0.f;

  if (m_widthJus == LEFT)       width = x1;
  else if (m_widthJus == RIGHT) width = x2-x1;
  else if (m_widthJus == CENTER)width = x2 / 2.f;

  if (m_heightJus == BOTTOM)     height = y1;
  else if (m_heightJus == TOP)   height = y2-y1;
  else if (m_heightJus == MIDDLE)height = y2 / 2.f;
    
  if (m_depthJus == FRONT)       depth = z1;
  else if (m_depthJus == BACK)   depth = z2-z1;
  else if (m_depthJus == HALFWAY)depth = z2 / 2.f;

  glScalef(FONT_SCALE, FONT_SCALE, FONT_SCALE);
  glTranslatef(-width, -height, -depth);
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
		  gensym("justify"), A_GIMME, A_NULL);
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
void TextBase :: justifyMessCallback(void *data, t_symbol *s, int argc, t_atom*argv)
{
  JustifyWidth  wType;
  JustifyHeight hType;
  JustifyDepth  dType;
  char c;

  switch(argc){
  case 3:
    c=atom_getsymbol(argv+2)->s_name[2];
    switch (c){
    case 'o': case 'O': dType = FRONT; break;
    case 'c': case 'C': dType = BACK; break;
    case 'l': case 'L': dType = HALFWAY; break;
    default:
      error("GEM: TextBase: invalid depth justification: %s", atom_getsymbol(argv+2)->s_name);
      return;
    }
  case 2:    
    c=atom_getsymbol(argv+1)->s_name[2];
    switch (c){
    case 't': case 'T': hType = BOTTOM; break;
    case 'p': case 'P': hType = TOP; break;
    case 'd': case 'D': hType = MIDDLE; break;
    default:
      error("GEM: TextBase: invalid height justification: %s", atom_getsymbol(argv+1)->s_name);
      return;
    }
  case 1:
    c=atom_getsymbol(argv)->s_name[2];
    switch (c){
    case 'f': case 'F': wType = LEFT; break;
    case 'g': case 'G': wType = RIGHT; break;
    case 'n': case 'N': wType = CENTER; break;
    default:
      error("GEM: TextBase: invalid width justification: %s", atom_getsymbol(argv+0)->s_name);
      return;
    }
    break;
  default:
    error("GEM: TextBase: justification most be \"width [height [depth]]\"");
    return;
  }

  switch(argc){
  case 1: GetMyClass(data)->setJustification(wType); break;
  case 2: GetMyClass(data)->setJustification(wType, hType); break;
  case 3: GetMyClass(data)->setJustification(wType, hType, dType); break;
  }
}
void TextBase :: fontSizeMessCallback(void *data, t_floatarg size)
{
  GetMyClass(data)->setFontSize((int)size);
}
void TextBase :: precisionMessCallback(void *data, t_floatarg prec)
{
  GetMyClass(data)->setPrecision((float)prec);
}
