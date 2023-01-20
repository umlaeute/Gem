////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.at
//
// Implementation file
//
//    Copyright (c) 2008-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
#define GEM_ARGMESSAGES 0

#include "gemargs.h"

#if 0
# define debug_post ::post
#else
# include "Utils/nop.h"
# define debug_post nop_post
#endif



CPPEXTERN_NEW_WITH_ONE_ARG(gemargs, int, A_DEFFLOAT);

/////////////////////////////////////////////////////////
//
// gemargs
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
gemargs :: gemargs(int depth)
  : CPPExtern()
{
  const t_canvas *canvas=getCanvas();
  if(depth<0)depth=0;

#if 0
  while(depth && canvas) {
    canvas=canvas->gl_owner;
    depth--;
  }
#endif

  m_argcanvas = canvas;

  m_msgout = outlet_new(this->x_obj, 0);
  m_argout = outlet_new(this->x_obj, 0);
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
gemargs :: ~gemargs()
{
}


/////////////////////////////////////////////////////////
// receiving
//
/////////////////////////////////////////////////////////
void gemargs :: bang()
{
  t_symbol*semi = gensym(";");
  t_object*obj = (t_object*)m_argcanvas;
  t_binbuf*b = obj->te_binbuf;
  int argc = b?binbuf_getnatom(b):0;
  t_atom*argv = b?binbuf_getvec(b):0;
  int i;

  if(!argc)return;
  argc--;
  argv++;

  struct MsgOut {
    t_outlet*m_out;
    MsgOut(t_outlet*out)
      : m_out(out)
      { }
    void forward(int argc, t_atom*argv) {
      if(argc<1)return;
      if(A_SYMBOL == argv->a_type)
        outlet_anything(m_out, argv->a_w.w_symbol, argc-1, argv+1);
      else
        outlet_list(m_out, gensym("list"), argc, argv);
    }
    static bool isSemi(t_atom*argv, t_symbol*semi) {
      if (A_SEMI == argv->a_type) return true;
      return (A_SYMBOL == argv->a_type) && (semi == argv->a_w.w_symbol);
    }
  };
  MsgOut out(m_msgout);

  for(i=0; i<argc; i++) {
    if (out.isSemi(argv+i, semi)) {
      outlet_list(m_argout, gensym("list"), i, argv);
      int start = i+1;
      for(i=start; i<argc; i++) {
        if (out.isSemi(argv+i, semi)) {
          out.forward(i-start, argv+start);
          start = i = i+1;
        }
      }
      if(i>argc)
        i = argc;
      if(i>start)
        out.forward(i-start, argv+start);
      return;
    }
  }
  outlet_list(m_argout, gensym("list"), argc, argv);
}


/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void gemargs :: obj_setupCallback(t_class *classPtr)
{
  CPPEXTERN_MSG0(classPtr, "bang", bang);
}
