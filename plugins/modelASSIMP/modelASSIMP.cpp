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
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#ifdef HAVE_LIBASSIMP

#include "modelASSIMP.h"
#include "plugins/PluginFactory.h"
#include "Gem/RTE.h"
#include "Gem/Exception.h"
#include "Gem/Properties.h"

#include "Utils/Functions.h"

#include <string>
#include <math.h>

using namespace gem::plugins;

REGISTER_MODELLOADERFACTORY("ASSIMP", modelASSIMP);


namespace {

#define aisgl_min(x,y) (x<y?x:y)
#define aisgl_max(x,y) (y>x?y:x)

// ----------------------------------------------------------------------------
static void get_bounding_box_for_node (const struct aiScene*scene, const struct aiNode* nd, 
	struct aiVector3D* min, 
	struct aiVector3D* max, 
	struct aiMatrix4x4* trafo
){
	struct aiMatrix4x4 prev;
	unsigned int n = 0, t;

	prev = *trafo;
	aiMultiplyMatrix4(trafo,&nd->mTransformation);

	for (; n < nd->mNumMeshes; ++n) {
		const struct aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];
		for (t = 0; t < mesh->mNumVertices; ++t) {

			struct aiVector3D tmp = mesh->mVertices[t];
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

static void get_bounding_box (const struct aiScene*scene, struct aiVector3D* min, struct aiVector3D* max)
{
	struct aiMatrix4x4 trafo;
	aiIdentityMatrix4(&trafo);

	min->x = min->y = min->z =  1e10f;
	max->x = max->y = max->z = -1e10f;
	get_bounding_box_for_node(scene, scene->mRootNode,min,max,&trafo);
}

// ----------------------------------------------------------------------------

static void color4_to_float4(const struct aiColor4D *c, float f[4])
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
	struct aiColor4D diffuse;
	struct aiColor4D specular;
	struct aiColor4D ambient;
	struct aiColor4D emission;
	float shininess, strength;
	int two_sided;
	int wireframe;
	unsigned int max;

	set_float4(c, 0.8f, 0.8f, 0.8f, 1.0f);
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
		color4_to_float4(&diffuse, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, c);
  //post("diffuse: %g\t%g\t%g\t%g", c[0], c[1], c[2], c[3]);

	set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &specular))
		color4_to_float4(&specular, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);
  //post("specular: %g\t%g\t%g\t%g", c[0], c[1], c[2], c[3]);

	set_float4(c, 0.2f, 0.2f, 0.2f, 1.0f);
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &ambient))
		color4_to_float4(&ambient, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, c);
  //post("ambient: %g\t%g\t%g\t%g", c[0], c[1], c[2], c[3]);

	set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &emission))
		color4_to_float4(&emission, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, c);
  //  post("emission: %g\t%g\t%g\t%g", c[0], c[1], c[2], c[3]);

	max = 1;
	ret1 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS, &shininess, &max);
	max = 1;
	ret2 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS_STRENGTH, &strength, &max);
	if((ret1 == AI_SUCCESS) && (ret2 == AI_SUCCESS)) {
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess * strength);
    //post("shininess: %gx%g=%g\t%g", shininess, strength, shininess*strength);
  }
	else {
    /* JMZ: in modelOBJ the default shininess is 65 */
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
		set_float4(c, 0.0f, 0.0f, 0.0f, 0.0f);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);
	}

	max = 1;
	if(AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_ENABLE_WIREFRAME, &wireframe, &max))
		fill_mode = wireframe ? GL_LINE : GL_FILL;
	else
		fill_mode = GL_FILL;
	glPolygonMode(GL_FRONT_AND_BACK, fill_mode);

	max = 1;
	if((AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_TWOSIDED, &two_sided, &max)) && two_sided)
		glEnable(GL_CULL_FACE);
	else 
		glDisable(GL_CULL_FACE);
}

// ----------------------------------------------------------------------------

// Can't send color down as a pointer to aiColor4D because AI colors are ABGR.
static void Color4f(const struct aiColor4D *color)
{
	glColor4f(color->r, color->g, color->b, color->a);
}

// ----------------------------------------------------------------------------
static void recursive_render (const struct aiScene*scene, const struct aiScene *sc, const struct aiNode* nd, const bool use_material)
{
	int i;
	unsigned int n = 0, t;
	struct aiMatrix4x4 m = nd->mTransformation;

	// update transform
	aiTransposeMatrix4(&m);
	glPushMatrix();
	glMultMatrixf((float*)&m);

	// draw all meshes assigned to this node
	for (; n < nd->mNumMeshes; ++n) {
		const struct aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];
    if(use_material)
      apply_material(sc->mMaterials[mesh->mMaterialIndex]);

#if 0
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
				case 1: face_mode = GL_POINTS; break;
				case 2: face_mode = GL_LINES; break;
				case 3: face_mode = GL_TRIANGLES; break;
				default: face_mode = GL_POLYGON; break;
			}

			glBegin(face_mode);

			for(i = 0; i < face->mNumIndices; i++) {
				int index = face->mIndices[i];
        
				if(use_material && mesh->mColors[0] != NULL)
					Color4f(&mesh->mColors[0][index]);

				if(mesh->mNormals != NULL) 
					glNormal3fv(&mesh->mNormals[index].x);

        if(mesh->HasTextureCoords(0))
          glTexCoord2f(mesh->mTextureCoords[0][index].x, mesh->mTextureCoords[0][index].y);

				glVertex3fv(&mesh->mVertices[index].x);
			}

			glEnd();
		}

	}

	// draw all children
	for (n = 0; n < nd->mNumChildren; ++n) {
		recursive_render(scene, sc, nd->mChildren[n], use_material);
	}

	glPopMatrix();
}

};

