////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-2000 Mark Danks.
//    Copyright (c) G¸nther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f¸r::uml‰ute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "vertex_tabwrite.h"

#include "Base/GemState.h"

CPPEXTERN_NEW_WITH_GIMME(vertex_tabwrite)

/////////////////////////////////////////////////////////
//
// vertex_tabwrite
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
vertex_tabwrite :: vertex_tabwrite(int argc, t_atom*argv) :
  m_size(0),
  m_VertexArray(NULL),  m_ColorArray(NULL),  m_NormalArray(NULL),  m_TexCoordArray(NULL),
  m_Vtable(NULL), m_Ctable(NULL), m_Ttable(NULL), m_Ntable(NULL)
{
  if(argc)tableMess(argc, argv);
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
vertex_tabwrite :: ~vertex_tabwrite()
{ }

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////

///////////////
// check if array exists and whether it is a floatarray
//
///////////////
static t_float* checkarray(t_symbol *s, int *length)
{
  t_garray *a;
  t_float  *fp;
  *length = 0;

  if (!(a = (t_garray *)pd_findbyclass(s, garray_class)))    {
    if (*s->s_name) error("vertex_tabwrite: %s: no such array", s->s_name);
    fp = 0;
  } else if (!garray_getfloatarray(a, length, &fp))   {
    error("%s: bad template for vertex_tabwrite", s->s_name);
    fp = 0;
  }
  
  if (*length==0){
    error("vertex_tabwrite: table %s is zero-lengthed", s->s_name);
    fp=0;
  }
  return fp;
}

void vertex_tabwrite :: render(GemState *state)
{
  int      length=0;
  int      size=state->VertexArraySize;
  GLfloat*dummy=NULL;
  GLfloat*table=NULL;

  if(m_doit && size){

    if(m_Vtable && state->VertexArray){
      table=checkarray(m_Vtable, &length);
      length/=4;
      if (length>size)length=size;

      if(table && length>0){
	dummy=state->VertexArray;

	int i=length*4;
	while(i--)*table++=*dummy++;
      }
    }
    if(m_Ctable && state->ColorArray && state->HaveColorArray){ // colors
      table=checkarray(m_Ctable, &length);
      length/=4;
      if (length>size)length=size;

      if(table && length>0){
	dummy=state->ColorArray;

	int i=length*4;
	while(i--)*table++=*dummy++;
      }
    }
    if(m_Ntable && state->NormalArray && state->HaveNormalArray){ // normals
      table=checkarray(m_Ntable, &length);
      length/=4;
      if (length>size)length=size;

      if(table && length>0){
	dummy=state->NormalArray;
	int i=length*4;
	while(i--)*table++=*dummy++;
      }
    }
    if(m_Ttable && state->TexCoordArray && state->HaveTexCoordArray){ // texture-coords
      table=checkarray(m_Ttable, &length);
      length/=4;
      if (length>size)length=size;

      if(table && length>0){
	dummy=state->TexCoordArray;

	int i=length*4;
	while(i--)*table++=*dummy++;
      }
    }
  }

  m_doit = false;
}

void vertex_tabwrite :: tableMess(int argc, t_atom*argv){
  t_symbol*Vtable=NULL;
  t_symbol*Ctable=NULL;
  t_symbol*Ntable=NULL;
  t_symbol*Ttable=NULL;

  switch(argc){
  case 4:
    if((argv+3)->a_type!=A_SYMBOL){
      error("vertex_tabwrite: only symbolic table-names are accepted");
      return;
    }
    Ttable=atom_getsymbol(argv+3);
  case 3:
    if((argv+2)->a_type!=A_SYMBOL){
      error("vertex_tabwrite: only symbolic table-names are accepted");
      return;
    }
    Ntable=atom_getsymbol(argv+2);
  case 2:
    if((argv+1)->a_type!=A_SYMBOL){
      error("vertex_tabwrite: only symbolic table-names are accepted");
      return;
    }
    Ctable=atom_getsymbol(argv+1);
  case 1:
    if((argv+0)->a_type!=A_SYMBOL){
      error("vertex_tabwrite: only symbolic table-names are accepted");
      return;
    }
    Vtable=atom_getsymbol(argv+0);
    break;
  default:
    error("vertex_tabwrite: table must have 1, 2, 3 or 4 arguments");
    return;
  }

  m_Vtable=Vtable;
  m_Ctable=Ctable;
  m_Ntable=Ntable;
  m_Ttable=Ttable;
}

void vertex_tabwrite :: bangMess()
{
  m_doit = true;
}
 
/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void vertex_tabwrite :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr, (t_method)&vertex_tabwrite::tableMessCallback,
		  gensym("table"), A_GIMME, A_NULL);

  class_addbang(classPtr, (t_method)&vertex_tabwrite::bangMessCallback);
}

void vertex_tabwrite :: tableMessCallback(void *data, t_symbol*, int argc, t_atom*argv)
{
  GetMyClass(data)->tableMess(argc, argv);
}

void vertex_tabwrite :: bangMessCallback(void *data)
{
  GetMyClass(data)->bangMess();
}


