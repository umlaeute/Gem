/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    A vertex_model

    Copyright (c) 1997-2000 Mark Danks. mark@danks.org
    Copyright (c) G�nther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f�r::uml�ute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_vertex_model_H_
#define INCLUDE_vertex_model_H_

#include "Base/GemVertex.h"
#include "Geos/model_loader.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    vertex_model
    
    Creates a vertex_model

KEYWORDS
    geo
    
DESCRIPTION
    
-----------------------------------------------------------------*/
class GEM_EXTERN vertex_model : public GemBase
{
    CPPEXTERN_HEADER(vertex_model, GemBase)

    public:

        //////////
        // Constructor
    	vertex_model();
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~vertex_model();
        
        //int	m_blend;
        float	*m_ColorArray;
        float	*m_VertexArray;
        float	*m_tempCA;
        float	*m_tempVA;
        float	*m_TexCoordArray;
        float	*m_tempTA;
        float	*m_NormalArray;
        float	*m_tempNA;
        GLMmodel       *m_model;
        int	m_vertcount;
        int	m_haveModel;
        int	m_oldVSize,m_oldCSize;
        float	maxX, maxY, oldmaxX, oldmaxY;
        
        //////////
	// Should we rescale the model when loaded
	// Default is yes
	int		m_rescaleModel;

    	//////////
    	// Do the rendering
    	virtual void 	render(GemState *state);
        virtual void	openMess(t_symbol *filename);
        
        private:
        static void	openMessCallback(void *data, t_symbol *filename);

};

#endif	// for header file