modelASSIMP :: modelASSIMP(void) : 
  m_rebuild(true),
  m_scene(NULL), m_dispList(0),
  m_scale(1.f),
  m_useMaterial(false)
{
}

modelASSIMP ::~modelASSIMP(void) {
  destroy();
}

bool modelASSIMP :: open(const std::string&name, const gem::Properties&requestprops) {
  destroy();

	m_scene = aiImportFile(name.c_str(), aiProcessPreset_TargetRealtime_Quality);
  if(!m_scene)return false;
  
  get_bounding_box(m_scene, &m_min,&m_max);
  m_center.x=(m_min.x+m_max.x)/2.f;
  m_center.y=(m_min.y+m_max.y)/2.f;
  m_center.z=(m_min.z+m_max.z)/2.f;

  /* default is to rescale the object */
  float tmp;
  tmp = m_max.x-m_min.x;
  tmp = aisgl_max(m_max.y - m_min.y,tmp);
  tmp = aisgl_max(m_max.z - m_min.z,tmp);
  m_scale = 2.f / tmp;

  m_offset.x=-m_center.x;
  m_offset.y=-m_center.y;
  m_offset.z=-m_center.z;

  gem::Properties props=requestprops;
  setProperties(props);

  m_rebuild=true;
  return true;
}

bool modelASSIMP :: render(void) {
  if(m_rebuild || 0==m_dispList)
    compile();

  if(m_dispList) {
    glPushMatrix();

    // scale the model
    glScalef(m_scale, m_scale, m_scale);
    // center the model
    glTranslatef( m_offset.x, m_offset.y, m_offset.z );

    glCallList(m_dispList);

    glPopMatrix();
  }

  return (!m_dispList);
}
void modelASSIMP :: close(void)  {
  destroy();
}

bool modelASSIMP :: enumProperties(gem::Properties&readable,
                                gem::Properties&writeable) {
  readable.clear();
  writeable.clear();
  return false;
}

void modelASSIMP :: setProperties(gem::Properties&props) {
  double d;


#if 0
  std::vector<std::string>keys=props.keys();
  unsigned int i;
  for(i=0; i<keys.size(); i++) {
    post("key[%d]=%s ... %d", i, keys[i].c_str(), props.type(keys[i]));
  }
#endif

  if(props.get("rescale", d)) {
    bool b=(bool)d;
    if(b) {
      float tmp;
      tmp = m_max.x-m_min.x;
      tmp = aisgl_max(m_max.y - m_min.y,tmp);
      tmp = aisgl_max(m_max.z - m_min.z,tmp);
      m_scale = 2.f / tmp;

      m_offset.x=-m_center.x;
      m_offset.y=-m_center.y;
      m_offset.z=-m_center.z;
    } else {
      m_scale=1.;
      m_offset.x=m_offset.y=m_offset.z=0.f;
    }
  }
  if(props.get("usematerials", d)) {
    bool useMaterial=d;
    if(useMaterial!=m_useMaterial)
      m_rebuild=true;
    m_useMaterial=useMaterial;
  }

}
void modelASSIMP :: getProperties(gem::Properties&props) {
}

bool modelASSIMP :: compile(void)  {
  if(!m_scene) return false;
  if(!(GLEW_VERSION_1_1)) {
    //    verbose(1, "cannot build display-list now...do you have a window?");
    return false;
  }
  if (m_dispList) {
    glDeleteLists(m_dispList, 1);
    m_dispList=0;
  }

  m_dispList=glGenLists(1);

  if(m_dispList) {
    glNewList(m_dispList, GL_COMPILE);
    // now begin at the root node of the imported data and traverse
    // the scenegraph by multiplying subsequent local transforms
    // together on GL's matrix stack.
    recursive_render(m_scene, m_scene, m_scene->mRootNode, m_useMaterial);
    glEndList();
  }

  bool res = (0 != m_dispList);
  if(res) m_rebuild=false;
  return res;
}
void modelASSIMP :: destroy(void)  {
  /* LATER: check valid contexts (remove glDelete from here) */
  if (m_dispList) {
    // destroy display list
    glDeleteLists(m_dispList, 1);
    m_dispList = 0;
  }

  if(m_scene)
    aiReleaseImport(m_scene);
  m_scene=NULL;
}
#endif
