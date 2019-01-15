////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.at
//
// Implementation file
//
//    Copyright (c) 1997-2000 Mark Danks.
//    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    Copyright (c) 2002 James Tittle & Chris Clepper
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
// this is based on EffecTV by Fukuchi Kentarou
// * Copyright (C) 2001 FUKUCHI Kentarou
//
/////////////////////////////////////////////////////////

#include "pix_convert.h"

CPPEXTERN_NEW_WITH_ONE_ARG(pix_convert, t_symbol*, A_DEFSYM);

/////////////////////////////////////////////////////////
//
// pix_convert
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_convert :: pix_convert(t_symbol*s)
{
  m_image.xsize=128;
  m_image.ysize=128;
  int fmt = getPixFormat(s->s_name);
  m_image.setCsizeByFormat(fmt?fmt:GEM_RGBA);

  m_image.reallocate();
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_convert :: ~pix_convert()
{
}

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void pix_convert :: processImage(imageStruct &image)
{
  if (image.format==m_image.format) {
    return;
  }
  if(!m_image.convertFrom(&image)) {
    post("no method for this format !!!");
    post("if you know how to convert this format (0x%X) to (0x%X),\n"
         "please contact the authors of this software",
         image.format, m_image.format);
    return;
  }
  image.data = m_image.data;
  image.notowned = true;
  image.setCsizeByFormat(m_image.format);

}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_convert :: obj_setupCallback(t_class *classPtr)
{
  CPPEXTERN_MSG1(classPtr, "color", colorMess, t_symbol*);
}
void pix_convert :: colorMess(t_symbol*s)
{
  int fo = getPixFormat(s->s_name);
  if(fo) {
    m_image.setCsizeByFormat(fo);
  }
  setPixModified();
}
