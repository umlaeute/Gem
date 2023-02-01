////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.at
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

  glmTexture(m_model, m_textype, m_currentW, m_currentH);
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
    glmTexture(m_model, m_textype, m_currentW, m_currentH);
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
  readable.set("texwidth", 1);
  readable.set("texheight", 1);
  readable.set("groups", 0);

  writeable.clear();
  writeable.set("textype", std::string("default"));
  writeable.set("_texwidth", 1);
  writeable.set("_texheight", 1);
  writeable.set("rescale", 1);
  writeable.set("smooth", 0);
  writeable.set("usematerials", 0);
  writeable.set("group", 0);
  writeable.set("reverse", 0);
  return true;
}

void modelOBJ :: setProperties(gem::Properties&props)
{
  std::vector<std::string>keys=props.keys();
  for(unsigned int i=0; i<keys.size(); i++) {
    std::string key=keys[i];
    std::string s;
    double d;
    if("smooth" == key) {
      if(props.get(key, d)) {
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
      continue;
    }

    if("textype" == key) {
      if(props.get(key, s)) {
        if("UV"==s) {
          m_textype= GLM_TEX_UV;
        } else if("linear"==s) {
          m_textype= GLM_TEX_LINEAR;
        } else if("spheremap"==s) {
          m_textype= GLM_TEX_SPHEREMAP;
        }

        m_rebuild=true;
      }
      continue;
    }

    if("_texwidth" == key) {
      if(props.get(key, d)) {
        if(d!=m_currentW) {
          m_rebuild=true;
        }

        m_currentW=d;
      }
      continue;
    }

    if("_texheight" == key) {
      if(props.get(key, d)) {
        if(d!=m_currentH) {
          m_rebuild=true;
        }
        m_currentH=d;
      }
      continue;
    }

    if("usematerials" == key) {
      if(props.get(key, d)) {
        int flags=GLM_SMOOTH | GLM_TEXTURE;
        if(d) {
          flags |= GLM_MATERIAL;
        }

        if(flags!=m_flags) {
          m_rebuild=true;
        }
        m_flags=flags;
      }
      continue;
    }

    if("group" == key) {
      if(props.get(key, d)) {
        m_group=d;
        m_rebuild=true;
      }
      continue;
    }

    if("reverse" == key) {
      if(props.get(key, d)) {
        // LATER:move this to compile()
        bool reverse=d;
        if((reverse!=m_reverse) && m_model) {
          glmReverseWinding(m_model);
          m_rebuild=true;
        }
        m_reverse=reverse;
      }
      continue;
    }
  }

  render();
}
void modelOBJ :: getProperties(gem::Properties&props)
{
  std::vector<std::string>keys=props.keys();
  unsigned int i;
  props.clear();
  for(i=0; i<keys.size(); i++) {
    std::string key=keys[i];
    if("texwidth" == key) {
      props.set(key, m_currentW);
    }
    if("texheight" == key) {
      props.set(key, m_currentH);
    }
    if("groups" == key) {
      if(m_model) {
        props.set(key, glmGetNumGroups(m_model));
      }
    }
  }
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
