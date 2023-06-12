/*-----------------------------------------------------------------
  LOG
  GEM - Graphics Environment for Multimedia

  plain C-style data-structure for modelloaders to return the data

  this is heavily informed by assimp's aiScene structure
  (but somewhat simplified).
  **LATER** we might extend it to include bone & animation data

  Copyright (c) 2023 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

  -----------------------------------------------------------------*/

#ifndef _INCLUDE__GEM_MODELDATA_H_
#define _INCLUDE__GEM_MODELDATA_H_

namespace gem
{
struct modeldata {
  struct matrix4x4 {
    float a11, a12, a13, a14;
    float a21, a22, a23, a24;
    float a31, a32, a33, a34;
    float a41, a42, a43, a44;
  };
  struct coordinate_f {
    float x, y, z;
  };
  struct color_f {
    float r, g, b, a;
  };
  struct texcoord_f {
    float u, v;
  };
  struct triangle_i {
    int a, b, c;
  };

  struct material {
    struct color_f diffuse;
    struct color_f specular;
    struct color_f ambient;
    struct color_f emissive;

    float shininess;
  };
  struct mesh {

    /* per vertex data */
    unsigned int numVertices;
    struct coordinate_f*vertices;
    struct coordinate_f*normals;
    struct color_f*colors;
    struct texcoord_f*texcoords;

    /* faces bult from vertices (we only accept triangles) */
    unsigned int numFaces;
    struct triangle_i*faces; /* triangles */

    /* material data */
    struct material material;
  };
  struct node {
    /* a single node in the model hierarchy
     * may consist of multiple meshes and sub-nodes.
     * all of them have a common transformation matrix
     */
    const char*name;

    matrix4x4 transformation;

    unsigned int numChildren;
    struct node*children;

    unsigned int numMeshes;
    unsigned int*meshes; /* index to modeldata::meshes */
  };

  /* the actual scene */
  struct node*root;

  unsigned int numMeshes;
  struct mesh*meshes;
};
};

#endif  // for header file
