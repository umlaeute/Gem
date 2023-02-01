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
  unsigned int n = 0, t;

  prev = *trafo;
  aiMultiplyMatrix4(trafo,&nd->mTransformation);

  for (; n < nd->mNumMeshes; ++n) {
    const struct aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];
    for (t = 0; t < mesh->mNumVertices; ++t) {

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

  for (n = 0; n < nd->mNumChildren; ++n) {
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
static void apply_material(const struct aiMaterial *mtl)
{
  float c[4];

  GLenum fill_mode;
  int ret1, ret2;
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
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, c);

  set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
  if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR,
                                      &specular)) {
    color4_to_float4(&specular, c);
  }
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);

  set_float4(c, 0.2f, 0.2f, 0.2f, 1.0f);
  if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT,
                                      &ambient)) {
    color4_to_float4(&ambient, c);
  }
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, c);

  set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
  if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE,
                                      &emission)) {
    color4_to_float4(&emission, c);
  }
  glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, c);

  max = 1;
  ret1 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS, &shininess, &max);
  max = 1;
  ret2 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS_STRENGTH,
                                 &strength, &max);
  if((ret1 == AI_SUCCESS) && (ret2 == AI_SUCCESS)) {
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess * strength);
  } else {
    /* JMZ: in modelOBJ the default shininess is 65 */
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
    set_float4(c, 0.0f, 0.0f, 0.0f, 0.0f);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);
  }

  max = 1;
  if(AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_ENABLE_WIREFRAME,
      &wireframe, &max)) {
    fill_mode = wireframe ? GL_LINE : GL_FILL;
  }   else {
    fill_mode = GL_FILL;
  }
  glPolygonMode(GL_FRONT_AND_BACK, fill_mode);

  max = 1;
  if((AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_TWOSIDED,
      &two_sided, &max)) && two_sided) {
    glEnable(GL_CULL_FACE);
  } else {
    glDisable(GL_CULL_FACE);
  }
}

// ----------------------------------------------------------------------------
static void recursive_render (const struct aiScene*scene,
                              const struct aiScene *sc, const struct aiNode* nd,
                              const bool use_material,
                              const aiVector2D&tex_scale,
                              std::vector<std::vector<float> >& vertices,
                              std::vector<std::vector<float> >& normals,
                              std::vector<std::vector<float> >& texcoords,
                              std::vector<std::vector<float> >& colors,
                              aiMatrix4x4* trafo
                              )
{
  int i;
  unsigned int n = 0, t;
  aiMatrix4x4 prev = *trafo;

  // update transform
  aiMultiplyMatrix4(trafo,&nd->mTransformation);

  // draw all meshes assigned to this node
  for (; n < nd->mNumMeshes; ++n) {
    const struct aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];
    if(use_material) {
      apply_material(sc->mMaterials[mesh->mMaterialIndex]);
    }

#if 0 /* handled globally */
    if(mesh->mNormals == NULL) {
      glDisable(GL_LIGHTING);
    } else {
      glEnable(GL_LIGHTING);
    }
#endif

#if 0
    if(mesh->mColors[0] != NULL) {
      glEnable(GL_COLOR_MATERIAL);
    } else {
      glDisable(GL_COLOR_MATERIAL);
    }
#endif
    for (t = 0; t < mesh->mNumFaces; ++t) {
      const struct aiFace* face = &mesh->mFaces[t];
      GLenum face_mode;

      switch(face->mNumIndices) {
      case 1:
        face_mode = GL_POINTS;
        break;
      case 2:
        face_mode = GL_LINES;
        break;
      case 3:
        face_mode = GL_TRIANGLES;
        break;
      default:
        face_mode = GL_POLYGON;
        break;
      }

      float* pt;
      std::vector<float> vec;

      for(i = 0; i < face->mNumIndices; i++) {
        int index = face->mIndices[i];

        if(use_material && mesh->mColors[0] != NULL) {
          pt = (float*) &mesh->mColors[0][index];
          vec = std::vector<float>(pt,pt+4);
          colors.push_back(vec);
        }

        if(mesh->mNormals != NULL) {
          pt = &mesh->mNormals[index].x;
          vec = std::vector<float>(pt,pt+3);
          normals.push_back(vec);
        }

        if(mesh->HasTextureCoords(0)) {
          vec.clear();
          vec.push_back(mesh->mTextureCoords[0][index].x * tex_scale.x);
          vec.push_back(mesh->mTextureCoords[0][index].y * tex_scale.y);
          texcoords.push_back(vec);
        }

        aiVector3D tmp = mesh->mVertices[index];
        aiTransformVecByMatrix4(&tmp,trafo);

        pt = &tmp.x;
        vec = std::vector<float>(pt,pt+3);
        vertices.push_back(vec);
      }
    }
  }

  // draw all children
  for (n = 0; n < nd->mNumChildren; ++n) {
    recursive_render(scene, sc, nd->mChildren[n], use_material, tex_scale,
                     vertices, normals, texcoords, colors, trafo);
  }

  *trafo = prev;
}

};

modelASSIMP3 :: modelASSIMP3(void)
  : m_rebuild(true)
  , m_scene(NULL)
  , m_scale(1.f)
  , m_useMaterial(false)
  , m_refresh(false)
  , m_have_texcoords(false)
  , m_textype("")
  , m_texscale(1., 1.)
  , m_smooth(175.)
{
}

modelASSIMP3 ::~modelASSIMP3(void)
{
  destroy();
}

std::vector<std::vector<float> > modelASSIMP3 :: getVector(
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
  verbose(0, "[GEM:modelASSIMP3] there is no \"%s\" vector !",
          vectorName.c_str());
  return std::vector<std::vector<float> >();
}

