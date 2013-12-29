////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-2000 Mark Danks.
//    Copyright (c) G�nther Geiger.
//    Copyright (c) 2001-2011 IOhannes m zm�lnig. forum::f�r::uml�ute. IEM. zmoelnig@iem.at
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
  size(0),
  stride(stride_),
  vbo(0),
  array(NULL),
  dirty(false),
  enabled(false)
{
  resize(size_);
}
gemvertexbuffer :: VertexBuffer:: ~VertexBuffer (void) {
  //  ::post("destroying VertexBuffer[%p] with %dx%d elements at %p", this, size, stride, array);
  destroy();

  if(array)
    delete[]array;
  array=NULL;
}
void gemvertexbuffer :: VertexBuffer:: resize (unsigned int size_) {
  float*tmp=NULL;
  try {
    tmp=new float[size_*stride];
  } catch (std::bad_alloc& ba)  {
    ::error("vertexbuffer resize failed: %s ", ba.what());
    return;
  }
  if(array)delete[]array;
  array=tmp;
  size=size_;

  unsigned int i;
  for(i=0; i<size*stride; i++) {
    array[i]=0;
  }

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
  m_draw_start_id(0),
  m_draw_end_id(vbo_size),
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
  if(m_position.render()) {
    glVertexPointer(m_position.stride, GL_FLOAT, 0, 0);
    glEnableClientState(GL_VERTEX_ARRAY);
  }
  if(m_texture.render()) {
    glTexCoordPointer(m_texture.stride, GL_FLOAT, 0, 0);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  }
  if(m_color.render()) {
    glColorPointer(m_color.stride, GL_FLOAT, 0, 0);
    glEnableClientState(GL_COLOR_ARRAY);
  }
  if(m_normal.render()) {
    glNormalPointer(GL_FLOAT, 0, 0);
    glEnableClientState(GL_NORMAL_ARRAY);
  }
		
  glDrawArrays(m_drawType, m_draw_start_id, m_draw_end_id-m_draw_start_id);
		
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
  CPPEXTERN_MSG (classPtr, "position", positionMess);
  CPPEXTERN_MSG (classPtr, "posX", posxMess);
  CPPEXTERN_MSG (classPtr, "posY", posyMess);
  CPPEXTERN_MSG (classPtr, "posZ", poszMess);

  CPPEXTERN_MSG (classPtr, "color", colorMess);
  CPPEXTERN_MSG (classPtr, "colorR", colrMess);
  CPPEXTERN_MSG (classPtr, "colorG", colgMess);
  CPPEXTERN_MSG (classPtr, "colorB", colbMess);
  CPPEXTERN_MSG (classPtr, "colorA", colaMess);

  CPPEXTERN_MSG (classPtr, "texture", textureMess);
  CPPEXTERN_MSG (classPtr, "textureU", texuMess);
  CPPEXTERN_MSG (classPtr, "textureV", texvMess);

  CPPEXTERN_MSG (classPtr, "normal", normalMess);
  CPPEXTERN_MSG (classPtr, "normalX", normxMess);
  CPPEXTERN_MSG (classPtr, "normalY", normyMess);
  CPPEXTERN_MSG (classPtr, "normalZ", normzMess);

  CPPEXTERN_MSG1(classPtr, "resize", resizeMess, unsigned int);
  CPPEXTERN_MSG2(classPtr, "partial_draw", partialDrawMess, unsigned int, unsigned int);

  CPPEXTERN_MSG (classPtr, "enable", enableMess);
  CPPEXTERN_MSG (classPtr, "disable", disableMess);

  CPPEXTERN_MSG1(classPtr, "position_enable", posVBO_enableMess , bool);
  CPPEXTERN_MSG1(classPtr, "color_enable"   , colVBO_enableMess , bool);
  CPPEXTERN_MSG1(classPtr, "texture_enable" , texVBO_enableMess , bool);
  CPPEXTERN_MSG1(classPtr, "normal_enable"  , normVBO_enableMess, bool);

  /* legacy */
  CPPEXTERN_MSG1(classPtr, "posVBO_enable" , posVBO_enableMess , bool);
  CPPEXTERN_MSG1(classPtr, "colVBO_enable" , colVBO_enableMess , bool);
  CPPEXTERN_MSG1(classPtr, "texVBO_enable" , texVBO_enableMess , bool);
  CPPEXTERN_MSG1(classPtr, "normVBO_enable", normVBO_enableMess, bool);
}

