////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.at
//
// Implementation file
//
//    Copyright (c) 1997-2000 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "GemSplash.h"

CPPEXTERN_NEW(GemSplash);

/////////////////////////////////////////////////////////
//
// GemSplash
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GemSplash :: GemSplash(void)
{
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GemSplash :: ~GemSplash(void)
{ }

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void GemSplash :: render(GemState *state)
{
  // this should do something cool.
  // and it should display "Gem" and the version-number
  // probably the core people that were involved
  // should be mentioned too

  // probably we should do a GemSplash contest
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void GemSplash :: obj_setupCallback(t_class *classPtr)
{
  class_addcreator(reinterpret_cast<t_newmethod>(create_GemSplash),
                   gensym("Gem"), A_GIMME, A_NULL);

  /* debugging GL errors */
  struct _CallbackClass_debugGL {
    static void callback(void*data, t_float v0)
    {
      static bool sending = false;
      if(!sending) {
        sending = true;
        t_symbol*s = gensym("__gemBase");
        if(s->s_thing) {
          t_atom a[1];
          SETFLOAT(a+0, v0);
          pd_typedmess(s->s_thing, gensym("debugGL"), 1, a);
        }
      }
      sending = false;

    }
    explicit _CallbackClass_debugGL (struct _class*c)
    {
      class_addmethod(c, reinterpret_cast<t_method>(callback),
                      gensym("debugGL"), A_FLOAT, A_NULL);
    }
  };
  _CallbackClass_debugGL _CallbackClassInstance_debugGL (classPtr);
}