std::vector<modelloader::VBOarray> modelASSIMP3 :: getVBOarray()
{
  return m_VBOarray;
}

void modelASSIMP3 :: unsetRefresh()
{
  m_refresh = false;
}
bool modelASSIMP3 :: needRefresh()
{
  return m_refresh;
}

bool modelASSIMP3 :: open(const std::string&name,
                          const gem::Properties&requestprops)
{
  destroy();
  int flags = aiProcessPreset_TargetRealtime_Quality;
  flags &= ~aiProcess_GenNormals;
  flags &= ~aiProcess_GenSmoothNormals;
  flags |= aiProcess_FlipUVs;

#if 0
  if(m_smooth > 90.)
    flags |= aiProcess_GenSmoothNormals;
  else
    flags |= aiProcess_GenNormals;

  m_scene = aiImportFile(name.c_str(), flags);
#else
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
#endif
  if(!m_scene) {
    return false;
  }

  get_bounding_box(m_scene, &m_min,&m_max);
  m_center.x=(m_min.x+m_max.x)/2.f;
  m_center.y=(m_min.y+m_max.y)/2.f;
  m_center.z=(m_min.z+m_max.z)/2.f;

  /* default is to rescale the object */
  float tmp;
  tmp = m_max.x-m_min.x;
  tmp = aisgl_max(m_max.y - m_min.y, tmp);
  tmp = aisgl_max(m_max.z - m_min.z, tmp);
  m_scale = 2.f / tmp;

  m_offset = m_center * (-m_scale);

  m_rebuild=true;
  m_refresh=true;

  gem::Properties props=requestprops;
  setProperties(props);

  compile();
  return true;
}

bool modelASSIMP3 :: render(void)
{
  bool res=true;
  if(m_rebuild) {
    res = compile();
  }
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
  writeable.set("rescale", 0);
  writeable.set("smooth", 0);
  writeable.set("usematerials", 0);

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
        if(m_have_texcoords && "UV" == s) {
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

    if("rescale" == key) {
      if(props.get(key, d)) {
        bool b=(bool)d;
        if(b) {
          float tmp;
          tmp = m_max.x-m_min.x;
          tmp = aisgl_max(m_max.y - m_min.y,tmp);
          tmp = aisgl_max(m_max.z - m_min.z,tmp);
          m_scale = 2.f / tmp;
          m_offset = m_center * (-m_scale);
        } else {
          // FIXXME shouldn't this be the default???
          m_scale=1.;
          m_offset.x=m_offset.y=m_offset.z=0.f;
        }
      }
      continue;
    }

    if("usematerials" == key) {
      if(props.get(key, d)) {
        bool useMaterial=d;
        if(useMaterial!=m_useMaterial) {
          m_rebuild=true;
        }
        m_useMaterial=useMaterial;
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
  unsigned int i;
  props.clear();
  for(i=0; i<keys.size(); i++) {
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

void modelASSIMP3 :: fillVBOarray()
{
  m_VBOarray.clear();
  VBOarray vboarray;

  vboarray.data = &m_vertices;
  vboarray.type = VertexBuffer::GEM_VBO_VERTICES;
  m_VBOarray.push_back(vboarray);

  vboarray.data = &m_normals;
  vboarray.type = VertexBuffer::GEM_VBO_NORMALS;
  m_VBOarray.push_back(vboarray);

  vboarray.data = &m_texcoords;
  vboarray.type = VertexBuffer::GEM_VBO_TEXCOORDS;
  m_VBOarray.push_back(vboarray);

  vboarray.data = &m_colors;
  vboarray.type = VertexBuffer::GEM_VBO_COLORS;
  m_VBOarray.push_back(vboarray);
}

bool modelASSIMP3 :: compile(void)
{
  if(!m_scene) {
    return false;
  }
#ifdef __GNUC__
# warning drop gl* invocations
#endif
  bool use_material = false;
  GLboolean useColorMaterial=GL_FALSE;
  if(glGetBooleanv) {
    glGetBooleanv(GL_COLOR_MATERIAL, &useColorMaterial);
    glDisable(GL_COLOR_MATERIAL);
    use_material = m_useMaterial;
  }

  // now begin at the root node of the imported data and traverse
  // the scenegraph by multiplying subsequent local transforms
  // together on GL's matrix stack.
  m_vertices.clear();
  m_normals.clear();
  m_texcoords.clear();
  m_colors.clear();

  aiMatrix4x4 trafo = aiMatrix4x4(aiVector3t<float>(m_scale),
                                  aiQuaterniont<float>(), m_offset);

  recursive_render(m_scene, m_scene, m_scene->mRootNode, use_material, m_texscale,
                   m_vertices, m_normals, m_texcoords, m_colors, &trafo);
  m_have_texcoords = (m_texcoords.size() > 0);

  float texscale[2];
  texscale[0] = m_texscale.x;
  texscale[1] = m_texscale.y;

  if (m_textype.empty() && m_have_texcoords) {;}
  else if("spheremap" == m_textype) {
    modelutils::genTexture_Spheremap(m_texcoords, m_normals, texscale);
  } else {
    modelutils::genTexture_Linear(m_texcoords, m_vertices, texscale);
  }

  fillVBOarray();
  if(GL_FALSE != useColorMaterial) {
    glEnable(GL_COLOR_MATERIAL);
  }

  bool res = !(m_vertices.empty() && m_normals.empty()
               && m_texcoords.empty() && m_colors.empty());
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
  m_scene=NULL;
}
