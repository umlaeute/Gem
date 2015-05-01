////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 2011 Cyrille Henry
//    Copyright (c) 2014 Antoine Villeret
//    Copyright (c) 2014 jprtzk
//    Copyright (c) 2012-2015 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "gemvertexbuffer.h"

#include "Gem/State.h"
#include "Gem/Manager.h"
#include "Utils/Functions.h"

#ifdef _MSC_VER
# ifdef min
#   undef min
# endif
# ifdef max
#  undef max
# endif
#endif

CPPEXTERN_NEW_WITH_ONE_ARG(gemvertexbuffer, t_floatarg, A_DEFFLOAT);

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
  glsl_program(0),
  m_position(vbo_size,3),
  m_texture (vbo_size,2),
  m_color   (vbo_size,4),
  m_normal  (vbo_size,3),
  m_idmapper("glsl.program")
{
  m_range[0]=0;
  m_range[1]=-1;
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
  if ( m_drawType == GL_DEFAULT_GEM ) {
    m_drawType = GL_POINTS;
  }
  int attrib_vbos = 1;
  for(unsigned int i=0; i<m_attribute.size(); i++) {
    if(!m_attribute[i].vbo) {
      attrib_vbos = 0;
      break;
    }
  }
  if ( !m_position.vbo || !m_texture.vbo || !m_color.vbo || !m_normal.vbo
       || !attrib_vbos || size_change_flag ) {
    createVBO();
    size_change_flag = false;
  }
  // render from the VBO
  for(unsigned int i=0; i<m_attribute.size(); i++) {
    if(m_attribute[i].render()) {
      glEnableVertexAttribArray(m_attribute[i].attrib_index);
      glVertexAttribPointer(m_attribute[i].attrib_index, m_attribute[i].dimen,
                            GL_FLOAT, GL_FALSE, 0, 0);
    }
  }
  if(m_position.render()) {
    glVertexPointer(m_position.dimen, GL_FLOAT, 0, 0);
    glEnableClientState(GL_VERTEX_ARRAY);
  }
  if(m_texture.render()) {
    glTexCoordPointer(m_texture.dimen, GL_FLOAT, 0, 0);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  }
  if(m_color.render()) {
    glColorPointer(m_color.dimen, GL_FLOAT, 0, 0);
    glEnableClientState(GL_COLOR_ARRAY);
  }
  if(m_normal.render()) {
    glNormalPointer(GL_FLOAT, 0, 0);
    glEnableClientState(GL_NORMAL_ARRAY);
  }

  unsigned int start = std::min(m_range[0], m_range[1]);
  unsigned int end   = std::max(m_range[0], m_range[1]);

  if(start>=vbo_size) {
    start=vbo_size;
  }
  if(end  >=vbo_size) {
    end  =vbo_size;
  }

  if (start == end && 0 == start) {
    start=0;
    end=vbo_size;
  }

  glDrawArrays(m_drawType, start, end-start);

  for(unsigned int i=0; i<m_attribute.size(); i++) {
    if ( m_attribute[i].enabled ) {
      glDisableVertexAttribArray(m_attribute[i].attrib_index);
    }
  }
  if ( m_position.enabled ) {
    glDisableClientState(GL_VERTEX_ARRAY);
  }
  if ( m_color.enabled    ) {
    glDisableClientState(GL_COLOR_ARRAY);
  }
  if ( m_texture.enabled  ) {
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  }
  if ( m_normal.enabled   ) {
    glDisableClientState(GL_NORMAL_ARRAY);
  }
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

  CPPEXTERN_MSG (classPtr, "enable", enableMess);
  CPPEXTERN_MSG (classPtr, "disable", disableMess);
  CPPEXTERN_MSG2(classPtr, "draw_range", partialDrawMess, unsigned int,
                 unsigned int);

  CPPEXTERN_MSG1(classPtr, "position_enable", posVBO_enableMess , bool);
  CPPEXTERN_MSG1(classPtr, "color_enable"   , colVBO_enableMess , bool);
  CPPEXTERN_MSG1(classPtr, "texture_enable" , texVBO_enableMess , bool);
  CPPEXTERN_MSG1(classPtr, "normal_enable"  , normVBO_enableMess, bool);

  /* legacy */
  CPPEXTERN_MSG1(classPtr, "posVBO_enable" , posVBO_enableMess , bool);
  CPPEXTERN_MSG1(classPtr, "colVBO_enable" , colVBO_enableMess , bool);
  CPPEXTERN_MSG1(classPtr, "texVBO_enable" , texVBO_enableMess , bool);
  CPPEXTERN_MSG1(classPtr, "normVBO_enable", normVBO_enableMess, bool);
  /* attributes */
  CPPEXTERN_MSG1 (classPtr, "program"          , setProgramID, float);
  CPPEXTERN_MSG  (classPtr, "attribute"        , attribute);
  CPPEXTERN_MSG1 (classPtr, "attribute_enable" , attribVBO_enableMess ,
                  bool);
  CPPEXTERN_MSG0 (classPtr, "reset_attributes" , resetAttributes);
  CPPEXTERN_MSG0 (classPtr, "print_attributes" , printAttributes);
}

