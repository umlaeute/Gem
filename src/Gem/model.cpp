////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.at
//
// Implementation file
//
//    Copyright (c) 2023 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "model.h"
#include <algorithm> // std::min
#include "m_pd.h"
#include "plugins/modelloader.h"

typedef struct bbox {
  float minX, minY, minZ;
  float maxX, maxY, maxZ;
} bbox_t;

namespace
{
  /* various little helpers */
  template <typename T>
  void setvector(std::vector<T>&vec, T*data, size_t size) {
    if(data && size)
      vec.insert(vec.end(), data, data+size);
    else
      vec.clear();
  }
  template <typename T>
  T max(const T&a, const T&b) {
    return (a>b)?a:b;
  }

  GLfloat*color2gl(const gem::plugins::modelloader::color&c, GLfloat buf[4]) {
    buf[0] = c.r;
    buf[1] = c.g;
    buf[2] = c.b;
    buf[3] = c.a;
    return buf;
  }
  void render_material(const gem::plugins::modelloader::material&m)
  {
    GLfloat glcol[4];
#define COLOR2FLOAT4(col) color2gl(col, glcol)
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, COLOR2FLOAT4(m.diffuse));
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, COLOR2FLOAT4(m.specular));
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, COLOR2FLOAT4(m.ambient));
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, COLOR2FLOAT4(m.emissive));
    glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, m.shininess);
#if 0
    post("============== material ================");
    post("diffuse  = %f/%f/%f/%f", m.diffuse.r, m.diffuse.g, m.diffuse.b, m.diffuse.a);
    post("specular = %f/%f/%f/%f", m.specular.r, m.specular.g, m.specular.b, m.specular.a);
    post("ambient  = %f/%f/%f/%f", m.ambient.r, m.ambient.g, m.ambient.b, m.ambient.a);
    post("emissive = %f/%f/%f/%f", m.emissive.r, m.emissive.g, m.emissive.b, m.emissive.a);
    post("shininess= %f", m.shininess);
