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
//    Copyright (c) 2001-2014 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    Copyright (c) 2005 Georg Holzmann <grh@mur.at>
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
#ifndef MARK
# define MARK() post("%s:%d\t%s", __FILE__, __LINE__, __FUNCTION__)
#endif
/*
 * FIXXME: check how font handling behaves with multiple contexts
 */

#include "TextBase.h"
#include "Gem/Settings.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
# include <io.h>
# define close _close
typedef unsigned __int8 uint8_t;
#else
# include <unistd.h>
#endif

#include "Utils/GemString.h"

std::string TextBase::DEFAULT_FONT = "vera.ttf";

/////////////////////////////////////////////////////////
//
// TextBase
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
TextBase :: TextBase(int argc, t_atom *argv)
  :
  m_dist(1), m_valid(0), m_fontSize(20), m_fontDepth(20), m_precision(3.f),
  m_widthJus(CENTER), m_heightJus(MIDDLE), m_depthJus(HALFWAY),
  m_inlet(NULL),
  m_infoOut(gem::RTE::Outlet(this)),
  m_font(NULL), m_fontname(NULL)
{
  // initial text
  gem::Settings::get("font.face", DEFAULT_FONT);
  gem::Settings::get("font.size", m_fontSize);


  m_theText.push_back(L"gem");
  makeLineDist();
  if(argc) {
    textMess(argc, argv);
  }

  m_inlet = inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"),
                      gensym("ft1"));
}

void TextBase :: startRendering(void)
{
  if(NULL==m_font) {
    if(m_fontname) {
      fontNameMess(m_fontname->s_name);
    }
  }
}


////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void TextBase :: renderLine(const char*line, float dist)
{
  float x1=0, y1=0, z1=0, x2=0, y2=0, z2=0;
  m_font->BBox(line, x1, y1, z1, x2, y2, z2); // FTGL

  glPushMatrix();
  glNormal3f(0.0, 0.0, 1.0);

  Justification just=justifyFont(x1, y1, z1, x2, y2, z2, dist);
  glScalef(just.scale, just.scale, just.scale);
  glTranslatef(-just.width, -just.height, -just.depth);

  m_font->Render(line);
  glPopMatrix();
}

void TextBase :: renderLine(const wchar_t*line, float dist)
{
  float x1=0, y1=0, z1=0, x2=0, y2=0, z2=0;
  m_font->BBox(line, x1, y1, z1, x2, y2, z2); // FTGL

  glPushMatrix();
  glNormal3f(0.0, 0.0, 1.0);

  Justification just=justifyFont(x1, y1, z1, x2, y2, z2, dist);
  glScalef(just.scale, just.scale, just.scale);
  glTranslatef(-just.width, -just.height, -just.depth);

  m_font->Render(line);
  glPopMatrix();
}

void TextBase :: render(GemState *)
{
  unsigned int i=0;
  if (m_theText.empty() || !m_font) {
    return;
  }
  // step through the lines
  for(i=0; i<m_theText.size(); i++) {
    renderLine(m_theText[i].c_str(),
               m_lineDist[i]*m_fontSize*m_dist*m_precision);
  }
  fontInfo();
}

////////////////////////////////////////////////////////
// setFontSize
//
////////////////////////////////////////////////////////
void TextBase :: setFontSize(float size)
{
  m_fontSize = size;
  setFontSize();
}
////////////////////////////////////////////////////////
// setPrecision
//
////////////////////////////////////////////////////////
void TextBase :: setPrecision(float prec)
{
  if(prec<=0.f) {
    prec=1.f;
  }
  m_precision = 3.*prec;

  setFontSize();
}

////////////////////////////////////////////////////////
// fontNameMess
//
////////////////////////////////////////////////////////
void TextBase :: fontNameMess(const std::string&filename)
{
  m_valid = 0;
  const char *bufptr=NULL;

  if(filename.empty()) {
    error("no font-file specified");
    return;
  }
  std::string fn = findFile(filename);
  bufptr=fn.c_str();

  /* try to open the file */
  FILE*file = fopen(bufptr, "r");
  if (!file) {
    error("cannot find font-file '%s'", bufptr);
    return;
  }
  fclose(file);

  /* now read font */
  m_font=makeFont(bufptr);
  if (NULL==m_font) {
    error("unable to open font '%s'", bufptr);
    return;
  }
  m_fontname=gensym(filename.c_str());

  setFontSize();
  m_font->Depth(m_fontDepth);
  m_font->CharMap(ft_encoding_unicode);

  setModified();
}

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

/////////////////////////////////////////////////////////
// setJustification
//
/////////////////////////////////////////////////////////
void TextBase :: setFontSize()
{
  if (!m_font) {
    return;
  }

  int fs=static_cast<int>(m_fontSize*m_precision);
  if(fs<0) {
    fs=-fs;
  }

  if(!m_font->FaceSize(fs)) {
    error("unable to set fontsize !");
  }

  setModified();
}

