/*-----------------------------------------------------------------
  LOG
  GEM - Graphics Environment for Multimedia

  VertexBuffer class

  Copyright (c) 1997-2000 Mark Danks. mark@danks.org
  Copyright (c) Günther Geiger. geiger@epy.co.at
  Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

  -----------------------------------------------------------------*/

#ifndef _INCLUDE__GEM_GEM_VERTEXBUFFER_H_
#define _INCLUDE__GEM_GEM_VERTEXBUFFER_H_

#include "Gem/GemGL.h"
#include <string>

namespace gem
{
class VertexBuffer
{
public:
  VertexBuffer();
  VertexBuffer(unsigned int size, unsigned int dimen);
  VertexBuffer(const VertexBuffer&);
  ~VertexBuffer(void);

  void resize(unsigned int);

  /* creates an openGL VBO (requires a valid context) */
  bool create(void);
  /* renders an openGL VBO (requires a valid context) */
  bool render(void);
  /* destroys an openGL VBO (requires a valid context) */
  void destroy(void);

  unsigned int size;
  unsigned int dimen;

  GLuint vbo;
  float*array;
  bool dirty;
  bool enabled;
  GLuint attrib_index;
  std::string attrib_name;
  std::string attrib_array;
  unsigned int offset;

  enum Type { GEM_VBO_VERTICES=0,
              GEM_VBO_NORMALS,
              GEM_VBO_TEXCOORDS,
              GEM_VBO_COLORS,
              GEM_VBO_AMBIENT,
              GEM_VBO_DIFFUSE,
              GEM_VBO_SPECULAR,
              GEM_VBO_SHININESS
            };
  Type type;
};
}; /* namespace: gem */
#endif // _INCLUDE__GEM_GEM_VERTEXBUFFER_H_