#endif
  }

  static void genTex_Linear(std::vector<float>& tex,
                                const std::vector<float>& pos)
  {
    tex.clear();
    const unsigned int size = pos.size()/3;
    for (unsigned int i=0; i<size; i++) {
      tex.push_back((pos[3*i+0] + 1.0) / 2.0);
      tex.push_back((pos[3*i+2] + 1.0) / 2.0);
    }
  }
  static void genTex_Spheremap(std::vector<float>& tex,
                                   const std::vector<float>& norm)

  {
    tex.clear();
    const unsigned int size = norm.size()/3;
    for (unsigned int i=0; i<size; i++) {
      float z = norm[3*i+0];  /* re-arrange for pole distortion */
      float y = norm[3*i+1];
      float x = norm[3*i+2];
      float r = (float)sqrt((x * x) + (y * y));
      float rho = (float)sqrt((r * r) + (z * z));
      float theta = 0.f, phi = 0.f;

      if(r == 0.0) {
        theta = 0.0f;
        phi = 0.0f;
      } else {
        if(z == 0.0) {
          phi = M_PI / 2.0f;
        } else {
          phi = (float)acos(z / rho);
        }

        if(y == 0.0) {
          theta = M_PI / 2.0f;
        } else {
          theta = (float)asin(y / r) + (M_PI / 2.0f);
        }
      }
      tex.push_back(theta / M_PI);
      tex.push_back(phi   / M_PI);
    }
  }


  /* a mesh of a model */
  struct modelmesh {
    unsigned int size;
    /* reference mesh */
    std::vector<float> vVertices, vColors;
    std::vector<float> vNormals;
    std::vector<float> vTexCoordsUV, vTexCoordsLinear, vTexCoordsSpheremap;

    /* processed data */
    std::vector<float> vTexCoords; /* scaled version of one of ...UV/Linear/Spheremap */

    /* VBOs */
    gem::VBO vertices, normals, colors, texcoords;
    gem::plugins::modelloader::material material;

    modelmesh(gem::plugins::modelloader::mesh*m)
      : size(m->size)
      , vertices(GL_VERTEX_ARRAY)
      , normals(GL_NORMAL_ARRAY)
      , colors(GL_COLOR_ARRAY)
      , texcoords(GL_TEXTURE_COORD_ARRAY)
      , material(m->material)
    {
      setvector(vVertices, m->vertices, m->size * 3);
      setvector(vNormals, m->normals, m->size * 3);
      setvector(vColors, m->colors, m->size * 4);

      /* generate linxear/spheremap texcoords */
      genTex_Linear(vTexCoordsLinear, vVertices);
      genTex_Spheremap(vTexCoordsSpheremap, vNormals);

      /* use UV texture if there is one, fallback to linear */
      if(m->texcoords) {
        setvector(vTexCoordsUV, m->texcoords, m->size * 2);
      } else {
        vTexCoordsUV = vTexCoordsLinear;
      }

    }
    void update(enum gem::modelGL::texturetype t, float texW, float texH)
    {
      vertices.update(size, vVertices.data());
      normals.update(size, vNormals.data());
      colors.update(size, vColors.data());

      float*texCoords = 0;
      switch(t) {
      case gem::modelGL::LINEAR:
        texCoords = vTexCoordsLinear.data();
        break;
      case gem::modelGL::SPHEREMAP:
        texCoords = vTexCoordsSpheremap.data();
        break;
      default:
      case gem::modelGL::UV:
        texCoords = vTexCoordsUV.data();
        break;
      }
      vTexCoords.resize(size * 2);
      if(texCoords) {
        for(unsigned int i=0; i<size; i++) {
          vTexCoords[2*i+0] = *texCoords++ * texW;
          vTexCoords[2*i+1] = *texCoords++ * texH;
        }
      }
      texcoords.update(size, vTexCoords.data());
    }
    void render(GLenum drawType) const
    {
      std::vector<size_t>sizes;
      size_t sizeV=0, sizeN=0, sizeC=0, sizeT=0;
      sizeV = vertices.render();
      if(sizeV>0)sizes.push_back(sizeV);
      sizeN = normals.render();
      if(sizeN>0)sizes.push_back(sizeN);
      sizeC = colors.render();
      if(sizeC>0)sizes.push_back(sizeC);
      sizeT = texcoords.render();
      if(sizeT>0)sizes.push_back(sizeT);

      if ( sizes.size() > 0 ) {
        unsigned int npoints = *std::min_element(sizes.begin(),sizes.end());
        glDrawArrays(drawType, 0, npoints);
      }

      if(sizeV)glDisableClientState(GL_VERTEX_ARRAY);
      if(sizeN)glDisableClientState(GL_NORMAL_ARRAY);
      if(sizeC)glDisableClientState(GL_COLOR_ARRAY);
      if(sizeT)glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }
    void bbox(bbox&bb, bool initialize=false) const {
      const float*vert = vVertices.data();
      if(initialize) {
        if(size) {
          bb.minX = bb.maxX = vert[0];
          bb.minY = bb.maxY = vert[1];
          bb.minZ = bb.maxZ = vert[2];
        } else {
          bb.minX = bb.maxX = 0;
          bb.minY = bb.maxY = 0;
          bb.minZ = bb.maxZ = 0;
        }
      }

      for(unsigned int i=0; i<size; i++) {
        float x=*vert++;
        float y=*vert++;
        float z=*vert++;

        if(x<bb.minX)bb.minX=x; if(x>bb.maxX)bb.maxX=x;
        if(y<bb.minY)bb.minY=y; if(y>bb.maxY)bb.maxY=y;
        if(z<bb.minZ)bb.minZ=z; if(z>bb.maxZ)bb.maxZ=z;
      }
    }
  };


};

namespace gem {
  class modelGL :: PIMPL {
  public:
    std::vector<struct modelmesh>mesh;

    bool update;
    unsigned int drawType;
    bool useMaterial;
    float texScale[2];
    enum modelGL::texturetype texType;
    enum modelGL::rescale rescale;
    float offset[3];
    float scale;

    PIMPL()
      : update(true)
      , drawType(GL_TRIANGLES)
      , useMaterial(false)
      , texScale{1.0, 1.0}
      , texType(LINEAR)
      , rescale(NORMALIZE_CENTER)
      , offset{0.0, 0.0, 0.0}
      , scale(1.0)
      {
        ; }

    bbox_t bbox(void) {
      bbox_t bb;
      bool first=true;
      for (auto&m: mesh) {
        m.bbox(bb, first);
        first = false;
      }
      return bb;
    }
  };


