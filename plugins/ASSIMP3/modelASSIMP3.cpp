////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.at
//
// Implementation file
//
//    Copyright (c) 2001-2014 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
#include "modelASSIMP3.h"
#include "plugins/PluginFactory.h"
#include "Gem/RTE.h"
#include "Gem/Exception.h"
#include "Gem/Properties.h"
#include "Gem/VertexBuffer.h"
#include "Gem/GemGL.h"

#include "Utils/Functions.h"

#include <string>

using namespace gem::plugins;

REGISTER_MODELLOADERFACTORY("ASSIMP3", modelASSIMP3);

namespace
{
#define aisgl_min(x,y) (x<y?x:y)
#define aisgl_max(x,y) (y>x?y:x)

// ----------------------------------------------------------------------------
static void get_bounding_box_for_node (const struct aiScene*scene,
                                       const struct aiNode* nd,
                                       aiVector3D* min,
                                       aiVector3D* max,
                                       aiMatrix4x4* trafo
                                      )
{
  aiMatrix4x4 prev;

  prev = *trafo;
  aiMultiplyMatrix4(trafo,&nd->mTransformation);

  for (unsigned int n=0; n < nd->mNumMeshes; ++n) {
    const struct aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];
    for (unsigned int t = 0; t < mesh->mNumVertices; ++t) {

      aiVector3D tmp = mesh->mVertices[t];
      aiTransformVecByMatrix4(&tmp,trafo);

      min->x = aisgl_min(min->x,tmp.x);
      min->y = aisgl_min(min->y,tmp.y);
      min->z = aisgl_min(min->z,tmp.z);

      max->x = aisgl_max(max->x,tmp.x);
      max->y = aisgl_max(max->y,tmp.y);
      max->z = aisgl_max(max->z,tmp.z);
    }
  }

  for (unsigned int n = 0; n < nd->mNumChildren; ++n) {
    get_bounding_box_for_node(scene, nd->mChildren[n],min,max,trafo);
  }
  *trafo = prev;
}

// ----------------------------------------------------------------------------

static void get_bounding_box (const aiScene*scene, aiVector3D* min,
                              aiVector3D* max)
{
  aiMatrix4x4 trafo;
  aiIdentityMatrix4(&trafo);

  min->x = min->y = min->z =  1e10f;
  max->x = max->y = max->z = -1e10f;
  get_bounding_box_for_node(scene, scene->mRootNode, min, max, &trafo);
}

// ----------------------------------------------------------------------------

static void color4_to_float4(const aiColor4D *c, float f[4])
{
  f[0] = c->r;
  f[1] = c->g;
  f[2] = c->b;
  f[3] = c->a;
}

// ----------------------------------------------------------------------------

static void set_float4(float f[4], float a, float b, float c, float d)
{
  f[0] = a;
  f[1] = b;
  f[2] = c;
  f[3] = d;
}

// ----------------------------------------------------------------------------
  static void apply_material(gem::plugins::modelloader::material&material
                             , const struct aiMaterial *mtl)
{
  float c[4];

  GLenum fill_mode;
  aiColor4D diffuse;
  aiColor4D specular;
  aiColor4D ambient;
  aiColor4D emission;
  float shininess, strength;
  int two_sided;
  int wireframe;
  unsigned int max;

  set_float4(c, 0.8f, 0.8f, 0.8f, 1.0f);
  if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE,
                                      &diffuse)) {
    color4_to_float4(&diffuse, c);
  }
  material.diffuse = {c[0], c[1], c[2], c[3]};

  set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
  if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR,
                                      &specular)) {
    color4_to_float4(&specular, c);
  }
  material.specular = {c[0], c[1], c[2], c[3]};

  set_float4(c, 0.2f, 0.2f, 0.2f, 1.0f);
  if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT,
                                      &ambient)) {
    color4_to_float4(&ambient, c);
  }
  material.ambient = {c[0], c[1], c[2], c[3]};

  set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
  if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE,
                                      &emission)) {
    color4_to_float4(&emission, c);
  }
  material.emissive = {c[0], c[1], c[2], c[3]};

  max = 1;
  if (AI_SUCCESS == aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS, &shininess, &max)) {
    max = 1;
    if( AI_SUCCESS == aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS_STRENGTH,
                                              &strength, &max) )
      shininess *= strength;
    material.shininess = shininess;
  } else {
    /* JMZ: in modelOBJ the default shininess is 65 */
    material.shininess = 0.;
    material.specular = {0., 0., 0., 0.};
  }
}


static bool hasMeshes(const struct aiNode* nd) {
  if (nd->mNumMeshes>0)
    return true;
  for (int n = 0; n < nd->mNumChildren; ++n) {
    if (hasMeshes(nd->mChildren[n]))
      return true;
  }
  return false;
}

