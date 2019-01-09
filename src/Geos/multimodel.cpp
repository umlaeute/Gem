////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.at
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

#include "multimodel.h"
#include "plugins/modelloader.h"
#include <algorithm> // std::min
#include <string.h>
#include <stdio.h>

namespace {
static char mytolower(char in)
{
  if(in<='Z' && in>='A') {
    return in-('Z'-'z');
  }
  return in;
}
};


CPPEXTERN_NEW_WITH_FOUR_ARGS(multimodel, t_symbol *, A_DEFSYM, t_floatarg,
                             A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT);

/////////////////////////////////////////////////////////
//
// multimodel
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
multimodel :: multimodel(t_symbol *filename, t_floatarg baseModel,
                         t_floatarg topModel, t_floatarg skipRate)
  :  m_loader(NULL),
     m_size_change_flag(false),
     m_position(256,3),
     m_texture (256,2),
     m_color   (256,4),
     m_normal  (256,3),
     m_infoOut(gem::RTE::Outlet(this)),
     m_drawType(GL_TRIANGLES)
{
  m_drawTypes.clear();
  m_drawTypes["default"]=m_drawType;
  m_drawTypes["point"]=GL_POINTS;
  m_drawTypes["points"]=GL_POINTS;
  m_drawTypes["line"]=GL_LINES;
  m_drawTypes["lines"]=GL_LINES;
  m_drawTypes["fill"]=GL_TRIANGLES;

  inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("mdl_num"));

  // make sure that there are some characters
  if (filename&&filename->s_name&&*filename->s_name) {
    openMess(filename->s_name, baseModel, topModel, skipRate);
  }
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
multimodel :: ~multimodel(void)
{
  close();
}

/////////////////////////////////////////////////////////
// close
//
/////////////////////////////////////////////////////////
void multimodel :: close(void)
{
  unsigned int i;
  for(i=0; i<m_loaders.size(); i++) {
    if (m_loaders[i]) {
      m_loaders[i]->close();
      delete m_loaders[i];
    }
    m_loaders[i]=NULL;
  }
  m_loaders.clear();
  m_loader = NULL;
}


void multimodel :: applyProperties(void)
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
void multimodel :: materialMess(int material)
{
  gem::any value=material;
  m_properties.set("usematerials", value);
  applyProperties();
}

/////////////////////////////////////////////////////////
// textureMess
//
/////////////////////////////////////////////////////////
void multimodel :: textureMess(int state)
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
void multimodel :: smoothMess(t_float fsmooth)
{
  m_properties.set("smooth", fsmooth);
  applyProperties();
}

/////////////////////////////////////////////////////////
// reverseMess
//
/////////////////////////////////////////////////////////
void multimodel :: reverseMess(bool reverse)
{
  gem::any value=(double)reverse;
  m_properties.set("reverse", value);
  applyProperties();
}
/////////////////////////////////////////////////////////
// matrialMess
//
/////////////////////////////////////////////////////////
void multimodel :: rescaleMess(bool state)
{
  gem::any value=(double)state;
  m_properties.set("rescale", value);
  applyProperties();
}

/////////////////////////////////////////////////////////
// groupMess
//
/////////////////////////////////////////////////////////
void multimodel :: groupMess(int state)
{
  gem::any value=state;
  m_properties.set("group", value);
  applyProperties();
}

/////////////////////////////////////////////////////////
// drawStyle
//
/////////////////////////////////////////////////////////
void multimodel :: drawMess(int type)
{
  /* raw */
  m_drawType = type;
}

void multimodel :: drawMess(std::string name)
{
  if(0==m_drawTypes.size()) {
    error("unable to change drawstyle");
    return;
  }

  std::transform(name.begin(), name.end(), name.begin(), mytolower);

  std::map<std::string, GLenum>::iterator it=m_drawTypes.find(name);
  if(m_drawTypes.end() == it) {
    error ("unknown draw style '%s'... possible values are:", name.c_str());
    it=m_drawTypes.begin();
    while(m_drawTypes.end() != it) {
      error("\t %s", it->first.c_str());
      ++it;
    }
    return;
  }
  m_drawType=it->second;
}


