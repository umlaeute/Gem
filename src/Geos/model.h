/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    read in a model file

    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyright (c) G�nther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f�r::uml�ute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_MODEL_H_
#define INCLUDE_MODEL_H_

#include "Base/GemBase.h"
#include "model_loader.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    model
    
    read in a model file

DESCRIPTION

    Inlet for a list - "model"

    "open" - the RGB model to set the object to
    
-----------------------------------------------------------------*/
class GEM_EXTERN model : public GemBase
{
    CPPEXTERN_HEADER(model, GemBase)

    public:

	    //////////
	    // Constructor
    	model(t_symbol *filename);
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~model();

    	//////////
    	// When an open is received
    	virtual void	openMess(t_symbol *filename);
    	
    	//////////
    	// When a rescale is received
    	virtual void	rescaleMess(int state);
  	//////////
    	// When a reverse is received
    	virtual void	reverseMess(int state);
  	//////////
    	// Which texture type (linear, spheric)
    	virtual void	textureMess(int state);

  	//////////
    	// Set smoothing factor
    	virtual void	smoothMess(t_float fsmooth);
    	
  	//////////
    	// Set material mode
	virtual void    materialMess(int material);
        
        //////////
    	// Set material mode
	virtual void    groupMess(int group);

   	//////////
    	virtual void	cleanModel();   	
	//////////
    	virtual void    buildList();	

    	//////////
    	virtual void	render(GemState *state);
    	//////////
    	virtual void	startRendering();

    	//////////
	GLMmodel       *m_model;
    	GLint	    	m_dispList;
        
	//////////
	// Should we rescale the model when loaded
	// Default is yes
	int		m_rescaleModel;
        
	GLfloat         m_smooth;
	int             m_material;

	int             m_flags;
        
        int             m_group;
    	
        float		currentH, currentW;
        
    private:
    
    	//////////
    	// static member functions
    	static void 	openMessCallback(void *data, t_symbol *filename);
    	static void	rescaleMessCallback(void *data, t_floatarg);
    	static void	smoothMessCallback(void *data, t_floatarg);
	static void     reverseMessCallback(void *data, t_floatarg);
	static void     materialMessCallback(void *data, t_floatarg);
	static void     textureMessCallback(void *data, t_floatarg);
        static void     groupMessCallback(void *data, t_floatarg);
};

#endif	// for header file