void gemvertexbuffer :: tableMess (VertexBuffer&vb, std::string name, int argc, t_atom *argv){
  int offset=0;
  std::string tabname;
  unsigned int i;

  /*
   * it's either interleaved data (1 tablename [+ offset])
   * or separate data (<stride> tablenames [+ offset])
   */

  if(argc==1 || argc==2) {
    if(argc>1) {
      if(A_FLOAT==argv[1].a_type)
        offset=atom_getfloat(argv+1);
      else
        goto failed;
    }
    if(A_SYMBOL==argv[0].a_type)
      tabname=std::string(atom_getsymbol(argv)->s_name);
    else
      goto failed;

    copyArray(tabname, vb, 1, offset*vb.stride);

  } else if (argc == vb.stride || argc == (vb.stride+1)) {
    if(((unsigned int)argc)>vb.stride) {
      if(A_FLOAT==argv[vb.stride].a_type)
        offset=atom_getfloat(argv+vb.stride);
      else
        goto failed;
    }
    for(i=0; i<vb.stride; i++) {
      if(A_SYMBOL!=argv[i].a_type) goto failed;
    }
    for(i=0; i<vb.stride; i++) {
      tabname=std::string(atom_getsymbol(argv+i)->s_name);
      copyArray(tabname, vb, vb.stride, offset*vb.stride+i);
    }
  } else {
      goto failed;
  }

  vb.enabled=true;
  return;

  failed:
  error("illegal arguments to '%s': must be <table[1||%d]> [<offset>]", name.c_str(), vb.stride);
  return;
}
void gemvertexbuffer :: positionMess (t_symbol*s, int argc, t_atom *argv){
  tableMess(m_position, s->s_name, argc, argv);
}
void gemvertexbuffer :: textureMess (t_symbol*s, int argc, t_atom *argv){
  tableMess(m_texture, s->s_name, argc, argv);
}
void gemvertexbuffer :: colorMess (t_symbol*s, int argc, t_atom *argv){
  tableMess(m_color, s->s_name, argc, argv);
}
void gemvertexbuffer :: normalMess (t_symbol*s, int argc, t_atom *argv){
  tableMess(m_normal, s->s_name, argc, argv);
}


void gemvertexbuffer :: posxMess (t_symbol*s, int argc, t_atom *argv){	tabMess(argc,argv, m_position, 0); }
void gemvertexbuffer :: posyMess (t_symbol*s, int argc, t_atom *argv){	tabMess(argc,argv, m_position, 1); }
void gemvertexbuffer :: poszMess (t_symbol*s, int argc, t_atom *argv){	tabMess(argc,argv, m_position, 2); }

void gemvertexbuffer :: colrMess (t_symbol*s, int argc, t_atom *argv){	tabMess(argc,argv, m_color   , 0); }
void gemvertexbuffer :: colgMess (t_symbol*s, int argc, t_atom *argv){	tabMess(argc,argv, m_color   , 1); }
void gemvertexbuffer :: colbMess (t_symbol*s, int argc, t_atom *argv){	tabMess(argc,argv, m_color   , 2); }
void gemvertexbuffer :: colaMess (t_symbol*s, int argc, t_atom *argv){	tabMess(argc,argv, m_color   , 3); }

void gemvertexbuffer :: texuMess (t_symbol*s, int argc, t_atom *argv){	tabMess(argc,argv, m_texture , 0); }
void gemvertexbuffer :: texvMess (t_symbol*s, int argc, t_atom *argv){	tabMess(argc,argv, m_texture , 1); }

void gemvertexbuffer :: normxMess(t_symbol*s, int argc, t_atom *argv){	tabMess(argc,argv, m_normal  , 0); }
void gemvertexbuffer :: normyMess(t_symbol*s, int argc, t_atom *argv){	tabMess(argc,argv, m_normal  , 1); }
void gemvertexbuffer :: normzMess(t_symbol*s, int argc, t_atom *argv){	tabMess(argc,argv, m_normal  , 2); }

