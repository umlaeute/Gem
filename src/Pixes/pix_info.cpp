////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-1998 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    Copyright (c) 2002 James Tittle & Chris Clepper
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "pix_info.h"

CPPEXTERN_NEW(pix_info)

/////////////////////////////////////////////////////////
//
// pix_info
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_info :: pix_info()
{
    m_x = outlet_new(this->x_obj, 0);
    m_y = outlet_new(this->x_obj, 0);
    m_c = outlet_new(this->x_obj, 0);
    m_format = outlet_new(this->x_obj, 0);
    m_data = outlet_new(this->x_obj, 0);
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_info :: ~pix_info()
{
	outlet_free(m_x);
	outlet_free(m_y);
	outlet_free(m_c);
	outlet_free(m_format);
	outlet_free(m_data);
}

/////////////////////////////////////////////////////////
// trigger
//
/////////////////////////////////////////////////////////
void pix_info :: render(GemState *state)
{
  if (!state || !state->image|| !&state->image->image||!state->image->image.data)return;
  // send out the color information
  outlet_float(m_x, (t_float)state->image->image.xsize);
  outlet_float(m_y, (t_float)state->image->image.ysize);
  outlet_float(m_c, (t_float)state->image->image.csize);

  outlet_float(m_format, (t_float)state->image->image.format);
  outlet_pointer(m_data, (t_gpointer*)state->image->image.data);
}
void pix_info :: obj_setupCallback(t_class *classPtr){
}