// ----------------------------------------------------------------------------
static void recursive_render(
  std::vector<struct gem::plugins::modelASSIMP3::meshdata>& meshes
  , const struct aiScene*scene
  , const struct aiScene *sc, const struct aiNode* nd
  , const aiVector2D&tex_scale
  , aiMatrix4x4* trafo
  )
{
  aiMatrix4x4 prev = *trafo;
  // update transform
  aiMultiplyMatrix4(trafo,&nd->mTransformation);

  // draw all meshes assigned to this node
  for (unsigned int n=0; n < nd->mNumMeshes; ++n) {
    const struct aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];
    if(!mesh->mNumFaces)continue;
    if(mesh->mFaces[0].mNumIndices < 3)continue;
    struct gem::plugins::modelASSIMP3::meshdata newmesh;
    meshes.push_back(std::move(newmesh));
    struct gem::plugins::modelASSIMP3::meshdata&outmesh = meshes.back();
    std::vector<float>&vertices = outmesh.vertices;
    std::vector<float>&normals = outmesh.normals;
    std::vector<float>&texcoords = outmesh.texcoords;
    std::vector<float>&colors = outmesh.colors;
    size_t numVertices = 0;

    apply_material(outmesh.mesh.material, sc->mMaterials[mesh->mMaterialIndex]);

    for (unsigned int t = 0; t < mesh->mNumFaces; ++t) {
      const struct aiFace* face = &mesh->mFaces[t];
      for(int i = 0; i < face->mNumIndices; i++) {
        int index = face->mIndices[i];
        numVertices++;

        if(mesh->mColors[0] != NULL) {
          float *pt = (float*) &mesh->mColors[0][index];
          colors.insert( colors.end(), pt, pt+4);
        }

        if(mesh->mNormals != NULL) {
          float *pt = &mesh->mNormals[index].x;
          normals.insert( normals.end(), pt, pt+3);
        }

        if(mesh->HasTextureCoords(0)) {
          texcoords.push_back(mesh->mTextureCoords[0][index].x * tex_scale.x);
          texcoords.push_back(mesh->mTextureCoords[0][index].y * tex_scale.y);
        }

        aiVector3D tmp = mesh->mVertices[index];
        aiTransformVecByMatrix4(&tmp,trafo);

        float *pt = &tmp.x;
        vertices.insert (vertices.end(), pt, pt+3);
      }
    }

#define SET_OUTMESH(name) outmesh.mesh.name = (name.size()>0) ? name.data() : 0
    SET_OUTMESH(vertices);
    SET_OUTMESH(normals);
    SET_OUTMESH(colors);
    SET_OUTMESH(texcoords);
    outmesh.mesh.size = numVertices;
  }

  // draw all children
  for (unsigned int n = 0; n < nd->mNumChildren; ++n) {
    recursive_render(meshes, scene, sc, nd->mChildren[n], tex_scale,
                     trafo);
  }

  *trafo = prev;
}

};

modelASSIMP3 :: modelASSIMP3(void)
  : m_rebuild(true)
  , m_scene(NULL)
  , m_scale(1.f)
  , m_refresh(false)
  , m_textype("")
  , m_texscale(1., 1.)
  , m_smooth(175.)
{
}

modelASSIMP3 ::~modelASSIMP3(void)
{
  destroy();
}
bool modelASSIMP3 :: open(const std::string&name,
                          const gem::Properties&requestprops)
{
  destroy();
  int flags = aiProcessPreset_TargetRealtime_Quality;
  flags |= aiProcess_Triangulate | aiProcess_SortByPType;
  flags &= ~aiProcess_GenNormals;
  flags &= ~aiProcess_GenSmoothNormals;
  flags |= aiProcess_FlipUVs;

#if 0
  double d;
  if(requestprops.get("reverse", d)) {
    bool reverse = d;
    if (reverse)
      flags |= aiProcess_FlipWindingOrder;
  }

#endif


  aiPropertyStore *aiprops = aiCreatePropertyStore();
  if(aiprops) {
    flags |= aiProcess_GenSmoothNormals;
    aiSetImportPropertyFloat(aiprops, AI_CONFIG_PP_GSN_MAX_SMOOTHING_ANGLE, m_smooth);
  } else {
    if(m_smooth > 90.)
      flags |= aiProcess_GenSmoothNormals;
    else
      flags |= aiProcess_GenNormals;
  }
  m_scene = aiImportFileExWithProperties(name.c_str(), flags, 0, aiprops);
  aiReleasePropertyStore(aiprops);

  if(!m_scene) {
    return false;
  }

  get_bounding_box(m_scene, &m_min,&m_max);
  m_center.x=(m_min.x+m_max.x)/2.f;
  m_center.y=(m_min.y+m_max.y)/2.f;
  m_center.z=(m_min.z+m_max.z)/2.f;

  m_offset.x = m_offset.y = m_offset.z = 0.f;
  m_scale = 1.f;
#if 0
  /* default is to rescale the object */
  float tmp;
  tmp = m_max.x-m_min.x;
  tmp = aisgl_max(m_max.y - m_min.y, tmp);
  tmp = aisgl_max(m_max.z - m_min.z, tmp);
  m_scale = 2.f / tmp;

  m_offset = m_center * (-m_scale);
#endif
  m_rebuild=true;
  m_refresh=true;

  gem::Properties props=requestprops;
  setProperties(props);

  /* setProperties() already calls render() which compile()s, as m_rebuild=True, so skip compile() */
  //compile();
  return true;
}

