////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "gemmouse.h"

#include "Base/GemEvent.h"
#include "Base/GemMan.h"

CPPEXTERN_NEW_WITH_GIMME(gemmouse)

/////////////////////////////////////////////////////////
//
// gemmouse
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
gemmouse :: gemmouse(int argc, t_atom*argv)
{
    m_outXPos = outlet_new(this->x_obj, 0);
    m_outYPos = outlet_new(this->x_obj, 0);
    m_outLBut = outlet_new(this->x_obj, 0);
    m_outMBut = outlet_new(this->x_obj, 0);
    m_outRBut = outlet_new(this->x_obj, 0);

    m_doY=true;

    switch(argc){
    case 0:
      m_scaleX = m_scaleY = 0.f;
      break;
    case 1:
      m_scaleX = m_scaleY = atom_getfloat(argv);
      m_doY =false;
      break;
    default:
      m_scaleX=atom_getfloat(argv);
      m_scaleY=atom_getfloat(argv+1);
    }

    // register event callback
    setMotionCallback(&gemmouse::mouseMotionCallback, this);
    setButtonCallback(&gemmouse::mouseButtonCallback, this);
    setWheelCallback(&gemmouse::mouseWheelCallback, this);
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
gemmouse :: ~gemmouse()
{
    // remove event callback
    removeMotionCallback(&gemmouse::mouseMotionCallback, this);
    removeButtonCallback(&gemmouse::mouseButtonCallback, this);
    removeWheelCallback(&gemmouse::mouseWheelCallback, this);

    outlet_free(m_outXPos);
    outlet_free(m_outYPos);
    outlet_free(m_outLBut);
    outlet_free(m_outMBut);
    outlet_free(m_outRBut);
}


/////////////////////////////////////////////////////////
// mouseMotion
//
/////////////////////////////////////////////////////////
void gemmouse :: mouseMotion(int x, int y, int width, int height)
{
  t_float scalex = (m_scaleX==0.f)?1.f:m_scaleX/width;
  t_float scaley = (m_doY)?((m_scaleY==0.f)?1.f:m_scaleY/height):scalex;

  outlet_float(m_outYPos, (t_float)(y*scaley));
  outlet_float(m_outXPos, (t_float)(x*scalex));
}

/////////////////////////////////////////////////////////
// mouseButton
//
/////////////////////////////////////////////////////////
void gemmouse :: mouseButton(int which, int state, int x, int y)
{
  switch (which){
  case 0: outlet_float(m_outLBut, (t_float)state); break;
  case 1: outlet_float(m_outMBut, (t_float)state); break;
  case 2: outlet_float(m_outRBut, (t_float)state); break;
  }
}

/////////////////////////////////////////////////////////
// mouseButton
//
/////////////////////////////////////////////////////////
void gemmouse :: mouseWheel(int axis, int value)
{
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void gemmouse :: obj_setupCallback(t_class *)
{ }
void gemmouse :: mouseMotionCallback(int x, int y, int w, int h, void *data)
{
  ((gemmouse *)data)->mouseMotion(x, y, w, h);
}
void gemmouse :: mouseButtonCallback(int which, int state, int x, int y, void *data)
{
    ((gemmouse *)data)->mouseButton(which, state, x, y);
}
void gemmouse :: mouseWheelCallback(int axis, int value, void *data)
{
    ((gemmouse *)data)->mouseWheel(axis, value);
}
