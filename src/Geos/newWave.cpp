/*
 *  GEM - Graphics Environment for Multimedia
 *
 *  newWave.h
 *  gem_darwin
 *
 *  Created by Jamie Tittle on Thu Oct 10 2002.
 *  Copyright (c) 2002 tigital. All rights reserved.
 *    For information on usage and redistribution, and for a DISCLAIMER OF ALL
 *    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
 *
 */

#include "newWave.h"
#include "Base/GemState.h"
#include <string.h>
#include <math.h>

#ifndef M_PI	
#define M_PI 3.1415926535898
#endif

/* Grid */
enum {WIREFRAME, HIDDENLINE, FLATSHADED, SMOOTHSHADED, TEXTURED};
enum {FACENORMALS, ENVMAP};
enum {VWEAK, WEAK, NORMAL, STRONG};
enum {SMALL, MEDIUM, LARGE, XLARGE};
enum {CURRENT, FLAT, SPIKE, DIAGONALWALL, SIDEWALL, HOLE, 
      MIDDLEBLOCK, DIAGONALBLOCK, CORNERBLOCK, HILL, HILLFOUR};
int displayMode = WIREFRAME;
int resetMode = DIAGONALBLOCK;
int grid = 17;
float dt = 0.004;

bool waving = false, editing = false, 
     drawFaceNorms = false, antialias = false,
     envMap = false;

#define SQRTOFTWOINV 1.0 / 1.414213562

static inline int powerOfTwo(int value)
{
    int x = 1;
    while(x < value) x <<= 1;

    return(x);
}


CPPEXTERN_NEW_WITH_TWO_ARGS(newWave, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// newWave
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
newWave :: newWave( t_floatarg width, t_floatarg height )
    	     : GemShape(width), m_height(height), m_size(MEDIUM),
               m_speed(NORMAL), alreadyInit(0)
{
    if (m_height == 0.f)
		m_height = 1.f;

    // the height inlet
    m_inletH = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("Ht"));
    inletM = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("M"));
    inletSp = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("Sp"));

    m_blend = 0;
    m_drawType = GL_TRIANGLE_STRIP;
    alreadyInit = 0;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
newWave :: ~newWave()
{
	 inlet_free(m_inletH);
         inlet_free(inletM);
         inlet_free(inletSp);
         alreadyInit = 0;
}

