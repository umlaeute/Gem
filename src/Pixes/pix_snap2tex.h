/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    Snap a pix of the render buffer into a texture

    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyright (c) G�nther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f�r::uml�ute. IEM. zmoelnig@iem.kug.ac.at
    Copyright (c) 2003 Daniel Heckenberg

    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_PIX_SNAP2TEX_H_
#define INCLUDE_PIX_SNAP2TEX_H_

#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_snap2tex
    
    Snaps a pix of the render buffer into a texture
    
KEYWORDS
    pix
    
DESCRIPTION

    Inlet for a list - "vert_size"
    Inlet for a list - "vert_pos"

    "snap" - Snap a pix
    "vert_size" - Set the size of the pix
    "vert_pos" - Set the position of the pix
	"quality" - set the texture mapping algorithm
    "texture" - turn texture mapping on/off

-----------------------------------------------------------------*/
class pix_snap2tex : public GemBase
{
    CPPEXTERN_HEADER(pix_snap2tex, GemBase)

    public:

        //////////
        // Constructor
    	pix_snap2tex(int argc, t_atom *argv);
 
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~pix_snap2tex();

    	//////////
    	// When a snap is received
    	virtual void	snapMess();
    	
    	//////////
    	// Do the rendering
    	virtual void 	render(GemState *state);

    	//////////
    	// Clear the dirty flag on the pixBlock
    	virtual void 	postrender(GemState *state);

    	//////////
    	// When a size message is received
    	virtual void	sizeMess(int width, int height);
    	
    	//////////
    	// When a position message is received
    	virtual void	posMess(int x, int y);
    	
    	//////////
    	// Clean up the image
    	void	    	cleanImage();
    	//////////
    	// Establish texture object
    	virtual void	startRendering();

    	//////////
    	// Delete texture object
    	virtual void	stopRendering();
    	
        //////////
        // Turn on/off texture mapping
        void            textureOnOff(int on);

		//////////
		// Set up the texture state
		void			setUpTextureState();

        //////////
        int             m_textureOnOff;

        //////////
        GLuint			m_textureQuality;

        //////////
        // Set the texture quality
        // [in] type - if == 0, then GL_NEAREST, else GL_LINEAR
        void            textureQuality(int type);
    	
    	//////////
    	// The x position
    	int     	m_x;
    	
    	//////////
    	// The y position
    	int     	m_y;
    	
    	//////////
    	// The width
    	int     	m_width;
    	
    	//////////
    	// The height
    	int     	m_height;

		//////////
    	// The texture width
    	int     	m_texWidth;
    	
    	//////////
    	// The texture height
    	int     	m_texHeight;

			//////////
		// The last image size
		int			m_oldWidth;
		int			m_oldHeight;

		  // The texture coordinates
		  TexCoord    	m_coords[4];

    	//////////
    	// The texture object number
    	GLuint	    	m_textureObj;

    private:
    	
    	//////////
    	// static member functions
    	static void 	snapMessCallback(void *data);
    	static void 	sizeMessCallback(void *data, t_floatarg width, t_floatarg height );
    	static void 	posMessCallback(void *data, t_floatarg x, t_floatarg y);
    	static void 	floatMessCallback(void *data, float n);
    	static void 	textureMessCallback(void *data, t_floatarg n);
};

#endif	// for header file