void gemvertexbuffer :: posVBO_enableMess (bool flag){	m_position.enabled = flag; }
void gemvertexbuffer :: colVBO_enableMess (bool flag){	m_color   .enabled = flag; }
void gemvertexbuffer :: texVBO_enableMess (bool flag){	m_texture .enabled = flag; }
void gemvertexbuffer :: normVBO_enableMess(bool flag){	m_normal  .enabled = flag; }

void gemvertexbuffer :: enableMess (t_symbol*s, int argc, t_atom *argv){
  int i;
  for(i=0; i<argc; i++) {
    if(0) {;}
    else if("position"==std::string(atom_getsymbol(argv+i)->s_name))m_position.enabled=true;
    else if("color"   ==std::string(atom_getsymbol(argv+i)->s_name))m_color   .enabled=true;
    else if("texture" ==std::string(atom_getsymbol(argv+i)->s_name))m_texture .enabled=true;
    else if("normal"  ==std::string(atom_getsymbol(argv+i)->s_name))m_normal  .enabled=true;
    else {
      error("enable: illegal argument#%d: must be 'position', 'color', 'texture' or 'normal'", i);
    }
  }
}
void gemvertexbuffer :: disableMess (t_symbol*s, int argc, t_atom *argv){
  int i;
  for(i=0; i<argc; i++) {
    if(0) {;}
    else if("position"==std::string(atom_getsymbol(argv+i)->s_name))m_position.enabled=false;
    else if("color"   ==std::string(atom_getsymbol(argv+i)->s_name))m_color   .enabled=false;
    else if("texture" ==std::string(atom_getsymbol(argv+i)->s_name))m_texture .enabled=false;
    else if("normal"  ==std::string(atom_getsymbol(argv+i)->s_name))m_normal  .enabled=false;
    else {
      error("disable: illegal argument#%d: must be 'position', 'color', 'texture' or 'normal'", i);
    }
  }
}



void gemvertexbuffer :: tabMess(int argc, t_atom *argv, VertexBuffer&array, int offset)
{
	int offset2 = 0;
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
  std::string tab_name = atom_getsymbol(argv)->s_name;
	copyArray(tab_name, array, array.stride, offset2 * array.stride + offset);
  array.enabled=true;
}

void gemvertexbuffer :: resizeMess(unsigned int size)
{
	vbo_size = size>1?size:1;
  m_draw_end_id=vbo_size;
	//~ printf("cleanup\n");
  m_position.resize(vbo_size);
  m_texture .resize(vbo_size);
  m_color   .resize(vbo_size);
  m_normal  .resize(vbo_size);

	size_change_flag = true;
}

void gemvertexbuffer :: partialDrawMess(unsigned int start, unsigned int end) { m_draw_start_id = start; m_draw_end_id = end;}

// Create VBO
//*****************************************************************************
void gemvertexbuffer :: createVBO(void)
{
  m_position.create();
  m_texture .create();
  m_color   .create();
  m_normal  .create();
}

void gemvertexbuffer :: copyArray(const std::string&tab_name, VertexBuffer&vb, unsigned int stride, unsigned int offset)
{
	t_garray *a;
	int npoints, i;
	t_word *vec;

  float*array=vb.array;
  t_symbol*s=gensym(tab_name.c_str());
	pd_findbyclass(s, garray_class);
	if (!(a = (t_garray *)pd_findbyclass(s, garray_class)))
		error("%s: no such array", tab_name.c_str());
  else if (!garray_getfloatwords(a, &npoints, &vec))
    error("%s: bad template for tabLink", tab_name.c_str());
	else {
    if(((unsigned int)npoints)>vb.size)
      npoints=vb.size;

		//~ printf("start copying %d values\n",npoints);
		for ( i = 0 ; i < npoints ; i++ )	{
			array[offset + i*stride] = vec[i].w_float;
		}
    vb.dirty=true;
	}
	//~ printf("copy done\n");
}
