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
#if 0
    glNormal3f(0.0f, 0.0f, 1.0f);
    if (m_drawType == GL_LINE_LOOP)
        glLineWidth(m_linewidth);

    if (GemShape::m_texType && GemShape::m_texNum)
    {
        int curCoord = 0;
	    glBegin(m_drawType);
	        glTexCoord2f(GemShape::m_texCoords[curCoord].s, GemShape::m_texCoords[curCoord].t);
   	        glVertex3f(-m_size, -m_size, 0.f);

	        if (GemShape::m_texNum > 1) curCoord = 1;
	    	glTexCoord2f(GemShape::m_texCoords[curCoord].s, GemShape::m_texCoords[curCoord].t);
   	        glVertex3f(m_size, -m_size, 0.f);

	        if (GemShape::m_texNum > 2) curCoord = 2;
	    	glTexCoord2f(GemShape::m_texCoords[curCoord].s, GemShape::m_texCoords[curCoord].t);
   	        glVertex3f(0.f,	m_size, 0.f);
	    glEnd();
    }
    else
    {
	    glBegin(m_drawType);
    	        glTexCoord2f(0.f, 0.f); glVertex3f(-m_size, -m_size, 0.f);
    	        glTexCoord2f(1.f, 0.f); glVertex3f( m_size, -m_size, 0.f);
    	        glTexCoord2f(.5f, 1.f); glVertex3f( 0.f,     m_size, 0.f);
	    glEnd();
    }
    if (m_drawType == GL_LINE_LOOP)
        glLineWidth(1.0);
#endif
#if 0
    if (m_drawType == GL_LINE_LOOP)
      glLineWidth(m_linewidth);
    if(!m_vertexbuffer) {
      // Generate 1 buffer, put the resulting identifier in vertexbuffer
      glGenBuffers(1, &m_vertexbuffer);

      // The following commands will talk about our 'vertexbuffer' buffer
      glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);

      // Give our vertices to OpenGL.
      glBufferData(GL_ARRAY_BUFFER, sizeof(s_vertex_buffer_data), s_vertex_buffer_data, GL_STATIC_DRAW);
      post("VBO=%d", m_vertexbuffer);
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glEnableClientState(GL_VERTEX_ARRAY);

    // Draw the triangle !
    glDrawArrays(m_drawType, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle

    glDisableClientState(GL_VERTEX_ARRAY);
    if (m_drawType == GL_LINE_LOOP)
      glLineWidth(1.0);
#endif
#if 1
    if(!m_vertexArrayID) {
      glGenVertexArrays(1, &m_vertexArrayID);
      glBindVertexArray(m_vertexArrayID);
      post("array=%d", m_vertexArrayID);
    }
    if(!m_vertexbuffer) {
      // Generate 1 buffer, put the resulting identifier in vertexbuffer
      glGenBuffers(1, &m_vertexbuffer);
      glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
      glBufferData(GL_ARRAY_BUFFER, sizeof(s_vertex_buffer_data), s_vertex_buffer_data, GL_STATIC_DRAW);
      post("VBO=%d", m_vertexbuffer);
    }
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
    glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle

    glDisableVertexAttribArray(0);
#endif
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void triangle :: obj_setupCallback(t_class *)
{ }

