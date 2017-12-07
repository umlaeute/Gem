////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 2001-2012 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "modelOBJ.h"
#include "plugins/PluginFactory.h"
#include "Gem/RTE.h"
#include "Gem/Exception.h"

#include "Gem/Properties.h"
#include <string>

using namespace gem::plugins;

REGISTER_MODELLOADERFACTORY("OBJ", modelOBJ);

modelOBJ :: modelOBJ(void) :
  m_model(NULL),
  m_material(0),
  m_flags(GLM_SMOOTH | GLM_TEXTURE),
  m_group(0),
  m_rebuild(false),
  m_currentH(1.f), m_currentW(1.f),
  m_textype(GLM_TEX_DEFAULT),
  m_reverse(false),
  m_refresh(false)
{
}

modelOBJ ::~modelOBJ(void)
{
  destroy();
}

bool modelOBJ :: open(const std::string&name,
                      const gem::Properties&requestprops)
{
  destroy();

#if 0
  std::vector<std::string>keys=requestprops.keys();
  unsigned int i;
  for(i=0; i<keys.size(); i++) {
    verbose(1, "[GEM:modelOBJ] key[%d]=%s", i, keys[i].c_str());
  }
#endif
  m_model = glmReadOBJ(name.c_str());
  if (!m_model) {
    return false;
  }
  m_reverse=false;

  double d=1;
  requestprops.get("rescale", d);
  if(d) {
    glmUnitize(m_model);
  }
  glmFacetNormals (m_model);

  gem::Properties props=requestprops;
  if(gem::Properties::UNSET==requestprops.type("smooth")
      && 0==glmGetNumNormals(m_model)) {
    props.set("smooth", 0.5);
  }
  setProperties(props);

  glmTexture(m_model, m_textype, 1,1);
  m_rebuild=true;
  compile();
  return true;
}

std::vector<std::vector<float> > modelOBJ :: getVector(
  std::string vectorName)
{
  if ( vectorName == "vertices" ) {
    return m_vertices;
  }
  if ( vectorName == "normals" ) {
    return m_normals;
  }
  if ( vectorName == "texcoords" ) {
    return m_texcoords;
  }
  if ( vectorName == "colors" ) {
    return m_colors;
  }
  verbose(0, "[GEM:modelOBJ] there is no \"%s\" vector !",
          vectorName.c_str());
  return std::vector<std::vector<float> >();
}

std::vector<gem::plugins::modelloader::VBOarray> modelOBJ :: getVBOarray()
{
  return m_VBOarray;
}

bool modelOBJ :: render(void)
{
  bool res = true;
  if(m_rebuild) {
    glmTexture(m_model, m_textype, 1,1);
    res = compile();
  }
  return res;
}
void modelOBJ :: close(void)
{
  destroy();
}

void modelOBJ :: unsetRefresh()
{
  m_refresh = false;
}
bool modelOBJ :: needRefresh()
{
  return m_refresh;
}

bool modelOBJ :: enumProperties(gem::Properties&readable,
                                gem::Properties&writeable)
{
  readable.clear();
  writeable.clear();
  return false;
}

void modelOBJ :: setProperties(gem::Properties&props)
{
  double d;

  if(props.get("smooth", d)) {
    if(d<0.) {
      d=0.;
    }
    if(d>1.) {
      d=1.;
    }
    if(m_model) {
      glmVertexNormals(m_model, d*180.);
    }
    m_rebuild=true;
  }
  if(props.get("texwidth", d)) {
    if(d!=m_currentW) {
      m_rebuild=true;
    }

    m_currentW=d;
  }
  if(props.get("texheight", d)) {
    if(d!=m_currentH) {
      m_rebuild=true;
    }
    m_currentH=d;
  }
  if(props.get("usematerials", d)) {
    int flags=GLM_SMOOTH | GLM_TEXTURE;
    if(d) {
      flags |= GLM_MATERIAL;
    }

    if(flags!=m_flags) {
      m_rebuild=true;
    }
    m_flags=flags;
  }


  std::string s;
  if(props.get("textype", s)) {
    if("UV"==s) {
      m_textype= GLM_TEX_UV;
    } else if("linear"==s) {
      m_textype= GLM_TEX_LINEAR;
    } else if("spheremap"==s) {
      m_textype= GLM_TEX_SPHEREMAP;
    }

    m_rebuild=true;
  }

  if(props.get("group", d)) {
    m_group=d;
    m_rebuild=true;
  }

  if(props.get("reverse", d)) {
    // LATER:move this to compile()
    bool reverse=d;
    if((reverse!=m_reverse) && m_model) {
      glmReverseWinding(m_model);
      m_rebuild=true;
    }
    m_reverse=reverse;
  }

  render();
}
void modelOBJ :: getProperties(gem::Properties&props)
{
  props.clear();
}

bool modelOBJ :: compile(void)
{
  m_vertices.clear();
  m_normals.clear();
  m_texcoords.clear();
  m_colors.clear();
  if (!m_group) {
    glmDraw(m_model, m_flags, m_vertices, m_normals, m_texcoords, m_colors);
  } else {
    glmDrawGroup(m_model, m_flags, m_group, m_vertices, m_normals, m_texcoords,
                 m_colors);
  }
  bool res = !(m_vertices.empty() && m_normals.empty()
               && m_texcoords.empty() && m_colors.empty());
  if(res) {
    m_rebuild=false;
    m_refresh=true;
  }
  return res;
}

void modelOBJ :: destroy(void)
{
  if(m_model) {
    glmDelete(m_model);
    m_model=NULL;
  }
}