void gemvertexbuffer :: tableMess (gem::VertexBuffer&vb, std::string name,
                                   unsigned int argc, t_atom *argv)
{
  int offset=0;
  std::string tabname;
  unsigned int i;

  /*
   * it's either interleaved data (1 tablename [+ offset])
   * or separate data (<dimen> tablenames [+ offset])
   */

  if(argc==0) {
    vb.enabled=false;
    return;
  }

  if(A_SYMBOL!=argv[0].a_type) {
    goto failed;
  }

  if(argc==1 || (argc==2
                 && A_FLOAT == argv[1].a_type)) { // interleaved (+offset)
    bool resize=true;
    if(argc>1) {
      offset=atom_getfloat(argv+1);
      resize=false;
    }
    tabname=std::string(atom_getsymbol(argv)->s_name);

    copyArray(tabname, vb, 0, offset*vb.dimen, resize);
    vb.enabled=true;
    return;
  }

  if (argc == vb.dimen || (argc == (vb.dimen+1))
      && A_FLOAT == argv[vb.dimen].a_type) {  // planar (+offset)
    bool resize=true;
    if(((unsigned int)argc)>vb.dimen) {
      offset=atom_getfloat(argv+vb.dimen);
      resize=false;
    }
    for(i=0; i<vb.dimen; i++) {
      if(A_SYMBOL!=argv[i].a_type) {
        goto failed;
      }
    }
    for(i=0; i<vb.dimen; i++) {
      tabname=std::string(atom_getsymbol(argv+i)->s_name);
      copyArray(tabname, vb, vb.dimen, offset*vb.dimen+i, resize);
    }
  } else {
    goto failed;
  }

  vb.enabled=true;
  return;

failed:
  error("illegal arguments to '%s': must be <table[1..%d]> [<offset>]",
        name.c_str(), vb.dimen);
  return;
}
void gemvertexbuffer :: positionMess (t_symbol*s, int argc, t_atom *argv)
{
  tableMess(m_position, s->s_name, argc, argv);
}
void gemvertexbuffer :: textureMess (t_symbol*s, int argc, t_atom *argv)
{
  tableMess(m_texture, s->s_name, argc, argv);
}
void gemvertexbuffer :: colorMess (t_symbol*s, int argc, t_atom *argv)
{
  tableMess(m_color, s->s_name, argc, argv);
}
void gemvertexbuffer :: normalMess (t_symbol*s, int argc, t_atom *argv)
{
  tableMess(m_normal, s->s_name, argc, argv);
}


void gemvertexbuffer :: posxMess (t_symbol*s, int argc, t_atom *argv)
{
  tabMess(argc,argv, m_position, 0);
}
void gemvertexbuffer :: posyMess (t_symbol*s, int argc, t_atom *argv)
{
  tabMess(argc,argv, m_position, 1);
}
void gemvertexbuffer :: poszMess (t_symbol*s, int argc, t_atom *argv)
{
  tabMess(argc,argv, m_position, 2);
}

