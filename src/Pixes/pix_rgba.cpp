////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// Implementation file
//
// SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
// SPDX-License-Identifier: GPL-2.0-or-later
//
////////////////////////////////////////////////////////

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
  m_image.setFormat(GEM_RGBA);
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

  m_image.setFormat();
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
  image.not_owned = true;
  image.setFormat(m_image.format);
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_rgba :: obj_setupCallback(t_class *classPtr)
{
}
