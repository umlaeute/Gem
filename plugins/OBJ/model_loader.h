/*
      glm.h
      Nate Robins, 1997, 2000
      nate@pobox.com, http://www.pobox.com/~nate

      Wavefront OBJ model file format reader/writer/manipulator.

      Includes routines for generating smooth normals with
      preservation of edges, welding redundant vertices & texture
      coordinate generation (spheremap and planar projections) + more.

 */
#ifndef GLM_H
#define GLM_H


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string>
#include <assert.h>

#ifdef __ppc__
#include "Utils/Functions.h"
#undef sqrt
#define sqrt fast_sqrtf
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include "Gem/GemGL.h"

#ifndef M_PI
#define M_PI 3.14159265f
#endif

#define GLM_NONE     (0)            /* render with only vertices */
#define GLM_FLAT     (1 << 0)       /* render with facet normals */
#define GLM_SMOOTH   (1 << 1)       /* render with vertex normals */
#define GLM_TEXTURE  (1 << 2)       /* render with texture coords */
#define GLM_COLOR    (1 << 3)       /* render with colors */
#define GLM_MATERIAL (1 << 4)       /* render with materials */

/*
 * types of texturing the model
 */
typedef enum {
  GLM_TEX_DEFAULT,
  GLM_TEX_UV,
  GLM_TEX_LINEAR,
  GLM_TEX_SPHEREMAP
}  glmtexture_t;

typedef struct _GLMmodel GLMmodel;

/* GLMmaterial: Structure that defines a material in a model.
 */
typedef struct _GLMmaterial {
  std::string name;                   /* name of material */
  GLfloat diffuse[4];           /* diffuse component */
  GLfloat ambient[4];           /* ambient component */
  GLfloat specular[4];          /* specular component */
  // GLfloat emmissive[4];         /* emmissive component */
  GLfloat shininess;            /* specular exponent */
} GLMmaterial;


/* glmUnitize: "unitize" a model by translating it to the origin and
 * scaling it to fit in a unit cube around the origin.  Returns the
 * scalefactor used.
 *
 * model - properly initialized GLMmodel structure
 */
GLfloat
glmUnitize(GLMmodel* model);

/* glmDimensions: Calculates the dimensions (width, height, depth) of
 * a model.
 *
 * model      - initialized GLMmodel structure
 * dimensions - array of 3 GLfloats (GLfloat dimensions[3])
 */
GLvoid
glmDimensions(const GLMmodel* model, GLfloat* dimensions);

/* glmScale: Scales a model by a given amount.
 *
 * model - properly initialized GLMmodel structure
 * scale - scalefactor (0.5 = half as large, 2.0 = twice as large)
 */
GLvoid
glmScale(GLMmodel* model, GLfloat scale);

/* glmReverseWinding: Reverse the polygon winding for all polygons in
 * this model.  Default winding is counter-clockwise.  Also changes
 * the direction of the normals.
 *
 * model - properly initialized GLMmodel structure
 */
GLvoid
glmReverseWinding(GLMmodel* model);

/* glmFacetNormals: Generates facet normals for a model (by taking the
 * cross product of the two vectors derived from the sides of each
 * triangle).  Assumes a counter-clockwise winding.
 *
 * model - initialized GLMmodel structure
 */
GLvoid
glmFacetNormals(GLMmodel* model);

/* glmVertexNormals: Generates smooth vertex normals for a model.
 * First builds a list of all the triangles each vertex is in.  Then
 * loops through each vertex in the the list averaging all the facet
 * normals of the triangles each vertex is in.  Finally, sets the
 * normal index in the triangle for the vertex to the generated smooth
 * normal.  If the dot product of a facet normal and the facet normal
 * associated with the first triangle in the list of triangles the
 * current vertex is in is greater than the cosine of the angle
 * parameter to the function, that facet normal is not added into the
 * average normal calculation and the corresponding vertex is given
 * the facet normal.  This tends to preserve hard edges.  The angle to
 * use depends on the model, but 90 degrees is usually a good start.
 *
 * model - initialized GLMmodel structure
 * angle - maximum angle (in degrees) to smooth across
 */
GLvoid
glmVertexNormals(GLMmodel* model, GLfloat angle);

/* glmTexture: setup texture coordinates according to the specified type
 * some types (like UV) are read from files, whereas others might be generated
 * a "default" type will try to use saved texcoords and fallback to generated
 *
 * model - pointer to initialized GLMmodel structure
 */
GLvoid
glmTexture(GLMmodel* model, glmtexture_t type=GLM_TEX_DEFAULT, float h=1.0,
           float w=1.0);


/*
 * glmGetNumVertices: get number of vertices in the model
 *
 * model - pointer to initialized GLMmodel structure
 */
GLuint
glmGetNumVertices(const GLMmodel *model);
/*
 * glmGetNumNormals: get number of normals in the model
 *
 * model - pointer to initialized GLMmodel structure
 */