void gemvertexbuffer :: colrMess (t_symbol*s, int argc, t_atom *argv)
{
  tabMess(argc,argv, m_color   , 0);
}
void gemvertexbuffer :: colgMess (t_symbol*s, int argc, t_atom *argv)
{
  tabMess(argc,argv, m_color   , 1);
}
void gemvertexbuffer :: colbMess (t_symbol*s, int argc, t_atom *argv)
{
  tabMess(argc,argv, m_color   , 2);
}
void gemvertexbuffer :: colaMess (t_symbol*s, int argc, t_atom *argv)
{
  tabMess(argc,argv, m_color   , 3);
}

void gemvertexbuffer :: texuMess (t_symbol*s, int argc, t_atom *argv)
{
  tabMess(argc,argv, m_texture , 0);
}
void gemvertexbuffer :: texvMess (t_symbol*s, int argc, t_atom *argv)
{
  tabMess(argc,argv, m_texture , 1);
}

void gemvertexbuffer :: normxMess(t_symbol*s, int argc, t_atom *argv)
{
  tabMess(argc,argv, m_normal  , 0);
}
void gemvertexbuffer :: normyMess(t_symbol*s, int argc, t_atom *argv)
{
  tabMess(argc,argv, m_normal  , 1);
}
void gemvertexbuffer :: normzMess(t_symbol*s, int argc, t_atom *argv)
{
  tabMess(argc,argv, m_normal  , 2);
}

void gemvertexbuffer :: posVBO_enableMess (bool flag)
{
  m_position.enabled = flag;
}
void gemvertexbuffer :: colVBO_enableMess (bool flag)
{
  m_color   .enabled = flag;
}
void gemvertexbuffer :: texVBO_enableMess (bool flag)
{
  m_texture .enabled = flag;
}
void gemvertexbuffer :: normVBO_enableMess(bool flag)
{
  m_normal  .enabled = flag;
}
void gemvertexbuffer :: attribVBO_enableMess (bool flag)
{
  for(unsigned int i=0; i<m_attribute.size(); i++) {
    m_attribute[i].enabled=flag;
  }
}

void gemvertexbuffer :: enableMess (t_symbol*s, int argc, t_atom *argv)
{
  int i;
  for(i=0; i<argc; i++) {
    if(0) {
      ;
    } else if("attribute"==std::string(atom_getsymbol(argv+i)->s_name)) {
      attribVBO_enableMess(true);
    } else if("position"==std::string(atom_getsymbol(argv+i)->s_name)) {
      m_position.enabled=true;
    } else if("color"   ==std::string(atom_getsymbol(argv+i)->s_name)) {
      m_color   .enabled=true;
    } else if("texture" ==std::string(atom_getsymbol(argv+i)->s_name)) {
      m_texture .enabled=true;
    } else if("normal"  ==std::string(atom_getsymbol(argv+i)->s_name)) {
      m_normal  .enabled=true;
    } else {
      error("enable: illegal argument#%d: must be 'position', 'color', 'texture', 'normal' or 'attribute'",
            i);
    }
  }
}
void gemvertexbuffer :: disableMess (t_symbol*s, int argc, t_atom *argv)
{
  int i;
  for(i=0; i<argc; i++) {
    if(0) {
      ;
    } else if("attribute"==std::string(atom_getsymbol(argv+i)->s_name)) {
      attribVBO_enableMess(false);
    } else if("position"==std::string(atom_getsymbol(argv+i)->s_name)) {
      m_position.enabled=false;
    } else if("color"   ==std::string(atom_getsymbol(argv+i)->s_name)) {
      m_color   .enabled=false;
    } else if("texture" ==std::string(atom_getsymbol(argv+i)->s_name)) {
      m_texture .enabled=false;
    } else if("normal"  ==std::string(atom_getsymbol(argv+i)->s_name)) {
      m_normal  .enabled=false;
    } else {
      error("disable: illegal argument#%d: must be 'position', 'color', 'texture', 'normal' or 'attribute'",
            i);
    }
  }
}