/////////////////////////////////////////////////////////
// setJustification
//
/////////////////////////////////////////////////////////
void TextBase :: setJustification(JustifyWidth wType, JustifyHeight hType,
                                  JustifyDepth dType)
{
  m_widthJus = wType;
  m_heightJus = hType;
  m_depthJus = dType;

  makeLineDist();
}
void TextBase :: setJustification(JustifyWidth wType, JustifyHeight hType)
{
  m_widthJus = wType;
  m_heightJus = hType;

  makeLineDist();
}
void TextBase :: setJustification(JustifyWidth wType)
{
  m_widthJus = wType;
}


void TextBase :: getBBox(float&x0,float&y0,float&z0,
                         float&x1,float&y1,float&z1)
{

}
void TextBase :: fontInfo(void)
{
  if(!m_font) {
    return;
  }
  std::vector<gem::any>atoms;
  gem::any value;

  value = m_font->Ascender();
  atoms.clear();
  atoms.push_back(value);
  m_infoOut.send("ascender", atoms);

  value = m_font->Descender();
  atoms.clear();
  atoms.push_back(value);
  m_infoOut.send("descender", atoms);

  value = m_font->LineHeight();
  atoms.clear();
  atoms.push_back(value);
  m_infoOut.send("height", atoms);

  if(!m_theText.empty()) {
    float x0, y0, z0, x1, y1, z1;
    x0=y0=z0=0;
    x1=y1=z1=0;
    getBBox(x0, y0, z0, x1, y1, z1);
    atoms.clear();
    atoms.push_back(x0);
    atoms.push_back(y0);
    atoms.push_back(z0);
    atoms.push_back(x1);
    atoms.push_back(y1);
    atoms.push_back(z1);
    m_infoOut.send("bbox", atoms);
  }
}

TextBase::Justification TextBase :: justifyFont(float x1, float y1,
    float z1,
    float x2, float y2, float z2, float y_offset)
{
  float width  = 0.f;
  float height = 0.f;
  float depth  = 0.f;

  // Get ascender height (= height of the text)
  float ascender = m_font->Ascender();

  switch (m_widthJus) {
  case LEFT:
    width=x1;
    break;
  case RIGHT:
    width=x2-x1;
    break;
  default:
  case CENTER:
    width=x2 / 2.f;
    break;
  case BASEW:
    width=0;
    break;
  }

  //  if (m_heightJus == BOTTOM)     height = y1;
  //  else if (m_heightJus == TOP)   height = y2-y1;
  //  else if (m_heightJus == MIDDLE)height = y2 / 2.f;
  //  else if (m_heightJus == BASEH) height = 0;
  switch(m_heightJus) {
  case BOTTOM:
  case BASEH:
    height = y_offset;
    break;
  case TOP:
    height = ascender + y_offset;
    break;
  default:
  case MIDDLE:
    height = (ascender/2.f) + y_offset;
    break;
  }

  switch(m_depthJus) {
  case FRONT:
    depth = z1;
    break;
  case BACK:
    depth = z2-z1;
    break;
  default:
  case HALFWAY:
    depth = z2 / 2.f;
    break;
  case BASED:
    depth = 0;
    break;
  }
  TextBase::Justification result;
  result.scale=FONT_SCALE/m_precision;
  result.width=width;
  result.height=height;
  result.depth=depth;
  return result;
}


/////////////////////////////////////////////////////////
// textMess
//
/////////////////////////////////////////////////////////
void TextBase :: breakLine(wstring line)
{
  // split the string wherever there is a '\n'
  while(line.length()>0) {
    size_t pos=line.find('\n');

    // if not found, we're done
    if(wstring::npos == pos) {
      break;
    }
    wstring lin=line.substr(0,pos);

    m_theText.push_back(gem::string::getVisualLine(lin));
    line=line.erase(0,pos+1);
  }

  // if there is still a text append it
  if(line.length()) {
    //m_theText.push_back(line);
    m_theText.push_back(gem::string::getVisualLine(line));
  }
  makeLineDist();
  setModified();
}

void TextBase :: textMess(int argc, t_atom *argv)
{
  m_theText.clear();
  if ( argc < 1 ) {
    return;
  }

  wstring line = L"";
  int i=0;

  // convert the atom-list into 1 string
  for (i = 0; i < argc; ++i) {
    string newtext;
    if (A_FLOAT == argv[i].a_type) {
      char str[MAXPDSTRING];
      char*sp=str;

      atom_string(&argv[i], str, MAXPDSTRING);
      while(*sp) {
        unsigned char c=*sp++;
        line+=c;
      }
    } else {
      char*sp=atom_getsymbol(&argv[i])->s_name;
      try {
        std::wstring ws=gem::string::toWstring(sp);
        line+=ws;
      } catch (int i) {
        i=0;
        while(*sp) {
          unsigned char c=*sp++;
          line+=c;
        }
      }
    }
    if(argc-1>i) {
      line += L' ';
    }
  }

  breakLine(line);
}

