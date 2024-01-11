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

namespace
{
  template <typename T>
  void setvector(std::vector<T>&vec, T*data, size_t size) {
    if(data && size)
      vec.insert(vec.end(), data, data+size);
    else
      vec.clear();
  }
  struct modelmesh {
    unsigned int size;
    std::vector<float> vVertices, vNormals, vColors, vTexcoords;
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
      setvector(vTexcoords, m->texcoords, m->size * 2);
    }
    void update(void)
    {
      vertices.update(size, vVertices.data());
      normals.update(size, vNormals.data());
      colors.update(size, vColors.data());
      texcoords.update(size, vTexcoords.data());
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
  };


};

namespace gem {
  class modelGL :: PIMPL {
  public:
    std::vector<struct modelmesh>mesh;

    unsigned int drawType;
    bool useMaterial;
    float texScale[2];

    PIMPL()
      : drawType(GL_TRIANGLES)
      , useMaterial(false)
      , texScale{1.0, 1.0}
    { ; }
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
  }
  bool modelGL::update(void) {
    if(true) {
      if(GLEW_VERSION_1_5) {
        /* update the VBOs */
        for (auto&m: m_pimpl->mesh) {
          m.update();
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
    const unsigned int numMeshes = m_pimpl->mesh.size();
    if(!GLEW_VERSION_1_5) {
      for(auto n: meshes) {
        if(n >= numMeshes)
          continue;
        const auto &m = m_pimpl->mesh[n];
        const size_t size = m.size;
        const float*positions = size?m.vVertices.data():0;
        const float*textures = size?m.vTexcoords.data():0;
        const float*colors = size?m.vColors.data():0;
        const float*normals = size?m.vNormals.data():0;

        if(m_pimpl->useMaterial) {
          gem::plugins::modelutils::render_material(m.material);
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
          gem::plugins::modelutils::render_material(m.material);
        }
        m.render(m_pimpl->drawType);
      }
    }
  }

  void modelGL :: setDrawType(GLenum drawtype) {
    m_pimpl->drawType = drawtype;
  }
  void modelGL :: useMaterial(bool use) {
    m_pimpl->useMaterial = use;
  }
  void modelGL :: setTexture(float w, float h) {
    m_pimpl->texScale[0] = w;
    m_pimpl->texScale[1] = h;
  }

};
