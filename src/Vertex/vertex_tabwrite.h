/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    A vertex_tabwrite

    Copyright (c) 1997-2000 Mark Danks. mark@danks.org
    Copyright (c) G¸nther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f¸r::uml‰ute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_VERTEX_TABWRITE_H_
#define INCLUDE_VERTEX_TABWRITE_H_

#include "Base/GemVertex.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    vertex_tabwrite
    
    writes vertices to a table (interleaved)

KEYWORDS
    Vertex, table
    
DESCRIPTION
    
-----------------------------------------------------------------*/
class GEM_EXTERN vertex_tabwrite : public GemVertex
{
    CPPEXTERN_HEADER(vertex_tabwrite, GemVertex)

    public:

        //////////
        // Constructor
  vertex_tabwrite(int, t_atom*);
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~vertex_tabwrite();


	//////////
	// the vertex data
	int      m_size;
        GLfloat	*m_VertexArray;
        GLfloat	*m_ColorArray;
	GLfloat *m_NormalArray;
        GLfloat	*m_TexCoordArray;

	//////////
	// do write the vertex data in the next cycle
	void     bangMess();
	bool     m_doit;
	
   	//////////
    	// Do the rendering
    	virtual void 	render(GemState *state);

	//////////
	// set the tables that hold the interleaved data (vertex, color, ...)
	virtual void tableMess(int,t_atom*);
	t_symbol *m_Vtable, *m_Ctable, *m_Ttable, *m_Ntable;

 private:
        static void 	tableMessCallback(void *data, t_symbol*,int,t_atom*);
        static void 	bangMessCallback(void *data);
};

#endif	// for header file
