/*-----------------------------------------------------------------
  LOG
  GEM - Graphics Environment for Multimedia

  texture map a pixBlock of any size onto an openGL-geo

  Copyright (c) 1997-1999 Mark Danks. mark@danks.org
  Copyright (c) Günther Geiger. geiger@epy.co.at
  Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  Copyright (c) 2002 James Tittle & Chris Clepper
  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

/*-----------------------------------------------------------------
pix_texture2

  2001:forum::für::umläute:2001
  IOhannes m zmoelnig
  mailto:zmoelnig@iem.kug.ac.at
-----------------------------------------------------------------*/

#ifndef INCLUDE_PIX_TEXTURE2_H_
#define INCLUDE_PIX_TEXTURE2_H_

#define TEX2

#include "Base/GemBase.h"
#include "Base/GemPixUtil.h"
#ifndef MACOSX
#include <GL/glu.h>
#else
#include <OpenGL/glu.h>
#endif


/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_texture2
    
    Turns on texture mapping, even if the image to be textured does
	not match the GL_TEXTURE size (powerOfTwo);
	this is much the same as in Mark Danks' pix_movie

KEYWORDS
    pix
    
DESCRIPTION

-----------------------------------------------------------------*/
class GEM_EXTERN pix_texture2 : public GemBase
{
  CPPEXTERN_HEADER(pix_texture2, GemBase)

    public:

  //////////
  // Constructor
  pix_texture2();
    	
 protected:
    	
  //////////
  // Destructor
  virtual ~pix_texture2();

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

  //////////
  // Set up the texture state
  void		setUpTextureState();

  //////////
  int           m_textureOnOff;

  //////////
  GLuint        m_textureQuality;

  //////////
  // Set the texture quality
  // [in] type - if == 0, then GL_NEAREST, else GL_LINEAR
  void          textureQuality(int type);

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
  // The texture coordinates
  TexCoord    	m_coords[4];

 private:

  //////////
  // static member functions
  static void 	floatMessCallback(void *data, float n);
  static void 	textureMessCallback(void *data, t_floatarg n);
};

#endif	// for header file
