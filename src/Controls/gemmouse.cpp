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

CPPEXTERN_NEW(gemmouse)

/////////////////////////////////////////////////////////
//
// gemmouse
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
gemmouse :: gemmouse()
{
    m_outXPos = outlet_new(this->x_obj, 0);
    m_outYPos = outlet_new(this->x_obj, 0);
    m_outLBut = outlet_new(this->x_obj, 0);
    m_outMBut = outlet_new(this->x_obj, 0);
    m_outRBut = outlet_new(this->x_obj, 0);

    // register event callback
    setMotionCallback(&gemmouse::mouseMotionCallback, this);
    setButtonCallback(&gemmouse::mouseButtonCallback, this);
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
void gemmouse :: mouseMotion(int x, int y)
{
    outlet_float(m_outYPos, (t_float)y);
    outlet_float(m_outXPos, (t_float)x);
}

/////////////////////////////////////////////////////////
// mouseButton
//
/////////////////////////////////////////////////////////
void gemmouse :: mouseButton(int which, int state, int x, int y)
{
    if (which == 0)
        outlet_float(m_outLBut, (t_float)state);
    else if (which == 1)
        outlet_float(m_outMBut, (t_float)state);
    else if (which == 2)
        outlet_float(m_outRBut, (t_float)state);
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void gemmouse :: obj_setupCallback(t_class *)
{ }
void gemmouse :: mouseMotionCallback(int x, int y, void *data)
{
    ((gemmouse *)data)->mouseMotion(x, y);
}
void gemmouse :: mouseButtonCallback(int which, int state, int x, int y, void *data)
{
    ((gemmouse *)data)->mouseButton(which, state, x, y);
}
