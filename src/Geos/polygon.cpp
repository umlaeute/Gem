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

#include "polygon.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Base/GemState.h"

CPPEXTERN_NEW_WITH_ONE_ARG(polygon, t_floatarg, A_FLOAT)

/////////////////////////////////////////////////////////
//
// polygon
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
polygon :: polygon(t_floatarg numInputs)
    	 : m_numInputs(0), m_drawType(GL_POLYGON)
{
  int i;
  int realNum = (int)numInputs;

  // configure the inlets
  if (realNum <= 0)   {
    error("GEM: polygon: illegal number of points");
    return;
  }
  m_numInputs = realNum;

  m_vert = new float*[realNum];
  m_vertarray = new float[realNum*3];

  for (i = 0; i < realNum*3; i++)  {
    m_vertarray[i]=0.0f;
  }
  for (i = 0; i < realNum; i++)  {
    m_vert[i]=m_vertarray+3*i;
  }


  char tempVt[7];
  // create the proper number of inputs
  for (i = 0; i < realNum; i++) {
    sprintf(tempVt, "%d", i+1);
    inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_list, gensym(tempVt) );
  }
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
polygon :: ~polygon()
{
  delete[]m_vert;
  delete[]m_vertarray;
 }

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void polygon :: render(GemState *state)
{
  if(m_drawType==-1)m_drawType=GL_POLYGON;

  glNormal3f(0.0f, 0.0f, 1.0f);
  glLineWidth(m_linewidth);
  if (state->texture && state->numTexCoords)
    {
      glBegin(m_drawType);
      for (int i = 0; i < m_numInputs; i++)
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
      float maxVal[2];
      maxVal[0] = maxVal[1] = 0;
      if (state->texture)
	{
	  for (int i = 0; i < m_numInputs; i++)
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
	}
      glBegin(m_drawType);
      for(int n=0; n < m_numInputs; n++)
	{
	  if (state->texture)
	    glTexCoord2f(m_vert[n][0] / maxVal[0],
			 m_vert[n][1] / maxVal[1]);
	  glVertex3fv(m_vert[n]);
	}
      glEnd();
    }
  glLineWidth(1.0);
}

/////////////////////////////////////////////////////////
// linewidthMess
//
/////////////////////////////////////////////////////////
void polygon :: linewidthMess(float linewidth)
{
    m_linewidth = (linewidth < 0.0f) ? 0.0f : linewidth;
    setModified();
}

/////////////////////////////////////////////////////////
// setVert
//
/////////////////////////////////////////////////////////
void polygon :: setVert(int whichOne, float x, float y, float z)
{
    m_vert[whichOne][0] = x;
    m_vert[whichOne][1] = y;
    m_vert[whichOne][2] = z;
    setModified();
}

/////////////////////////////////////////////////////////
// typeMess
//
/////////////////////////////////////////////////////////
void polygon :: typeMess(t_symbol *type)
{
    if (!strcmp(type->s_name, "line")) 
	    m_drawType = GL_LINE_LOOP;
    else if (!strcmp(type->s_name, "fill")) 
	    m_drawType = GL_POLYGON;
    else if (!strcmp(type->s_name, "point"))
	    m_drawType = GL_POINTS;
    else if (!strcmp(type->s_name, "linestrip")) 
	    m_drawType = GL_LINE_STRIP;
    else if (!strcmp(type->s_name, "tri")) 
	    m_drawType = GL_TRIANGLES;
    else if (!strcmp(type->s_name, "tristrip")) 
	    m_drawType = GL_TRIANGLE_STRIP;
    else if (!strcmp(type->s_name, "trifan")) 
	    m_drawType = GL_TRIANGLE_FAN;
    else if (!strcmp(type->s_name, "quad")) 
	    m_drawType = GL_QUADS;
    else if (!strcmp(type->s_name, "quadstrip")) 
	    m_drawType = GL_QUAD_STRIP;
    
        

    else
    {
	    error ("GEM: polygon: draw style");
	    return;
    }
    setModified();
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void polygon :: obj_setupCallback(t_class *classPtr)
{
    class_addmethod(classPtr, (t_method)&polygon::linewidthMessCallback,
    	    gensym("linewidth"), A_FLOAT, A_NULL);
    class_addmethod(classPtr, (t_method)&polygon::linewidthMessCallback,
    	    gensym("width"), A_FLOAT, A_NULL);
    class_addmethod(classPtr, (t_method)&polygon::typeMessCallback,
    	    gensym("draw"), A_SYMBOL, A_NULL);

    class_addanything(classPtr, (t_method)&polygon::vertCallback);
}
void polygon :: linewidthMessCallback(void *data, t_floatarg linewidth)
{
    GetMyClass(data)->linewidthMess((float)linewidth);
}
void polygon :: typeMessCallback(void *data, t_symbol *type)
{
    GetMyClass(data)->typeMess(type);
}

void polygon :: vertCallback(void *data, t_symbol*s, int argc, t_atom*argv)
{
  int i = atoi(s->s_name);
  if (i>0 && argc==3){
    GetMyClass(data)->setVert(i-1, atom_getfloat(argv), atom_getfloat(argv+1), atom_getfloat(argv+2));
  }
}
