////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.at
//
// Implementation file
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
#include "scopeXYZ~.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

CPPEXTERN_NEW_NAMED_WITH_ONE_ARG(scopeXYZ, "scopeXYZ~", t_floatarg, A_DEFFLOAT);

/////////////////////////////////////////////////////////
//
// scopeXYZ
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
scopeXYZ :: scopeXYZ(t_floatarg len)
  : GemShape()
  , m_blocksize(64), m_length(64)
  , m_position(0)
  , m_vertices(64, 3)
{
  lengthMess(static_cast<int>(len));

  /* channels inlet */
  int i;
  for (i=0; i<3; i++) {
    inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_signal, &s_signal);
  }

}

////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
scopeXYZ :: ~scopeXYZ(void)
{
}

void scopeXYZ :: doLengthMess(unsigned int l)
{
  if(l < m_blocksize) {
    l = m_blocksize;
  }
  m_length=l;

  m_vertices.resize(2 * m_length);
  m_position%=m_length;
}
void scopeXYZ :: lengthMess(int l)
{
  if(l<=0) {
    return;
  }
  doLengthMess(l);
}
void scopeXYZ :: setBlocksize(unsigned int bs)
{
  m_blocksize = bs;
  m_vertices.enabled = true;
  doLengthMess(m_length);
}

/////////////////////////////////////////////////////////
// renderShape
//
/////////////////////////////////////////////////////////
void scopeXYZ :: renderShape(GemState *state)
{
  float*vertices=m_vertices.array+3*m_position;
  int count=m_length/2;
  GLenum typ=GL_FLOAT;
  GLenum drawtype=m_drawType;
  if(drawtype==GL_DEFAULT_GEM) {
    drawtype=GL_LINE_STRIP;
  }
  glNormal3f(0.0f, 0.0f, 1.0f);
  glLineWidth(m_linewidth);

  if(GLEW_VERSION_1_5) {
    if ( !m_vertices.vbo || m_vertices.dirty ) {
      m_vertices.create();
    }
    if(m_vertices.render()) {
      glVertexPointer(m_vertices.dimen, GL_FLOAT, 0, 0);
      glEnableClientState(GL_VERTEX_ARRAY);

      if(GemShape::m_texType) {
        m_vertices.render();
        if(count==GemShape::m_texNum && false) {
          /* disabled for now, as we have a bound buffer object
           * and thus the 4th arg is not really a pointer...
           */
          glTexCoordPointer(2, GL_FLOAT, 0, GemShape::m_texCoords);
        } else {
          glTexCoordPointer(2, GL_FLOAT, sizeof(float), 0);
        }
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
      }


      glDrawArrays(drawtype, m_position, m_length);

      glDisableClientState(GL_VERTEX_ARRAY);
      if(GemShape::m_texType) {
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
      }
    }

  } else { /* legacy code for old openGL */
    // activate and specify pointer to vertex array
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);

    if(GemShape::m_texType) {
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);
      if(count==GemShape::m_texNum) {
        // in the original code, we used whatever GemShape::m_texCoords were present
        // if we had more vertices, the remaining vs would just use the last texCoord available
        glTexCoordPointer(2, GL_FLOAT, 0, GemShape::m_texCoords);
      } else {
        // in the original code, the texcoords where normalized(!)
        glTexCoordPointer(2, GL_FLOAT, sizeof(float), vertices);
      }
    }

    // draw a cube
    glDrawArrays(drawtype, 0, m_length);

    // deactivate vertex arrays after drawing
    glDisableClientState(GL_VERTEX_ARRAY);

    if(GemShape::m_texType) {
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    }
  }

  glLineWidth(1.0);
}

void scopeXYZ :: bangMess(void)
{
  unsigned int i;
  auto vertL=m_vertices.array;
  auto vertR=m_vertices.array+m_length*m_vertices.dimen;

  post("x\ty\tz\t\tx\ty\tz");
  for(i=0; i<m_length; i++) {
    post("%f\t%f\t%f\t\t%f\t%f\t%f",
         vertL[i*3+0],
         vertL[i*3+1],
         vertL[i*3+2],

         vertR[i*3+0],
         vertR[i*3+1],
         vertR[i*3+2]);
  }
}


/////////////////////////////////////////////////////////
// linewidthMess
//
/////////////////////////////////////////////////////////
void scopeXYZ :: linewidthMess(float linewidth)
{
  m_linewidth = (linewidth < 0.0f) ? 0.0f : linewidth;
  setModified();
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void scopeXYZ :: obj_setupCallback(t_class *classPtr)
{
  CPPEXTERN_MSG0(classPtr, "bang", bangMess);
  CPPEXTERN_MSG1(classPtr, "linewidth", linewidthMess, float);
  CPPEXTERN_MSG1(classPtr, "length", lengthMess, int);

  class_addmethod(classPtr,
      reinterpret_cast<t_method>(&scopeXYZ::dspCallback),
      gensym("dsp"), A_CANT, A_NULL);
  class_addmethod(classPtr, nullfn, gensym("signal"), A_NULL);
}
void scopeXYZ ::  dspCallback(void *data,t_signal** sp)
{
  GetMyClass(data)->setBlocksize(sp[1]->s_n);
  dsp_add(perform, 5, data, sp[1]->s_vec, sp[2]->s_vec, sp[3]->s_vec,
          sp[1]->s_n);
}

void scopeXYZ :: perform(unsigned int count, t_sample*X, t_sample*Y,
                         t_sample*Z)
{
  int position=m_position;
  float*vertices = m_vertices.array;
  float*vert=vertices+3*position;

  unsigned int i=0;

  // TODO: add some protection against segfaults when buffer is very small
  if(m_length<count) {
    count=m_length;
  }

  /* fill in the left-side of the double-array */
  for(i=0; i<count; i++) {
    *vert++=*X++;
    *vert++=*Y++;
    *vert++=*Z++;
  }

  /* copy the data over to the right-side of the double-array */
  if((m_position+count)>m_length) {
    // wrap around!
    // 1st copy the lower half
    memcpy(vertices+3*(position+m_length),
           vertices+3*(position),
           3*(m_length-m_position)*sizeof(t_sample));
    // then the upper half
    memcpy(vertices,
           vertices+3*(m_length),
           3*(m_position+count-m_length)*sizeof(t_sample));

  } else {
    // ordinary copy

    memcpy(vertices+3*(position+m_length),
           vertices+3*(position),
           3*count*sizeof(t_sample));
  }
  m_position=(m_position+count)%m_length;
  m_vertices.dirty = true;
  setModified();
}

t_int* scopeXYZ :: perform(t_int* w)
{
  int index=1;
  scopeXYZ *x = GetMyClass(reinterpret_cast<void*>(w[index++]));
  t_sample* in_X = reinterpret_cast<t_sample*>(w[index++]);
  t_sample* in_Y = reinterpret_cast<t_sample*>(w[index++]);
  t_sample* in_Z = reinterpret_cast<t_sample*>(w[index++]);

  t_int n = (w[index++]);

  x->perform(n, in_X, in_Y, in_Z);

  return (w+index);
}
