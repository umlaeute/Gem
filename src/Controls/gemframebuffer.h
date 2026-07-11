/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2005, James Tittle II and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_CONTROLS_GEMFRAMEBUFFER_H_
#define _INCLUDE__GEM_CONTROLS_GEMFRAMEBUFFER_H_

#include "Base/GemBase.h"
#include "Gem/GemGL.h"
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
  CPPEXTERN_HEADER(gemframebuffer, GemBase);

public:

  //////////
  // Constructor
  gemframebuffer(int, t_atom*);

protected:

  //////////
  // Destructor
  ~gemframebuffer(void);

  //////////
  // A render message
  void         render(GemState *state);
  void         postrender(GemState *state);
  void         initFBO(void);
  void         destroyFBO(void);
  void         initMSAAFBO(void);
  void         destroyMSAAFBO(void);

  //////////
  // Set up the modifying flags
  void         startRendering(void);
  //////////
  // Clean up the modifying flags
  void         stopRendering(void);

  // extension checks
  virtual bool isRunnable(void);

  //////////
  // change the size dimensions
  void         dimMess(int width, int height);

  //////////
  // format-message
  virtual void formatMess(std::string);
  virtual void typeMess(std::string);

  virtual void colorMess(t_symbol*,int argc, t_atom*argv);
  virtual void perspectiveMess(t_symbol*,int argc, t_atom*argv);

  virtual void rectangleMess(bool mode);
  virtual void texunitMess(int mode);
  virtual void qualityMess(int mode);
  virtual void repeatMess(int mode);
  virtual void clearMess(bool mode);
  virtual void verboseMess(bool mode);
  virtual void msaaMess(int samples);


  virtual void fixFormat(GLenum wantedFormat);
  virtual void printInfo(void);

private:
  GLboolean             m_haveinit, m_wantinit;
  GLuint      m_frameBufferIndex;
  GLuint      m_depthBufferIndex;
  GLuint      m_offScreenID;
  GLuint      m_texTarget;
  GLuint      m_texunit;
  int         m_width, m_height;
  bool        m_rectangle; // 1=TEXTURE_RECTANGLE_EXT, 0=TEXTURE_2D
  GLenum      m_canRectangle; // whichever rectangle formats are supported
  int         m_internalformat;
  int         m_format;
  GLenum      m_wantFormat;
  int         m_type;
  GLint       m_vp[4];
  GLfloat     m_color[4];
  GLfloat     m_FBOcolor[4];
  t_outlet   *m_outTexInfo;
  GLfloat     m_perspect[6];
  GLint       m_quality;
  GLint       m_repeat;
  bool        m_clear;

  bool        m_verbose; // print debugging info when changing parametres

  // MSAA support
  int         m_msaaSamples;
  GLuint      m_msaaFBO;
  GLuint      m_msaaColorBuffer;
  GLuint      m_msaaDepthBuffer;

  void        bangMess(void);
};

#endif   // for header file
