/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    load texture ID's to specific texture units

  Copyright (c) 2005 James Tittle II <tigita AT mac.com>
  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_PIX_MULTITEXTURE_H_
#define INCLUDE_PIX_MULTITEXTURE_H_

#include "Base/GemBase.h"
#include "Base/GemPixUtil.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_multitexture
    
    Assigns texID's to texUnits for later combining or shading

KEYWORDS
    pix
    
DESCRIPTION

-----------------------------------------------------------------*/
class GEM_EXTERN pix_multitexture : public GemBase
{
    CPPEXTERN_HEADER(pix_multitexture, GemBase)

    public:

        //////////
        // Constructor
    	pix_multitexture(t_floatarg);
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~pix_multitexture();

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

		int				m_reqTexUnits; // requested # of texture Units, defaults to m_max
		GLint			m_max; // maximum # of texture units suppport by the specific card
		GLint			m_texID[32];
		GLint			m_textureType;
		int				m_mode; // 1=TEXTURE_RECTANGLE_EXT, 0=TEXTURE_2D
		
		//////////
		// The texture coordinates
		TexCoord    	m_coords[4];
		float			m_xRatio;
		float			m_yRatio;
		GLboolean		upsidedown;
        
	

 private:

	//////////
	// static member functions
	static void 	texUnitMessCallback(void *data, float n, float texture);
	static void		modeCallback(void *data, t_floatarg quality);
};

#endif	// for header file
