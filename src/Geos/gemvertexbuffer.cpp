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
//    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "gemvertexbuffer.h"

#include "Gem/State.h"
#include "Gem/Manager.h"
#include <math.h>
#include <stdio.h>

#if defined (__APPLE__) || defined(MACOSX)
   #define GL_SHARING_EXTENSION "cl_APPLE_gl_sharing"
#else
   #define GL_SHARING_EXTENSION "cl_khr_gl_sharing"
#endif

#define UNIX

CPPEXTERN_NEW_WITH_ONE_ARG(gemvertexbuffer, t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// gemvertexbuffer
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
gemvertexbuffer :: gemvertexbuffer(t_floatarg size)
{ 
	//~ printf("gemvertexbuffer build on %s at %s\n", __DATE__, __TIME__);
	vbo_size = size>0?size:256*256;
	
	posArray = new float[vbo_size*3];
	texArray = new float[vbo_size*2];
	colArray = new float[vbo_size*4];
	normArray = new float[vbo_size*3];
	
	posVBO_enable = 0;
	colVBO_enable = 0;
	texVBO_enable = 0;
	normVBO_enable = 0;
	
	posVBO = texVBO = colVBO = normVBO = 0;
	 
	size_change_flag = 0;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
gemvertexbuffer :: ~gemvertexbuffer()
{
	cleanUp();
}

/////////////////////////////////////////////////////////
// cleanup
//
/////////////////////////////////////////////////////////
void gemvertexbuffer :: cleanUp()
{
	// delete VBO
	//~ printf("delete VBO\n");
	if ( posVBO ){
		glBindBuffer(1, posVBO);
		glDeleteBuffers(1, &posVBO);
	}
    if ( texVBO ){
		glBindBuffer(1, texVBO);
		glDeleteBuffers(1, &texVBO);
	}
    if ( colVBO ){
		glBindBuffer(1, colVBO);
		glDeleteBuffers(1, &colVBO);
	}
	if ( normVBO ){
		glBindBuffer(1, normVBO);
		glDeleteBuffers(1, &normVBO);
	}
    
    posVBO = texVBO = colVBO = normVBO = 0;
    
    // delete Array 
    //~ printf("free array\n");
    free(posArray);
    free(texArray);
    free(colArray);
    free(normArray);
}

/////////////////////////////////////////////////////////
// renderShape
//
/////////////////////////////////////////////////////////
void gemvertexbuffer :: renderShape(GemState *state)
{
	if ( m_drawType == GL_DEFAULT_GEM ) m_drawType = GL_POINTS;
	if ( !posVBO || !texVBO || !colVBO || !normVBO || size_change_flag ) {
//		printf("create VBO\n");
		createVBO();
		size_change_flag = 0;
	}	
		// render from the VBO
		if ( posVBO_enable )
		{
			glBindBuffer(GL_ARRAY_BUFFER, posVBO);
			if ( posArray_dirty ) {
				//~ printf("push pos vertex\n");
				glBufferData(GL_ARRAY_BUFFER, vbo_size * 3 * sizeof(float), posArray, GL_DYNAMIC_DRAW);
				posArray_dirty = 0;
			}
			glVertexPointer(3, GL_FLOAT, 0, 0);
		}

		if ( texVBO_enable )
		{
			glBindBuffer(GL_ARRAY_BUFFER, texVBO);
			if ( texArray_dirty ) {
				//~ printf("push tex vertex\n");
				glBufferData(GL_ARRAY_BUFFER, vbo_size * 2 * sizeof(float), texArray, GL_DYNAMIC_DRAW);
				texArray_dirty = 0;
			}
			glTexCoordPointer(2, GL_FLOAT, 0, 0);
		}
		
		if ( colVBO_enable )
		{
			glBindBuffer(GL_ARRAY_BUFFER, colVBO);
			if ( colArray_dirty ) {
				//~ printf("push col vertex\n");
				glBufferData(GL_ARRAY_BUFFER, vbo_size * 4 * sizeof(float), colArray, GL_DYNAMIC_DRAW);
				colArray_dirty = 0;
			}
			glColorPointer(4, GL_FLOAT, 0, 0);
		}
		
		if ( normVBO_enable )
		{
			glBindBuffer(GL_ARRAY_BUFFER, normVBO);
			if ( normArray_dirty ) {
				//~ printf("push norm vertex\n");
				glBufferData(GL_ARRAY_BUFFER, vbo_size * 3 * sizeof(float), normArray, GL_DYNAMIC_DRAW);
				normArray_dirty = 0;
			}
			glNormalPointer(GL_FLOAT, 0, 0);
		}
		
		if ( posVBO_enable ) glEnableClientState(GL_VERTEX_ARRAY);
		if ( colVBO_enable ) glEnableClientState(GL_COLOR_ARRAY);
		if ( texVBO_enable ) glEnableClientState(GL_TEXTURE_COORD_ARRAY); 
		if ( normVBO_enable ) glEnableClientState(GL_NORMAL_ARRAY); 
		
		glDrawArrays(m_drawType, 0, vbo_size);
		
		if ( posVBO_enable ) glDisableClientState(GL_VERTEX_ARRAY);
		if ( colVBO_enable ) glDisableClientState(GL_COLOR_ARRAY);
		if ( texVBO_enable ) glDisableClientState(GL_TEXTURE_COORD_ARRAY); 
		if ( normVBO_enable ) glDisableClientState(GL_NORMAL_ARRAY); 
		
}

/////////////////////////////////////////////////////////
// Setup callback functions
//
/////////////////////////////////////////////////////////
void gemvertexbuffer :: obj_setupCallback(t_class *classPtr)
{
	class_addmethod(classPtr, reinterpret_cast<t_method>(&gemvertexbuffer::posxMessCallback), 
		gensym("posX"), A_GIMME, A_NULL);
	class_addmethod(classPtr, reinterpret_cast<t_method>(&gemvertexbuffer::posyMessCallback), 
		gensym("posY"), A_GIMME, A_NULL);
	class_addmethod(classPtr, reinterpret_cast<t_method>(&gemvertexbuffer::poszMessCallback), 
		gensym("posZ"), A_GIMME, A_NULL);
	class_addmethod(classPtr, reinterpret_cast<t_method>(&gemvertexbuffer::colrMessCallback), 
		gensym("colorR"), A_GIMME, A_NULL);
	class_addmethod(classPtr, reinterpret_cast<t_method>(&gemvertexbuffer::colgMessCallback), 
		gensym("colorG"), A_GIMME, A_NULL);
	class_addmethod(classPtr, reinterpret_cast<t_method>(&gemvertexbuffer::colbMessCallback), 
		gensym("colorB"), A_GIMME, A_NULL);
	class_addmethod(classPtr, reinterpret_cast<t_method>(&gemvertexbuffer::colaMessCallback), 
		gensym("colorA"), A_GIMME, A_NULL);
	class_addmethod(classPtr, reinterpret_cast<t_method>(&gemvertexbuffer::texuMessCallback), 
		gensym("textureU"), A_GIMME, A_NULL);
	class_addmethod(classPtr, reinterpret_cast<t_method>(&gemvertexbuffer::texvMessCallback), 
		gensym("textureV"), A_GIMME, A_NULL);
	class_addmethod(classPtr, reinterpret_cast<t_method>(&gemvertexbuffer::normxMessCallback), 
		gensym("normalX"), A_GIMME, A_NULL);
	class_addmethod(classPtr, reinterpret_cast<t_method>(&gemvertexbuffer::normyMessCallback), 
		gensym("normalY"), A_GIMME, A_NULL);
	class_addmethod(classPtr, reinterpret_cast<t_method>(&gemvertexbuffer::normzMessCallback), 
		gensym("normalZ"), A_GIMME, A_NULL);
	class_addmethod(classPtr, reinterpret_cast<t_method>(&gemvertexbuffer::resizeMessCallback), 
		gensym("resize"), A_FLOAT, A_NULL);
	class_addmethod(classPtr, reinterpret_cast<t_method>(&gemvertexbuffer::posVBO_enableMessCallback), 
		gensym("posVBO_enable"), A_FLOAT, A_NULL);
	class_addmethod(classPtr, reinterpret_cast<t_method>(&gemvertexbuffer::colVBO_enableMessCallback), 
		gensym("colVBO_enable"), A_FLOAT, A_NULL);
	class_addmethod(classPtr, reinterpret_cast<t_method>(&gemvertexbuffer::texVBO_enableMessCallback), 
		gensym("texVBO_enable"), A_FLOAT, A_NULL);
	class_addmethod(classPtr, reinterpret_cast<t_method>(&gemvertexbuffer::normVBO_enableMessCallback), 
		gensym("normVBO_enable"), A_FLOAT, A_NULL);			
}

void gemvertexbuffer :: posxMessCallback(void *data, t_symbol *symbol, int argc, t_atom *argv)
{	
	GetMyClass(data)->tabMess(argc,argv, GetMyClass(data)->posArray, 3, 0);
	GetMyClass(data)->posArray_dirty = 1;
	GetMyClass(data)->posVBO_enable = 1;
	   
}
void gemvertexbuffer :: posyMessCallback(void *data, t_symbol *symbol, int argc, t_atom *argv)
{
	GetMyClass(data)->tabMess(argc,argv, GetMyClass(data)->posArray, 3, 1);
	GetMyClass(data)->posArray_dirty = 1;
	GetMyClass(data)->posVBO_enable = 1;
}
void gemvertexbuffer :: poszMessCallback(void *data, t_symbol *symbol, int argc, t_atom *argv)
{
	GetMyClass(data)->tabMess(argc,argv, GetMyClass(data)->posArray, 3, 2);
	GetMyClass(data)->posArray_dirty = 1;
	GetMyClass(data)->posVBO_enable = 1;
}

void gemvertexbuffer :: colrMessCallback(void *data, t_symbol *symbol, int argc, t_atom *argv)
{
	GetMyClass(data)->tabMess(argc,argv, GetMyClass(data)->colArray, 4, 0);
	GetMyClass(data)->colArray_dirty = 1;
	GetMyClass(data)->colVBO_enable = 1;
}

void gemvertexbuffer :: colgMessCallback(void *data, t_symbol *symbol, int argc, t_atom *argv)
{
	GetMyClass(data)->tabMess(argc,argv, GetMyClass(data)->colArray, 4, 1);
	GetMyClass(data)->colArray_dirty = 1;
	GetMyClass(data)->colVBO_enable = 1;
}

void gemvertexbuffer :: colbMessCallback(void *data, t_symbol *symbol, int argc, t_atom *argv)
{
	GetMyClass(data)->tabMess(argc,argv, GetMyClass(data)->colArray, 4, 2);
	GetMyClass(data)->colArray_dirty = 1;
	GetMyClass(data)->colVBO_enable = 1;
}

void gemvertexbuffer :: colaMessCallback(void *data, t_symbol *symbol, int argc, t_atom *argv)
{
	GetMyClass(data)->tabMess(argc,argv, GetMyClass(data)->colArray, 4, 3);
	GetMyClass(data)->colArray_dirty = 1;
	GetMyClass(data)->colVBO_enable = 1;
}

void gemvertexbuffer :: texuMessCallback(void *data, t_symbol *symbol, int argc, t_atom *argv)
{
	GetMyClass(data)->tabMess(argc,argv, GetMyClass(data)->texArray, 2, 0);
	GetMyClass(data)->texArray_dirty = 1;
	GetMyClass(data)->texVBO_enable = 1;
}

void gemvertexbuffer :: texvMessCallback(void *data, t_symbol *symbol, int argc, t_atom *argv)
{
	GetMyClass(data)->tabMess(argc,argv, GetMyClass(data)->texArray, 2, 1);
	GetMyClass(data)->texArray_dirty = 1;
	GetMyClass(data)->texVBO_enable = 1;
}

void gemvertexbuffer :: normxMessCallback(void *data, t_symbol *symbol, int argc, t_atom *argv)
{
	GetMyClass(data)->tabMess(argc,argv, GetMyClass(data)->normArray, 3, 0);
	GetMyClass(data)->normArray_dirty = 1;
	GetMyClass(data)->normVBO_enable = 1;
}

void gemvertexbuffer :: normyMessCallback(void *data, t_symbol *symbol, int argc, t_atom *argv)
{
	GetMyClass(data)->tabMess(argc,argv, GetMyClass(data)->normArray, 3, 1);
	GetMyClass(data)->normArray_dirty = 1;
	GetMyClass(data)->normVBO_enable = 1;
}

void gemvertexbuffer :: normzMessCallback(void *data, t_symbol *symbol, int argc, t_atom *argv)
{
	GetMyClass(data)->tabMess(argc,argv, GetMyClass(data)->normArray, 3, 2);
	GetMyClass(data)->normArray_dirty = 1;
	GetMyClass(data)->normVBO_enable = 1;
}

void gemvertexbuffer :: resizeMessCallback(void *data, float size)
{
	GetMyClass(data)->resizeMess(size);
}

void gemvertexbuffer :: posVBO_enableMessCallback(void *data, float flag)
{
	GetMyClass(data)->posVBO_enable = flag != 0;
}

void gemvertexbuffer :: colVBO_enableMessCallback(void *data, float flag)
{
	GetMyClass(data)->colVBO_enable = flag != 0;
}

void gemvertexbuffer :: texVBO_enableMessCallback(void *data, float flag)
{
	GetMyClass(data)->texVBO_enable = flag != 0;
}

void gemvertexbuffer :: normVBO_enableMessCallback(void *data, float flag)
{
	GetMyClass(data)->normVBO_enable = flag != 0;
}

void gemvertexbuffer :: tabMess(int argc, t_atom *argv, float *array, int stride, int offset)
{
	int offset2 = 0;
	t_symbol *tab_name;
	if ( argv[0].a_type != A_SYMBOL ) 
	{
		error("first arg must be symbol (table name)");
		return;
	}
	if ( argc > 1 ) 
	{
		if ( argv[1].a_type != A_FLOAT )
		{
			error("second arg must be float (offset)");
		}
		else offset2 = argv[1].a_w.w_float;
	}
	offset2 = offset2<0?0:offset2;
	tab_name = argv[0].a_w.w_symbol;
	copyArray(tab_name, array, stride, offset2 * stride + offset);
}

void gemvertexbuffer :: resizeMess(float size)
{
	vbo_size = size>1?size:1;
	//~ printf("cleanup\n");
	cleanUp();
	//~ printf("define array\n");
	posArray = new float[vbo_size*3];
	texArray = new float[vbo_size*2];
	colArray = new float[vbo_size*4];
	normArray = new float[vbo_size*3];
	//~ printf("printf clear VBO\n");
	size_change_flag = 1;
}

// Create VBO
//*****************************************************************************
void gemvertexbuffer :: createVBO()
{
    // create VBO
 	unsigned int i, j;
	
	// create buffer object
	//~ printf("create buffer object\n");
	glGenBuffers(1, &posVBO);
	glGenBuffers(1, &colVBO);
	glGenBuffers(1, &normVBO);
	glGenBuffers(1, &texVBO);
	
	//~ printf("bind buffer\n");
	glBindBuffer(GL_ARRAY_BUFFER, posVBO);
	glBufferData(GL_ARRAY_BUFFER, vbo_size * 3 * sizeof(float), posArray, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, texVBO);
	glBufferData(GL_ARRAY_BUFFER, vbo_size * 2 * sizeof(float), texArray, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, colVBO);
	glBufferData(GL_ARRAY_BUFFER, vbo_size * 4 * sizeof(float), colArray, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, normVBO);
	glBufferData(GL_ARRAY_BUFFER, vbo_size * 3 * sizeof(float), normArray, GL_DYNAMIC_DRAW);
	
	//~ printf("done\n");
}

void gemvertexbuffer :: copyArray(t_symbol *tab_name, float *array, unsigned int stride, unsigned int offset)
{
	t_garray *a;
	int npoints, i;
	t_word *vec;
	t_float posx;
	
	//~ printf("copy table %s to array\n", tab_name->s_name);
	pd_findbyclass(tab_name, garray_class);
	if (!(a = (t_garray *)pd_findbyclass(tab_name, garray_class)))
		error("%s: no such array", tab_name->s_name);
    else if (!garray_getfloatwords(a, &npoints, &vec))
        error("%s: bad template for tabLink", tab_name->s_name);
	else
    {		
		npoints = npoints>vbo_size?vbo_size:npoints;
		//~ printf("start copying %d values\n",npoints);
		for ( i = 0 ; i < npoints ; i++ )
		{	
			array[offset + i*stride] = vec[i].w_float;
		}
	}
	//~ printf("copy done\n");
}