/////////////////////////////////////////////////////////
// line distance, the offset
// of the individual lines
/////////////////////////////////////////////////////////
void TextBase :: makeLineDist()
{
  unsigned int i=0;
  m_lineDist.clear();
  if (m_heightJus == BOTTOM || m_heightJus == BASEH) {
    // so the offset will be a simple
    // [0 1 2 3 ... n] sequence
    for(i=0; i<m_theText.size(); i++) {
      m_lineDist.push_back(i);
    }
    return;
  }

  if (m_heightJus == TOP) {
    // now in the other direction:
    // [-n ... -2 -1 0]
    signed long j;
    for(j=m_theText.size()-1; j>=0; j--) {
      m_lineDist.push_back(-j);
    }
    return;
  }

  // else:
  // calculate the y offset of each line, so
  // that the text will be centered:
  /*lines    y-offset        calculation
    1:   0                 = 0- 0
    2:   -0.5 0.5          = [0 1]   - 0.5
    3:   -1 0 1            = [0 1 2] - 1
    4:   -1.5 -0.5 0.5 1.5 = [0 1 2 3] - 1.5
    5:   -2 -1 0 1 2       = [0 1 2 3 4] - 2
    ...
  */

  float diff = (m_theText.size()-1)*0.5;
  for(i=0; i<m_theText.size(); i++) {
    m_lineDist.push_back((i-diff));
  }
}


//-- moocow: modified version of "textMess" for float lists
// this can be used with moocow's pd-string external
// available at http://www.ling.uni-potsdam.de/~moocow/projects/pd/#externs
// it works like this:   a string is represented as a list of ASCII-values
/////////////////////////////////////////////////////////
// stringMess
//
/////////////////////////////////////////////////////////
void TextBase :: stringMess(int argc, t_atom *argv)
{
  m_theText.clear();

  if ( argc < 1 ) {
    return;
  }

  int i;
  wstring line = L"";

  for (i = 0; i < argc; i++)    {
    int v=atom_getint(argv+i);
    /*
     * i experienced crashes in FTGL with indices>65535;
     * since TrueType fonts cannot hold more than 65536 entries
     * we just clamp it...
     * note: at least wikipedia claims that TTF can only hold 2^16 glyphs
     *       i have seen ttf-fonts with (seemingly) more
     */
    if(v<0||v>65535) {
      verbose(1, "invalid character %d: using ' ' instead", v);
      v=32;
    }
    line += static_cast<wchar_t>(v);
  }
  //line += L'\0';

  breakLine(line);
}
//-- /moocow

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

void TextBase :: justifyMessCallback(void *data, t_symbol *s, int argc,
                                     t_atom*argv)
{
  JustifyWidth  wType=CENTER;
  JustifyHeight hType=MIDDLE;
  JustifyDepth  dType=HALFWAY;
  char c;

  switch(argc) {
  case 3:
    c=atom_getsymbol(argv+2)->s_name[2];
    switch (c) {
    case 'o':
    case 'O':
      dType = FRONT;
      break;
    case 'c':
    case 'C':
      dType = BACK;
      break;
    case 's':
    case 'S':
      dType = BASED;
      break;
    case 'l':
    case 'L':
    case 'n':
    case 'N':
      dType = HALFWAY;
      break;
    default:
      GetMyClass(
        data)->error("invalid depth justification: %s (must be: front|back|halfway|base)",
                     atom_getsymbol(argv+2)->s_name);
      return;
    }
  case 2:
    c=atom_getsymbol(argv+1)->s_name[2];
    switch (c) {
    case 't':
    case 'T':
      hType = BOTTOM;
      break;
    case 'p':
    case 'P':
      hType = TOP;
      break;
    case 'd':
    case 'D':
    case 'n':
    case 'N':
      hType = MIDDLE;
      break;
    case 's':
    case 'S':
      hType = BASEH;
      break;
    default:
      GetMyClass(
        data)->error("invalid height justification: %s (must be bottom|top|middle|base)",
                     atom_getsymbol(argv+1)->s_name);
      return;
    }
  case 1:
    c=atom_getsymbol(argv)->s_name[2];
    switch (c) {
    case 'f':
    case 'F':
      wType = LEFT;
      break;
    case 'g':
    case 'G':
      wType = RIGHT;
      break;
    case 'n':
    case 'N':
      wType = CENTER;
      break;
    case 's':
    case 'S':
      wType = BASEW;
      break;
    default:
      GetMyClass(
        data)->error("invalid width justification: %s (must be left|right|center|base)",
                     atom_getsymbol(argv+0)->s_name);
      return;
    }
    break;
  default:
    GetMyClass(
      data)->error("justification most be \"width [height [depth]]\"");
    return;
  }

  switch(argc) {
  case 1:
    GetMyClass(data)->setJustification(wType);
    break;
  case 2:
    GetMyClass(data)->setJustification(wType, hType);
    break;
  case 3:
    GetMyClass(data)->setJustification(wType, hType, dType);
    break;
  }
}
void TextBase :: linedistMess(float dist)
{
  m_dist = dist;
  makeLineDist();
}
