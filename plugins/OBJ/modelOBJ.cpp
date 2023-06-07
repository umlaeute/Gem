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


namespace {
  gem::plugins::modelloader::color float4_to_color(GLfloat col[4]) {
    gem::plugins::modelloader::color c;
    c.r = col[0];
    c.g = col[1];
    c.b = col[2];
    c.a = col[3];
    return c;
  }
};

modelOBJ :: modelOBJ(void) :
  m_model(NULL),
  m_material(0),
  m_flags(GLM_SMOOTH | GLM_TEXTURE | GLM_MATERIAL),
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

bool modelOBJ :: render(void)
{
  bool res = true;
  if(m_rebuild) {
    glmTexture(m_model, m_textype, m_currentW, m_currentH);
    res = compile();
  }
  m_rebuild = false;
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
  GLuint groups = glmGetNumGroups(m_model);
  GLuint validgroups = 0;
  for(GLuint gid=0; gid < groups; gid++) {
    if (glmGetGroup(m_model, gid))
      validgroups++;
  }

  if(validgroups != m_meshes.size()) {
    m_meshes.clear();
    for(GLuint gid=0; gid < validgroups; gid++) {
      struct meshdata mesh;
      m_meshes.push_back(std::move(mesh));
    }
  }

  for(GLuint gid = 0; gid < groups; gid++) {
    auto&m = m_meshes[gid];
    GLMmaterial*mat = 0;
    struct _GLMgroup*g = glmGetGroup(m_model, gid);
    if(!g)
      continue;
    m.vertices.clear();
    m.normals.clear();
    m.colors.clear();
    m.texcoords.clear();
    if(glmGroupData(m_model, g, m_flags, m.vertices, m.normals, m.texcoords, &mat)) {
      if(mat) {
        struct material&material=m.mesh.material;
        material.diffuse = float4_to_color(mat->diffuse);
        material.specular = float4_to_color(mat->specular);
        material.ambient = float4_to_color(mat->ambient);
        //material.emissive
        material.shininess = mat->shininess;
      }
    }
    m.mesh.size = m.vertices.size() / 3;
    m.mesh.vertices = m.vertices.data();
    m.mesh.normals = m.normals.data();
    m.mesh.colors = 0;
    m.mesh.texcoords = m.texcoords.data();
  }
  bool res = !(m_vertices.empty());
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

struct gem::plugins::modelloader::mesh* modelOBJ :: getMesh(size_t meshNum) {
  if (meshNum>=m_meshes.size())
    return nullptr;
  struct meshdata& mesh = m_meshes[meshNum];
  return &mesh.mesh;
}
size_t modelOBJ :: getNumMeshes(void) {
  post("%d meshes", m_meshes.size());
  return m_meshes.size();
}
bool modelOBJ :: updateMeshes(void) {
  bool ret = m_refresh || m_rebuild;
  m_refresh = false;
  return ret;
}







std::vector<std::vector<float> > modelOBJ :: getVector(
  std::string vectorName)
{
  return std::vector<std::vector<float> >();
}

std::vector<gem::plugins::modelloader::VBOarray> modelOBJ :: getVBOarray()
{
  return m_VBOarray;
}
