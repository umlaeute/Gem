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
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "gemorb.h"

#include <stdio.h>

#include "orb.h"

static int orb_button_list[] = 
{ ORB_BUTTON_A, ORB_BUTTON_B, ORB_BUTTON_C,
  ORB_BUTTON_D, ORB_BUTTON_E, ORB_BUTTON_F,
  ORB_BUTTON_RESET };

CPPEXTERN_NEW_WITH_ONE_ARG(gemorb, t_floatarg, A_FLOAT)

  /////////////////////////////////////////////////////////
//
// gemorb
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
gemorb :: gemorb(t_floatarg portNumber)
  : m_interval(20.f)
{
  // create all of the outputs
  m_outPos = outlet_new(this->x_obj, 0);
  m_outRot = outlet_new(this->x_obj, 0);
  m_outBut = outlet_new(this->x_obj, 0);

  // create the clock
  m_clock = clock_new(this, (t_method)&gemorb::orbTimerCallback);

  int portNum = (int)portNumber;
  char orbname[64];
  sprintf(orbname, "%d", portNum);
  if ((m_orb = orb_open(orbname)) == NULL)
    {
      error("GEM: gemorb: Could not open Orb comm port: %s", orbname);
      return;
    }
  post("GEM: gemorb: Connected to Space Orb comm port: %s", orbname);
	
  setNullRegion(65);
  setInterval(10.f);
  for (int i = 0; i < 7; i++)
    m_but[i] = 0;

  clock_delay(m_clock, m_interval);
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
gemorb :: ~gemorb()
{
  outlet_free(m_outPos);
  outlet_free(m_outRot);
  outlet_free(m_outBut);

  clock_unset(m_clock);
  clock_free(m_clock);

  if (m_orb)
    orb_close(m_orb);
}

/////////////////////////////////////////////////////////
// timer
//
/////////////////////////////////////////////////////////
void gemorb :: timer()
{
  if (!m_orb)
    return;

  int tx, ty, tz, rx, ry, rz, buttons;
 
  if (orb_getstatus(m_orb, &tx, &ty, &tz, &rx, &ry, &rz, &buttons))
    {
      t_atom atom[6];
      int match = 0;
      for (int i = 0; i < 7; i++)
	{
	  if (m_but[i] != (buttons & orb_button_list[i]) )
	    {
	      match = 1;
	      m_but[i] = (buttons & orb_button_list[i]);
	    }
	}

      // send out the button information if there has been a change
      if (match)
	{
	  // is this a reset?
	  if (buttons & ORB_BUTTON_RESET)
	    {
	      post("GEM: gemorb: resetting");
	      reset();
	    }
	  for (int i = 0 ; i < 6; i++)
	    {
	      if (m_but[i])
		SETFLOAT(&atom[i], (t_float)1);
	      else
		SETFLOAT(&atom[i], (t_float)0);
	    }
	  outlet_list(m_outBut, &s_list, 6, atom);
	}

      // send out the rotation information
      SETFLOAT(&atom[0], (t_float)rx);
      SETFLOAT(&atom[1], (t_float)ry);
      SETFLOAT(&atom[2], (t_float)rz);
      outlet_list(m_outRot, &s_list, 3, atom);

      // send out the position information
      SETFLOAT(&atom[0], (t_float)tx);
      SETFLOAT(&atom[1], (t_float)ty);
      SETFLOAT(&atom[2], (t_float)tz);
      outlet_list(m_outPos, &s_list, 3, atom);
    }

  clock_delay(m_clock, m_interval);
}

/////////////////////////////////////////////////////////
// setNullRegion
//
/////////////////////////////////////////////////////////
void gemorb :: setNullRegion(int region)
{
  if (!m_orb)
    return;
  orb_set_nullregion(m_orb, region, region, region, region, region, region);
}

/////////////////////////////////////////////////////////
// setInterval
//
/////////////////////////////////////////////////////////
void gemorb :: setInterval(float interval)
{
  if (interval <= 0.f)
    interval = 20.f;
  m_interval = interval;
}

/////////////////////////////////////////////////////////
// reset
//
/////////////////////////////////////////////////////////
void gemorb :: reset()
{
  orb_init(m_orb);
  setNullRegion(65);
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void gemorb :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr, (t_method)&gemorb::resetCallback,
		  gensym("reset"), A_NULL);
  class_addmethod(classPtr, (t_method)&gemorb::timerIntervalCallback,
		  gensym("timer"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&gemorb::nullRegionCallback,
		  gensym("nullregion"), A_FLOAT, A_NULL);
}
void gemorb :: resetCallback(void *data)
{
  GetMyClass(data)->reset();
}
void gemorb :: orbTimerCallback(void *data)
{
  ((gemorb *)data)->timer();
}
void gemorb :: timerIntervalCallback(void *data, t_floatarg interval)
{
  GetMyClass(data)->setInterval((float)interval);
}
void gemorb :: nullRegionCallback(void *data, t_floatarg region)
{
  GetMyClass(data)->setNullRegion((int)region);
}
