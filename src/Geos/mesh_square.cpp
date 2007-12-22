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
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "mesh_square.h"

#include "Base/GemState.h"

CPPEXTERN_NEW_WITH_TWO_ARGS(mesh_square, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// mesh_square
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
mesh_square :: mesh_square(t_floatarg sizeX, t_floatarg sizeY)
        : GemShape(1)
{
    m_linewidth=1.0;
//    m_drawType = GL_QUADS;
    m_blend=0;
	setSize((int)sizeX,(int)sizeY);
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
mesh_square :: ~mesh_square()
{ }

/////////////////////////////////////////////////////////
// getTexCoords
//
/////////////////////////////////////////////////////////
void mesh_square :: getTexCoords(void)
{
    for ( int i = 0; i < gridX; ++i)
    {
        for ( int j = 0; j < gridY; ++j)
        {
            texCoords[i][j][0] = ((xsize*(float)i/(float)(gridX-1)) + xsize0 );
			texCoords[i][j][1] = ((ysize*(float)j/(float)(gridY-1)) + ysize0 );
            //post("texCoords[%d][%d] = %f\t%f",i,j,texCoords[i][j][0],texCoords[i][j][1]);
        }
    }
}

/////////////////////////////////////////////////////////
// setSize
//
/////////////////////////////////////////////////////////
void mesh_square :: setSize( int valueX, int valueY )
{
	if(valueX>1) gridX = valueX;
	else gridX = 5;

	if(valueY>1) gridY = valueY;
	else gridY = gridX;

    getTexCoords();
}



/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void mesh_square :: render(GemState *state)
{
	int i,j;
    GLfloat sizeX = 2. / (GLfloat)(gridX-1);
    GLfloat sizeY = 2. / (GLfloat)(gridY-1);

   if(m_drawType==GL_DEFAULT_GEM)m_drawType=GL_TRIANGLE_STRIP;
//m_drawType=GL_LINES;

    glNormal3f(0.0f, 0.0f, 1.0f);
	if (m_drawType == GL_LINE_LOOP)
		m_drawType = GL_LINES;

    if (m_drawType == GL_LINES)
        glLineWidth(m_linewidth);

    if (m_blend) {
        glEnable(GL_POLYGON_SMOOTH);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE);
        glHint(GL_POLYGON_SMOOTH_HINT,GL_DONT_CARE); 
    }

    glNormal3f( 0.0f, 0.0f, 1.0f);

	if (state->texture && state->numTexCoords>=3)
    {
		if ((xsize0!= state->texCoords[0].s) ||
		(xsize != state->texCoords[1].s-xsize0) ||
		(ysize0!= state->texCoords[1].t) ||
		(ysize != state->texCoords[2].t-ysize0))
		alreadyInit = 0;

        if (!alreadyInit)
        {
			xsize0 = state->texCoords[0].s;
	    	xsize  = state->texCoords[1].s-xsize0;
	    	ysize0 = state->texCoords[1].t;
	    	ysize  = state->texCoords[2].t-ysize0;

            setSize( gridX, gridY );
            alreadyInit = 1;
        }

        for (int i=0; i<(gridX-1) ; i++)
        {
    		glBegin(m_drawType);
            for (int j = 0; j < gridY ; j++)
            {
                glTexCoord2fv( texCoords[i][j] );
                glVertex3f( m_size * (i*sizeX - 1), m_size * (j*sizeY -1) , 0);

                glTexCoord2fv( texCoords[i+1][j] );
                glVertex3f(  m_size * ((i+1)*sizeX - 1), m_size * (j*sizeY -1), 0);
            }
            glEnd();
        }
    }else
    {
        if (!alreadyInit)
        {
            xsize = 1;
            ysize = 1;
			ysize0= 0;
			xsize0= 0;

            setSize( gridX, gridY);
            alreadyInit = 1;
        }

        for ( i = 0; i<(gridX -1); i++)
        {
            glBegin(m_drawType);
            for ( j = 0; j < gridY  ; j++)
            {
				glTexCoord2fv( texCoords[i][j] );
                glVertex3f( m_size * (i*sizeX -1), m_size * (j*sizeY -1), 0 );
            
                glTexCoord2fv( texCoords[i+1][j] );
                glVertex3f( m_size * ((i+1)*sizeX -1), m_size * (j*sizeY -1), 0 );
            }
            glEnd();
        }
    }

    if (m_blend) {
        glDisable(GL_POLYGON_SMOOTH);
        glDisable(GL_BLEND);
    }
}


/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void mesh_square :: obj_setupCallback(t_class *classPtr)
{     class_addmethod(classPtr, (t_method)&mesh_square::blendMessCallback,
    	    gensym("blend"), A_FLOAT, A_NULL);
	class_addmethod(classPtr, (t_method)&mesh_square::gridMessCallback,
    	    gensym("grid"), A_FLOAT, A_NULL);
    class_addmethod(classPtr, (t_method)&mesh_square::gridXMessCallback,
    	    gensym("gridX"), A_FLOAT, A_NULL);
    class_addmethod(classPtr, (t_method)&mesh_square::gridYMessCallback,
    	    gensym("gridY"), A_FLOAT, A_NULL);
}

void mesh_square :: blendMessCallback(void *data, t_floatarg blend)
{
    GetMyClass(data)->m_blend=((int)blend);
}

/////////////////////////////////////////////////////////
// setGrid
//
/////////////////////////////////////////////////////////

void mesh_square :: gridMessCallback(void *data, t_floatarg grid)
{
    GetMyClass(data)->gridX=(int)grid;
    GetMyClass(data)->gridY=(int)grid;
    GetMyClass(data)->setSize(GetMyClass(data)->gridX,GetMyClass(data)->gridY);
}

void mesh_square :: gridXMessCallback(void *data, t_floatarg grid)
{
    GetMyClass(data)->gridX=(int)grid;
    GetMyClass(data)->setSize(GetMyClass(data)->gridX,GetMyClass(data)->gridY);
}

void mesh_square :: gridYMessCallback(void *data, t_floatarg grid)
{
    GetMyClass(data)->gridY=(int)grid;
    GetMyClass(data)->setSize(GetMyClass(data)->gridX,GetMyClass(data)->gridY);
}


