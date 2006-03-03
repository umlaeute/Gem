/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    texture map a pixBlock onto a openGL-Geo

  Copyright (c) 1997-1999 Mark Danks. mark@danks.org
  Copyright (c) Günther Geiger. geiger@epy.co.at
  Copyright (c) 2001-2005 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  Copyright (c) 2002-2005 James Tittle & Chris Clepper
  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_PIX_TEXTURE_H_
#define INCLUDE_PIX_TEXTURE_H_

#include "Base/GemBase.h"
#include "Base/GemPixUtil.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_texture
    
    Turns on texture mapping

KEYWORDS
    pix
    
DESCRIPTION

-----------------------------------------------------------------*/
class GEM_EXTERN pix_texture : public GemBase
{
  //////////
  // texturing in [pix_movie] is done as a friend
  friend class pix_movieNEW;

    CPPEXTERN_HEADER(pix_texture, GemBase)

    public:

        //////////
        // Constructor
    	pix_texture();
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~pix_texture();

    	//////////
    	// Do the rendering
    	virtual void 	render(GemState *state);

    	//////////
    	// Turn back off texture mapping
    	virtual void 	postrender(GemState *state);

    	//////////
    	// Establish texture object
    	virtual void	startRendering();

    	//////////
    	// Delete texture object
    	virtual void	stopRendering();
    	
		//////////
		// Turn on/off texture mapping
		void          textureOnOff(int on);
		int           m_textureOnOff;

		//////////
		// Set up the texture state
		void		setUpTextureState();

		//////////
		// Set the texture quality
		// [in] type - if == 0, then GL_NEAREST, else GL_LINEAR
		void          textureQuality(int type);
		GLuint		  m_textureQuality;
		
		//////////
		// Set the texture quality
		// [in] type - if == 1, then GL_REPEAT, else GL_CLAMP_TO_EDGE
		void          repeatMess(int type);
		GLuint        m_repeat;

		//////////
		// Do we need to rebuild the display List
		int           m_rebuildList;

		//////////
		// The size of the texture (so we can use sub image)
		int	        m_dataSize[3];

		//////////
		// The texture object number
		GLuint	    m_textureObj;

		//////////
		// The resizing buffer
		imageStruct   m_buffer;
		//////////
		// a buffer for colour-space conversion
		imageStruct   m_imagebuf;

		//////////
		// The texture coordinates
		TexCoord    	m_coords[4];

		int             m_textureType; // GL_TEXTURE_2D, GL_TEXTURE_RECTANGLE_EXT
		bool            m_normalized;  // whether the image is power of 2
        int				m_mode; //rectangle or power of 2
		
		//////////
		// texture envirnoment mode
		void			envMess(int num);
		GLint			m_env; // GL_TEXTURE_ENV_MODE
		
        int				m_clientStorage; //for Apple's client storage extension
        GLenum			m_internalFormat;
        int				m_yuv; // try to texture YUV-images directly when gxf-card says it is possible to do so
        
		t_outlet		*m_outTexID;

 private:

	//////////
	// static member functions
	static void 	floatMessCallback(void *data, float n);
	static void 	textureMessCallback(void *data, t_floatarg n);
	static void 	modeCallback(void *data, t_floatarg n);
	static void     envMessCallback(void *data, t_floatarg n);
	static void 	repeatMessCallback(void *data, t_floatarg n);
	static void 	clientStorageCallback(void *data, t_floatarg n);
	static void 	yuvCallback(void *data, t_floatarg n);
};

#endif	// for header file
