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

#include "pix_rgba.h"

CPPEXTERN_NEW(pix_rgba);

/////////////////////////////////////////////////////////
//
// pix_rgba
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_rgba :: pix_rgba()
{
  m_image.setCsizeByFormat(GEM_RGBA);
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_rgba :: ~pix_rgba()
{}

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void pix_rgba :: processImage(imageStruct &image)
{
  // note: [pix_yuv] and [pix_grey] inherit this function from [pix_rgba]
  // thus you shouldn't undefine anything below for performance reasons

  if (image.format==m_image.format) {
    return;  // we don't need to convert as we are already there
  }

  if(!m_image.convertFrom(&image)) {
    error("no method for this format !!!");
    error("if you know how to convert this format (%X),\n"
          "please contact the authors of this software", image.format);
    return;
  }

  image.data   = m_image.data;
  image.notowned = true;
  image.setCsizeByFormat(m_image.format);
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_rgba :: obj_setupCallback(t_class *classPtr)
{
}
