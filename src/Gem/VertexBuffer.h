/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2014, Antoine Villeret and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_GEM_VERTEXBUFFER_H_
#define _INCLUDE__GEM_GEM_VERTEXBUFFER_H_

#include "Gem/GemGL.h"
#include "Gem/ContextData.h"
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

  gem::ContextData<GLuint> vbo;
  float*array;
  bool dirty;
  bool enabled;
  GLuint attrib_index;
  std::string attrib_name;
  std::string attrib_array;
  unsigned int offset;

  enum Type {
    GEM_VBO_VERTICES=0,
    GEM_VBO_NORMALS,
    GEM_VBO_COLORS,
    GEM_VBO_TEXCOORDS,
  };
};
class VBO
{
private:
  gem::ContextData<GLuint> m_vbo;
  size_t m_size;
  unsigned char m_dimen;
  GLenum m_type;
  bool m_valid;
public:
  VBO(GLenum type, unsigned char dimen=0);

  /* all these functions must be called with a valid openGL context */
  int render(void) const;
  bool update(size_t, const float*);
  void destroy(void);

};
}; /* namespace: gem */
#endif // _INCLUDE__GEM_GEM_VERTEXBUFFER_H_
