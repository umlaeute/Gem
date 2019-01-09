////////////////////////////////////////////////////////
//
// GEMi - Graphics Environment for Multimedia
//
// zmoelnig@iem.at
//
// Implementation file
//
//    Copyright (c) 1997-1998 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "pix_zoom.h"
#include "Gem/GemGL.h"
#include "Gem/Exception.h"

CPPEXTERN_NEW_WITH_GIMME(pix_zoom);

/////////////////////////////////////////////////////////
//
// pix_zoom
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_zoom :: pix_zoom(int argc, t_atom*argv)
{
  t_float x = 1., y=1.;
  switch(argc) {
  case 0:
    break;
  case 1:
    x = y = atom_getfloat(argv);
    break;
  case 2:
    x = atom_getfloat(argv+0);
    y = atom_getfloat(argv+1);
    break;
  default:
    throw(GemException("needs 0, 1 or 2 arguments"));
  }

  zoomMess(x, y);

  // create the new inlet
  inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("list"),
            gensym("zoom"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_zoom :: ~pix_zoom()
{ }

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void pix_zoom :: render(GemState *)
{
  glPixelZoom(m_xZoom, m_yZoom);
}

/////////////////////////////////////////////////////////
// postrender
//
/////////////////////////////////////////////////////////
void pix_zoom :: postrender(GemState *)
{
  glPixelZoom(1.0, 1.0);
}

/////////////////////////////////////////////////////////
// zoom_magMess
//
/////////////////////////////////////////////////////////
void pix_zoom :: zoomMess(t_float xZoom, t_float yZoom)
{
  m_xZoom = xZoom;
  m_yZoom = yZoom;
  setModified();
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_zoom :: obj_setupCallback(t_class *classPtr)
{
  CPPEXTERN_MSG2(classPtr, "zoom", zoomMess, t_float, t_float);
}