void newWave :: modeMess(float mode)
{
    reset((int)mode);
    setModified();
}
void newWave :: speedMess(float speed)
{
    setSpeed((float)speed);
    setModified();
    
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void newWave :: render(GemState *state)
{
    int i, j;
    
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
#ifdef __APPLE__
        if (xsize != state->texCoords[1].s)
#else
        if (xsize != state->texCoords[2].s)
#endif
            alreadyInit = 0;
        
        if (!alreadyInit)
        {
#ifdef __APPLE__
            xsize = state->texCoords[1].s;
            ysize = state->texCoords[1].t;
#else
            xsize = state->texCoords[2].s;
            ysize = state->texCoords[2].t;
#endif
            setSize( SMALL );
            setSpeed( NORMAL);
            setOther( ENVMAP );
            reset( HILLFOUR );
            alreadyInit = 1;
        }
    
        getFaceNorms();
        getVertNorms();
    
        getforce();
        getvelocity();
        getposition();
        for (int i=0; i<grid -1; ++i)
        {
            glBegin(GL_TRIANGLE_STRIP);
            for (int j = 0; j < grid; ++j)
            {
                glNormal3fv( vertNorms[i][j] );
                glTexCoord2fv( texCoords[i][j] );
                glVertex3f( i, j, posit[i][j] );
            
                glNormal3fv( vertNorms[i+1][j] );
                glTexCoord2fv( texCoords[i+1][j] );
                glVertex3f( i+1, j, posit[i+1][j] );
            }
            glEnd();
        }
    }else
    {
        if (!alreadyInit)
        {
            xsize = 1;
            ysize = 1;
            setSize( SMALL );
            setSpeed( NORMAL);
            setOther( ENVMAP );
            reset( HILLFOUR );
            alreadyInit = 1;
        }
    
        getFaceNorms();
        getVertNorms();
    
        getforce();
        getvelocity();
        getposition();
        
        for ( i=0; i<grid -1; ++i)
        {
            glBegin(m_drawType);
            for ( j = 0; j < grid; ++j)
            {
                glNormal3fv( vertNorms[i][j] );
                glTexCoord2fv( texCoords[i][j] );
                glVertex3f( i, j, posit[i][j] );
            
                glNormal3fv( vertNorms[i+1][j] );
                glTexCoord2fv( texCoords[i+1][j] );
                glVertex3f( i+1, j, posit[i+1][j] );
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
    post("height changed %f",m_height);
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
    for ( i=2; i<grid-2; i++)
        for (int j=2;j<grid-2; j++)
        {
            d = posit[i][j] - posit[i][j-1];
            force[i][j] -= d;
            force[i][j-1] += d;
            
            d = posit[i][j] - posit[i-1][j];
            force[i][j] -= d;
            force[i-1][j] += d;
            
            d = (posit[i][j] - posit[i][j+1]);
            force[i][j] -= d;
            force[i][j+1] += d;
            
            d = (posit[i][j] - posit[i+1][j]);
            force[i][j] -= d;
            force[i+1][j] += d;
            
            d= (posit[i][j]-posit[i+1][j+1])*SQRTOFTWOINV; 
            force[i][j] -= d ;
            force[i+1][j+1] += d;

            d= (posit[i][j]-posit[i-1][j-1])*SQRTOFTWOINV; 
            force[i][j] -= d ;
            force[i-1][j-1] += d;

            d= (posit[i][j]-posit[i+1][j-1])*SQRTOFTWOINV; 
            force[i][j] -= d ;
            force[i+1][j-1] += d;

            d= (posit[i][j]-posit[i-1][j+1])*SQRTOFTWOINV; 
            force[i][j] -= d ;
            force[i- 1][j+1] += d;
        }
}

/////////////////////////////////////////////////////////
// getvelocity
//
/////////////////////////////////////////////////////////
void newWave :: getvelocity()
{
    for (int i=0; i<grid; i++)
        for (int j=0;j<grid; j++)
            veloc[i][j] += force[i][j] * dt;
}

/////////////////////////////////////////////////////////
// getposition
//
/////////////////////////////////////////////////////////
void newWave :: getposition()
{
    for ( int i=0; i<grid; i++)
        for ( int j=0;j<grid; j++)
            posit[i][j] += veloc[i][j];
}

/////////////////////////////////////////////////////////
// getTexCoords
//
/////////////////////////////////////////////////////////
void newWave :: getTexCoords()
{
    for ( int i = 0; i < grid; ++i)
    {
        for ( int j = 0; j < grid; ++j)
        {
            texCoords[i][j][0] = ( ((float)j/(float)(grid-1)) );
            texCoords[i][j][1] = ( ((float)i/(float)(grid-1)) );
            //post("texCoords[%d][%d][0] = %f",i,j,texCoords[i][j][0]);
            //post("texCoords[%d][%d][1] = %f",i,j,texCoords[i][j][1]);
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
    switch(value) 
    {
        case SMALL : grid = MAXGRID/4; break;
        case MEDIUM: grid = MAXGRID/2; break;
        case LARGE : grid = (int)(MAXGRID/1.5); break;
        case XLARGE : grid = MAXGRID; break;
    }
    if (prevGrid > grid)
    {
        reset(resetMode);
    }

    getTexCoords();
}

/////////////////////////////////////////////////////////
// setSpeed
//
/////////////////////////////////////////////////////////
void newWave :: setSpeed(float value)
{
  /*  switch(value) 
    {
        case VWEAK : dt = 0.0005; break;
        case WEAK  : dt = 0.001; break;
        case NORMAL: dt = 0.004; break;
        case STRONG: dt = 0.008; break;
    } */
    dt = value * 0.001;
    post("speed changed %f",dt);
    
    
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
    //if (waving)
    //    stop();

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
    switch (value)
    {
        case FACENORMALS: 
            drawFaceNorms = !drawFaceNorms;
            break;
        case ENVMAP: 
            envMap = !envMap;
            if (envMap)
            {
                glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
                glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
                glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
                glEnable(GL_TEXTURE_GEN_S);
                glEnable(GL_TEXTURE_GEN_T);
            }
            else
            {
                glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
                glDisable(GL_TEXTURE_GEN_S);
                glDisable(GL_TEXTURE_GEN_T);
            }
            break;
    }
}
/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void newWave :: obj_setupCallback(t_class *classPtr)
{
    class_addmethod(classPtr, (t_method)&newWave::heightMessCallback,
    	    gensym("Ht"), A_FLOAT, A_NULL);
    class_addmethod(classPtr, (t_method)&newWave::modeMessCallback,
    	    gensym("M"), A_FLOAT, A_NULL);
    class_addmethod(classPtr, (t_method)&newWave::speedMessCallback,
    	    gensym("Sp"), A_FLOAT, A_NULL);
    class_addmethod(classPtr, (t_method)&newWave::blendMessCallback,
    	    gensym("blend"), A_FLOAT, A_NULL);
}

void newWave :: heightMessCallback(void *data, t_floatarg size)
{
    GetMyClass(data)->heightMess((float)size);
}
void newWave :: modeMessCallback(void *data, t_floatarg mode)
{
    GetMyClass(data)->modeMess((float)mode);
}
void newWave :: speedMessCallback(void *data, t_floatarg speed)
{
    GetMyClass(data)->speedMess((float)speed);
}
void newWave :: blendMessCallback(void *data, t_floatarg size)
{
    GetMyClass(data)->m_blend=((int)size);
}
