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

#include "gemtablet.h"

#include "Base/GemEvent.h"

CPPEXTERN_NEW(gemtablet)

/////////////////////////////////////////////////////////
//
// gemtablet
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
gemtablet :: gemtablet()
{
    m_outXPos = outlet_new(this->x_obj, 0);
    m_outYPos = outlet_new(this->x_obj, 0);
    m_outPress = outlet_new(this->x_obj, 0);
    m_outAz   = outlet_new(this->x_obj, 0);
    m_outAlt  = outlet_new(this->x_obj, 0);
    m_outTwist = outlet_new(this->x_obj, 0);
    m_outLBut = outlet_new(this->x_obj, 0);
    m_outMBut = outlet_new(this->x_obj, 0);
    m_outRBut = outlet_new(this->x_obj, 0);

    // register event callback
    setTabletMotionCallback(&gemtablet::tabletMotionCallback, this);
    setTabletRotationCallback(&gemtablet::tabletRotationCallback, this);
    setTabletButtonCallback(&gemtablet::tabletButtonCallback, this);
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
gemtablet :: ~gemtablet()
{
    // remove event callback
    removeTabletMotionCallback(&gemtablet::tabletMotionCallback, this);
    removeTabletRotationCallback(&gemtablet::tabletRotationCallback, this);
    removeTabletButtonCallback(&gemtablet::tabletButtonCallback, this);

    outlet_free(m_outXPos);
    outlet_free(m_outYPos);
    outlet_free(m_outPress);
    outlet_free(m_outAz);
    outlet_free(m_outAlt);
    outlet_free(m_outTwist);
    outlet_free(m_outLBut);
    outlet_free(m_outMBut);
    outlet_free(m_outRBut);
}

/////////////////////////////////////////////////////////
// tabletMotion
//
/////////////////////////////////////////////////////////
void gemtablet :: tabletMotion(int x, int y, float pressure)
{
    outlet_float(m_outPress, (t_float)pressure);
    outlet_float(m_outYPos, (t_float)y);
    outlet_float(m_outXPos, (t_float)x);
}

/////////////////////////////////////////////////////////
// tabletRotation
//
/////////////////////////////////////////////////////////
void gemtablet :: tabletRotation(int azimuth, int altitude, int twist)
{
    outlet_float(m_outTwist, (t_float)twist);
    outlet_float(m_outAlt, (t_float)altitude);
    outlet_float(m_outAz, (t_float)azimuth);
}

/////////////////////////////////////////////////////////
// tabletButton
//
/////////////////////////////////////////////////////////
void gemtablet :: tabletButton(int which, int state, int x, int y)
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
void gemtablet :: obj_setupCallback(t_class *)
{ }
void gemtablet :: tabletMotionCallback(int x, int y, float pressure, void *data)
{
    ((gemtablet *)data)->tabletMotion(x, y, pressure);
}
void gemtablet :: tabletRotationCallback(int az, int alt, int twist, void *data)
{
    ((gemtablet *)data)->tabletRotation(az, alt, twist);
}
void gemtablet :: tabletButtonCallback(int which, int state, int x, int y, void *data)
{
    ((gemtablet *)data)->tabletButton(which, state, x, y);
}
