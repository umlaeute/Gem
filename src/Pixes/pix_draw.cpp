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

#include "pix_draw.h"

#include "Base/GemPixUtil.h"

CPPEXTERN_NEW(pix_draw)

/////////////////////////////////////////////////////////
//
// pix_draw
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_draw :: pix_draw()
{ }

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_draw :: ~pix_draw()
{ }

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void pix_draw :: render(GemState *state)
{
    if ( !state->image ) return;

    glRasterPos2i(0, 0);
    // hack to center image at 0,0
    glBitmap(0, 0, 0.f, 0.f, -(state->image->image.xsize)/2.f, 
	    	    	  -(state->image->image.ysize)/2.f, 0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glDrawPixels(state->image->image.xsize,
	     state->image->image.ysize,
		 state->image->image.format,
		 state->image->image.type,
		 state->image->image.data);
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_draw :: obj_setupCallback(t_class *)
{ }
