////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
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

#include "gemvertexbuffer.h"

#include "Gem/State.h"
#include "Gem/Manager.h"
#include <math.h>
#include <stdio.h>

#if defined (__APPLE__) || defined(MACOSX)
   #define GL_SHARING_EXTENSION "cl_APPLE_gl_sharing"
#else
   #define GL_SHARING_EXTENSION "cl_khr_gl_sharing"
#endif

#define UNIX

CPPEXTERN_NEW_WITH_ONE_ARG(gemvertexbuffer, t_floatarg, A_DEFFLOAT);

gemvertexbuffer :: VertexBuffer:: VertexBuffer (unsigned int size_, unsigned int stride_) :
  size(size_),
  stride(stride_),
  vbo(0),
  array(new float[size*stride]),
  dirty(false),
  enabled(false) {
  //  ::post("created VertexBuffer[%p] with %dx%d elements at %p", this, size, stride, array);
}
gemvertexbuffer :: VertexBuffer:: ~VertexBuffer (void) {
  //  ::post("destroying VertexBuffer[%p] with %dx%d elements at %p", this, size, stride, array);
  destroy();
  if(array)
    delete[]array;
  array=NULL;
}
void gemvertexbuffer :: VertexBuffer:: resize (unsigned int size_) {
  if(array)delete[]array;
  array=new float[size_*stride];
  size=size_;
  dirty=true;
}

bool gemvertexbuffer :: VertexBuffer:: create (void) {
  if(!vbo) {
    glGenBuffers(1, &vbo);
  }
  if(vbo) {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, size * stride * sizeof(float), array, GL_DYNAMIC_DRAW);
  }
  return (0!=vbo);
}
bool gemvertexbuffer :: VertexBuffer:: render (void) {
  // render from the VBO
  if ( enabled ) {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    if ( dirty ) {
      //      ::post("push vertex %p\n", this);
      glBufferData(GL_ARRAY_BUFFER, size * stride * sizeof(float), array, GL_DYNAMIC_DRAW);
      dirty = false;
    }
  }
  return enabled;
}
void gemvertexbuffer :: VertexBuffer:: destroy (void) {
	if ( vbo ){
		glBindBuffer(1, vbo);
		glDeleteBuffers(1, &vbo);
	}
  vbo=0;
}



