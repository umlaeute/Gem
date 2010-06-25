////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "scopeXYZ.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Base/GemState.h"

#define MARK post("%s:%d:: %s", __FILE__, __LINE__, __FUNCTION__)

CPPEXTERN_NEW_WITH_ONE_ARG(scopeXYZ, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// scopeXYZ
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
scopeXYZ :: scopeXYZ(t_floatarg len)
  : GemShape(), 
    m_drawType(GL_LINE_STRIP),
    m_length(0), m_position(0),
    m_vertarray(NULL), m_vert(NULL)
{
  lengthMess(static_cast<int>(len));

  /* channels inlet */
  int i;
  for (i=0; i<3; i++)
    inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_signal, &s_signal);
}

////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
scopeXYZ :: ~scopeXYZ()
{
  if(m_vert)delete[]m_vert;
  if(m_vertarray)delete[]m_vertarray;
}

void scopeXYZ :: doLengthMess(int L) {
  // this resizes to m_length if this is set, or to L otherwise
  // actually, resizing is done to the double-size!

  int length=0;
  if(m_length>0)
    length=m_length;
  else if (L>0)
    length=L;

  if(0==length)return; // oops

  if(length*2==m_realLength) {
    // cool, no need to resize
    return;
  }

  if(m_vert)delete[]m_vert;
  if(m_vertarray)delete[]m_vertarray;


  m_realLength=length;
  length=m_realLength*2;

  m_vert = new t_sample*[length];
  m_vertarray = new t_sample[length*3];
  post("m_vertarray: %d*3 samples at %x", length, m_vertarray);

  int i;
  for (i = 0; i < length*3; i++)  {
    m_vertarray[i]=0.0f;
  }
  for (i = 0; i < length; i++)  {
    m_vert[i]=m_vertarray+3*i;
  }

  m_position=0;
}


void scopeXYZ :: lengthMess(int l)
{
  int i;
  if(l<=0){
    return;
  }

  m_length=l;
  doLengthMess();
}

/////////////////////////////////////////////////////////
// renderShape
//
/////////////////////////////////////////////////////////
void scopeXYZ :: renderShape(GemState *state)
{
  GLenum drawtype=m_drawType;
  if(drawtype==GL_DEFAULT_GEM)drawtype=GL_LINE_STRIP;
  glNormal3f(0.0f, 0.0f, 1.0f);
  glLineWidth(m_linewidth);


  // activate and specify pointer to vertex array
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, m_vertarray+3*m_position);

  if(state->texture) {
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    MARK;
    glTexCoordPointer(3, GL_FLOAT, 0, m_vertarray+3*m_position);
  }

  // draw a cube
  glDrawArrays(drawtype, 0, m_length);

  // deactivate vertex arrays after drawing
  glDisableClientState(GL_VERTEX_ARRAY);

  if(state->texture) {
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  }


#if 0
  if (state->texture && state->numTexCoords) {
    // texture and texCoords present
    glBegin(drawtype);
    for (int i = 0; i < m_length; i++) {
      if (state->numTexCoords < i)
        glTexCoord2f(state->texCoords[state->numTexCoords - 1].s,
                     state->texCoords[state->numTexCoords - 1].t);
      else
        glTexCoord2f(state->texCoords[i].s,
                     state->texCoords[i].t);
      glVertex3fv(m_vert[i]);
    }
    glEnd();
  } else {
    if (state->texture) {
      // texture present (but no texcoords)
      // generate texcoords automatically from x/y coords
      float maxVal[2];
      maxVal[0] = maxVal[1] = 0.f;

      if(maxVal[0]<0)maxVal[0]*=-1.f;
      if(maxVal[1]<0)maxVal[1]*=-1.f;

      for (int i = 0; i < m_length; i++) {
        for (int j = 0; j < 2; j++) {
          if (m_vert[i][j] < 0) {
            if (-m_vert[i][j] > maxVal[j])
              maxVal[j] = -m_vert[i][j];
          } else {
            if (m_vert[i][j] > maxVal[j])
              maxVal[j] = m_vert[i][j];
          }
        }
      }
      glBegin(drawtype);
      for(int n=0; n < m_length; n++) {
        glTexCoord2f(m_vert[n][0] / maxVal[0],
                     m_vert[n][1] / maxVal[1]);
        glVertex3fv(m_vert[n]);
      }
      glEnd();
    } else {
      // activate and specify pointer to vertex array
      glEnableClientState(GL_VERTEX_ARRAY);
      glVertexPointer(3, GL_FLOAT, 0, m_vertarray+3*m_position);

      // draw a cube
      glDrawArrays(drawtype, 0, m_length);

      // deactivate vertex arrays after drawing
      glDisableClientState(GL_VERTEX_ARRAY);
    }
  }