void gemvertexbuffer :: tabMess(unsigned int argc, t_atom *argv,
                                gem::VertexBuffer&array, unsigned int offset)
{
  int offset2 = 0;
  bool resize=true;
  if ( argv[0].a_type != A_SYMBOL ) {
    error("first arg must be symbol (table name)");
    return;
  }
  if ( argc > 1 ) {
    if ( argv[1].a_type != A_FLOAT ) {
      error("second arg must be float (offset)");
    } else {
      offset2 = argv[1].a_w.w_float;
      resize=false;
    }
  }
  offset2 = offset2<0?0:offset2;
  std::string tab_name = atom_getsymbol(argv)->s_name;
  copyArray(tab_name, array, array.dimen, offset2 * array.dimen + offset, resize);
  array.enabled=true;
}

void gemvertexbuffer :: resizeMess(unsigned int size)
{
  vbo_size = size>1?size:1;
  for(unsigned int i=0; i<m_attribute.size(); i++) {
    m_attribute[i].resize(vbo_size);
  }
  m_position.resize(vbo_size);
  m_texture .resize(vbo_size);
  m_color   .resize(vbo_size);
  m_normal  .resize(vbo_size);

  size_change_flag = true;
}

void gemvertexbuffer :: partialDrawMess(unsigned int start,
                                        unsigned int end)
{
  m_range[0] = start;
  m_range[1] = end;
}

// Create VBO
//*****************************************************************************
void gemvertexbuffer :: createVBO(void)
{
  for(unsigned int i=0; i<m_attribute.size(); i++) {
    m_attribute[i].create();
  }
  m_position.create();
  m_texture .create();
  m_color   .create();
  m_normal  .create();
}

void gemvertexbuffer :: copyArray(const std::string&tab_name,
                                  gem::VertexBuffer&vb,
				  unsigned int dimen, unsigned int offset,
				  bool resize)
{
  t_garray *a;
  int npoints_;
  unsigned int npoints;
  unsigned int i;
  t_word *vec;
  const bool interleaved = (0==dimen);

  if(offset>vb.size) {
    error("offset %d is bigger than vertexbuffer size (%d)", offset, vb.size);
    return;
  }

  t_symbol*s=gensym(tab_name.c_str());
  pd_findbyclass(s, garray_class);
  if (!(a = (t_garray *)pd_findbyclass(s, garray_class))) {
    error("%s: no such array", tab_name.c_str());
    return;
  }
  if (!garray_getfloatwords(a, &npoints_, &vec)) {
    error("%s: bad template for tabLink", tab_name.c_str());
    return;
  }
  if(npoints_<0) {
    error("%s: illegal number of elements %d", tab_name.c_str(), npoints_);
  }
  npoints=npoints_;

  unsigned int size=npoints;
  if(interleaved) {
    size=(npoints/vb.dimen);
    npoints=size*vb.dimen;
  }
  if(size!=vb.size) {
    if(resize)
      vb.resize(size);
  }

  float*array=vb.array;

  unsigned int npts=(unsigned int)npoints;

  if(interleaved) { // interleaved channels
    if(size>vb.size-offset) {
      size=vb.size-offset;
    }
    npoints=size*vb.dimen;

    for ( i = 0 ; i < npoints ; i++ ) {
      array[offset+i] = vec[i].w_float;
    }
  } else { // single channel
    const unsigned int maxindex=vb.size*vb.dimen;

    for ( i = 0 ; i < size ; i++ )	{
      const unsigned int index=offset+i*dimen;
      if(index>=maxindex)break;
      array[index] = vec[i].w_float;
    }
  }
  vb.dirty=true;
}

// attributes
//*****************************************************************************

void gemvertexbuffer :: setProgramID(float ID)
{
  // add 0.5 to glsl_program ID
  glsl_program=0;
  try {
    glsl_program = m_idmapper.get(ID);
  } catch(GemException&x) {}
}

