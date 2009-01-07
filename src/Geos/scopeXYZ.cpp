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
  lengthMess((int)len);

  /* channels inlet */
  int i;
  for (i=0; i<3; i++)
    inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_signal, &s_signal);
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
scopeXYZ :: ~scopeXYZ()
{
  if(m_vert)delete[]m_vert;
  if(m_vertarray)delete[]m_vertarray;
}


void scopeXYZ :: lengthMess(int l)
{
  int i;
  if(l<=0){
    return;
  }

  if(l==m_length)return;

  if(m_vert)delete[]m_vert;
  if(m_vertarray)delete[]m_vertarray;

  m_length = l;

  m_vert = new float*[m_length];
  m_vertarray = new float[m_length*3];

  for (i = 0; i < m_length*3; i++)  {
    m_vertarray[i]=0.0f;
  }
  for (i = 0; i < m_length; i++)  {
    m_vert[i]=m_vertarray+3*i;
  }
}

/////////////////////////////////////////////////////////
// renderShape
//
/////////////////////////////////////////////////////////
void scopeXYZ :: renderShape(GemState *state)
{
  if(m_drawType==GL_DEFAULT_GEM)m_drawType=GL_LINE_STRIP;
  glNormal3f(0.0f, 0.0f, 1.0f);
  glLineWidth(m_linewidth);
  if (state->texture && state->numTexCoords)
    {
      glBegin(m_drawType);
      for (int i = 0; i < m_length; i++)
	{
	  if (state->numTexCoords < i)
	    glTexCoord2f(state->texCoords[state->numTexCoords - 1].s,
			 state->texCoords[state->numTexCoords - 1].t);
	  else
	    glTexCoord2f(state->texCoords[i].s,
			 state->texCoords[i].t);
	  glVertex3fv(m_vert[i]);
	}
      glEnd();
    }
  else
    {
      if (state->texture)
	{
	  float maxVal[2];
	  maxVal[0] = maxVal[1] = 0.f;

	  if(maxVal[0]<0)maxVal[0]*=-1.f;
	  if(maxVal[1]<0)maxVal[1]*=-1.f;

	  for (int i = 0; i < m_length; i++)
    	    {
	      for (int j = 0; j < 2; j++)
		{
		  if (m_vert[i][j] < 0)
		    {
		      if (-m_vert[i][j] > maxVal[j])
			maxVal[j] = -m_vert[i][j];
		    }
		  else
		    {
		      if (m_vert[i][j] > maxVal[j])
			maxVal[j] = m_vert[i][j];
		    }
		}
    	    }
	  glBegin(m_drawType);
	  for(int n=0; n < m_length; n++)
	    {
	      glTexCoord2f(m_vert[n][0] / maxVal[0],
			   m_vert[n][1] / maxVal[1]);
	      glVertex3fv(m_vert[n]);
	    }
	  glEnd();
	}
      else
	{
	  glBegin(m_drawType);
	  for(int n=0; n < m_length; n++)
	    {
	      glVertex3fv(m_vert[(n+m_position)%m_length]);
	    }
	  glEnd();
	}
    }
  glLineWidth(1.0);
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
  class_addcreator((t_newmethod)create_scopeXYZ, gensym("scopeXYZ~"), A_DEFFLOAT, A_NULL);

    class_addmethod(classPtr, (t_method)&scopeXYZ::linewidthMessCallback,
    	    gensym("linewidth"), A_FLOAT, A_NULL);
    class_addmethod(classPtr, (t_method)&scopeXYZ::lengthMessCallback,
    	    gensym("length"), A_FLOAT, A_NULL);

    class_addmethod(classPtr, (t_method)&scopeXYZ::dspCallback, 
		    gensym("dsp"), A_NULL);
    class_addmethod(classPtr, nullfn, gensym("signal"), A_NULL);
}
void scopeXYZ :: linewidthMessCallback(void *data, t_floatarg linewidth)
{
    GetMyClass(data)->linewidthMess((float)linewidth);
}
void scopeXYZ :: lengthMessCallback(void *data, t_floatarg l)
{
    GetMyClass(data)->lengthMess((int)l);
}
void scopeXYZ ::  dspCallback(void *data,t_signal** sp)
{
  if(GetMyClass(data)->m_length==0)GetMyClass(data)->lengthMess(sp[1]->s_n);
  dsp_add(perform, 5, data, sp[1]->s_vec, sp[2]->s_vec, sp[3]->s_vec, sp[1]->s_n);
}

t_int* scopeXYZ :: perform(t_int* w)
{
  int index=1;
  scopeXYZ *x = GetMyClass((void*)w[index++]);
  t_float* in_X = (t_float*)(w[index++]);
  t_float* in_Y = (t_float*)(w[index++]);
  t_float* in_Z = (t_float*)(w[index++]);

  t_int n = (t_int)(w[index++]);

  int length=x->m_length;
  int position=0;

  if (length<n){
    n=length;
  } else 
    position=(x->m_position)%length;

  if(position<0)position=0;

  t_float*vert=x->m_vert[position];

  x->m_position=position+n;

  if(x->m_position>length){
    // we have a wrap inside !
    for(int i=0; i<n; i++){
      x->m_vert[i%length][0]=*in_X++;
      x->m_vert[i%length][1]=*in_Y++;
      x->m_vert[i%length][2]=*in_Z++;
    }
  } else {
    while(n--){
      *vert++=*in_X++;
      *vert++=*in_Y++;
      *vert++=*in_Z++;
    }
  }    

  x->setModified();
  return (w+index);
}
