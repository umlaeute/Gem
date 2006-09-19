/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    render to offscreen buffer and make texture
	
	created 11/27/2005
	
	Copyright (c) 2005-2006 James Tittle II, tigital AT mac DOT com
	For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_GEMFRAMEBUFFER_H_
#define INCLUDE_GEMFRAMEBUFFER_H_

#include "Base/GemBase.h"
#include <iostream>

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    gemframebuffer
    
    render to offscreen buffer and make texture

DESCRIPTION
    
    "bang" - sends out a state list
    
-----------------------------------------------------------------*/
class GEM_EXTERN gemframebuffer : public GemBase
{
    	CPPEXTERN_HEADER(gemframebuffer, GemBase)

    public:

	    //////////
	    // Constructor
    	gemframebuffer( void );
		gemframebuffer(t_symbol *format, t_symbol *type);

    protected:
    	
    	//////////
    	// Destructor
    	~gemframebuffer();
		    	
    	//////////
    	// A render message
    	void	    	render(GemState *state);
		void	    	postrender(GemState *state);
		void			initFBO();
		void			destroyFBO();
    	    	
    	//////////
    	// Set up the modifying flags
    	void	    	startRendering();

    	//////////
    	// Clean up the modifying flags
    	void	    	stopRendering();


    	//////////
    	// change the size dimensions
    	void	    	dimMess(int width, int height);
		
		////////// 
		// format-message
		virtual void	formatMess(char* format);
		virtual void	typeMess(char* type);
		
		virtual void colorMess(float red, float green, float blue, float alpha);
    	
	private:

		int             m_init;
		GLuint			m_frameBufferIndex;
		GLuint			m_depthBufferIndex;
		GLuint			m_offScreenID;
		GLuint			m_texTarget;
		int				m_width, m_height;
		int				m_mode; // 1=TEXTURE_RECTANGLE_EXT, 0=TEXTURE_2D
		int				m_internalformat;
		int				m_format;
		int				m_type;
		GLint			m_vp[4];
		GLfloat			m_color[4];
		GLfloat			m_FBOcolor[4];
		t_outlet		*m_outTexInfo;
		
    	void	    	bangMess();
    	
    	//////////
    	// static member functions
    	static void 	bangMessCallback(void *data);
		static void		modeCallback(void *data, t_floatarg quality);
		static void 	dimMessCallback(void *data, t_floatarg width, t_floatarg height );
		static void		texTargetCallback(void *data, t_floatarg tt);
		static void		formatMessCallback(void *data, t_symbol *format);
		static void		typeMessCallback(void *data, t_symbol *type);
		static void 	colorMessCallback(void *data, t_floatarg red, t_floatarg green, t_floatarg blue, t_floatarg alpha);
};

#endif	// for header file
