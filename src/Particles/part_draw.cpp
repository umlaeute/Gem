////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
//    Copyright (c) 1997-2000 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "part_draw.h"

#include <string.h>
#include "Base/GemState.h"

#include "papi.h"

CPPEXTERN_NEW(part_draw)

/////////////////////////////////////////////////////////
//
// part_draw
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
part_draw :: part_draw()
		   : m_drawType(GL_LINES)
{ }

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
part_draw :: ~part_draw()
{ }

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void part_draw :: render(GemState *state)
{
	if (state->lighting)
	{
		glDisable(GL_LIGHTING);
	}
	if (state->doUpdate)
	{
		pMove();
	}
	pDrawGroupp(m_drawType);
	if (state->lighting)
	{
		glEnable(GL_LIGHTING);
	}
}

/////////////////////////////////////////////////////////
// typeMess
//
/////////////////////////////////////////////////////////
void part_draw :: typeMess(int ac,t_atom* av)
{

  m_drawType = (int)atom_getfloatarg(0,ac,av);

  post("type set to %d",m_drawType);

  if (0)
    {
	    error("GEM: part_draw: draw style");
    }
}


/////////////////////////////////////////////////////////
// static member functions
//
/////////////////////////////////////////////////////////
void part_draw :: obj_setupCallback(t_class *classPtr)
{
    class_addmethod(classPtr, (t_method)&part_draw::typeMessCallback,
    	    gensym("draw"), A_GIMME, A_NULL);
}
void part_draw :: typeMessCallback(void *data,  t_symbol *s, int ac,t_atom* av)
{
    GetMyClass(data)->typeMess(ac,av);
}