/////////////////////////////////////////////////////////
// backendMess
//
/////////////////////////////////////////////////////////
void multimodel :: backendMess(t_symbol*s, int argc, t_atom*argv)
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
void multimodel :: openMess(const std::string&filename,
                            float baseModel, float topModel, float skipRate)
{
  int skipRatei=static_cast<int>(skipRate);
  int topModeli=static_cast<int>(topModel);
  int baseModeli=static_cast<int>(baseModel);
  if (skipRatei == 0) {
    if (topModeli == 0) {
      open(filename, 0, baseModeli, 1);
    } else {
      open(filename, baseModeli, topModeli, 1);
    }
  } else {
    open(filename, baseModeli, topModeli, skipRatei);
  }
}
void multimodel :: open(const std::string&filename, int baseModel,
                        int topModel, int skipRate)
{
  gem::Properties wantProps = m_properties;
  if(!m_backends.empty()) {
    wantProps.set("backends", m_backends);
  }
  std::vector<gem::plugins::modelloader*>loaders;

  if (!topModel) {
    error("requires an int for number of models");
    return;
  }
  if (baseModel > topModel) {
    error("top range less than base model");
    return;
  }
  if (skipRate < 1) {
    skipRate = 1;
  }
  char preName[256];
  char postName[256];

  int i = 0;
  const char *strPtr = filename.c_str();
  while (strPtr[i] && strPtr[i] != '*') {
    preName[i] = strPtr[i];
    i++;
  }

  if (!strPtr[i]) {
    error("unable to find * in file name");
    return;
  }

  preName[i] = '\0';
  strncpy(postName, &(strPtr[i+1]), 255);
  postName[255]='\0';

  // need to figure out how many filenames there are to load
  int numModels = (topModel + 1 - baseModel) / skipRate;

  int realNum = baseModel;
  char bufName[MAXPDSTRING];
  canvas_makefilename(const_cast<t_canvas*>(getCanvas()), preName, bufName,
                      MAXPDSTRING);

  char newName[MAXPDSTRING];
  newName[0]=0;

  for (i = 0; i < numModels; i++, realNum += skipRate) {
    snprintf(newName, MAXPDSTRING, "%s%d%s", bufName, realNum, postName);
    newName[MAXPDSTRING-1]=0;
    verbose(1, "trying to load '%s'", newName);

    gem::plugins::modelloader*loader=gem::plugins::modelloader::getInstance();
    if(!loader) {
      break;
    }

    if(loader->open(newName, wantProps)) {
      loaders.push_back(loader);
    } else {
      delete loader;
      break;
    }
  }

  if(loaders.size()!=numModels) {
    /* ouch, something went wrong! */
    error("failed to load model#%d of %d (%s)...resetting to original models",
          i, numModels, newName);
    unsigned int ui;
    for(ui=0; ui<loaders.size(); ui++) {
      if(loaders[ui]) {
        delete loaders[ui];
      }
      loaders[ui]=NULL;
    }
    loaders.clear();
    return;
  }

  close();
  m_loaders=loaders;
  if(m_loaders.size()>0) {
    m_loader = m_loaders[0];
  }

  post("loaded models: %s %s from %d to %d skipping %d",
       bufName, postName, baseModel, topModel, skipRate);

  getVBOarray();
  setModified();
}

/////////////////////////////////////////////////////////
// changeModel
//
/////////////////////////////////////////////////////////
void multimodel :: changeModel(int modelNum)
{
  if (modelNum < 0 || ((unsigned int)modelNum) >= m_loaders.size()) {
    error("selection %d out of range: 0..%d", modelNum, m_loaders.size()-1);
    return;
  }
  m_loader = m_loaders[modelNum];

  getVBOarray();
  setModified();
}

void multimodel :: startRendering()
{
  if (m_loader) {
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
void multimodel :: render(GemState *state)
{
  if(!m_loader) {
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
    glDrawArrays(m_drawType, 0, npoints);
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
void multimodel :: obj_setupCallback(t_class *classPtr)
{
  CPPEXTERN_MSG4(classPtr, "open", openMess, std::string, float, float, float);
  CPPEXTERN_MSG1(classPtr, "mdl_num", changeModel, int);

  CPPEXTERN_MSG1(classPtr, "rescale", rescaleMess, bool);
  CPPEXTERN_MSG1(classPtr, "smooth", smoothMess, float);
  CPPEXTERN_MSG1(classPtr, "revert", reverseMess, bool);
  CPPEXTERN_MSG1(classPtr, "material", materialMess, int);
  CPPEXTERN_MSG1(classPtr, "texture", textureMess, int);
  CPPEXTERN_MSG1(classPtr, "group", groupMess, int);
  CPPEXTERN_MSG (classPtr, "loader", backendMess);

  CPPEXTERN_MSG1(classPtr, "draw", drawMess, std::string);
}

void multimodel :: createVBO(void)
{
  m_position.create();
  m_texture .create();
  m_color   .create();
  m_normal  .create();
}

void multimodel :: copyArray(const std::vector<std::vector<float> >&tab,
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

void multimodel :: copyAllArrays()
{
  if (m_loader && m_loader->needRefresh()) {
    copyArray(m_loader->getVector("vertices"), m_position);
    copyArray(m_loader->getVector("texcoords"), m_texture);
    copyArray(m_loader->getVector("normals"), m_normal);
    copyArray(m_loader->getVector("colors"), m_color);
    m_loader->unsetRefresh();
  }
}

void multimodel :: getVBOarray()
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
