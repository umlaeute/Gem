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
//    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "model.h"
#include "Gem/State.h"
#include "plugins/modelloader.h"
#include <algorithm> // std::min

CPPEXTERN_NEW_WITH_ONE_ARG(model, t_symbol *, A_DEFSYM);

/////////////////////////////////////////////////////////
//
// model
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
model :: model(t_symbol *filename) :
  m_loader(gem::plugins::modelloader::getInstance()),
  m_loaded(false),
  m_size_change_flag(false),
  m_position(256,3),
  m_texture (256,2),
  m_color   (256,4),
  m_normal  (256,3),
  m_infoOut(gem::RTE::Outlet(this))
{
  // make sure that there are some characters
  if (filename&&filename->s_name&&*filename->s_name) {
    openMess(filename->s_name);
  }
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
model :: ~model(void)
{
  if(m_loaded) {
    m_loader->close();
  }
  if(m_loader) {
    delete m_loader;
    m_loader=NULL;
  }
}

void model :: applyProperties(void)
{
#if 0
  std::vector<std::string>keys=m_properties.keys();
  unsigned int i;
  for(i=0; i<keys.size(); i++) {
    post("key[%d]=%s ... %d", i, keys[i].c_str(), m_properties.type(keys[i]));
  }
#endif

  if(m_loader) {
    m_loader->setProperties(m_properties);
  }
}

/////////////////////////////////////////////////////////
// materialMess
//
/////////////////////////////////////////////////////////
void model :: materialMess(int material)
{
  gem::any value=material;
  m_properties.set("usematerials", value);
  applyProperties();
}

/////////////////////////////////////////////////////////
// materialMess
//
/////////////////////////////////////////////////////////
void model :: textureMess(int state)
{
  std::string textype;
  switch(state) {
  case 0:
    textype="linear";
    break;
  case 1:
    textype="spheremap";
    break;
  case 2:
    textype="UV";
    break;
  default:
    break;
  }
  if(textype.empty()) {
    m_properties.erase("textype");
  } else {
    gem::any value=textype;
    m_properties.set("textype", value);
  }
  applyProperties();
}

/////////////////////////////////////////////////////////
// smoothMess
//
/////////////////////////////////////////////////////////
void model :: smoothMess(t_float fsmooth)
{
  m_properties.set("smooth", fsmooth);
  applyProperties();
}

/////////////////////////////////////////////////////////
// rescaleMess
//
/////////////////////////////////////////////////////////
void model :: reverseMess(bool reverse)
{
  gem::any value=(double)reverse;
  m_properties.set("reverse", value);
  applyProperties();
}
/////////////////////////////////////////////////////////
// matrialMess
//
/////////////////////////////////////////////////////////
void model :: rescaleMess(bool state)
{
  gem::any value=(double)state;
  m_properties.set("rescale", value);
  applyProperties();
}

/////////////////////////////////////////////////////////
// matrialMess
//
/////////////////////////////////////////////////////////
void model :: groupMess(int state)
{
  gem::any value=state;
  m_properties.set("group", value);
  applyProperties();
}

/////////////////////////////////////////////////////////
// backendMess
//
/////////////////////////////////////////////////////////
void model :: backendMess(t_symbol*s, int argc, t_atom*argv)
{
#if 0
  gem::any value=ids;
  m_properties.set("backends", value);
  applyProperties();
#endif
  int i;

  m_backends.clear();
  if(argc) {
    for(i=0; i<argc; i++) {
      if(A_SYMBOL == argv->a_type) {
        t_symbol *b=atom_getsymbol(argv+i);
        m_backends.push_back(b->s_name);
      } else {
        error("%s must be symbolic", s->s_name);
      }
    }
  } else {
    /* no backend requested, just enumerate them */
    if(m_loader) {
      std::vector<gem::any>atoms;
      gem::any value;
      t_atom at;
      t_atom*ap=&at;
      gem::Properties props;
      std::vector<std::string> backends;
      props.set("backends", value);
      m_loader->getProperties(props);
      if(props.type("backends")!=gem::Properties::UNSET) {
        props.get("backends", backends);
      }
      atoms.clear();
      atoms.push_back(value=(int)(backends.size()));
      m_infoOut.send("loaders", atoms);
      if(!backends.empty()) {
        for(i=0; i<backends.size(); i++) {
          atoms.clear();
          atoms.push_back(value=backends[i]);
          post("loader[%d] %s", i, backends[i].c_str());
          m_infoOut.send("loader", atoms);
        }
      } else {
        post("no model-loading backends found!");
      }
    }
  }
}

/////////////////////////////////////////////////////////
// openMess
//
/////////////////////////////////////////////////////////
void model :: openMess(const std::string&filename)
{
  gem::Properties wantProps = m_properties;

  if(!m_loader) {
    error("no model loader backends found");
    return;
  }
  m_loader->close();
  m_loaded=false;

  if(!m_backends.empty()) {
    wantProps.set("backends", m_backends);
  }

  char buf[MAXPDSTRING];
  canvas_makefilename(const_cast<t_canvas*>(getCanvas()),
                      const_cast<char*>(filename.c_str()), buf, MAXPDSTRING);
  if(!m_loader->open(buf, wantProps)) {
    error("unable to read model '%s'", buf);
    return;
  }

  m_loaded=true;
  getVBOarray();
  setModified();
}

void model :: startRendering()
{
  if (m_loaded) {
    copyArray(m_loader->getVector("vertices"), m_position);
    copyArray(m_loader->getVector("texcoords"), m_texture);
    copyArray(m_loader->getVector("normals"), m_normal);
    copyArray(m_loader->getVector("colors"), m_color);
  }
}
/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void model :: render(GemState *state)
{
  if(!m_loaded) {
    return;
  }

  if ( !m_position.vbo || !m_texture.vbo || !m_color.vbo || !m_normal.vbo
       || m_size_change_flag ) {
    createVBO();
    m_size_change_flag = false;
  }
  getVBOarray();

  std::vector<unsigned int> sizeList;

  if(m_position.render()) {
    glVertexPointer(m_position.dimen, GL_FLOAT, 0, 0);
    glEnableClientState(GL_VERTEX_ARRAY);
    sizeList.push_back(m_position.size);
  }
  if(m_texture.render()) {
    glTexCoordPointer(m_texture.dimen, GL_FLOAT, 0, 0);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    sizeList.push_back(m_texture.size);
  }
  if(m_color.render()) {
    glColorPointer(m_color.dimen, GL_FLOAT, 0, 0);
    glEnableClientState(GL_COLOR_ARRAY);
    sizeList.push_back(m_color.size);
  }
  if(m_normal.render()) {
    glNormalPointer(GL_FLOAT, 0, 0);
    glEnableClientState(GL_NORMAL_ARRAY);
    sizeList.push_back(m_normal.size);
  }

  if ( sizeList.size() > 0 ) {
    unsigned int npoints = *std::min_element(sizeList.begin(),sizeList.end());
    glDrawArrays(GL_TRIANGLES, 0, npoints);
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
// static member function
//
/////////////////////////////////////////////////////////
void model :: obj_setupCallback(t_class *classPtr)
{
  CPPEXTERN_MSG1(classPtr, "open", openMess, std::string);
  CPPEXTERN_MSG1(classPtr, "rescale", rescaleMess, bool);
  CPPEXTERN_MSG1(classPtr, "smooth", smoothMess, float);
  CPPEXTERN_MSG1(classPtr, "revert", reverseMess, bool);
  CPPEXTERN_MSG1(classPtr, "material", materialMess, int);
  CPPEXTERN_MSG1(classPtr, "texture", textureMess, int);
  CPPEXTERN_MSG1(classPtr, "group", groupMess, int);
  CPPEXTERN_MSG (classPtr, "loader", backendMess);
}

void model :: createVBO(void)
{
  m_position.create();
  m_texture .create();
  m_color   .create();
  m_normal  .create();
}

void model :: copyArray(const std::vector<std::vector<float> > tab,
                        gem::VertexBuffer&vb)
{
  unsigned int size(0), i(0), npts(0);

  //~std::vector<std::vector<float> > tab = m_loader->getVector(vectorName);
  if ( tab.empty() ) {
    return;
  }
  size=tab.size();

  if(size!=vb.size) {
    vb.resize(size);
    m_size_change_flag=true;
  }

  for ( i = 0 ; i < size ; i++ ) {
    for ( int j=0 ; j< std::min(vb.dimen,(unsigned int)tab[i].size()) ; j++) {
      vb.array[i*vb.dimen + j] = tab[i][j];
    }
  }
  vb.dirty=true;
  vb.enabled=true;
}

void model :: copyAllArrays()
{
  if (m_loader && m_loader->needRefresh()) {
    copyArray(m_loader->getVector("vertices"), m_position);
    copyArray(m_loader->getVector("texcoords"), m_texture);
    copyArray(m_loader->getVector("normals"), m_normal);
    copyArray(m_loader->getVector("colors"), m_color);
    m_loader->unsetRefresh();
  }
}

void model :: getVBOarray()
{
  if (m_loader && m_loader->needRefresh()) {

    std::vector<gem::plugins::modelloader::VBOarray>  vboArray =
      m_loader->getVBOarray();

    if ( vboArray.empty() ) {
      copyAllArrays();
    } else {
      for (int i = 0; i<vboArray.size(); i++) {
        switch (vboArray[i].type) {
        case gem::VertexBuffer::GEM_VBO_VERTICES:
          copyArray(*vboArray[i].data, m_position);
          break;
        case gem::VertexBuffer::GEM_VBO_TEXCOORDS:
          copyArray(*vboArray[i].data, m_texture);
          break;
        case gem::VertexBuffer::GEM_VBO_NORMALS:
          copyArray(*vboArray[i].data, m_normal);
          break;
        case gem::VertexBuffer::GEM_VBO_COLORS:
          copyArray(*vboArray[i].data, m_color);
          break;
        default:
          error("VBO type %d not supported\n",vboArray[i].type);
        }
      }
      m_loader->unsetRefresh();
    }
  }
}
