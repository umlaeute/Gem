/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    A gemvertexbuffer

    Copyright (c) 1997-2000 Mark Danks. mark@danks.org
    Copyright (c) Günther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#include "Base/GemShape.h"
#include "Gem/Image.h"
#include "Gem/State.h"
	
/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    gemvertexbuffer
    
    Creates a gemvertexbuffer

KEYWORDS
    geo
    
DESCRIPTION
    
-----------------------------------------------------------------*/
class GEM_EXTERN gemvertexbuffer : public GemShape
{
    CPPEXTERN_HEADER(gemvertexbuffer, GemShape);

    public:

        //////////
        // Constructor
    	gemvertexbuffer(t_floatarg size);
    	
    	/////////////
    	// Variables
    	//float range_x;
    	//float range_y;
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~gemvertexbuffer(void);
    	
    	//////////
    	// Destructor
    	void cleanUp(void);    	
        
    	//////////
    	// Do the rendering
    	virtual void renderShape(GemState *state);
    	//virtual void 	runKernel();
    	void tabMess(int argc, t_atom *argv, float *array, int stride, int offset);
 		void resizeMess(float size);

    	

	private :
    	    	
		// GL functionality
		void createVBO(void);
		void copyArray(t_symbol *tab_name, float *array, unsigned int stride, unsigned int offset);
		static void posxMessCallback(void *data, t_symbol *symbol, int argc, t_atom *argv);
		static void posyMessCallback(void *data, t_symbol *symbol, int argc, t_atom *argv);
		static void poszMessCallback(void *data, t_symbol *symbol, int argc, t_atom *argv);
		static void colrMessCallback(void *data, t_symbol *symbol, int argc, t_atom *argv);
		static void colgMessCallback(void *data, t_symbol *symbol, int argc, t_atom *argv);
		static void colbMessCallback(void *data, t_symbol *symbol, int argc, t_atom *argv);
		static void colaMessCallback(void *data, t_symbol *symbol, int argc, t_atom *argv);
		static void texuMessCallback(void *data, t_symbol *symbol, int argc, t_atom *argv);
		static void texvMessCallback(void *data, t_symbol *symbol, int argc, t_atom *argv);
		static void normxMessCallback(void *data, t_symbol *symbol, int argc, t_atom *argv);
		static void normyMessCallback(void *data, t_symbol *symbol, int argc, t_atom *argv);
		static void normzMessCallback(void *data, t_symbol *symbol, int argc, t_atom *argv);
		static void resizeMessCallback(void *data, float size);
		static void posVBO_enableMessCallback(void *data, float flag);
		static void colVBO_enableMessCallback(void *data, float flag);
		static void texVBO_enableMessCallback(void *data, float flag);
		static void normVBO_enableMessCallback(void *data, float flag);

		
		// Rendering window vars
		int vbo_size;
		bool posArray_dirty, 	texArray_dirty, colArray_dirty, normArray_dirty;
		bool posVBO_enable, 	texVBO_enable, 	colVBO_enable, 	normVBO_enable;
		bool size_change_flag;

		float *posArray, *texArray, *colArray, *normArray;

		// vbo variables
		GLuint posVBO, texVBO, colVBO, normVBO;
};