GLuint
glmGetNumNormals(const GLMmodel *model);
/*
 * glmGetNumTexCoords: get number of texcoords in the model
 *
 * model - pointer to initialized GLMmodel structure
 */
GLuint
glmGetNumTexCoords(const GLMmodel *model);
/*
 * glmGetNumFacetNorms: get number of facetnorms in the model
 *
 * model - pointer to initialized GLMmodel structure
 */
GLuint
glmGetNumFacetNorms(const GLMmodel *model);
/*
 * glmGetNumTriangles: get number of triangles in the model
 *
 * model - pointer to initialized GLMmodel structure
 */
GLuint
glmGetNumTriangles(const GLMmodel *model);
/*
 * glmGetNumMaterials: get number of materials in the model
 *
 * model - pointer to initialized GLMmodel structure
 */
GLuint
glmGetNumMaterials(const GLMmodel *model);
/*
 * glmGetNumGroups: get number of groups in the model
 *
 * model - pointer to initialized GLMmodel structure
 */
GLuint
glmGetNumGroups(const GLMmodel *model);


/* glmDelete: Deletes a GLMmodel structure.
 *
 * model - initialized GLMmodel structure
 */
GLvoid
glmDelete(GLMmodel* model);

/* glmReadOBJ: Reads a model description from a Wavefront .OBJ file.
 * Returns a pointer to the created object which should be free'd with
 * glmDelete().
 *
 * filename - name of the file containing the Wavefront .OBJ format data.
 */
GLMmodel*
glmReadOBJ(const char* filename);

/* glmWriteOBJ: Writes a model description in Wavefront .OBJ format to
 * a file.
 *
 * model    - initialized GLMmodel structure
 * filename - name of the file to write the Wavefront .OBJ format data to
 * mode     - a bitwise or of values describing what is written to the file
 *            GLM_NONE    -  write only vertices
 *            GLM_FLAT    -  write facet normals
 *            GLM_SMOOTH  -  write vertex normals
 *            GLM_TEXTURE -  write texture coords
 *            GLM_FLAT and GLM_SMOOTH should not both be specified.
 */
GLint
glmWriteOBJ(const GLMmodel* model, const char* filename, GLuint mode);

/* glmDraw: "Renders" the model to a generic mesh structure
 * using the mode specified.
 *
 * model    - initialized GLMmodel structure
 * group    - which group to render
 * mode     - a bitwise OR of values describing what is to be rendered.
 *            GLM_NONE    -  render with only vertices
 *            GLM_FLAT    -  render with facet normals
 *            GLM_SMOOTH  -  render with vertex normals
 *            GLM_TEXTURE -  render with texture coords
 *            GLM_FLAT and GLM_SMOOTH should not both be specified.
 * vertices: will hold 3D-vectors (x,y,z,x,y,z,...)
 * normals : will hold 3D-vectors (x,y,z,x,y,z,...)
 * texcoords: will hold "d coords (u,v,u,v,...)
 * colors: will hold 4D coords (r,g,b,a,r,g,b,a,...)
 *
 * material: will hold a pointer to the group's material (or NULL)
 */
bool
glmGroupData(const GLMmodel* model, struct _GLMgroup*group, GLuint mode,
        std::vector<float>& vertices,
        std::vector<float>& normals,
        std::vector<float>& texcoords,
        GLMmaterial**material);

  /* glmGetGroup: get a reference to the numbered group
   * if <group> is out of range, return nullptr
   */
struct _GLMgroup*
glmGetGroup(const GLMmodel* model, GLuint group);


/* glmWeld: eliminate (weld) vectors that are within an epsilon of
 * each other.
 *
 * model      - initialized GLMmodel structure
 * epsilon    - maximum difference between vertices
 *              ( 0.00001 is a good start for a unitized model)
 *
 */
GLvoid
glmWeld(GLMmodel* model, GLfloat epsilon);

/* glmReadPPM: read a PPM raw (type P6) file.  The PPM file has a header
 * that should look something like:
 *
 *    P6
 *    # comment
 *    width height max_value
 *    rgbrgbrgb...
 *
 * where "P6" is the magic cookie which identifies the file type and
 * should be the only characters on the first line followed by a
 * carriage return.  Any line starting with a # mark will be treated
 * as a comment and discarded.   After the magic cookie, three integer
 * values are expected: width, height of the image and the maximum
 * value for a pixel (max_value must be < 256 for PPM raw files).  The
 * data section consists of width*height rgb triplets (one byte each)
 * in binary format (i.e., such as that written with fwrite() or
 * equivalent).
 *
 * The rgb data is returned as an array of unsigned chars (packed
 * rgb).  The malloc()'d memory should be free()'d by the caller.  If
 * an error occurs, an error message is sent to stderr and NULL is
 * returned.
 *
 * filename   - name of the .ppm file.
 * width      - will contain the width of the image on return.
 * height     - will contain the height of the image on return.
 *
 */
GLubyte*
glmReadPPM(const char* filename, int* width, int* height);

#ifdef __cplusplus
}
#endif

#endif /* TD_H */
