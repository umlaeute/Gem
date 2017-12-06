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
  virtual ~pix_snap2tex(void);

  ////////
  // extension check
  virtual bool isRunnable(void);

  //////////
  // Do the rendering
  virtual void  render(GemState *state);

  //////////
  // Clear the dirty flag on the pixBlock
  virtual void  postrender(GemState *state);

  //////////
  // Establish texture object
  virtual void  startRendering(void);

  //////////
  // Delete texture object
  virtual void  stopRendering(void);


  //////////
  // When a snap is received
  virtual void  snapMess(void);
  GLuint m_textureObj;
  GLint   m_textureType;
  bool m_didTexture; // used in postrender() to check whether render() did texture
  bool m_init;

  //////////
  // Turn on/off texture mapping
  void textureOnOff(bool on);
  bool             m_textureOnOff;

  //////////
  // Set up the texture state
  void setUpTextureState(void);

  //////////
  // Set the texture quality
  // [in] type - if == 0, then GL_NEAREST, else GL_LINEAR
  void      textureQuality(int type);
  GLuint                        m_textureQuality;

  //////////
  // Set the texture quality
  // [in] type - if == 1, then GL_REPEAT, else GL_CLAMP_TO_EDGE
  void                    repeatMess(int type);
  GLuint                  m_repeat;

  void                    rectangleMess(int);
  int                             m_rectangle, m_canRectangle;

  void                    texUnitMess(int unit);
  GLuint    m_texUnit;

  //////////
  // position & size for grabbing
  virtual void  sizeMess(int width, int height);
  virtual void  posMess(int x, int y);
  int           m_x, m_y;
  int           m_width, m_height;

  //////////
  // The texture size
  int           m_texWidth, m_texHeight;

  void setTexCoords(float x, float y);
  // The texture coordinates
  TexCoord        m_coords[4];
  float m_xRatio, m_yRatio;

  //////////
  // this is what we get from upstream
  TexCoord        *m_oldTexCoords;
  int             m_oldNumCoords;
  int             m_oldTexture;

  //////////
  // outlet for texture-infor
  t_outlet        *m_outTexInfo;
};

#endif  // for header file