#endif
  glLineWidth(1.0);
}

void scopeXYZ :: bangMess(void){
  int i;
  t_float*vertL=m_vertarray;
  t_float*vertR=m_vertarray+m_realLength;

  post("x\ty\tz\t\tx\ty\tz");
  for(i=0; i<m_realLength; i++) {
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
// typeMess
//
/////////////////////////////////////////////////////////
void scopeXYZ :: typeMess(t_symbol *type)
{
  char*s=type->s_name;
  char c=*s;
  switch(c){
  case 'd': // default
  case 'f': // fill
    m_drawType = GL_POLYGON;
    break;
  case 'p': // point
    m_drawType = GL_POINTS;
    break;
  case 'l': 
    { // line, linestrip
      char c2=s[4];
      switch(c2){
      case 's':
        if(s[5])
          m_drawType = GL_LINE_STRIP;
        else
          m_drawType = GL_LINES;
        break;
      default:
        m_drawType = GL_LINE_LOOP;
        break;
      }
    }
    break;
  case 't':
    { // tri, tristrip, trifan
      char c2=s[3];
      switch(c2){
      case 's':
        m_drawType = GL_TRIANGLE_STRIP;
        break;
      case 'f':
        m_drawType = GL_TRIANGLE_FAN;
        break;
      default:
        m_drawType = GL_TRIANGLES;
        break;
      }
    }
    break;
  case 'q': 
    { // quad, quadstrip
      char c2=s[4];
      switch(c2){
      case 's':
        m_drawType = GL_QUAD_STRIP;
        break;
      default:
        m_drawType = GL_QUADS;
        break;
      }
    }
    break;
  default:
    error ("draw style");
    return;
  }
  setModified();
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void scopeXYZ :: obj_setupCallback(t_class *classPtr)
{
  class_addcreator(reinterpret_cast<t_newmethod>(create_scopeXYZ), 
                   gensym("scopeXYZ~"), A_DEFFLOAT, A_NULL);

  class_addmethod(classPtr, reinterpret_cast<t_method>(&scopeXYZ::linewidthMessCallback),
                  gensym("linewidth"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, reinterpret_cast<t_method>(&scopeXYZ::lengthMessCallback),
                  gensym("length"), A_FLOAT, A_NULL);

  class_addmethod(classPtr, reinterpret_cast<t_method>(&scopeXYZ::dspCallback), 
                  gensym("dsp"), A_NULL);
  class_addmethod(classPtr, nullfn, gensym("signal"), A_NULL);

  class_addbang(classPtr, reinterpret_cast<t_method>(&scopeXYZ::bangCallback));
}
void scopeXYZ :: bangCallback(void *data)
{
  GetMyClass(data)->bangMess();
}

void scopeXYZ :: linewidthMessCallback(void *data, t_floatarg linewidth)
{
  GetMyClass(data)->linewidthMess(linewidth);
}
void scopeXYZ :: lengthMessCallback(void *data, t_floatarg l)
{
  GetMyClass(data)->lengthMess(static_cast<int>(l));
}
void scopeXYZ ::  dspCallback(void *data,t_signal** sp)
{
  GetMyClass(data)->doLengthMess(sp[1]->s_n);
  dsp_add(perform, 5, data, sp[1]->s_vec, sp[2]->s_vec, sp[3]->s_vec, sp[1]->s_n);
}

void scopeXYZ :: perform(t_int count, t_sample*X, t_sample*Y, t_sample*Z)
{
  int length=m_length;
  int position=m_position;

  t_float*vert=m_vertarray+3*position;
  int i=0;
  
  /* fill in the left-side of the double-array */
  for(i=0; i<count; i++) {
    *vert++=*X++;
    *vert++=*Y++;
    *vert++=*Z++;
  }
  /* copy the data over to the right-side of the double-array */
  if((m_position+count)>m_realLength) {
    // wrap around!
    // 1st copy the lower half
    memcpy(m_vertarray+3*(position+m_realLength),
           m_vertarray+3*(position),
           3*(m_realLength-m_position)*sizeof(t_sample));
    // then the upper half
    memcpy(m_vertarray,
           m_vertarray+3*(m_realLength),
           3*(m_position+count-m_realLength)*sizeof(t_sample));

  } else {
    // ordinary copy
  
    memcpy(m_vertarray+3*(position+m_realLength),
           m_vertarray+3*(position),
           3*count*sizeof(t_sample));
  }
  m_position=(m_position+count)%m_realLength;

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