void gemvertexbuffer :: attribute(t_symbol*s, int argc, t_atom *argv)
{
  unsigned int vbo_dimen=0;
  unsigned int tab_offset=0;
  int glsl_index =-1;
  GLenum attrib_type;
  std::string tabname;
  std::string name;
  const char* name_ch;
  bool resize=true;

  if(glsl_program==0) {
    error("glsl_program has not been set");
    return;
  }

  if((argc!=2 && argc!=3) || (argv[0].a_type!=A_SYMBOL
                              || argv[1].a_type!=A_SYMBOL)) {
    error("illegal arguments to 'attribute': must be <vbo_index> <attribute_name> <table> [<offset>]");
    return;
  }
  if(argc==3) {
    if(argv[2].a_type==A_FLOAT) {
      tab_offset=atom_getfloat(argv+2);
      resize=false;
    } else {
      error("illegal arguments to 'attribute': must be <vbo_index> <attribute_name> <table> [<offset>]");
      return;
    }
  }

  name=std::string(atom_getsymbol(argv+0)->s_name);
  for(unsigned int i=0; i<m_attribute.size(); i++) {
    if(name.compare(m_attribute[i].attrib_name) == 0) {
      tabname=std::string(atom_getsymbol(argv+1)->s_name);
      copyArray(tabname, m_attribute[i], 1,
                tab_offset*m_attribute[i].dimen,
		resize);  // always interleaved
      m_attribute[i].attrib_array = tabname;
      m_attribute[i].offset = tab_offset;
      return;
    }
  }

  name_ch = name.c_str();
  glsl_index = glGetAttribLocation(glsl_program, name_ch);
  if(glsl_index==-1) {
    error("illegal arguments to 'attribute': '%s' not found in glsl_program",
          name_ch);
    return;
  }

  GLsizei length;
  GLint size;
  GLchar glname;

  glGetActiveAttrib(glsl_program,
                    glsl_index,  /* GLuint program, GLuint index */
                    0, &length, &size,          /* GLsizei maxLength, GLsizei* length, GLint* size */
                    &attrib_type, &glname);       /* GLenum* type, GLchar* name */
  switch(attrib_type) {
  case GL_FLOAT:
    vbo_dimen=1;
    break;
  case GL_FLOAT_VEC2:
    vbo_dimen=2;
    break;
  case GL_FLOAT_VEC3:
    vbo_dimen=3;
    break;
  case GL_FLOAT_VEC4:
    vbo_dimen=4;
    break;
  case GL_FLOAT_MAT2:
    vbo_dimen=4;
    break;
  case GL_FLOAT_MAT3:
    vbo_dimen=9;
    break;
  case GL_FLOAT_MAT4:
    vbo_dimen=16;
    break;
  default:
    error("illegal arguments to 'attribute': must be type 'float', 'vec2-4' or 'mat2-4'");
    return;
  }

  tabname=std::string(atom_getsymbol(argv+1)->s_name);
  m_attribute.reserve(m_attribute.size()+1);

  m_attribute.push_back(gem::VertexBuffer(0, vbo_dimen));
  m_attribute.back().attrib_index = glsl_index;
  m_attribute.back().attrib_name = name;
  m_attribute.back().attrib_array = tabname;
  m_attribute.back().offset = tab_offset;
  m_attribute.back().enabled = true;

  for(unsigned int i=0; i<m_attribute.size(); i++) {
    m_attribute[i].resize(vbo_size);
  }
  for(unsigned int i=0; i<m_attribute.size(); i++) {
    copyArray(m_attribute[i].attrib_array, m_attribute[i], 1,
              m_attribute[i].offset*m_attribute[i].dimen,
	      resize);
  }
  return;
}

void gemvertexbuffer :: resetAttributes(void)
{
  std::vector<gem::VertexBuffer>().swap(m_attribute);
}

void gemvertexbuffer :: printAttributes(void)
{
  if(glsl_program) {
    post("GLSL-ProgramID: %d", glsl_program);
  }
  for(unsigned int i=0; i<m_attribute.size(); i++) {
    int j = i + 1;  // start counting at 1
    post("Attribute%d: %s", j, m_attribute[i].attrib_name.c_str());
    post("\tsize: %d", m_attribute[i].size);
    post("\tdimen: %d", m_attribute[i].dimen);
    post("\tprogram_index: %d", m_attribute[i].attrib_index);
  }
}