bool modelASSIMP3 :: render(void)
{
  bool res=true;
  if(m_rebuild) {
    res = compile();
  }
  m_rebuild = false;
  return res;
}
void modelASSIMP3 :: close(void)
{
  destroy();
}

bool modelASSIMP3 :: enumProperties(gem::Properties&readable,
                                    gem::Properties&writeable)
{
  gem::any typ;
  readable.clear();
  readable.set("texwidth", 1);
  readable.set("texheight", 1);

  writeable.clear();
  writeable.set("textype", std::string("UV"));
  writeable.set("_texwidth", 1);
  writeable.set("_texheight", 1);
  writeable.set("smooth", 0);

  return true;
}

void modelASSIMP3 :: setProperties(gem::Properties&props)
{
  std::vector<std::string>keys=props.keys();
  for(unsigned int i=0; i<keys.size(); i++) {
    std::string key=keys[i];
    std::string s;
    double d;
#if 0
    verbose(1, "[GEM:modelASSIMP3] key[%d]=%s ... %d", i, keys[i].c_str(),
            props.type(keys[i]));
#endif
    if("textype" == key) {
      if(props.get(key, s)) {
        // if there are NO texcoords, we only accept 'linear' and 'spheremap'
        // else, we also allow 'UV'
        // not-accepted textype, simply use the last one
        if("UV" == s) {
          m_textype = "";
        } else if(("linear" == s) || ("spheremap" == s)) {
          m_textype = s;
        }
        m_rebuild = true;
      }
      continue;
    }
    if("_texwidth" == key) {
      if(props.get(key, d)) {
        if(d != m_texscale.x) {
          m_rebuild=true;
        }

        m_texscale.x = d;
      }
      continue;
    }
    if("_texheight" == key) {
      if(props.get(key, d)) {
        if(d != m_texscale.y) {
          m_rebuild=true;
        }
        m_texscale.y = d;
      }
      continue;
    }

    if("smooth" == key) {
      if(props.get(key, d)) {
        if(d<0.) {
          d=0.;
        }
        m_smooth = d*180.;
        if(m_smooth >= 175.)
          m_smooth = 175.;
        m_rebuild=true;
      }
      continue;
    }

  }

  render();
}
void modelASSIMP3 :: getProperties(gem::Properties&props)
{
  std::vector<std::string>keys=props.keys();
  props.clear();
  for(unsigned int i=0; i<keys.size(); i++) {
    std::string key=keys[i];
    if("texwidth" == key) {
      props.set(key, m_texscale.x);
      continue;
    }
    if("texheight" == key) {
      props.set(key, m_texscale.y);
      continue;
    }
  }
}

bool modelASSIMP3 :: compile(void)
{
  if(!m_scene) {
    return false;
  }
  GLboolean useColorMaterial=GL_FALSE;

  // now begin at the root node of the imported data and traverse
  // the scenegraph by multiplying subsequent local transforms
  // together on GL's matrix stack.
  m_meshes.clear();

  aiMatrix4x4 trafo = aiMatrix4x4(aiVector3t<float>(m_scale),
                                  aiQuaterniont<float>(), m_offset);

  recursive_render(m_meshes,
                   m_scene, m_scene, m_scene->mRootNode, m_texscale,
                   &trafo);
  bool haveTexcoords = false;
  for(const auto&m: m_meshes) {
    if(m.texcoords.size()>0) {
      haveTexcoords = true;
      break;
    }
  }

  float texscale[2];
  texscale[0] = m_texscale.x;
  texscale[1] = m_texscale.y;

  if (m_textype.empty() && haveTexcoords) {
    /* use built-in texcoords */
  } else if("spheremap" == m_textype) {
    for(auto&m: m_meshes) {
      modelutils::genTexture_Spheremap(m.texcoords, m.normals, texscale);
    }
  } else {
    /* fallback to linear texcoords */
    for(auto&m: m_meshes) {
      modelutils::genTexture_Linear(m.texcoords, m.vertices, texscale);
    }
  }

  bool res = !(m_meshes.empty());
  if(res) {
    m_rebuild=false;
    m_refresh=true;
  }
  return res;
}
void modelASSIMP3 :: destroy(void)
{
  if(m_scene) {
    aiReleaseImport(m_scene);
  }
  m_meshes.clear();
  m_scene=NULL;
}

struct gem::plugins::modelloader::mesh* modelASSIMP3 :: getMesh(size_t meshNum) {
  size_t numMeshes = m_meshes.size();
  if (meshNum>=numMeshes)
    return nullptr;
  struct meshdata& mesh = m_meshes[numMeshes-meshNum-1];
  return &mesh.mesh;
}
size_t modelASSIMP3 :: getNumMeshes(void) {
  return m_meshes.size();
}

  /**
   * update the mesh data (for all meshes)
   * the data pointers in previously obtained t_mesh'es stay valid
   * (but the data they point to might change)
   * returns TRUE if there was a change, FALSE otherwise
   */
bool modelASSIMP3 :: updateMeshes(void) {
  bool ret = m_refresh || m_rebuild;
  m_refresh = false;
  return ret;
}
