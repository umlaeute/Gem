////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) G�nther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f�r::uml�ute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "polygon.h"

#include <stdio.h>
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
    	 : GemShape(1.0),
	   m_linewidth(1.0), m_numInputs(0), m_drawType(GL_POLYGON)
{
    int i;
	int realNum = (int)numInputs;
	for (i = 0; i < 10; i++)
    {
	    m_vert[i][0] = m_vert[i][1] = m_vert[i][2] = 0.0;
    }

    // configure the inlets
    if (realNum <= 0 || realNum > 10)
    {
	    error("GEM: polygon: illegal number of points");
	    return;
    }
    m_numInputs = realNum;
    
    char tempVt[7];
    
    // create the proper number of inputs
    for (i = 0; i < realNum; i++)
    {
	    sprintf(tempVt, "vert_%d", i);
	    inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_list, gensym(tempVt) );
    }
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
polygon :: ~polygon()
{ }

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void polygon :: render(GemState *state)
{
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
    	    gensym("width"), A_FLOAT, A_NULL);
    class_addmethod(classPtr, (t_method)&polygon::typeMessCallback,
    	    gensym("draw"), A_SYMBOL, A_NULL);

    class_addmethod(classPtr, (t_method)&polygon::setVert0Callback,
    	    gensym("vert_0"), A_FLOAT, A_FLOAT, A_FLOAT, A_NULL);
    class_addmethod(classPtr, (t_method)&polygon::setVert1Callback,
    	    gensym("vert_1"), A_FLOAT, A_FLOAT, A_FLOAT, A_NULL);
    class_addmethod(classPtr, (t_method)&polygon::setVert2Callback,
    	    gensym("vert_2"), A_FLOAT, A_FLOAT, A_FLOAT, A_NULL);
    class_addmethod(classPtr, (t_method)&polygon::setVert3Callback,
    	    gensym("vert_3"), A_FLOAT, A_FLOAT, A_FLOAT, A_NULL);
    class_addmethod(classPtr, (t_method)&polygon::setVert4Callback,
    	    gensym("vert_4"), A_FLOAT, A_FLOAT, A_FLOAT, A_NULL);
    class_addmethod(classPtr, (t_method)&polygon::setVert5Callback,
    	    gensym("vert_5"), A_FLOAT, A_FLOAT, A_FLOAT, A_NULL);
    class_addmethod(classPtr, (t_method)&polygon::setVert6Callback,
    	    gensym("vert_6"), A_FLOAT, A_FLOAT, A_FLOAT, A_NULL);
    class_addmethod(classPtr, (t_method)&polygon::setVert7Callback,
    	    gensym("vert_7"), A_FLOAT, A_FLOAT, A_FLOAT, A_NULL);
    class_addmethod(classPtr, (t_method)&polygon::setVert8Callback,
    	    gensym("vert_8"), A_FLOAT, A_FLOAT, A_FLOAT, A_NULL);
    class_addmethod(classPtr, (t_method)&polygon::setVert9Callback,
    	    gensym("vert_9"), A_FLOAT, A_FLOAT, A_FLOAT, A_NULL);
}
void polygon :: linewidthMessCallback(void *data, t_floatarg linewidth)
{
    GetMyClass(data)->linewidthMess((float)linewidth);
}
void polygon :: typeMessCallback(void *data, t_symbol *type)
{
    GetMyClass(data)->typeMess(type);
}
void polygon :: setVert0Callback(void *data, t_floatarg x, t_floatarg y, t_floatarg z)
{
    GetMyClass(data)->setVert(0, (float)x, (float)y, (float)z);
}
void polygon :: setVert1Callback(void *data, t_floatarg x, t_floatarg y, t_floatarg z)
{
    GetMyClass(data)->setVert(1, (float)x, (float)y, (float)z);
}
void polygon :: setVert2Callback(void *data, t_floatarg x, t_floatarg y, t_floatarg z)
{
    GetMyClass(data)->setVert(2, (float)x, (float)y, (float)z);
}
void polygon :: setVert3Callback(void *data, t_floatarg x, t_floatarg y, t_floatarg z)
{
    GetMyClass(data)->setVert(3, (float)x, (float)y, (float)z);
}
void polygon :: setVert4Callback(void *data, t_floatarg x, t_floatarg y, t_floatarg z)
{
    GetMyClass(data)->setVert(4, (float)x, (float)y, (float)z);
}
void polygon :: setVert5Callback(void *data, t_floatarg x, t_floatarg y, t_floatarg z)
{
    GetMyClass(data)->setVert(5, (float)x, (float)y, (float)z);
}
void polygon :: setVert6Callback(void *data, t_floatarg x, t_floatarg y, t_floatarg z)
{
    GetMyClass(data)->setVert(6, (float)x, (float)y, (float)z);
}
void polygon :: setVert7Callback(void *data, t_floatarg x, t_floatarg y, t_floatarg z)
{
    GetMyClass(data)->setVert(7, (float)x, (float)y, (float)z);
}
void polygon :: setVert8Callback(void *data, t_floatarg x, t_floatarg y, t_floatarg z)
{
    GetMyClass(data)->setVert(8, (float)x, (float)y, (float)z);
}
void polygon :: setVert9Callback(void *data, t_floatarg x, t_floatarg y, t_floatarg z)
{
    GetMyClass(data)->setVert(9, (float)x, (float)y, (float)z);
}