/////////////////////////////////////////////////////////
//
// gemvertexbuffer
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
gemvertexbuffer :: gemvertexbuffer(t_floatarg size) :
  vbo_size(size>0?size:(256*256)),
	size_change_flag(false),
  m_position(vbo_size,3),
  m_texture (vbo_size,2),
  m_color   (vbo_size,4),
  m_normal  (vbo_size,3)
{
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
gemvertexbuffer :: ~gemvertexbuffer(void)
{
}

/////////////////////////////////////////////////////////
// renderShape
//
/////////////////////////////////////////////////////////
void gemvertexbuffer :: renderShape(GemState *state)
{
	if ( m_drawType == GL_DEFAULT_GEM ) m_drawType = GL_POINTS;
	if ( !m_position.vbo || !m_texture.vbo || !m_color.vbo || !m_normal.vbo || size_change_flag ) {
//		printf("create VBO\n");
		createVBO();
		size_change_flag = false;
	}
  // render from the VBO
  if(m_position.render())
    glVertexPointer(m_position.stride, GL_FLOAT, 0, 0);
  if(m_texture.render())
    glTexCoordPointer(m_texture.stride, GL_FLOAT, 0, 0);
  if(m_color.render())
			glColorPointer(m_color.stride, GL_FLOAT, 0, 0);
  if(m_normal.render())
			glNormalPointer(GL_FLOAT, 0, 0);
		
  if ( m_position.enabled ) glEnableClientState(GL_VERTEX_ARRAY);
  if ( m_color.enabled    ) glEnableClientState(GL_COLOR_ARRAY);
  if ( m_texture.enabled  ) glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  if ( m_normal.enabled   ) glEnableClientState(GL_NORMAL_ARRAY);
		
  glDrawArrays(m_drawType, 0, vbo_size);
		
  if ( m_position.enabled ) glDisableClientState(GL_VERTEX_ARRAY);
  if ( m_color.enabled    ) glDisableClientState(GL_COLOR_ARRAY);
  if ( m_texture.enabled  ) glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  if ( m_normal.enabled   ) glDisableClientState(GL_NORMAL_ARRAY);	
}

/////////////////////////////////////////////////////////
// Setup callback functions
//
/////////////////////////////////////////////////////////
void gemvertexbuffer :: obj_setupCallback(t_class *classPtr)
{
  CPPEXTERN_MSG (classPtr, "posX", posxMess);
  CPPEXTERN_MSG (classPtr, "posY", posyMess);
  CPPEXTERN_MSG (classPtr, "posZ", poszMess);

  CPPEXTERN_MSG (classPtr, "colorR", colrMess);
  CPPEXTERN_MSG (classPtr, "colorG", colgMess);
  CPPEXTERN_MSG (classPtr, "colorB", colbMess);
  CPPEXTERN_MSG (classPtr, "colorA", colaMess);

  CPPEXTERN_MSG (classPtr, "textureU", texuMess);
  CPPEXTERN_MSG (classPtr, "textureV", texvMess);

  CPPEXTERN_MSG (classPtr, "normalX", normxMess);
  CPPEXTERN_MSG (classPtr, "normalY", normyMess);
  CPPEXTERN_MSG (classPtr, "normalZ", normzMess);


  CPPEXTERN_MSG1(classPtr, "resize", resizeMess, unsigned int);

  CPPEXTERN_MSG1(classPtr, "posVBO_enable" , posVBO_enableMess , bool);
  CPPEXTERN_MSG1(classPtr, "colVBO_enable" , colVBO_enableMess , bool);
  CPPEXTERN_MSG1(classPtr, "texVBO_enable" , texVBO_enableMess , bool);
  CPPEXTERN_MSG1(classPtr, "normVBO_enable", normVBO_enableMess, bool);
}

void gemvertexbuffer :: posxMess (t_symbol*s, int argc, t_atom *argv){	tabMess(argc,argv, m_position, 3, 0); }
void gemvertexbuffer :: posyMess (t_symbol*s, int argc, t_atom *argv){	tabMess(argc,argv, m_position, 3, 1); }
void gemvertexbuffer :: poszMess (t_symbol*s, int argc, t_atom *argv){	tabMess(argc,argv, m_position, 3, 2); }

void gemvertexbuffer :: colrMess (t_symbol*s, int argc, t_atom *argv){	tabMess(argc,argv, m_color   , 4, 0); }
void gemvertexbuffer :: colgMess (t_symbol*s, int argc, t_atom *argv){	tabMess(argc,argv, m_color   , 4, 1); }
void gemvertexbuffer :: colbMess (t_symbol*s, int argc, t_atom *argv){	tabMess(argc,argv, m_color   , 4, 2); }
void gemvertexbuffer :: colaMess (t_symbol*s, int argc, t_atom *argv){	tabMess(argc,argv, m_color   , 4, 3); }

void gemvertexbuffer :: texuMess (t_symbol*s, int argc, t_atom *argv){	tabMess(argc,argv, m_texture , 2, 0); }
void gemvertexbuffer :: texvMess (t_symbol*s, int argc, t_atom *argv){	tabMess(argc,argv, m_texture , 2, 1); }

void gemvertexbuffer :: normxMess(t_symbol*s, int argc, t_atom *argv){	tabMess(argc,argv, m_normal  , 3, 0); }
void gemvertexbuffer :: normyMess(t_symbol*s, int argc, t_atom *argv){	tabMess(argc,argv, m_normal  , 3, 1); }
void gemvertexbuffer :: normzMess(t_symbol*s, int argc, t_atom *argv){	tabMess(argc,argv, m_normal  , 3, 2); }

void gemvertexbuffer :: posVBO_enableMess (bool flag){	m_position.enabled = flag; }
void gemvertexbuffer :: colVBO_enableMess (bool flag){	m_color   .enabled = flag; }
void gemvertexbuffer :: texVBO_enableMess (bool flag){	m_texture .enabled = flag; }
void gemvertexbuffer :: normVBO_enableMess(bool flag){	m_normal  .enabled = flag; }

void gemvertexbuffer :: tabMess(int argc, t_atom *argv, VertexBuffer&array, int stride, int offset)
{
	int offset2 = 0;
	t_symbol *tab_name;
	if ( argv[0].a_type != A_SYMBOL )
	{
		error("first arg must be symbol (table name)");
		return;
	}
	if ( argc > 1 )
	{
		if ( argv[1].a_type != A_FLOAT )
		{
			error("second arg must be float (offset)");
		}
		else offset2 = argv[1].a_w.w_float;
	}
	offset2 = offset2<0?0:offset2;
	tab_name = argv[0].a_w.w_symbol;
	copyArray(tab_name, array, stride, offset2 * stride + offset);
  array.enabled=true;
}

void gemvertexbuffer :: resizeMess(unsigned int size)
{
	vbo_size = size>1?size:1;
	//~ printf("cleanup\n");
  m_position.resize(vbo_size);
  m_texture .resize(vbo_size);
  m_color   .resize(vbo_size);
  m_normal  .resize(vbo_size);

	size_change_flag = true;
}

// Create VBO
//*****************************************************************************
void gemvertexbuffer :: createVBO(void)
{
  m_position.create();
  m_texture .create();
  m_color   .create();
  m_normal  .create();
}

void gemvertexbuffer :: copyArray(t_symbol *tab_name, VertexBuffer&vb, unsigned int stride, unsigned int offset)
{
	t_garray *a;
	int npoints, i;
	t_word *vec;
	t_float posx;

  t_float*array=vb.array;
	pd_findbyclass(tab_name, garray_class);
	if (!(a = (t_garray *)pd_findbyclass(tab_name, garray_class)))
		error("%s: no such array", tab_name->s_name);
    else if (!garray_getfloatwords(a, &npoints, &vec))
        error("%s: bad template for tabLink", tab_name->s_name);
	else
    {		
		npoints = npoints>vbo_size?vbo_size:npoints;

		//~ printf("start copying %d values\n",npoints);
		for ( i = 0 ; i < npoints ; i++ )
		{	
			array[offset + i*stride] = vec[i].w_float;
		}
	}
	//~ printf("copy done\n");
}
