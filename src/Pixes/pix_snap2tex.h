/*-----------------------------------------------------------------
  LOG
  GEM - Graphics Environment for Multimedia

  Snap a pix of the render buffer into a texture

  Copyright (c) 1997-1999 Mark Danks. mark@danks.org
  Copyright (c) Günther Geiger. geiger@epy.co.at
  Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
  Copyright (c) 2003 Daniel Heckenberg

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

  -----------------------------------------------------------------*/

#ifndef _INCLUDE__GEM_PIXES_PIX_SNAP_TEX_H_
#define _INCLUDE__GEM_PIXES_PIX_SNAP_TEX_H_

#include "Base/GemBase.h"
#include "Gem/State.h"

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
  CPPEXTERN_HEADER(pix_snap2tex, GemBase);

 public:

  //////////
  // Constructor
  pix_snap2tex(int argc, t_atom *argv);

 protected:

  //////////
  // Destructor
  virtual ~pix_snap2tex();

  ////////
  // extension check
  virtual bool isRunnable(void);

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
  void      textureQuality(int type);
	int				m_textureType;

	//////////
	// Set the texture quality
	// [in] type - if == 1, then GL_REPEAT, else GL_CLAMP_TO_EDGE
	void			repeatMess(int type);
	GLuint			m_repeat;

	void			modeMess(int type);
	int				m_mode;

	void			texUnitMess(int unit);
  GLuint    m_texUnit;


  //////////
  // The position
  int     	m_x, m_y;


  //////////
  // The size
  int     	m_width, m_height;

  //////////
  // The texture size
  int     	m_texWidth, m_texHeight;
  // for texcoords
  float m_xRatio, m_yRatio;

	//////////
	// The last image size (used to detect if image has changed)
  // LATER use setModified() for that
	int			m_oldWidth, m_oldHeight;

	// The texture coordinates
	TexCoord    	m_coords[4];

	//////////
	// this is what we get from upstream
	TexCoord        *m_oldTexCoords;
	int             m_oldNumCoords;
	int             m_oldTexture;


  //////////
  // The texture object number
  GLuint	   m_textureObj;


	//////////
	// did we really do texturing in render() ??
	// good to know in the postrender()...
	bool          m_didTexture;



	t_outlet	*m_outTexInfo;
};

#endif	// for header file