  modelGL::modelGL(gem::plugins::modelloader&loader)
    : m_pimpl(new PIMPL())
  {
    const size_t nummeshes = loader.getNumMeshes();
    for(size_t n=0; n < nummeshes; n++) {
      gem::plugins::modelloader::mesh*mesh = loader.getMesh(n);
      if(mesh) {
        m_pimpl->mesh.push_back(modelmesh(mesh));
      }
    }
    bbox_t bb = m_pimpl->bbox();
    float maxdim = bb.maxX-bb.minX;
    maxdim = max(maxdim, bb.maxY-bb.minY);
    maxdim = max(maxdim, bb.maxZ-bb.minZ);
    if(maxdim<=0.)maxdim = 2.;
    m_pimpl->scale = 2.f / maxdim;

    m_pimpl->offset[0] = ((bb.minX+bb.maxX) / -2.);
    m_pimpl->offset[1] = ((bb.minY+bb.maxY) / -2.);
    m_pimpl->offset[2] = ((bb.minZ+bb.maxZ) / -2.);
  }
  modelGL::~modelGL(void)
  {
    delete m_pimpl;
  }

  bool modelGL::update(void) {
    m_pimpl->update = false;
    if(true) {
      if(GLEW_VERSION_1_5) {
        /* update the VBOs */
        for (auto&m: m_pimpl->mesh) {
          m.update(m_pimpl->texType, m_pimpl->texScale[0], m_pimpl->texScale[1]);
#if 0
          const gem::plugins::modelloader::material&mat = m.material;
          post("============== material ================");
          post("diffuse  = %f/%f/%f/%f", mat.diffuse.r, mat.diffuse.g, mat.diffuse.b, mat.diffuse.a);
          post("specular = %f/%f/%f/%f", mat.specular.r, mat.specular.g, mat.specular.b, mat.specular.a);
          post("ambient  = %f/%f/%f/%f", mat.ambient.r, mat.ambient.g, mat.ambient.b, mat.ambient.a);
          post("emissive = %f/%f/%f/%f", mat.emissive.r, mat.emissive.g, mat.emissive.b, mat.emissive.a);
          post("shininess= %f", mat.shininess);
#endif
        }
      }
      return true;
    }
    return false;
  }
  void modelGL::render(void) {
    const unsigned int count = m_pimpl->mesh.size();
    std::vector<unsigned int>groups;
    for (unsigned int g=0; g<count; g++) {
      groups.push_back(g);
    }
    render(groups);
  }
  void modelGL::render(std::vector<unsigned int>&meshes) {
    if(m_pimpl->update) update();

    const unsigned int numMeshes = m_pimpl->mesh.size();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    if(m_pimpl->rescale & NORMALIZE) {
      GLfloat f = m_pimpl->scale;
      glScalef(f, f, f);
    }
    if(m_pimpl->rescale & CENTER) {
      glTranslatef(m_pimpl->offset[0], m_pimpl->offset[1], m_pimpl->offset[2]);
    }

    if(!GLEW_VERSION_1_5) {
      for(auto n: meshes) {
        if(n >= numMeshes)
          continue;
        const auto &m = m_pimpl->mesh[n];
        const size_t size = m.size;
        const float*positions = size?m.vVertices.data():0;
        const float*textures = size?m.vTexCoords.data():0;
        const float*colors = size?m.vColors.data():0;
        const float*normals = size?m.vNormals.data():0;

        if(m_pimpl->useMaterial) {
          render_material(m.material);
        }

        glBegin(m_pimpl->drawType);
        for (unsigned int i=0; i<size; i++) {
          if(normals) {
            glNormal3fv(normals);
            normals += 3;
          }
          if(textures) {
            glTexCoord2fv(textures);
            textures += 2;
          }
          if(colors) {
            glColor4fv(colors);
            colors += 4;
          }
          if(positions) {
            glVertex3fv(positions);
            positions += 3;
          }
        }
        glEnd();
      }
    } else { /* openGL-2+ */
      for(auto n: meshes) {
        if (n >= numMeshes) continue;
        const auto&m = m_pimpl->mesh[n];
        if(m_pimpl->useMaterial) {
          render_material(m.material);
        }
        m.render(m_pimpl->drawType);
      }
    }
    glPopMatrix();
  }

  void modelGL :: setDrawType(GLenum drawtype) {
    m_pimpl->drawType = drawtype;
  }
  void modelGL :: useMaterial(bool use) {
    m_pimpl->useMaterial = use;
  }
  void modelGL :: setTexture(float w, float h) {
    if((m_pimpl->texScale[0] != w) || (m_pimpl->texScale[1] != h))
      m_pimpl->update = true;

    m_pimpl->texScale[0] = w;
    m_pimpl->texScale[1] = h;
  }
  void modelGL :: setTextureType(enum texturetype t) {
    if (m_pimpl->texType != t)
      m_pimpl->update = true;
    m_pimpl->texType = t;
  }
  void modelGL :: setRescale(enum rescale r) {
    m_pimpl->rescale = r;
  }


};
