////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-2000 Mark Danks.
//    Copyright (c) G�nther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f�r::uml�ute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "primTri.h"

#include "Base/Matrix.h"
#include "Base/GemState.h"
#include <string.h>

CPPEXTERN_NEW_WITH_ONE_ARG(primTri, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// primTri
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
primTri :: primTri(t_floatarg size)
{
    m_drawType = GL_TRIANGLES;

	mVectors[0][0] = 0.f;
	mVectors[0][1] = 1.f;

	mVectors[1][0] = 1.f;
	mVectors[1][1] = -1.f;

	mVectors[2][0] = -1.f;
	mVectors[2][1] = -1.f;

	mVectors[0][2] = mVectors[1][2] = mVectors[2][2] = 0.f;

	mColors[0][0] = mColors[1][0] = mColors[2][0] = 1.f;
	mColors[0][1] = mColors[1][1] = mColors[2][1] = 1.f;
	mColors[0][2] = mColors[1][2] = mColors[2][2] = 1.f;
	mColors[0][3] = mColors[1][3] = mColors[2][3] = 1.f;

    // create the new inlets
    inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_list, gensym("vect1"));
    inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_list, gensym("vect2"));
    inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_list, gensym("vect3"));
    inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_list, gensym("col1"));
    inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_list, gensym("col2"));
    inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_list, gensym("col3"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
primTri :: ~primTri()
{ }

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void primTri :: render(GemState *state)
{
	float norm[3];
	Matrix::generateNormal(mVectors[0], mVectors[1], mVectors[2], norm);
    glNormal3fv(norm);
	if (!state->lighting)
		glShadeModel(GL_SMOOTH);

    if (state->texture && state->numTexCoords)
    {
        int curCoord = 0;
	    glBegin(m_drawType);
	        glTexCoord2f(state->texCoords[curCoord].s, state->texCoords[curCoord].t);
			glColor4fv(mColors[0]);
   	        glVertex3fv(mVectors[0]);

	        if (state->numTexCoords > 1)
				curCoord = 1;
	    	glTexCoord2f(state->texCoords[curCoord].s, state->texCoords[curCoord].t);
			glColor4fv(mColors[1]);
   	        glVertex3fv(mVectors[1]);

	        if (state->numTexCoords > 2)
				curCoord = 2;
	    	glTexCoord2f(state->texCoords[curCoord].s, state->texCoords[curCoord].t);
			glColor4fv(mColors[2]);
   	        glVertex3fv(mVectors[2]);
	    glEnd();
    }
    else
    {
	    glBegin(m_drawType);
    	        glTexCoord2f(0.f, 0.f);
 				glColor4fv(mColors[0]);
   				glVertex3fv(mVectors[0]);
    	        
				glTexCoord2f(1.f, 0.f);
 				glColor4fv(mColors[1]);
   				glVertex3fv(mVectors[1]);
    	        
				glTexCoord2f(.5f, 1.f);
 				glColor4fv(mColors[2]);
   				glVertex3fv(mVectors[2]);
	    glEnd();
    }

	if (!state->lighting)
		glShadeModel(GL_FLAT);
}

/////////////////////////////////////////////////////////
// typeMess
//
/////////////////////////////////////////////////////////
void primTri :: typeMess(t_symbol *type)
{
    if (!strcmp(type->s_name, "line")) 
	    m_drawType = GL_LINE_LOOP;
    else if (!strcmp(type->s_name, "fill")) 
	    m_drawType = GL_TRIANGLES;
    else if (!strcmp(type->s_name, "point"))
	    m_drawType = GL_POINTS;
    else
    {
	    error ("GEM: primTri: draw style");
	    return;
    }
    setModified();
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void primTri :: obj_setupCallback(t_class *classPtr)
{
    class_addmethod(classPtr, (t_method)&primTri::typeMessCallback,
    	    gensym("draw"), A_SYMBOL, A_NULL);

    class_addmethod(classPtr, (t_method)&primTri::vect1MessCallback,
    	    gensym("vect1"), A_FLOAT, A_FLOAT, A_FLOAT, A_NULL); 
    class_addmethod(classPtr, (t_method)&primTri::vect2MessCallback,
    	    gensym("vect2"), A_FLOAT, A_FLOAT, A_FLOAT, A_NULL); 
    class_addmethod(classPtr, (t_method)&primTri::vect3MessCallback,
    	    gensym("vect3"), A_FLOAT, A_FLOAT, A_FLOAT, A_NULL); 
    class_addmethod(classPtr, (t_method)&primTri::col1MessCallback,
    	    gensym("col1"), A_GIMME, A_NULL); 
    class_addmethod(classPtr, (t_method)&primTri::col2MessCallback,
    	    gensym("col2"), A_GIMME, A_NULL); 
    class_addmethod(classPtr, (t_method)&primTri::col3MessCallback,
    	    gensym("col3"), A_GIMME, A_NULL); 
}
void primTri :: typeMessCallback(void *data, t_symbol *type)
{
    GetMyClass(data)->typeMess(type);
}

void primTri :: vect1MessCallback(void *data, t_floatarg x, t_floatarg y, t_floatarg z)
{
    GetMyClass(data)->vectMess(0, (float)x, (float)y, (float)z);
}
void primTri :: vect2MessCallback(void *data, t_floatarg x, t_floatarg y, t_floatarg z)
{
    GetMyClass(data)->vectMess(1, (float)x, (float)y, (float)z);
}
void primTri :: vect3MessCallback(void *data, t_floatarg x, t_floatarg y, t_floatarg z)
{
    GetMyClass(data)->vectMess(2, (float)x, (float)y, (float)z);
}

void primTri :: col1MessCallback(void *data, t_symbol *, int argc, t_atom *argv)
{
    float alpha = 1.f;
    if (argc == 4)
		alpha = atom_getfloat(&argv[3]);
    GetMyClass(data)->colMess(0, atom_getfloat(&argv[0]), atom_getfloat(&argv[1]),
    	    	    	       atom_getfloat(&argv[2]), alpha);
}
void primTri :: col2MessCallback(void *data, t_symbol *, int argc, t_atom *argv)
{
    float alpha = 1.f;
    if (argc == 4)
		alpha = atom_getfloat(&argv[3]);
    GetMyClass(data)->colMess(1, atom_getfloat(&argv[0]), atom_getfloat(&argv[1]),
    	    	    	       atom_getfloat(&argv[2]), alpha);
}
void primTri :: col3MessCallback(void *data, t_symbol *, int argc, t_atom *argv)
{
    float alpha = 1.f;
    if (argc == 4)
		alpha = atom_getfloat(&argv[3]);
    GetMyClass(data)->colMess(2, atom_getfloat(&argv[0]), atom_getfloat(&argv[1]),
    	    	    	       atom_getfloat(&argv[2]), alpha);
}
