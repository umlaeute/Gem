////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-2000 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "triangle.h"
#include <string.h>

#include "Gem/State.h"

#define RENDER_STYLE_VERTEX 0
#define RENDER_STYLE_VERTEX_BUFFER 1
/* generic vertex arrays need a matching shader to actually do the drawing...:: */
#define RENDER_STYLE_VERTEX_ARRAY  2

#define RENDER_STYLE 0

static const GLfloat s_vertex_buffer_data[] = {
  -1.0f, -1.0f, 0.0f,
  1.0f, -1.0f, 0.0f,
  0.0f,  1.0f, 0.0f,
};

CPPEXTERN_NEW_WITH_ONE_ARG(triangle, t_floatarg, A_DEFFLOAT);

/////////////////////////////////////////////////////////
//
// triangle
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
triangle :: triangle(t_floatarg size)
  : GemShape(size),
    m_vertexbuffer(0),
    m_vertexArrayID(0)
{
    m_drawType = GL_TRIANGLES;

#if 0
# if RENDER_STYLE ==  RENDER_STYLE_VERTEX
    post("triangle: vertex");
# elif RENDER_STYLE == RENDER_STYLE_VERTEX_BUFFER
    post("triangle: vertex buffer");
# elif RENDER_STYLE == RENDER_STYLE_VERTEX_ARRAY
    post("triangle: vertex array");
# else
#  error render_style
# endif
#endif
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
triangle :: ~triangle()
{ }

/////////////////////////////////////////////////////////
// renderShape
//
/////////////////////////////////////////////////////////
void triangle :: renderShape(GemState *state)
{
  if(m_drawType==GL_DEFAULT_GEM)m_drawType=GL_TRIANGLES;
#if RENDER_STYLE == RENDER_STYLE_VERTEX
    glPushMatrix();
    glScalef(m_size, m_size, m_size);

    glNormal3f(0.0f, 0.0f, 1.0f);
    if (m_drawType == GL_LINE_LOOP)
        glLineWidth(m_linewidth);

    if (GemShape::m_texType && GemShape::m_texNum)
    {
        int curCoord = 0;
	    glBegin(m_drawType);
	        glTexCoord2f(GemShape::m_texCoords[curCoord].s, GemShape::m_texCoords[curCoord].t);
		glVertex3fv(s_vertex_buffer_data+0);

	        if (GemShape::m_texNum > 1) curCoord = 1;
	    	glTexCoord2f(GemShape::m_texCoords[curCoord].s, GemShape::m_texCoords[curCoord].t);
		glVertex3fv(s_vertex_buffer_data+3);

	        if (GemShape::m_texNum > 2) curCoord = 2;
	    	glTexCoord2f(GemShape::m_texCoords[curCoord].s, GemShape::m_texCoords[curCoord].t);
		glVertex3fv(s_vertex_buffer_data+6);
	    glEnd();
    }
    else
    {
	    glBegin(m_drawType);
	        glTexCoord2f(0.f, 0.f); glVertex3fv(s_vertex_buffer_data+0);
	        glTexCoord2f(1.f, 0.f); glVertex3fv(s_vertex_buffer_data+3);
	        glTexCoord2f(.5f, 1.f); glVertex3fv(s_vertex_buffer_data+6);
	    glEnd();
    }
    if (m_drawType == GL_LINE_LOOP)
        glLineWidth(1.0);
    glPopMatrix();
#elif RENDER_STYLE == RENDER_STYLE_VERTEX_BUFFER
    glPushMatrix();
    glScalef(m_size, m_size, m_size);
    if (m_drawType == GL_LINE_LOOP)
      glLineWidth(m_linewidth);

    GLuint vb0=m_vertexbuffer;
    post("m_vertexbuffer=%d", vb0);

    if(!m_vertexbuffer) {
      // Generate 1 buffer, put the resulting identifier in vertexbuffer
      GLuint vb=m_vertexbuffer;
      glGenBuffers(1, &vb);
      m_vertexbuffer=vb;

      // The following commands will talk about our 'vertexbuffer' buffer
      glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);

      // Give our vertices to OpenGL.
      glBufferData(GL_ARRAY_BUFFER, sizeof(s_vertex_buffer_data), s_vertex_buffer_data, GL_STATIC_DRAW);
      // post("VBO=%d", m_vertexbuffer);
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glEnableClientState(GL_VERTEX_ARRAY);

    // Draw the triangle !
     glDrawArrays(m_drawType, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle

    glDisableClientState(GL_VERTEX_ARRAY);
    if (m_drawType == GL_LINE_LOOP)
      glLineWidth(1.0);
    glPopMatrix();
#elif RENDER_STYLE == RENDER_STYLE_VERTEX_ARRAY
    if(!m_vertexArrayID) {
      glGenVertexArrays(1, &m_vertexArrayID);
      glBindVertexArray(m_vertexArrayID);
      //post("array=%d", m_vertexArrayID);
    }
    if(!m_vertexbuffer) {
      // Generate 1 buffer, put the resulting identifier in vertexbuffer
      glGenBuffers(1, &m_vertexbuffer);
      glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
      glBufferData(GL_ARRAY_BUFFER, sizeof(s_vertex_buffer_data), s_vertex_buffer_data, GL_STATIC_DRAW);
      //post("VBO=%d", m_vertexbuffer);
    }
    glBindVertexArray(m_vertexArrayID);

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
    glVertexAttribPointer(
                          0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
                          3,                  // size
                          GL_FLOAT,           // type
                          GL_FALSE,           // normalized?
                          0,                  // stride
                          (void*)0            // array buffer offset
                          );

    // Draw the triangle !
    glDrawArrays(m_drawType, 0, 3); // 3 indices starting at 0 -> 1 triangle

    glDisableVertexAttribArray(0);

    glBindVertexArray(0);

#else
    #error no render type defined
#endif
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void triangle :: obj_setupCallback(t_class *)
{ }

