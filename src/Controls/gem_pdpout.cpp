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

#include "gem_pdpout.h"


// this will create a [gem_pdpout] object that does not take any arguments!
CPPEXTERN_NEW_WITH_GIMME(gem_pdpout)

/////////////////////////////////////////////////////////
//
// gem_pdpout
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
gem_pdpout :: gem_pdpout(int argc, t_atom*argv)
{
  m_outPDP = outlet_new(this->x_obj, 0);
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
gem_pdpout :: ~gem_pdpout()
{
  outlet_free(m_outPDP);
}

/////////////////////////////////////////////////////////
// continueRender
//
/////////////////////////////////////////////////////////
void gem_pdpout :: render(GemState *state)
{
  // the state holds all relevant data for Gem
  // it might be NULL !
  /*
    excerpts from Base/GemState.h:
    <GemState.h>
    (float)state->tickTime : ms since last rendering
    (int)state->lighting   : lighting turned on
    (int)state->smooth     : smooth(1) or flat(0) shading
    </GemState.h>

    pixel data within Gem as defined in Base/GemPixUtil.h
    <GemPixUtil.h>
    (GLint)state->image->xsize         : height
    (GLint)state->image->ysize         : width
    (GLint)state->image->csize         : pixel-size (in bytes)
    (GLenum)state->image->type         : GL_UNSIGNED_BYTE || GL_UNSIGNED_SHORT_8_8_REV_APPLE
    (GLenum)state->image->format       : GL_RGBA (rgba), GL_BGRA ("rgba" on osX),
                                         GL_YCBCR_422_GEM (yuv422=uyvy), GL_LUMINANCE (grey)
    (GLboolean)state->image->upsidedown: is image flipped horizontally
    (uchar*)state->image->data         : the pointer to the data
    </GemPixUtil.h>
  */


  // put your code in here...


  t_symbol*s=gensym("ups");
  int packet=-1;

  t_atom ap[2];
  SETSYMBOL(ap, s); // not very brilliant
  SETFLOAT (ap+1, (t_float)packet);        // not brilliant either

  outlet_anything(this->m_outPDP, gensym("pdp"), 2, ap);
}

void gem_pdpout :: postrender(GemState *state){
  // want to do something in here ?
}


/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void gem_pdpout :: obj_setupCallback(t_class *)
{
  class_addcreator((t_newmethod)_classgem_pdpout, gensym("gem_pdp.out"), A_GIMME, A_NULL);
}
