/*
 *  GEM - Graphics Environment for Multimedia
 *
 *  newWave.h
 *  gem_darwin
 *
 *  Created by Jamie Tittle on Thu Oct 10 2002.
 *  modified by cyrille Henry
 *  Copyright (c) 2002 tigital. All rights reserved.
 *    For information on usage and redistribution, and for a DISCLAIMER OF ALL
 *    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
 *
 */

#include "newWave.h"
#include "Base/GemState.h"
#include "Base/GemFuncUtil.h"
#include <string.h>

/* Grid */
enum {WIREFRAME, HIDDENLINE, FLATSHADED, SMOOTHSHADED, TEXTURED};
enum {FACENORMALS, ENVMAP};
enum {VWEAK, WEAK, NORMAL, STRONG};
enum {SMALL, MEDIUM, LARGE, XLARGE};
enum {CURRENT, FLAT, SPIKE, DIAGONALWALL, SIDEWALL, HOLE,
      MIDDLEBLOCK, DIAGONALBLOCK, CORNERBLOCK, HILL, HILLFOUR};
int displayMode = WIREFRAME;
int resetMode = HILLFOUR;
int grid = 50;

#define SQRTOFTWOINV 1.0 / 1.414213562


CPPEXTERN_NEW_WITH_ONE_ARG(newWave, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// newWave
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
newWave :: newWave( t_floatarg width)
  : GemShape(MEDIUM), alreadyInit(0), m_textureMode(0)
{
    m_height = 1.f;

    grid = ( MIN((int)width, MAXGRID ));
    if (grid < 3) grid =3; // min grid

    m_blend = 0;
    m_drawType = GL_TRIANGLE_STRIP;
    alreadyInit = 0;

    // the height inlet
    m_inletH = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("height"));
    m_inletM = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("mode"));

    K1=K2=K3=D1=D2=D3=0;
    K1=0.05;
    D1=0.1;

}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
newWave :: ~newWave()
{
    alreadyInit = 0;
    if(m_inletH)inlet_free(m_inletH);
    if(m_inletM)inlet_free(m_inletM);
}

void newWave :: modeMess(float mode)
{
    reset((int)mode);
    setModified();
}

void newWave :: positionMess(float posX, float posY, float posZ)
{
    position((float) posX, (float) posY, (float) posZ);
    setModified();

}
void newWave :: forceMess(float posX, float posY, float valforce)
{
    setforce((float) posX, (float) posY, (float) valforce);
    setModified();
    
}

