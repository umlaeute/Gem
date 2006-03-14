/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    render to offscreen buffer and make texture
	
	created 11/27/2005
	
	Copyright (c) 2005 James Tittle II, tigital AT mac DOT com
	For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_GEMFRAMEBUFFER_H_
#define INCLUDE_GEMFRAMEBUFFER_H_

#include "Base/GemBase.h"
#include <iostream>
using namespace std;


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
    	gemframebuffer(t_symbol *colorspace);

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
		// colorspace-message
		virtual void	csMess(char* format);
		t_symbol *colorspace;
		int m_colorspace;
    	
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
		t_outlet		*m_outTexInfo;
		
		void			CheckErrorsGL( const char* location = NULL, 
										std::ostream& ostr = std::cerr );
		
    	void	    	bangMess();
    	
    	//////////
    	// static member functions
    	static void 	bangMessCallback(void *data);
		static void		modeCallback(void *data, t_floatarg quality);
		static void 	dimMessCallback(void *data, t_floatarg width, t_floatarg height );
		static void		texTargetCallback(void *data, t_floatarg tt);
		static void		csMessCallback(void *data, t_symbol *colorspace);
};

#endif	// for header file
