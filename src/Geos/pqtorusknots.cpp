////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// tigital@mac.com
//
// Implementation file
//
//    Copyright (c) 2004-2005 tigital
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "pqtorusknots.h"

#define PI2 3.1415926535897932384626433832795f*2

CPPEXTERN_NEW_WITH_TWO_ARGS(pqtorusknots, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT )

/////////////////////////////////////////////////////////
//
// pqtorusknots
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pqtorusknots :: pqtorusknots(t_floatarg width, t_floatarg m_Q)
  : GemShape(width), m_steps(256), m_facets(16), m_scale(1), m_thickness(0.2),
	m_clumps(12), m_clumpOffset(0), m_clumpScale(0.5), m_uScale(4), m_vScale(64), m_P(2),
	m_Q(5)
{
	m_Vertex = NULL;
    m_normal = NULL;
    m_Index = NULL;
    m_Indices = 0;
    m_PrimitiveType = 0;
    m_Texcoord[0] = m_Texcoord[1] = m_Texcoord[2] = m_Texcoord[3] = 0;
	
  if (m_P == 0.f)
    m_P = 1.f;
  if (m_Q == 0.f)
    m_Q = 0.f;
    m_drawType = GL_TRIANGLE_STRIP;
	m_blend = 0;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pqtorusknots :: ~pqtorusknots()
{
    delete[] m_Vertex;
    m_Vertex = NULL;

    delete[] m_normal;
    m_normal = NULL;

    delete[] m_Index;
    m_Index = NULL;

    m_Indices = 0;
    m_PrimitiveType = 0;

    int i,j;

    // clear tex coord arrays, guarding against double deletion
    for (i = 0; i < 4; i++)
    {
        delete[] m_Texcoord[i];
        for (j = i + 1; j < 4; j++)
        {
            if (m_Texcoord[j] == m_Texcoord[i])
                m_Texcoord[j] = NULL;
        }
        m_Texcoord[i] = NULL;
    }
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void pqtorusknots :: render(GemState *state)
{
    int i, j;

    m_thickness *= m_scale;

    GLfloat *vtx      = new GLfloat[((m_steps + 1) * (m_facets + 1) + 1) * 3];
    GLfloat *normal   = new GLfloat[((m_steps + 1) * (m_facets + 1) + 1) * 3];
    GLfloat *texcoord = new GLfloat[((m_steps + 1) * (m_facets + 1) + 1) * 2];
    GLuint *idx      = new GLuint[(m_steps + 1) * m_facets * 2];
	m_Vertex = vtx;
	m_normal = normal;
	m_Index = idx;
	for (i=0; i<4; i++)
		m_Texcoord[i] = texcoord;

    m_Indices = (m_steps + 1) * m_facets * 2;
    m_Vertices = ((m_steps + 1) * (m_facets + 1) + 1);
    m_PrimitiveType = GL_TRIANGLE_STRIP;

    for (j = 0; j < m_facets; j++)
    {
        for (i = 0; i < m_steps + 1; i++)
        {
            m_Index[i * 2 + 0 + j * (m_steps + 1) * 2] = ((j + 1) + i * (m_facets + 1));
            m_Index[i * 2 + 1 + j * (m_steps + 1) * 2] = (j + i * (m_facets + 1));
        }
    }

    for (i = 0; i < m_steps; i++)
    {       
        float centerpoint[3];
        float Pp = m_P * i * PI2 / m_steps;
        float Qp = m_Q * i * PI2 / m_steps;
        float r = (.5f * (2 + (float)sin(Qp))) * m_scale;
        centerpoint[0] = r * (float)cos(Pp);
        centerpoint[1] = r * (float)cos(Qp);
        centerpoint[2] = r * (float)sin(Pp);
        
        float nextpoint[3];
        Pp = m_P * (i + 1) * PI2 / m_steps;
        Qp = m_Q * (i + 1) * PI2 / m_steps;
        r = (.5f * (2 + (float)sin(Qp))) * m_scale;
        nextpoint[0] = r * (float)cos(Pp);
        nextpoint[1] = r * (float)cos(Qp);
        nextpoint[2] = r * (float)sin(Pp);
        
        float T[3];
        T[0] = nextpoint[0] - centerpoint[0];
        T[1] = nextpoint[1] - centerpoint[1];
        T[2] = nextpoint[2] - centerpoint[2];

        float N[3];
        N[0] = nextpoint[0] + centerpoint[0];
        N[1] = nextpoint[1] + centerpoint[1];
        N[2] = nextpoint[2] + centerpoint[2];


        float B[3];
        B[0] = T[1]*N[2] - T[2]*N[1];
        B[1] = T[2]*N[0] - T[0]*N[2];
        B[2] = T[0]*N[1] - T[1]*N[0];

        N[0] = B[1]*T[2] - B[2]*T[1];
        N[1] = B[2]*T[0] - B[0]*T[2];
        N[2] = B[0]*T[1] - B[1]*T[0];

        float l;
        l = (float)sqrt(B[0] * B[0] + B[1] * B[1] + B[2] * B[2]);
        B[0] /= l;
        B[1] /= l;
        B[2] /= l;

        l = (float)sqrt(N[0] * N[0] + N[1] * N[1] + N[2] * N[2]);
        N[0] /= l;
        N[1] /= l;
        N[2] /= l;

        for (j = 0; j < m_facets; j++)
        {
            float pointx = (float)sin(j * PI2 / m_facets) * m_thickness * (((float)sin(m_clumpOffset + m_clumps * i * PI2 / m_steps) * m_clumpScale) + 1);
            float pointy = (float)cos(j * PI2 / m_facets) * m_thickness * (((float)cos(m_clumpOffset + m_clumps * i * PI2 / m_steps) * m_clumpScale) + 1);
            
            vtx[i * (m_facets + 1) * 3 + j * 3 + 0] = N[0] * pointx + B[0] * pointy + centerpoint[0];
            vtx[i * (m_facets + 1) * 3 + j * 3 + 1] = N[1] * pointx + B[1] * pointy + centerpoint[1];
            vtx[i * (m_facets + 1) * 3 + j * 3 + 2] = N[2] * pointx + B[2] * pointy + centerpoint[2];

            normal[i * (m_facets + 1) * 3 + j * 3 + 0] = vtx[i * (m_facets + 1) * 3 + j * 3 + 0] - centerpoint[0];
            normal[i * (m_facets + 1) * 3 + j * 3 + 1] = vtx[i * (m_facets + 1) * 3 + j * 3 + 1] - centerpoint[1];
            normal[i * (m_facets + 1) * 3 + j * 3 + 2] = vtx[i * (m_facets + 1) * 3 + j * 3 + 2] - centerpoint[2];

            float l;
            l = (float)sqrt(normal[i * (m_facets + 1) * 3 + j * 3 + 0] * normal[i * (m_facets + 1) * 3 + j * 3 + 0] +
                            normal[i * (m_facets + 1) * 3 + j * 3 + 1] * normal[i * (m_facets + 1) * 3 + j * 3 + 1] +
                            normal[i * (m_facets + 1) * 3 + j * 3 + 2] * normal[i * (m_facets + 1) * 3 + j * 3 + 2]);
            
            normal[i * (m_facets + 1) * 3 + j * 3 + 0] /= l;
            normal[i * (m_facets + 1) * 3 + j * 3 + 1] /= l;
            normal[i * (m_facets + 1) * 3 + j * 3 + 2] /= l;

            texcoord[i * (m_facets + 1) * 2 + j * 2 + 0] = ((float)j / m_facets) * m_uScale;
            texcoord[i * (m_facets + 1) * 2 + j * 2 + 1] = ((float)i / m_steps) * m_vScale;
        }
        // create duplicate vertex for sideways wrapping
        // otherwise identical to first vertex in the 'ring' except for the U coordinate
        vtx[i * (m_facets + 1) * 3 + m_facets * 3 + 0] = vtx[i * (m_facets + 1) * 3 + 0];
        vtx[i * (m_facets + 1) * 3 + m_facets * 3 + 1] = vtx[i * (m_facets + 1) * 3 + 1];
        vtx[i * (m_facets + 1) * 3 + m_facets * 3 + 2] = vtx[i * (m_facets + 1) * 3 + 2];

        normal[i * (m_facets + 1) * 3 + m_facets * 3 + 0] = normal[i * (m_facets + 1) * 3 + 0];
        normal[i * (m_facets + 1) * 3 + m_facets * 3 + 1] = normal[i * (m_facets + 1) * 3 + 1];
        normal[i * (m_facets + 1) * 3 + m_facets * 3 + 2] = normal[i * (m_facets + 1) * 3 + 2];

        texcoord[i * (m_facets + 1) * 2 + m_facets * 2 + 0] = m_uScale;
        texcoord[i * (m_facets + 1) * 2 + m_facets * 2 + 1] = texcoord[i * (m_facets + 1) * 2 + 1];

    }

    // create duplicate ring of vertices for longways wrapping
    // otherwise identical to first 'ring' in the knot except for the V coordinate
    for (j = 0; j < m_facets; j++)
    {
        vtx[m_steps * (m_facets + 1) * 3 + j * 3 + 0] = vtx[j * 3 + 0];
        vtx[m_steps * (m_facets + 1) * 3 + j * 3 + 1] = vtx[j * 3 + 1];
        vtx[m_steps * (m_facets + 1) * 3 + j * 3 + 2] = vtx[j * 3 + 2];

        normal[m_steps * (m_facets + 1) * 3 + j * 3 + 0] = normal[j * 3 + 0];
        normal[m_steps * (m_facets + 1) * 3 + j * 3 + 1] = normal[j * 3 + 1];
        normal[m_steps * (m_facets + 1) * 3 + j * 3 + 2] = normal[j * 3 + 2];

        texcoord[m_steps * (m_facets + 1) * 2 + j * 2 + 0] = texcoord[j * 2 + 0];
        texcoord[m_steps * (m_facets + 1) * 2 + j * 2 + 1] = m_vScale;
    }

    // finally, there's one vertex that needs to be duplicated due to both U and V coordinate.

    vtx[m_steps * (m_facets + 1) * 3 + m_facets * 3 + 0] = vtx[0];
    vtx[m_steps * (m_facets + 1) * 3 + m_facets * 3 + 1] = vtx[1];
    vtx[m_steps * (m_facets + 1) * 3 + m_facets * 3 + 2] = vtx[2];

    normal[m_steps * (m_facets + 1) * 3 + m_facets * 3 + 0] = normal[0];
    normal[m_steps * (m_facets + 1) * 3 + m_facets * 3 + 1] = normal[1];
    normal[m_steps * (m_facets + 1) * 3 + m_facets * 3 + 2] = normal[2];

    texcoord[m_steps * (m_facets + 1) * 2 + m_facets * 2 + 0] = m_uScale;
    texcoord[m_steps * (m_facets + 1) * 2 + m_facets * 2 + 1] = m_vScale;
/*	
	if (m_blend) {
        glDisable(GL_POLYGON_SMOOTH);
        glDisable(GL_BLEND);
    }
*/
	glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    glVertexPointer(3, GL_FLOAT, 0, m_Vertex);            
    glNormalPointer(GL_FLOAT, 0, m_normal);
	
	for (int i = 0; i < 4; i++)
    {
        if (texcoord[i])
        {
            glClientActiveTextureARB(GL_TEXTURE0_ARB + i);
            glTexCoordPointer(2, GL_FLOAT, 0, &texcoord[i]);
        }
    }
    glClientActiveTextureARB(GL_TEXTURE0_ARB);
	glDrawElements(m_PrimitiveType,m_Indices,GL_UNSIGNED_INT,m_Index);
	
	delete[] m_Vertex;
    m_Vertex = NULL;

    delete[] m_normal;
    m_normal = NULL;

    delete[] m_Index;
    m_Index = NULL;
	
	delete[] texcoord;
	texcoord = NULL;

}
/////////////////////////////////////////////////////////
// scaleMess
//
/////////////////////////////////////////////////////////
void pqtorusknots :: scaleMess(float size)
{
    m_scale = size;
    setModified();
}
/////////////////////////////////////////////////////////
// stepsMess
//
/////////////////////////////////////////////////////////
void pqtorusknots :: stepsMess(float size)
{
	if (size<0)
		size = 0;
	m_steps = (GLint)size;
    setModified();
}
/////////////////////////////////////////////////////////
// facetsMess
//
/////////////////////////////////////////////////////////
void pqtorusknots :: facetsMess(float size)
{
	if(size<0)
		size=0;
	m_facets = (GLint)size;
    setModified();
}
/////////////////////////////////////////////////////////
// thicknessMess
//
/////////////////////////////////////////////////////////
void pqtorusknots :: thickMess(float size)
{
	m_thickness = size;
    setModified();
}
/////////////////////////////////////////////////////////
// clumpMess
//
/////////////////////////////////////////////////////////
void pqtorusknots :: clumpMess(float clump, float clumpOffset, float clumpScale)
{
    m_clumps = clump;
    m_clumpOffset = clumpOffset;
	m_clumpScale = clumpScale;
	setModified();
}
/////////////////////////////////////////////////////////
// uvScaleMess
//
/////////////////////////////////////////////////////////
void pqtorusknots :: uvScaleMess(float uScale, float vScale)
{
    m_uScale = uScale;
    m_vScale = vScale;
  setModified();
}
/////////////////////////////////////////////////////////
// pqMess
//
/////////////////////////////////////////////////////////
void pqtorusknots :: pqMess(float p, float q)
{
  m_P = p;
  m_Q = q;
  setModified();
}
#if 0
/////////////////////////////////////////////////////////
// typeMess
//
/////////////////////////////////////////////////////////
void pqtorusknots :: typeMess(t_symbol *type)
{
    if (!strcmp(type->s_name, "line")) 
	    m_drawType = GL_LINE_LOOP;
    else if (!strcmp(type->s_name, "fill")) 
	    m_drawType = GL_QUADS;
    else if (!strcmp(type->s_name, "point"))
	    m_drawType = GL_POINTS;
    else
    {
	    error ("GEM: pqtorusknots draw style");
	    return;
    }
    setModified();
}
#endif

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pqtorusknots :: obj_setupCallback(t_class *classPtr)
{
    class_addmethod(classPtr, (t_method)&pqtorusknots::scaleMessCallback,
    	    gensym("scale"), A_FLOAT, A_NULL);
    class_addmethod(classPtr, (t_method)&pqtorusknots::stepsMessCallback,
			gensym("steps"), A_FLOAT, A_NULL);
	class_addmethod(classPtr, (t_method)&pqtorusknots::facetsMessCallback,
			gensym("facets"), A_FLOAT, A_NULL);
    class_addmethod(classPtr, (t_method)&pqtorusknots::thickMessCallback,
    	    gensym("thick"), A_FLOAT, A_NULL);
	class_addmethod(classPtr, (t_method)&pqtorusknots::clumpMessCallback,
			gensym("clump"), A_FLOAT, A_FLOAT, A_FLOAT, A_NULL);
	class_addmethod(classPtr, (t_method)&pqtorusknots::uvScaleMessCallback,
			gensym("uvScale"), A_FLOAT, A_FLOAT, A_NULL);
	class_addmethod(classPtr, (t_method)&pqtorusknots::pqMessCallback,
			gensym("pq"), A_FLOAT, A_FLOAT, A_NULL);
}

void pqtorusknots :: scaleMessCallback(void *data, t_floatarg size)
{
    GetMyClass(data)->scaleMess((float)size);  
}

void pqtorusknots :: stepsMessCallback(void *data, t_floatarg size)
{
    GetMyClass(data)->stepsMess((float)size);  
}
void pqtorusknots :: facetsMessCallback(void *data, t_floatarg size)
{
    GetMyClass(data)->facetsMess((float)size);  
}
void pqtorusknots :: thickMessCallback(void *data, t_floatarg size)
{
    GetMyClass(data)->thickMess((float)size);  
}
void pqtorusknots :: clumpMessCallback(void *data, t_floatarg posX, t_floatarg posY, t_floatarg valforce )
{
    GetMyClass(data)->clumpMess((float)posX, (float)posY, (float)valforce);
}
void pqtorusknots :: uvScaleMessCallback(void *data, t_floatarg posX, t_floatarg posY)
{
    GetMyClass(data)->uvScaleMess((float)posX, (float)posY);
}
void pqtorusknots :: pqMessCallback(void *data, t_floatarg posX, t_floatarg posY)
{
    GetMyClass(data)->pqMess((float)posX, (float)posY);
}