void newWave :: textureMess(int mode)
{
  if(mode<0){
    error("newWave: textureMode must be >= 0");
    return;
  }
  m_textureMode = mode;
  setModified();
  alreadyInit=0;
  
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void newWave :: render(GemState *state)
{
    int i, j;

    //    post("m_size=%f", m_size);

    GLfloat size = 2.*m_size / (GLfloat)grid;

    if (m_drawType == GL_LINE_LOOP)
        glLineWidth(m_linewidth);
        
    if (m_blend) {
        glEnable(GL_POLYGON_SMOOTH);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE);
        glHint(GL_POLYGON_SMOOTH_HINT,GL_DONT_CARE);
    }
    glNormal3f( 0.0f, 0.0f, 1.0f);

    if (state->texture && state->numTexCoords)
    {
      if ((xsize0!= state->texCoords[0].s) ||
	  (xsize != state->texCoords[1].s-xsize0) ||
	  (ysize0!= state->texCoords[1].t) ||
	  (ysize != state->texCoords[2].t-ysize0))
	alreadyInit = 0;

      /*
      post("S\t%f=%f\t%f=%f", xsize0,state->texCoords[0].s,   xsize,state->texCoords[1].s-xsize0);
      post("T\t%f=%f\t%f=%f", ysize0,state->texCoords[1].t,   ysize,state->texCoords[2].t-ysize0);
      */

      // for (int II=0; II<4; II++)post("%d: %fx%f", II, state->texCoords[II].s, state->texCoords[II].t);


        if (!alreadyInit)
        {
	    xsize0 = state->texCoords[0].s;
	    xsize  = state->texCoords[1].s-xsize0;
	    ysize0 = state->texCoords[1].t;
	    ysize  = state->texCoords[2].t-ysize0;

            setSize( grid );
            setOther(m_textureMode);
            reset( resetMode );
            alreadyInit = 1;
        }

        for (int i=0; i<grid -1; ++i)
        {
            glBegin(m_drawType);
            for (int j = 0; j < grid; ++j)
            {
                glNormal3fv( vertNorms[i][j] );
                glTexCoord2fv( texCoords[i][j] );
                glVertex3f( (i-grid/2)*size, (j-grid/2)*size, posit[i][j]*m_height);
		/*
		  post("(%f\t%f)\t(%f\t%f)", (i-grid/2)*size, (j-grid/2)*size, (i+1-grid/2)*size, (j-grid/2)*size);
		*/

                glNormal3fv( vertNorms[i+1][j] );
                glTexCoord2fv( texCoords[i+1][j] );
                glVertex3f( (i+1-grid/2)*size, (j-grid/2)*size, posit[i+1][j]*m_height);
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
            setSize( grid );
            setOther(m_textureMode );
            reset( resetMode );
            alreadyInit = 1;
        }
 
        
// post("size=%f", size);
        for ( i=0; i<grid -1; ++i)
        {
            glBegin(m_drawType);
            for ( j = 0; j < grid; ++j)
            {
                glNormal3fv( vertNorms[i][j] );
                glTexCoord2fv( texCoords[i][j] );
                glVertex3f( (i-grid/2)*size, (j-grid/2)*size, posit[i][j]*m_height );
            
                glNormal3fv( vertNorms[i+1][j] );
                glTexCoord2fv( texCoords[i+1][j] );
                glVertex3f( (i+1-grid/2)*size, (j-grid/2)*size, posit[i+1][j]*m_height );
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
// heightMess
//
/////////////////////////////////////////////////////////
void newWave :: heightMess(float size)
{
    m_height = size;
    setModified();
}

/////////////////////////////////////////////////////////
// typeMess
//
/////////////////////////////////////////////////////////
void newWave :: typeMess(t_symbol *type)
{
    if (!strcmp(type->s_name, "line")) 
	    m_drawType = GL_LINE_LOOP;
    else if (!strcmp(type->s_name, "fill")) 
	    m_drawType = GL_TRIANGLE_STRIP;
    else if (!strcmp(type->s_name, "point"))
	    m_drawType = GL_POINTS;
    else
    {
	    error ("GEM: newWave draw style");
	    return;
    }
    setModified();
}


/////////////////////////////////////////////////////////
// getforce
//
/////////////////////////////////////////////////////////
void newWave :: getforce()
{
    float d;
    int i;
    int j;
    for (i=0; i<grid; i++)
        for ( j=0;j<grid; j++)
        {
            force[i][j] =0.0;
        }

		if (K1 != 0)
	{
		for ( i=1; i<grid; i++)
			for (int j=1;j<grid; j++)
			{
				d = K1 * (posit[i][j] - posit[i][j-1]);
				force[i][j] -= d;
				force[i][j-1] += d;

            
				d = K1 * (posit[i][j] - posit[i-1][j]);
				force[i][j] -= d;
				force[i-1][j] += d;
			}
    }

	if (K2 != 0)
	{
		for ( i=1; i<grid; i++)
			for (int j=1;j<grid; j++)
			{
				d = K2 * (posit[i][j] - posit[i-1][j-1]);
				force[i][j] -= d;
				force[i-1][j-1] += d;
            
			}

		for ( i=0; i<grid-1; i++)
			for (int j=1;j<grid; j++)
			{
				d = K2 * (posit[i][j] - posit[i+1][j-1]);
				force[i][j] -= d;
				force[i+1][j-1] += d;
			}
    }

	if (K3 != 0)
	{
		for ( i=1; i<grid-1; i++)
			for (int j=1;j<grid-1; j++)
			{
				d = K3 * posit[i][j];
			    force[i][j] -= d;
			}
    }

}
/////////////////////////////////////////////////////////
// getdamp
//
/////////////////////////////////////////////////////////
void newWave :: getdamp()
{
    float d;
    int i;
    int j;

    if (D1 != 0)
    {
        for ( i=1; i<grid; i++)
        {
            for (j=1;j<grid; j++)
            {
                d = D1 * ((posit[i][j] - posit[i][j-1])-(positold[i][j] - positold[i][j-1]));
                force[i][j] -= d;
                force[i][j-1] += d;
            
                d = D1 * ((posit[i][j] - posit[i-1][j])-(positold[i][j] - positold[i-1][j]));
                force[i][j] -= d;
                force[i-1][j] += d;
            }
        }
    }

    if (D2 != 0)
    {
        for ( i=1; i<grid; i++)
            for (j=1;j<grid; j++)
            {
                d = D2 * ((posit[i][j] - posit[i-1][j-1])-(positold[i][j] - positold[i-1][j-1]));
                force[i][j] -= d;
                force[i-1][j-1] += d;
            }

        for ( i=0; i<grid-1; i++)
            for (j=1;j<grid; j++)
            {
                d = D2 * ((posit[i][j] - posit[i+1][j-1])-(positold[i][j] - positold[i+1][j-1]));
                force[i][j] -= d;
                force[i+1][j-1] += d;
            }
    }

    if (D3 != 0)
    {
        for ( i=1; i<grid-1; i++)
            for (j=1;j<grid-1; j++)
            {
                d = D3 * (posit[i][j]-positold[i][j]);
                force[i][j] -= d;
            
                d = D3 * (posit[i][j]-positold[i][j]);
                force[i][j] -= d;
            }
    }
}

/////////////////////////////////////////////////////////
// force
//
/////////////////////////////////////////////////////////
void newWave :: setforce(float posX, float posY, float valforce)
{
    if ( ((int)posX > 0) & ((int)posX < grid - 1) & ((int)posY > 0) & ((int)posY < grid - 1) )
      force[(int)posX][(int)posY] += valforce;   
}

/////////////////////////////////////////////////////////
// position
//
/////////////////////////////////////////////////////////
void newWave :: position(float posX, float posY, float posZ)
{
    if ( ((int)posX > 0) & ((int)posX < grid - 1) & ((int)posY > 0) & ((int)posY < grid - 1) )
      posit[(int)posX][(int)posY] = posZ;                        
}


/////////////////////////////////////////////////////////
// savepos
//
/////////////////////////////////////////////////////////
void newWave :: savepos()
{
    int i;
    int j;
    for (i=0; i<grid; i++)
        for ( j=0;j<grid; j++)
        {
            positold[i][j] = posit[i][j];
        }
}


/////////////////////////////////////////////////////////
// getvelocity
//
/////////////////////////////////////////////////////////
void newWave :: getvelocity()
{
    for (int i=1; i<grid-1; i++)
        for (int j=1;j<grid-1; j++)
            veloc[i][j] += force[i][j] ;
}

/////////////////////////////////////////////////////////
// getposition
//
/////////////////////////////////////////////////////////
void newWave :: getposition()
{
    for ( int i=1; i<grid-1; i++)
        for ( int j=1;j<grid-1; j++)
            posit[i][j] += veloc[i][j];
}

/////////////////////////////////////////////////////////
// getTexCoords
//
/////////////////////////////////////////////////////////
void newWave :: getTexCoords(void)
{
  //  post("getTexCoords: x=%f\ty=%f %f", xsize, ysize0, ysize);
    for ( int i = 0; i < grid; ++i)
    {
        for ( int j = 0; j < grid; ++j)
        {
            texCoords[i][j][1] = ((xsize*(float)i/(float)(grid-1)) + xsize0 );
	    texCoords[i][j][0] = ((ysize*(float)j/(float)(grid-1)) + ysize0 );
            //post("texCoords[%d][%d] = %f\t%f",i,j,texCoords[i][j][0],texCoords[i][j][1]);
        }
    }
}

/////////////////////////////////////////////////////////
// setSize
//
/////////////////////////////////////////////////////////
void newWave :: setSize( int value )
{
    int prevGrid = grid;
    
    grid = value;
    
    if (prevGrid > grid)
    {
        reset(resetMode);
    }

    getTexCoords();
}

/////////////////////////////////////////////////////////
// bang
//
/////////////////////////////////////////////////////////
void newWave :: bangMess( )
{
    
		savepos();

		getvelocity();
		getposition();

		getFaceNorms();
        getVertNorms();

        getforce();
		getdamp();

}

/////////////////////////////////////////////////////////
// vector operations
//
/////////////////////////////////////////////////////////
void newWave :: copy(float vec0[3], float vec1[3])
{
    vec0[0] = vec1[0];
    vec0[1] = vec1[1];
    vec0[2] = vec1[2];
}

void newWave :: sub(float vec0[3], float vec1[3], float vec2[3])
{
    vec0[0] = vec1[0] - vec2[0];
    vec0[1] = vec1[1] - vec2[1];
    vec0[2] = vec1[2] - vec2[2];
}

void newWave :: add(float vec0[3], float vec1[3], float vec2[3])
{
    vec0[0] = vec1[0] + vec2[0];
    vec0[1] = vec1[1] + vec2[1];
    vec0[2] = vec1[2] + vec2[2];
}

void newWave :: scalDiv(float vec[3], float c)
{
    vec[0] /= c; vec[1] /= c; vec[2] /= c;
}

void newWave :: cross(float vec0[3], float vec1[3], float vec2[3])
{
    vec0[0] = vec1[1] * vec2[2] - vec1[2] * vec2[1];
    vec0[1] = vec1[2] * vec2[0] - vec1[0] * vec2[2];
    vec0[2] = vec1[0] * vec2[1] - vec1[1] * vec2[0];
}

void newWave :: norm(float vec[3])
{
    float c = sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
    scalDiv(vec, c); 
}

void newWave :: set(float vec[3], float x, float y, float z)
{
    vec[0] = x;
    vec[1] = y;
    vec[2] = z;
}

/////////////////////////////////////////////////////////
// getFaceNorms
// face normals - for flat shading
/////////////////////////////////////////////////////////

void newWave :: getFaceNorms(void)
{
    float vec0[3], vec1[3], vec2[3], norm0[3], norm1[3];
    float geom0[3], geom1[3], geom2[3], geom3[3];
    for (int i = 0; i < grid-1; ++i)
    {
        for ( int j = 0; j < grid-1; ++j)
        {
            /* get vectors from geometry points */
            geom0[0] = i; geom0[1] = j; geom0[2] = posit[i][j];
            geom1[0] = i; geom1[1] = j+1; geom1[2] = posit[i][j+1];
            geom2[0] = i+1; geom2[1] = j; geom2[2] = posit[i+1][j];
            geom3[0] = i+1; geom3[1] = j+1; geom3[2] = posit[i+1][j+1];

            sub( vec0, geom1, geom0 );
            sub( vec1, geom1, geom2 );
            sub( vec2, geom1, geom3 );

            /* get triangle face normals from vectors & normalize them */
            cross( norm0, vec0, vec1 );
            norm( norm0 );

            cross( norm1, vec1, vec2 ); 
            norm( norm1 );

            copy( faceNorms[0][i][j], norm0 );
            copy( faceNorms[1][i][j], norm1 );
        }
    }
}
/////////////////////////////////////////////////////////
// getVertNorms
// vertex normals - average of face normals for smooth shading
/////////////////////////////////////////////////////////
void newWave :: getVertNorms(void)
{
    float avg[3];
    for ( int i = 0; i < grid; ++i)
    {
        for ( int j = 0; j < grid; ++j)
        {
            /* For each vertex, average normals from all faces sharing */
            /* vertex.  Check each quadrant in turn */
            set(avg, 0.0, 0.0, 0.0);

            /* Right & above */
            if (j < grid-1 && i < grid-1)
            {
                add( avg, avg, faceNorms[0][i][j] );
            }
            /* Right & below */
            if (j < grid-1 && i > 0)
            {
                add( avg, avg, faceNorms[0][i-1][j] );
                add( avg, avg, faceNorms[1][i-1][j] );
            }
            /* Left & above */
            if (j > 0 && i < grid-1)
            {
                add( avg, avg, faceNorms[0][i][j-1] );
                add( avg, avg, faceNorms[1][i][j-1] );
            }
            /* Left & below */
            if (j > 0 && i > 0)
            {
                add( avg, avg, faceNorms[1][i-1][j-1] );
            }

            /* Normalize */
            norm( avg );
            copy( vertNorms[i][j], avg );
        }
    }
}

/////////////////////////////////////////////////////////
// getFaceNormSegs
//
/////////////////////////////////////////////////////////
void newWave :: getFaceNormSegs(void)
{
    float center0[3], center1[3], normSeg0[3], normSeg1[3];
    float geom0[3], geom1[3], geom2[3], geom3[3];
    for ( int i = 0; i < grid - 1; ++i)
    {
        for ( int j = 0; j < grid - 1; ++j)
        {
            geom0[0] = i; geom0[1] = j; geom0[2] = posit[i][j];
            geom1[0] = i; geom1[1] = j+1; geom1[2] = posit[i][j+1];
            geom2[0] = i+1; geom2[1] = j; geom2[2] = posit[i+1][j];
            geom3[0] = i+1; geom3[1] = j+1; geom3[2] = posit[i+1][j+1];

            /* find center of triangle face by averaging three vertices */
            add( center0, geom2, geom0 );
            add( center0, center0, geom1 );
            scalDiv( center0, 3.0 );

            add( center1, geom2, geom1 );
            add( center1, center1, geom3 );
            scalDiv( center1, 3.0 );

            /* translate normal to center of triangle face to get normal segment */
            add( normSeg0, center0, faceNorms[0][i][j] );
            add( normSeg1, center1, faceNorms[1][i][j] );

            copy( faceNormSegs[0][0][i][j], center0 );
            copy( faceNormSegs[1][0][i][j], center1 );

            copy( faceNormSegs[0][1][i][j], normSeg0 );
            copy( faceNormSegs[1][1][i][j], normSeg1 );
        }
    }
}

void newWave :: reset(int value)
{
    if (value != CURRENT)
        resetMode = value;
    for( int i=0;i<grid;i++)
        for( int j=0;j<grid;j++)
        {
            force[i][j]=0.0;
            veloc[i][j]=0.0;

            switch(resetMode)
            {
            case FLAT:
                posit[i][j] = 0.0;
                break;
            case SPIKE:
                 posit[i][j]= (i==j && i == grid/2) ? grid*1.5 : 0.0;
                break;
            case HOLE:
                posit[i][j]= (!((i > grid/3 && j > grid/3)&&(i < grid*2/3 && j < grid*2/3))) ? grid/4 : 0.0;
                break;
            case DIAGONALWALL:
                posit[i][j]= (((grid-i)-j<3) && ((grid-i)-j>0)) ? grid/6 : 0.0;
                break;
            case SIDEWALL:
                posit[i][j]= (i==1) ? grid/4 : 0.0;
                break;
            case DIAGONALBLOCK:
                posit[i][j]= ((grid-i)-j<3) ? grid/6 : 0.0;
                break;
            case MIDDLEBLOCK:
                posit[i][j]= ((i > grid/3 && j > grid/3)&&(i < grid*2/3 && j < grid*2/3)) ? grid/4 : 0.0;
                break;
            case CORNERBLOCK:
                posit[i][j]= ((i > grid*3/4 && j > grid*3/4)) ? grid/4 : 0.0;
                break;
            case HILL:
                posit[i][j]= 
                    (sin(M_PI * ((float)i/(float)grid)) +
                     sin(M_PI * ((float)j/(float)grid)))* grid/6.0;
				break;
            case HILLFOUR:
                posit[i][j]= 
                    (sin(M_PI*2 * ((float)i/(float)grid)) +
                     sin(M_PI*2 * ((float)j/(float)grid)))* grid/6.0;
				break;        
            }
            if (i==0||j==0||i==grid-1||j==grid-1) posit[i][j]=0.0;
        }
}

void newWave :: setOther(int value)
{
  switch(value){
  case 1:
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    break;
  default:
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
  }
}
/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void newWave :: obj_setupCallback(t_class *classPtr)
{
    class_addmethod(classPtr, (t_method)&newWave::heightMessCallback,
    	    gensym("height"), A_FLOAT, A_NULL);
    class_addmethod(classPtr, (t_method)&newWave::modeMessCallback,
    	    gensym("mode"), A_FLOAT, A_NULL);
    class_addmethod(classPtr, (t_method)&newWave::blendMessCallback,
    	    gensym("blend"), A_FLOAT, A_NULL);
    class_addmethod(classPtr, (t_method)&newWave::textureMessCallback,
    	    gensym("texture"), A_FLOAT, A_NULL);
	class_addmethod(classPtr, (t_method)&newWave::setK1MessCallback,
	   	    gensym("K1"), A_FLOAT, A_NULL);
	class_addmethod(classPtr, (t_method)&newWave::setD1MessCallback,
		    gensym("D1"), A_FLOAT, A_NULL);
	class_addmethod(classPtr, (t_method)&newWave::setK2MessCallback,
	   	    gensym("K2"), A_FLOAT, A_NULL);
	class_addmethod(classPtr, (t_method)&newWave::setD2MessCallback,
		    gensym("D2"), A_FLOAT, A_NULL);
	class_addmethod(classPtr, (t_method)&newWave::setK3MessCallback,
	   	    gensym("K3"), A_FLOAT, A_NULL);
	class_addmethod(classPtr, (t_method)&newWave::setD3MessCallback,
		    gensym("D3"), A_FLOAT, A_NULL);
	class_addmethod(classPtr, (t_method)&newWave::forceMessCallback,
		    gensym("force"), A_FLOAT, A_FLOAT, A_FLOAT, A_NULL);
	class_addmethod(classPtr, (t_method)&newWave::positionMessCallback,
		    gensym("position"), A_FLOAT, A_FLOAT, A_FLOAT, A_NULL);
	class_addbang(classPtr, (t_method)&newWave::bangMessCallback);
}

void newWave :: bangMessCallback(void *data)
{
   	GetMyClass(data)->bangMess();
    
}
void newWave :: heightMessCallback(void *data, t_floatarg size)
{
    GetMyClass(data)->heightMess((float)size);
}
void newWave :: forceMessCallback(void *data, t_floatarg posX, t_floatarg posY, t_floatarg valforce )
{
    GetMyClass(data)->forceMess((float)posX, (float)posY, (float)valforce);
}
void newWave :: positionMessCallback(void *data, t_floatarg posX, t_floatarg posY, t_floatarg posZ)
{
    GetMyClass(data)->positionMess((float)posX, (float)posY, (float)posZ);
}
void newWave :: modeMessCallback(void *data, t_floatarg mode)
{
    GetMyClass(data)->modeMess((float)mode);
}

void newWave :: blendMessCallback(void *data, t_floatarg size)
{
    GetMyClass(data)->m_blend=((int)size);
}

void newWave :: setK1MessCallback(void *data, t_floatarg K)
{
    GetMyClass(data)->K1=((float)K);
}
void newWave :: setK2MessCallback(void *data, t_floatarg K)
{
    GetMyClass(data)->K2=((float)K);
}
void newWave :: setK3MessCallback(void *data, t_floatarg K)
{
    GetMyClass(data)->K3=((float)K);
}


void newWave :: setD1MessCallback(void *data, t_floatarg D)
{
    GetMyClass(data)->D1=((float)D);
}
void newWave :: setD2MessCallback(void *data, t_floatarg D)
{
    GetMyClass(data)->D2=((float)D);
}
void newWave :: setD3MessCallback(void *data, t_floatarg D)
{
    GetMyClass(data)->D3=((float)D);
}

void newWave :: textureMessCallback(void *data, t_floatarg D)
{
    GetMyClass(data)->textureMess((int)D);
}



