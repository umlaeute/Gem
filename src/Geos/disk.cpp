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

/////////////////////////////////////////////////////////
// 2105:forum::für::umläute:2000
/////////////////////////////////////////////////////////
// added the gluPartialDisk
/////////////////////////////////////////////////////////

#include "disk.h"

CPPEXTERN_NEW_WITH_GIMME(disk)

/////////////////////////////////////////////////////////
//
// disk
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
disk :: disk(int argc, t_atom *argv)
      : GemGluObj(1.f)
{
	float size = 1.f;
	int numSlices = 10;
	m_innerRadius = 0.f;

    if (argc == 3)
    {
		size = atom_getfloat(&argv[0]);
		numSlices = (int)atom_getfloat(&argv[1]);
		m_innerRadius = atom_getfloat(&argv[2]);
    }
    else if (argc == 2)
    {
        size = atom_getfloat(&argv[0]);
        numSlices = (int)atom_getfloat(&argv[1]);
    }
    else if (argc == 1)
    {
		size = atom_getfloat(&argv[0]);
    }
    else
    {
        error("GEM: disk: needs 0, 2 or 3 arguments");
    }
	
	sizeMess(size);
	numSlicesMess(numSlices);
    inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"), gensym("inner"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
disk :: ~disk()
{ }

/////////////////////////////////////////////////////////
// innerRadius
//
/////////////////////////////////////////////////////////
void disk :: innerRadius(float radius)
{
    m_innerRadius = radius;
    setModified();
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void disk :: render(GemState *state)
{
    // lighting
    if (state->lighting)
    {
    	// normals
    	if (state->smooth)
			gluQuadricNormals(m_thing, (GLenum) GLU_SMOOTH);
    	else
			gluQuadricNormals(m_thing, (GLenum) GLU_FLAT);
    }
    
    // texture mapping
    if (state->texture)
		gluQuadricTexture(m_thing, GL_TRUE);
    else
		gluQuadricTexture(m_thing, GL_FALSE);
    
    gluQuadricDrawStyle(m_thing, m_drawType);

	gluDisk(m_thing, m_innerRadius, m_size, m_numSlices, m_numSlices);
}

/////////////////////////////////////////////////////////
// static member functions
//
/////////////////////////////////////////////////////////
void disk :: obj_setupCallback(t_class *classPtr)
{
    class_addmethod(classPtr, (t_method)&disk::innerRadiusCallback,
    	    gensym("inner"), A_FLOAT, A_NULL); 
}
void disk :: innerRadiusCallback(void *data, t_floatarg radius)
{
    GetMyClass(data)->innerRadius(radius);
}

